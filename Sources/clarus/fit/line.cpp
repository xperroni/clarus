#include <clarus/fit/line.hpp>

#include <cmath>

void LineModel::fit(Data &data) {
    Datum &p0 = data[0];
    Datum::first_type x0 = p0.first;
    Datum::second_type y0 = p0.second;

    Datum &p1 = data[data.size() - 1];
    Datum::first_type x1 = p1.first;
    Datum::second_type y1 = p1.second;

    a = ((y1 - y0) / (x1 - x0));
    b = y0 - x0 * ((y1 - y0) / (x1 - x0));
}

LineModel::Distance LineModel::distance(const Datum &datum) {
    Datum::first_type x = datum.first;
    Datum::second_type y = datum.second;
    return fabs(eval(x) - y);
}

LineModel::Domain LineModel::eval(const Domain &x) {
    return a * x + b;
}

