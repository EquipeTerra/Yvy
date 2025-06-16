#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct GateDetectionResult {
    std::string action;
    cv::Mat annotated_image;
    int status;
};

class Detector {
public:
    Detector();
    
    /**
     * @brief Detects gate in the image and determines alignment action
     * 
     * @param image Input image from AUV camera
     * @param status Current task status (1-3)
     * @return GateDetectionResult Contains action command, annotated image, and updated status
     */
    GateDetectionResult detectGate(const cv::Mat& image, int status);
    
private:
    /**
     * @brief Gets orientation of a contour
     * 
     * @param contour Input contour
     * @return double Orientation angle in degrees
     */
    double getOrientation(const std::vector<cv::Point>& contour);
    
    /**
     * @brief Gets center x coordinate of bounding box
     * 
     * @param contour Input contour
     * @return double Center x coordinate
     */
    double getCenterXBBox(const std::vector<cv::Point>& contour);
    
    /**
     * @brief Aligns AUV with gate center
     * 
     * @param center_gate_x Gate center x coordinate
     * @param center_gate_y Gate center y coordinate
     * @param ratio Legs ratio
     * @param num_contours Number of contours found
     * @param status Current status
     * @param threshold Alignment threshold
     * @return std::string Action command ("left", "right", etc.)
     */
    std::string alignGate(double center_gate_x, double center_gate_y, 
                         double ratio, int num_contours, int status, int threshold);
    
    /**
     * @brief Checks if gate is centered in image
     * 
     * @param center_x Gate center x coordinate
     * @param center_y Gate center y coordinate
     * @return true If gate is centered
     * @return false If gate is not centered
     */
    bool checkCenter(double center_x, double center_y);
};

#endif // DETECTOR_H