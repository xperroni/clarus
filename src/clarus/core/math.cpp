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

#include <clarus/core/math.hpp>
using clarus::List;
using clarus::ListIteratorConst;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/images.hpp>

#include <cmath>

List<double> clarus::vectorize(BinaryOp op, const List<double> &a, const List<double> &b) {
    List<double> c;
    for (ListIteratorConst<double> i(a), j(b); i.more() && j.more();) {
        double value = op(i.next(), j.next());
        c.append(value);
    }

    return c;
}

List<double> clarus::vectorize(UnaryOp op, const List<double> &a) {
    List<double> b;
    for (ListIteratorConst<double> i(a); i.more();) {
        double value = op(i.next());
        b.append(value);
    }

    return b;
}

double clarus::add(double a, double b) {
    return a + b;
}

double clarus::sub(double a, double b) {
    return a - b;
}

double clarus::mul(double a, double b) {
    return a * b;
}

double clarus::div(double a, double b) {
    return a / b;
}

double clarus::log(double x, double base) {
    return ::log(x) / ::log(base);
}

double clarus::log2(double x) {
    static double BASE2 = 1.0 / ::log(2.0);

    return ::log(x) * BASE2;
}

cv::Point clarus::argmax(const cv::Mat &data) {
    cv::Point maxLoc;
    cv::minMaxLoc(data, NULL, NULL, NULL, &maxLoc);
    return maxLoc;
}

cv::Point clarus::argmin(const cv::Mat &data) {
    cv::Point minLoc;
    cv::minMaxLoc(data, NULL, NULL, &minLoc);
    return minLoc;
}

double clarus::min(const cv::Mat &data) {
    double minVal = 0.0;
    cv::minMaxLoc(data, &minVal);
    return minVal;
}

double clarus::max(const cv::Mat &data) {
    double maxVal = 0.0;
    cv::minMaxLoc(data, NULL, &maxVal);
    return maxVal;
}

double clarus::mean(const cv::Mat &data) {
    cv::Scalar means = cv::mean(data);
    int n = data.channels();
    double total = means[0];
    for (int i = 1; i < n; i++) {
        total += means[i];
    }

    return total / ((double) n);
}

static cv::Mat collapse(const cv::Mat &data) {
    int count = data.channels();
    if (count == 1) {
        return (data.type() != CV_64F ? images::convert(data, CV_64F) : data);
    }

    cv::Mat collapsed(data.size(), CV_64F, cv::Scalar(0.0));
    if (count == 3 && data.type() == CV_8UC3) {
        double *v = (double*) collapsed.data;
        cv::Vec3b *u = (cv::Vec3b*) data.data;
        for (int i = 0, n = data.rows * data.cols; i < n; i++, u++, v++) {
            *v = (*u)[0] + (*u)[1] + (*u)[2];
        }
    }
    else {
        List<cv::Mat> channels = colors::channels(data);
        for (int i = 0, n = channels.size(); i < n; i++) {
            cv::add(collapsed, channels[i], collapsed, cv::noArray(), CV_64F);
        }
    }

    return collapsed;
}

cv::Mat clarus::mean(const cv::Mat &data, int dim) {
    cv::Mat averages;
    cv::reduce(collapse(data), averages, dim, CV_REDUCE_AVG, CV_64F);
    return averages;
}

void clarus::shift(cv::Mat &data, int rows, int cols) {
    cv::Mat transform = (cv::Mat_<double>(2, 3) << 1, 0, cols, 0, 1, rows);
    cv::warpAffine(data, data, transform, data.size());
}

cv::Mat clarus::pow(const cv::Mat &data, double power) {
    cv::Mat out;
    cv::pow(data, power, out);
    return out;
}
