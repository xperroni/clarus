#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <opencv2/opencv.hpp>

#include <string>

class Viewer {
    const std::string name;
public:
    Viewer(const std::string &name);

    virtual ~Viewer();

    void view(const cv::Mat &mat);

    void close();
};

#endif // VIEWER_HPP
