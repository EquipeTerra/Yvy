#include "detector.h"
#include <algorithm>
#include <cmath>

Detector::Detector() {}

GateDetectionResult Detector::detectGate(const cv::Mat& image, int status) {
    cv::Mat orig_image = image.clone();
    cv::Mat processed_image;
    
    // Convert to HSV and threshold for orange
    cv::cvtColor(image, processed_image, cv::COLOR_BGR2HSV);
    
    cv::Scalar low(5, 10, 10);
    cv::Scalar high(70, 150, 150);
    cv::Mat mask, result;
    cv::inRange(processed_image, low, high, mask);
    cv::bitwise_and(processed_image, processed_image, result, mask);
    
    // Convert HSV result back to BGR (para depois converter pra grayscale)
    cv::Mat bgr_result;
    cv::cvtColor(result, bgr_result, cv::COLOR_HSV2BGR);

    // Agora converte para grayscale 8-bit (CV_8UC1), garantindo o formato certo para findContours
    cv::Mat gray;
    cv::cvtColor(bgr_result, gray, cv::COLOR_BGR2GRAY);
    
    // Threshold para binarizar
    cv::Mat thresh;
    cv::threshold(gray, thresh, 30, 255, cv::THRESH_BINARY);
    
    // Morphological operations
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat eroded, dilated, close;
    cv::erode(thresh, eroded, kernel);
    cv::dilate(eroded, dilated, kernel, cv::Point(-1,-1), 2);
    
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(dilated, close, cv::MORPH_CLOSE, kernel);
    
    // Agora sim: findContours com imagem CV_8UC1
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(close, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    int number_poles = 2;
    
    // Filter contours by aspect ratio, area and orientation
    std::vector<std::vector<cv::Point>> filtered_contours;
    for (const auto& c : contours) {
        cv::RotatedRect rect = cv::minAreaRect(c);
        float width = rect.size.width;
        float height = rect.size.height;
        float aspect_ratio = std::max(width, height) / std::min(width, height);
        float area = cv::contourArea(c);
        double orientation = getOrientation(c);
        
        if (aspect_ratio > 5.0 && area > 500.0 && orientation >= 75.0 && orientation <= 105.0) {
            filtered_contours.push_back(c);
        }
    }
    
    // Sort by area and take top contours
    std::sort(filtered_contours.begin(), filtered_contours.end(), 
        [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
            return cv::contourArea(a) > cv::contourArea(b);
        });
    
    if (filtered_contours.size() > number_poles) {
        filtered_contours.resize(number_poles);
    }
    
    int number_contours_found = filtered_contours.size();
    double center_gate_x = 0, center_gate_y = 0;
    double ratio = 0.0;
    bool aligning_center_gate_to_image = false;
    std::string action = "";
    int counter = 0;
    
    if (status <= 2) {
        for (const auto& c : filtered_contours) {
            cv::RotatedRect rect = cv::minAreaRect(c);
            cv::Point2f center = rect.center;
            
            cv::Point2f vertices[4];
            rect.points(vertices);
            std::vector<cv::Point> box_points;
            for (int i = 0; i < 4; i++) {
                box_points.push_back(vertices[i]);
            }
            
            cv::drawContours(orig_image, std::vector<std::vector<cv::Point>>{box_points}, 0, cv::Scalar(0, 255, 0), 2);
            
            if (counter <= 1) {
                center_gate_x += center.x;
                center_gate_y += center.y;
            }
            counter++;
        }
        
        if (number_contours_found == 2) {
            std::sort(filtered_contours.begin(), filtered_contours.end(), 
                [this](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return getCenterXBBox(a) < getCenterXBBox(b);
                });
            
            cv::RotatedRect left_leg = cv::minAreaRect(filtered_contours[0]);
            cv::RotatedRect right_leg = cv::minAreaRect(filtered_contours[1]);
            
            ratio = std::max(left_leg.size.width, left_leg.size.height) / 
                   std::max(right_leg.size.width, right_leg.size.height);
            
            center_gate_x /= 2;
            center_gate_y /= 2;
            
            if (status == 1) {
                float left_area = left_leg.size.width * left_leg.size.height;
                float right_area = right_leg.size.width * right_leg.size.height;
                
                if (left_area >= 3000.0 && right_area >= 3000.0) {
                    if (checkCenter(center_gate_x, center_gate_y)) {
                        status = 2;
                        aligning_center_gate_to_image = false;
                    } else {
                        aligning_center_gate_to_image = true;
                        action = alignGate(center_gate_x, center_gate_y, ratio, number_contours_found, 2, 20);
                    }
                }
            } else {
                // TODO: Add object detection for "G_Man" if needed
                
                if (checkCenter(center_gate_x, center_gate_y)) {
                    status = 3;
                }
            }
        }
        
        cv::circle(orig_image, cv::Point(static_cast<int>(center_gate_x), static_cast<int>(center_gate_y)), 
                  5, cv::Scalar(0, 0, 255), 2);
    }
    
    if (!aligning_center_gate_to_image) {
        action = alignGate(center_gate_x, center_gate_y, ratio, number_contours_found, status, 20);
    }
    
    return {action, orig_image, status};
}

double Detector::getOrientation(const std::vector<cv::Point>& contour) {
    cv::RotatedRect rect = cv::minAreaRect(contour);
    return rect.angle;
}

double Detector::getCenterXBBox(const std::vector<cv::Point>& contour) {
    cv::RotatedRect rect = cv::minAreaRect(contour);
    return rect.center.x;
}

std::string Detector::alignGate(double center_gate_x, double center_gate_y, 
                                   double ratio, int num_contours, int status, int threshold) {
    if (num_contours < 2) {
        return "search";
    }
    
    double image_center_x = 320; // Assuming 640x480 image
    double offset = center_gate_x - image_center_x;
    
    if (std::abs(offset) < threshold) {
        return "forward";
    } else if (offset < 0) {
        return "left";
    } else {
        return "right";
    }
}

bool Detector::checkCenter(double center_x, double center_y) {
    double image_center_x = 320; // Assuming 640x480 image
    double image_center_y = 240;
    double threshold = 20;
    
    return (std::abs(center_x - image_center_x) < threshold && 
           std::abs(center_y - image_center_y) < threshold);
}
