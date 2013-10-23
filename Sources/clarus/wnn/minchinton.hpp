#ifndef MINCHINTON_HPP
#define MINCHINTON_HPP

#include <clarus/wnn/synapses.hpp>

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <bitset>

template<size_t n, class T> class Minchinton {
    typedef std::bitset<n> Data;

    boost::shared_ptr<Data> data;

public:
    Minchinton();

    Minchinton operator ^ (const Minchinton &that) const;

    void write(size_t offset, Synapses &synapses, cv::Mat &input);

    size_t count() const;
};

template<size_t n, class T> Minchinton<n, T>::Minchinton():
    data(new Data())
{
    // Nothing to do.
}

template<size_t n, class T> void Minchinton<n, T>::write(size_t offset, Synapses &synapses, cv::Mat &input) {
    size_t last = synapses.size() - 1;
    for (size_t k = 0; k < last; k++) {
        T a = synapses.sample<T>(input, k);
        T b = synapses.sample<T>(input, k + 1);
        Data &data = *(this->data);
        data.set(offset + k, a > b);
    }

    data->set(offset + last, synapses.sample<T>(input, last) > synapses.sample<T>(input, 0));
}

template<size_t n, class T> Minchinton<n, T> Minchinton<n, T>::operator ^ (const Minchinton<n, T> &that) const {
    Minchinton<n, T> other;
    *(other.data) = *(this->data) ^ *(that.data);
    return other;
}

template<size_t n, class T> size_t Minchinton<n, T>::count() const {
    return data->count();
}

#endif
