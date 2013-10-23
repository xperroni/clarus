#ifndef DATA_HPP
#define DATA_HPP

#include <opencv2/opencv.hpp>

#include <vector>

template<class Sample> class Samples: public std::vector<Sample> {
    static cv::RNG rng;
public:
    Samples();

    Samples(const Samples& samples);

    Samples(int n, const Samples& samples);

    void add(const Sample &sample);
};

template<class Sample> cv::RNG Samples<Sample>::rng;

template<class Sample> Samples<Sample>::Samples() {
    // Nothing to do.
}

template<class Sample> Samples<Sample>::Samples(const Samples& samples):
    std::vector<Sample>(samples)
{
    // Nothing to do.
}

template<class Sample> Samples<Sample>::Samples(int n, const Samples& samples) {
    int m = samples.size();
    for (int i = 0; i < n; i++) {
        int index = rng(m);
        push_back(samples[index]);
    }
}

template<class Sample> void Samples<Sample>::add(const Sample &sample) {
    push_back(sample);
}

#endif
