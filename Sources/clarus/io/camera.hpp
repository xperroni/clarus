#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <clarus/core/locker.hpp>

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

class Camera
{
    boost::shared_ptr<cv::VideoCapture> camera;

    Locker locker;

public:
    Camera();

    Camera(int index);

    void start(int index = 0);

    void stop();

    cv::Mat grab();
};

#endif // CAMERA_HPP
