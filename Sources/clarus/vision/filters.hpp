#ifndef CLARUS_VISION_EDGES_HPP
#define CLARUS_VISION_EDGES_HPP

#include <clarus/core/index.hpp>

#include <opencv2/opencv.hpp>

#include <set>

namespace sobel {
    cv::Mat filter(const cv::Mat &l);
}

namespace gaussian {
    class sampler;
}

class gaussian::sampler {
    typedef Index<2> point;

    std::set<point> points;

public:
    sampler();

    sampler(size_t n, double sigma, const cv::Size &size);

    template<class P> cv::Mat sample(const cv::Mat &sampling) const;
};

template<class P> cv::Mat gaussian::sampler::sample(const cv::Mat &sampling) const {
    cv::Mat samples(points.size(), 1, sampling.type(), cv::Scalar::all(0));

    int l = 0;
    for (std::set<point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const point &p = *k;
        samples.at<P>(l++) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

#endif
