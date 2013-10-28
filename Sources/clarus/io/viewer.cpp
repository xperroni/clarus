#include "viewer.hpp"

Viewer::Viewer(const std::string &_name):
    name(_name)
{
    // Nothing to do.
}

Viewer::~Viewer() {
    close();
}

void Viewer::view(const cv::Mat &mat) {
    cv::imshow(name, mat);
}

void Viewer::close() {
    cv::destroyWindow(name);
}
