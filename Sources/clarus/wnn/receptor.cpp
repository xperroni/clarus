#include <clarus/wnn/receptor.hpp>

Receptor::Receptor(std::vector<Synapses> _stem):
    stem(_stem)
{
    // Nothing to do.
}

Receptor::Receptor(int n, boost::function<cv::Mat()> synapses) {
    for (int i = 0; i < n; i++) {
        stem.push_back(Synapses(synapses()));
    }
}
