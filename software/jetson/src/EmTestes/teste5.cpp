#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// Gamma correction
cv::Mat gammaCorrection(const cv::Mat& img, double gamma) {
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for(int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

    cv::Mat res;
    cv::LUT(img, lookUpTable, res);
    return res;
}

// CLAHE per channel
cv::Mat claheColor(const cv::Mat& img, double clipLimit=2.0, cv::Size gridSize=cv::Size(5,5)) {
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, gridSize);
    for(auto& c : channels) {
        clahe->apply(c, c);
    }
    cv::Mat result;
    cv::merge(channels, result);
    return result;
}

// Get orientation of contour in degrees
double getOrientation(const std::vector<cv::Point>& contour) {
    cv::RotatedRect box = cv::minAreaRect(contour);
    double angle = box.angle;
    if (box.size.width < box.size.height)
        angle = 90 + angle;
    else
        angle = angle;
    return std::abs(angle);
}

// Get center x of contour's bounding box
double getCenterXBBox(const std::vector<cv::Point>& contour) {
    cv::RotatedRect box = cv::minAreaRect(contour);
    return box.center.x;
}

// Dummy checkCenter: verifica se o centro do gate está próximo do centro da imagem
bool checkCenter(int center_x, int center_y, int width, int height, int tol=20) {
    return (std::abs(center_x - width/2) < tol);
}

// Dummy alignGate: retorna a ação para alinhar (apenas strings para exemplo)
std::string alignGate(int center_x, int center_y, double ratio, int contoursFound, int status, int tolerance) {
    if (contoursFound < 2)
        return "search_gate";
    int center_image = 640/2; // exemplo para resolução 640x480
    if (std::abs(center_x - center_image) > tolerance) {
        if (center_x < center_image)
            return "move_left";
        else
            return "move_right";
    }
    if (ratio < 0.9)
        return "rotate_left";
    else if (ratio > 1.1)
        return "rotate_right";
    return "move_forward";
}

int main() {
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cerr << "Erro ao abrir camera" << std::endl;
        return -1;
    }

    cv::namedWindow("Preprocessed", cv::WINDOW_NORMAL);
    cv::namedWindow("Gate Detection", cv::WINDOW_NORMAL);

    int status = 1;

    while(true) {
        cv::Mat frame;
        cap >> frame;
        if(frame.empty()) break;

        // --- Pré-processamento ---
        cv::Mat img = gammaCorrection(frame, 1.7);
        img = claheColor(img);
        cv::GaussianBlur(img, img, cv::Size(5,5), 0);

        // HSV e segmentação laranja
        cv::Mat hsv;
        cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

        cv::Scalar low(5, 10, 10);
        cv::Scalar high(70, 150, 150);
        cv::Mat mask;
        cv::inRange(hsv, low, high, mask);

        cv::Mat result;
        cv::bitwise_and(hsv, hsv, result, mask);

        cv::Mat bgrResult;
        cv::cvtColor(result, bgrResult, cv::COLOR_HSV2BGR);
        cv::Mat gray;
        cv::cvtColor(bgrResult, gray, cv::COLOR_BGR2GRAY);

        cv::Mat thresh;
        cv::threshold(gray, thresh, 30, 100, cv::THRESH_BINARY);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        cv::Mat eroded;
        cv::erode(thresh, eroded, kernel);
        cv::Mat dilated;
        cv::dilate(eroded, dilated, kernel, cv::Point(-1,-1), 2);

        kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
        cv::Mat closed;
        cv::morphologyEx(dilated, closed, cv::MORPH_CLOSE, kernel);

        // Debug: printa quantos pixels brancos tem na imagem preprocessada
        int nonZero = cv::countNonZero(closed);
        std::cout << "Pixels brancos no preprocessado: " << nonZero << std::endl;

        cv::imshow("Preprocessed", closed);

        // resto do código segue igual (contornos, filtro, desenho, etc.)

        // ...

        // Imagem com contornos desenhados
        cv::Mat output = frame.clone();
        // desenha contornos e círculo, etc.

        cv::imshow("Gate Detection", output);

        char key = (char)cv::waitKey(30);
        if(key == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
