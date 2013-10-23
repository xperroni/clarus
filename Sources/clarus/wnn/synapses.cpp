#include <clarus/wnn/synapses.hpp>

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

cv::Mat gaussian(int n, double sigma, const cv::Size &size) {
    cv::Mat synapses(n, 2, CV_32S);
    int in = size.height;
    int jn = size.width;
    int ic = in / 2;
    int jc = jn / 2;

    for (int k = 0; k < n; k++) {
        synapses.at<int>(k, 0) = gauss(sigma, ic, in);
        synapses.at<int>(k, 1) = gauss(sigma, jc, jn);
    }

    return synapses;
}

Synapses::Synapses(const cv::Mat &_synapses):
    synapses(_synapses)
{
    // Nothing to do.
}

size_t Synapses::size() {
    return synapses.size().height;
}
