#include <clarus/vision/fourier.hpp>

cv::Size fourier::fit(const cv::Size &size) {
    return cv::Size(
        cv::getOptimalDFTSize(size.width),
        cv::getOptimalDFTSize(size.height)
    );
}

cv::Mat fourier::transform(const cv::Mat &data, const cv::Size &optimal) {
    cv::Size size = optimal;
    if (size.width == 0 || size.height == 0) {
        size = fit(data.size());
    }

    cv::Mat padded;
    int m = size.height - data.rows;
    int n = size.width - data.cols;
    cv::copyMakeBorder(data, padded, 0, m, 0, n, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv:: Mat plane[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};

    cv::Mat fourier;
    cv::merge(plane, 2, fourier);
    cv::dft(fourier, fourier, cv::DFT_COMPLEX_OUTPUT);

    return fourier;
}

cv::Mat fourier::inverse(const cv::Mat &fourier, const cv::Size &optimal) {
    cv::Size size = optimal;
    if (size.width == 0 || size.height == 0) {
        size = fourier.size();
    }

    cv::Mat real;
    cv::dft(fourier, real, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

    return real(cv::Rect(0, 0, size.width, size.height));
}

cv::Mat fourier::bgr(const cv::Mat &fourier) {
    const cv::Size &size = fourier.size();
    cv::Mat hls(size, CV_8UC3);

    std::vector<cv::Mat> plane;
    cv::split(fourier, plane);

    cv::Mat mag;
    cv::magnitude(plane[0], plane[1], mag);
    mag += cv::Scalar::all(1);
    cv::log(mag, mag);
    cv::normalize(mag, mag, 127, 255, CV_MINMAX);

    cv::Mat pha;
    cv::phase(plane[1], plane[0], pha);
    cv::normalize(pha, pha, 0, 127, CV_MINMAX);

    for (int i = 0, m = size.height; i < m; i++) {
        for (int j = 0, n = size.width; j < n; j++) {
            cv::Vec3b &pixel = hls.at<cv::Vec3b>(i, j);
            pixel[0] = (uchar) pha.at<float>(i, j);
            pixel[1] = (uchar) mag.at<float>(i, j);
            pixel[2] = 255;
        }
    }

    cv::Mat rgb;
    cv::cvtColor(hls, rgb, CV_HLS2BGR);
    return rgb;
}
