/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_VGRAM_OUTPUT_HPP
#define CLARUS_VGRAM_OUTPUT_HPP

#include <boost/shared_ptr.hpp>

#include <limits>

namespace vgram {
    template<class T> class output;
}

template<class T> class vgram::output {
    boost::shared_ptr<T> t;

    size_t d;

public:
    output();

    output(boost::shared_ptr<T> t, size_t d);

    output(const T &t, size_t d);

    bool empty() const;

    const T &value() const;

    size_t error() const;
};

template<class T> vgram::output<T>::output():
    t(),
    d(std::numeric_limits<size_t>::max())
{
    // Nothing to do.
}

template<class T> vgram::output<T>::output(boost::shared_ptr<T> _t, size_t _d):
    t(_t),
    d(_d)
{
    // Nothing to do.
}

template<class T> vgram::output<T>::output(const T &_t, size_t _d):
    t(new T(_t)),
    d(_d)
{
    // Nothing to do.
}

template<class T> bool vgram::output<T>::empty() const {
    return (t.get() == NULL);
}

template<class T> const T &vgram::output<T>::value() const {
    return *t;
}

template<class T> size_t vgram::output<T>::error() const {
    return d;
}

#endif
