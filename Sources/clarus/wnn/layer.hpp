#ifndef CLARUS_WNN_LAYER_HPP
#define CLARUS_WNN_LAYER_HPP

#include <clarus/core/table.hpp>

#include <boost/function.hpp>
#include <opencv2/opencv.hpp>

class WeightlessLayer{
protected:
    Table layer;

    //typedef boost::function<void(int, int, const cv::Rect&, const cv::Mat&, cv::Mat&)> Action;

    //void dispatch(Action action, const cv::Size &steps, const cv::Mat &input, cv::Mat &output);

    Table &get(int i, int j);

    Table &set(int i, int j, Table *neuron);
};

#endif // CLARUS_WNN_LAYER_HPP
