// ===============================
//  Preprocessor.cpp
// ===============================
#include "preprocessor.h"
#include <opencv2/imgproc.hpp>

cv::Mat Preprocessor::preprocess(const cv::Mat& input) {
    cv::Mat img = resize(input);
    img = toFloat(img);
    img = normalize(img);
    return img;
}

cv::Mat Preprocessor::resize(const cv::Mat& input) {
    cv::Mat output;
    cv::resize(input, output, cv::Size(PREPROC_TARGET_WIDTH, PREPROC_TARGET_HEIGHT));
    return output;
}

cv::Mat Preprocessor::toFloat(const cv::Mat& input) {
    cv::Mat output;
    input.convertTo(output, CV_32F, 1.0 / 255.0);
    return output;
}

cv::Mat Preprocessor::normalize(const cv::Mat& input) {
    std::vector<cv::Mat> channels(3);
    cv::split(input, channels);

    // Assume ordem BGR — se estiver usando RGB, troque a ordem das médias/STD conforme necessário
    channels[0] = (channels[0] - PREPROC_MEAN_B) / PREPROC_STD_B; // B
    channels[1] = (channels[1] - PREPROC_MEAN_G) / PREPROC_STD_G; // G
    channels[2] = (channels[2] - PREPROC_MEAN_R) / PREPROC_STD_R; // R

    cv::Mat output;
    cv::merge(channels, output);
    return output;
}