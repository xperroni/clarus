#include <clarus/wnn/layer.hpp>

/*
void WeightlessLayer::dispatch(Action action, const cv::Size &steps, const cv::Mat &input, cv::Mat &output) {
    int r = steps.height;
    int s = steps.width;

    const cv::Size &size = input.size();
    int m = size.height / r;
    int n = size.height / s;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cv::Rect roi(j * s, i * r, s, r);
            action(i, j, roi, input, output);
        }
    }
}
*/

static std::string key(int i, int j) {
    std::ostringstream stream;
    stream << i << ", " << j;
    return stream.str();
}

Table &WeightlessLayer::get(int i, int j) {
    return layer.get<Table>(key(i, j));
}

Table &WeightlessLayer::set(int i, int j, Table *neuron) {
    return layer.set<Table>(key(i, j), neuron);
}
