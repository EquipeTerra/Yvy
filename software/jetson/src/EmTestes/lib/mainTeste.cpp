#include "capture.h"
#include "detector.h"
#include "preprocessor.h"
#include <iostream>
#include <opencv2/opencv.hpp>
int main(){
    Capture capture;
    if (!capture.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    Detector detector;
    Preprocessor preprocessor;

    cv::Mat frame;
    int status = 1; // Initial status

    while (true) {
        if (!capture.getFrame(frame)) {
            std::cerr << "Error: Could not capture frame." << std::endl;
            break;
        }

        // Preprocess the frame
        //cv::Mat processed_frame = preprocessor.preprocess(frame);

        // Detect gate and get action
        GateDetectionResult result = detector.detectGate(frame, status);
        std::cout << GateDetectionResult.action << std::endl;
        // Display the annotated image
        cv::imshow("Gate Detection", result.annotated_image);
        
        // Update status based on action
        if (result.action == "align") {
            status = 2; // Aligning
        } else if (result.action == "center") {
            status = 3; // Centered
        } else {
            status = 1; // Default state
        }

        if (cv::waitKey(30) >= 0) break; // Exit on any key press
    }

    return 0;
}