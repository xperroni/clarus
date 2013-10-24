#include <clarus/fit/line.hpp>

#include <cmath>
#include <limits>
#include <sstream>

LineModel::LineModel():
    a(NAN),
    b(NAN),
    min_xd(1.0)
{
}

LineModel::LineModel(const Domain &_min_xd):
    a(NAN),
    b(NAN),
    min_xd(_min_xd)
{
}

void LineModel::fit(Data &data) {
    Datum &p0 = data[0];
    Datum::first_type x0 = p0.first;
    Datum::second_type y0 = p0.second;

    Datum &p1 = data[data.size() - 1];
    Datum::first_type x1 = p1.first;
    Datum::second_type y1 = p1.second;

    if (fabs(x1 - x0) < min_xd) {
        a = NAN;
        b = x0;
    }
    else {
        a = (y1 - y0) / (x1 - x0);
        b = y0 - x0 * a;
    }
}

LineModel::Distance LineModel::distance(const Datum &datum) const {
    Datum::first_type x = datum.first;
    Datum::second_type y = datum.second;

    if (isnan(a)) {
        return fabs(x - b);
    }
    else {
        return fabs(eval(x) - y);
    }
}

LineModel::Distance LineModel::distance(const LineModel &that) const {
    bool v_this = isnan(this->a);
    bool v_that = isnan(that.a);

    if (v_this && v_that) {
        return fabs(this->b - that.b);
    }

    if (v_this || v_that) {
        return std::numeric_limits<Distance>::max();
    }

    return (fabs(this->a - that.a) + fabs(this->b - that.b)) / 2;
}

LineModel::Domain LineModel::eval(const Domain &x) const {
    return a * x + b;
}

std::string LineModel::cartesian() {
    std::ostringstream stream;
    stream << b << " + x * " << a;
    return stream.str();
}

std::string LineModel::parametric() {
    std::ostringstream stream;
    if (isnan(a)) {
        stream << b << ",t";
    }
    else {
        stream << "t," << b << " + t * " << a;
    }

    return stream.str();
}

