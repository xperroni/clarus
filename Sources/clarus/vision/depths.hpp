#ifndef CLARUS_VISION_DEPTHS_HPP
#define CLARUS_VISION_DEPTHS_HPP

#include <opencv2/opencv.hpp>

#include <string>

namespace depths {
    typedef float Depth;

    cv::Mat load(const std::string &path, const cv::Size &size);

    void save(const std::string &path, const cv::Mat &depths);

    cv::Mat bgr(const cv::Mat &depths, const cv::Size &scale = cv::Size(0, 0));
}

#endif // CLARUS_VISION_DEPTHS_HPP
