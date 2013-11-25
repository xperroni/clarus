#ifndef CLARUS_CORE_INDEX_HPP
#define CLARUS_CORE_INDEX_HPP

#include <cstddef>
#include <cstdarg>
#include <sstream>
#include <string>

template<size_t n> class Index {
    size_t indices[n];

public:
    Index(size_t i0, ...);

    size_t &operator [] (size_t i);

    const size_t &operator [] (size_t i) const;

    bool operator < (const Index &that) const;

    operator std::string() const;
};

template<size_t n> Index<n>::Index(size_t i0, ...) {
    va_list args;
    va_start(args, i0);

    indices[0] = i0;
    for (size_t i = 1; i < n; i++) {
        indices[i] = va_arg(args, int);
    }

    va_end(args);
}

template<size_t n> size_t &Index<n>::operator [] (size_t i) {
    return indices[i];
}

template<size_t n> const size_t &Index<n>::operator [] (size_t i) const {
    return indices[i];
}

template<size_t n> bool Index<n>::operator < (const Index &that) const {
    for (size_t i = 0; i < n; i++) {
        size_t a = this->indices[i];
        size_t b = that.indices[i];
        if (a < b) {
            return true;
        }
        else if (a > b) {
            return false;
        }
    }

    return false;
}

template<size_t n> Index<n>::operator std::string() const {
    std::ostringstream out;
    out << "(";
    for (size_t i = 0;;) {
        out << indices[i];
        if (++i >= n) {
            break;
        }

        out << ", ";
    }

    out << ")";

    return out.str();
}

#endif // CLARUS_CORE_INDEX_HPP
