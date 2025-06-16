#include "capture.h"

Capture::Capture(int cam_index) {
    cap.open(cam_index);
}

Capture::~Capture() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool Capture::isOpened() const {
    return cap.isOpened();
}

bool Capture::getFrame(cv::Mat& output) {
    if (!cap.isOpened()) return false;
    return cap.read(output);
}
