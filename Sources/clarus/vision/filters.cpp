#include <clarus/vision/filters.hpp>

cv::Mat sobel::filter(const cv::Mat &l) {
    cv::Mat g, x, y, e;

    // Gradient X
    cv::Sobel(l, g, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(g, x);

    // Gradient Y
    cv::Sobel(l, g, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(g, y);

    // Total Gradient (approximate)
    cv::addWeighted(x, 0.5, y, 0.5, 0, g);
    cv::convertScaleAbs(g, e);

    return e;
}

static int gauss(double sigma, int c, int n) {
    static cv::RNG rng;

    int i = rng.gaussian(sigma) + c;
    if (i < 0) {
        return 0;
    }
    else if (i >= n) {
        return n - 1;
    }

    return i;
}

gaussian::sampler::sampler() {
    // Nothing to do.
}

gaussian::sampler::sampler(size_t n, double sigma, const cv::Size &size) {
    int in = size.height;
    int jn = size.width;
    int ic = in / 2;
    int jc = jn / 2;

    while (points.size() < n) {
        point p(
            gauss(sigma, ic, in),
            gauss(sigma, jc, jn)
        );

        points.insert(p);
    }
}
