#ifndef CLARUS_VISION_BAYER_HPP
#define CLARUS_VISION_BAYER_HPP

#include <opencv2/opencv.hpp>

#include <string>

namespace bayer {
    cv::Mat filter(const cv::Mat &image);

    cv::Mat filter(const std::string &path);

    cv::Mat bgr(const cv::Mat &mosaic);
}

#endif
