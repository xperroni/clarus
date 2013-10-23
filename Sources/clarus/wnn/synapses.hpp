#ifndef SYNAPSES_HPP
#define SYNAPSES_HPP

#include <opencv2/opencv.hpp>

class Synapses {
    cv::Mat synapses;

public:
    Synapses(const cv::Mat &synapses);

    template<class T> const T &sample(const cv::Mat &input, int index) const;

    size_t size();
};

template<class T> const T &Synapses::sample(const cv::Mat &input, int index) const {
    int i = synapses.at<int>(index, 0);
    int j = synapses.at<int>(index, 1);
    return input.at<T>(i, j);
}

cv::Mat gaussian(int n, double sigma, const cv::Size &size);

#endif // SYNAPSES_HPP
