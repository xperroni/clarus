#include "camera.hpp"

#include <stdexcept>

Camera::Camera() {
    camera.reset();
}

Camera::Camera(int index) {
    camera.reset();
    start(index);
}

void Camera::start(int index) {
    camera.reset(new cv::VideoCapture(index));
    if(!camera->isOpened()) {
        throw std::runtime_error("Could not connect to camera");
    }

    // Hack to avoid the first few frames to be the same.
    for (int i = 0; i < 10; i++) {
        grab();
    }
}

void Camera::stop() {
    camera.reset();
}

cv::Mat Camera::grab() {
    cv::Mat frame;
    locked_ptr<cv::VideoCapture> locked = locker.write(*camera);
    locked->read(frame);
    return frame;
}
