#ifndef RECEPTOR_HPP
#define RECEPTOR_HPP

#include <clarus/wnn/synapses.hpp>

#include <boost/function.hpp>
#include <opencv2/core/core.hpp>

#include <vector>

class Receptor {
    std::vector<Synapses> stem;

public:
    Receptor(std::vector<Synapses> stem);

    Receptor(int n, boost::function<cv::Mat()> synapses);

    template<class Samples> Samples sample(std::vector<cv::Mat> &inputs, cv::Rect &roi);
};

inline std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

template<class Samples> Samples Receptor::sample(std::vector<cv::Mat> &inputs, cv::Rect &roi) {
    Samples samples;
    for (int i = 0, d = 0, n = inputs.size(); i < n; i++) {
        Synapses &synapses = stem[i];
        cv::Mat input(inputs[i], roi);
        samples.write(d, synapses, input);
        d += synapses.size();
    }

    return samples;
}

#endif
