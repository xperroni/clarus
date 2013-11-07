#include <clarus/vision/depths.hpp>

#include <fstream>

cv::Mat depths::load(const std::string &path, const cv::Size &size) {
    std::ifstream file(path.c_str());
    cv::Mat depths(size, CV_32F, -1.0);
    for (int i = 0, m = size.height; i < m; i++) {
        for (int j = 0, n = size.width; j < n; j++) {
            file >> depths.at<Depth>(i, j);
        }
    }

    return depths;
}

void depths::save(const std::string &path, const cv::Mat &depths) {
    std::ofstream file(path.c_str());
    const cv::Size &sizes = depths.size();
    for (int i = 0, m = sizes.height; i < m; i++) {
        for (int j = 0, n = sizes.width;;) {
            file << depths.at<Depth>(i, j);
            if (++j >= n) {
                break;
            }

            file << " ";
        }

        file << std::endl;
    }
}

cv::Mat depths::bgr(const cv::Mat &depths, const cv::Size &scale) {
    cv::Mat h;
    cv::log(depths, h);
    cv::normalize(h, h, 0, 127, CV_MINMAX);

    const cv::Size &size = depths.size();
    cv::Mat channels[] = {
        cv::Mat_<uchar>(h),
        cv::Mat(size, CV_8U, cv::Scalar(127)),
        cv::Mat(size, CV_8U, cv::Scalar(255))
    };

    cv::Mat hls;
    merge(channels, 3, hls);

    cv::Mat rgb;
    cv::cvtColor(hls, rgb, CV_HLS2BGR);
    if (scale.width < 1 || scale.height < 1) {
        return rgb;
    }

    cv::Mat scaled;
    cv::resize(rgb, scaled, scale);
    return scaled;
}
