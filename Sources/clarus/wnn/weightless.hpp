#ifndef NEURON_HPP
#define NEURON_HPP

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <bitset>
#include <limits>
#include <vector>

template<class Input, class Output> class WeightlessNeuron {
    typedef std::vector<Input> Inputs;

    typedef std::vector<Output> Outputs;

    boost::shared_ptr<Inputs> inputs;

    boost::shared_ptr<Outputs> outputs;

public:
    WeightlessNeuron();

    Output& read(const Input &input);

    void write(const Input &input, const Output &output);
};

template<class Input, class Output> WeightlessNeuron<Input, Output>::WeightlessNeuron() {
    inputs.reset(new Inputs());
    outputs.reset(new Outputs());
}

template<class Input, class Output> Output &WeightlessNeuron<Input, Output>::read(const Input &input) {
    Inputs &inputs = *(this->inputs);
    Outputs &outputs = *(this->outputs);

    size_t distance = std::numeric_limits<size_t>::max();
    size_t best = 0;

    for (size_t i = 0, n = inputs.size(); i < n; i++) {
        size_t e = (inputs[i] ^ input).count();
        if (e == 0) {
            return outputs[i];
        }
        else if (e < distance) {
            distance = e;
            best = i;
        }
    }

    return outputs[best];
}

template<class Input, class Output> void WeightlessNeuron<Input, Output>::write(const Input &input, const Output &output) {
    inputs->push_back(input);
    outputs->push_back(output);
}

#endif // NEURON_HPP
