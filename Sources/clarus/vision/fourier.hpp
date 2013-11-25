#ifndef CLARUS_VISION_SNAPSHOT_HPP
#define CLARUS_VISION_SNAPSHOT_HPP

#include <opencv2/opencv.hpp>

namespace fourier {
    cv::Size fit(const cv::Size &size);

    cv::Mat convolve(const cv::Mat &data, const cv::Mat &kernel);

    cv::Mat transform(const cv::Mat &data, const cv::Size &optimal = cv::Size(0, 0));

    cv::Mat inverse(const cv::Mat &fourier, const cv::Size &optimal = cv::Size(0, 0));

    cv::Mat magnitude(const cv::Mat &fourier);

    cv::Mat phase(const cv::Mat &fourier);

    cv::Mat bgr(const cv::Mat &fourier);
}

#endif
