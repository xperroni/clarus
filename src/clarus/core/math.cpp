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
#include <clarus/core/matrix.hpp>
#include <clarus/vision/colors.hpp>
#include <clarus/vision/images.hpp>

#include <cmath>

namespace clarus
{

List<double> vectorize(BinaryOp op, const List<double> &a, const List<double> &b)
{
  List<double> c;
  for (ListIteratorConst<double> i(a), j(b); i.more() && j.more();)
  {
    double value = op(i.next(), j.next());
    c.append(value);
  }

  return c;
}

List<double> vectorize(UnaryOp op, const List<double> &a)
{
  List<double> b;
  for (ListIteratorConst<double> i(a); i.more();)
  {
    double value = op(i.next());
    b.append(value);
  }

  return b;
}

cv::Mat vectorize(BinaryOp op, const cv::Mat &a, const cv::Mat &b)
{
  cv::Mat c(a.size(), a.type());

  double *a_ptr = (double*) a.data;
  double *b_ptr = (double*) b.data;
  double *c_ptr = (double*) c.data;

  for (int i = 0, n = a.rows * a.cols; i < n; i++, a_ptr++, b_ptr++, c_ptr++)
    *c_ptr = op(*a_ptr, *b_ptr);

  return c;
}

cv::Mat vectorize(UnaryOp op, const cv::Mat &a)
{
  cv::Mat b(a.size(), a.type());

  double *a_ptr = (double*) a.data;
  double *b_ptr = (double*) b.data;

  for (int i = 0, n = a.rows * a.cols; i < n; i++, a_ptr++, b_ptr++)
    *b_ptr = op(*a_ptr);

  return b;
}

double add(double a, double b)
{
  return a + b;
}

double sub(double a, double b)
{
  return a - b;
}

double mul(double a, double b)
{
  return a * b;
}

double div(double a, double b)
{
  return a / b;
}

cv::Mat cos(const cv::Mat &values)
{
  return vectorize(::cos, values);
}

cv::Mat sin(const cv::Mat &values)
{
  return vectorize(::sin, values);
}

cv::Mat exp(const cv::Mat &data)
{
  if (data.channels() == 2)
  {
    List<cv::Mat> channels = split(data);
    return exp(channels[0], channels[1]);
  }

  cv::Mat exp_data;
  cv::exp(data, exp_data);
  return exp_data;
}

cv::Mat exp(const cv::Mat &real, const cv::Mat &imag)
{
  cv::Mat e = exp(real);
  cv::Mat r = e.mul(cos(imag));
  cv::Mat i = e.mul(sin(imag));
  return merge((List<cv::Mat>(), r, i));
}

cv::Mat log(const cv::Mat &data)
{
  cv::Mat log_data;
  cv::log(data, log_data);
  return log_data;
}

double log(double x, double base)
{
  return ::log(x) / ::log(base);
}

double log2(double x)
{
  static double BASE2 = 1.0 / ::log(2.0);

  return ::log(x) * BASE2;
}

cv::Point argmax(const cv::Mat &data)
{
  cv::Point maxLoc;
  cv::minMaxLoc(data, NULL, NULL, NULL, &maxLoc);
  return maxLoc;
}

cv::Point argmin(const cv::Mat &data)
{
  cv::Point minLoc;
  cv::minMaxLoc(data, NULL, NULL, &minLoc);
  return minLoc;
}

double min(const cv::Mat &data)
{
  double minVal = 0.0;
  cv::minMaxLoc(data, &minVal);
  return minVal;
}

double max(const cv::Mat &data)
{
  double maxVal = 0.0;
  cv::minMaxLoc(data, NULL, &maxVal);
  return maxVal;
}

double mean(const cv::Mat &data)
{
  cv::Scalar means = cv::mean(data);
  int n = data.channels();
  double total = means[0];
  for (int i = 1; i < n; i++)
    total += means[i];

  return total / ((double) n);
}

static cv::Mat collapse(const cv::Mat &data)
{
  int count = data.channels();
  if (count == 1)
    return (data.type() != CV_64F ? images::convert(data, CV_64F) : data);

  cv::Mat collapsed(data.size(), CV_64F, cv::Scalar(0.0));
  if (count == 3 && data.type() == CV_8UC3)
  {
    double *v = (double*) collapsed.data;
    cv::Vec3b *u = (cv::Vec3b*) data.data;
    for (int i = 0, n = data.rows * data.cols; i < n; i++, u++, v++)
      *v = (*u)[0] + (*u)[1] + (*u)[2];
  }
  else
  {
    List<cv::Mat> channels = colors::channels(data);
    for (int i = 0, n = channels.size(); i < n; i++)
      cv::add(collapsed, channels[i], collapsed, cv::noArray(), CV_64F);
  }

  return collapsed;
}

cv::Mat mean(const cv::Mat &data, int dim)
{
  cv::Mat averages;
  cv::reduce(collapse(data), averages, dim, CV_REDUCE_AVG, CV_64F);
  return averages;
}

void shift(cv::Mat &data, int rows, int cols)
{
  cv::Mat transform = (cv::Mat_<double>(2, 3) << 1, 0, cols, 0, 1, rows);
  cv::warpAffine(data, data, transform, data.size());
}

cv::Mat pow(const cv::Mat &data, double power)
{
  cv::Mat out;
  cv::pow(data, power, out);
  return out;
}

} // namespace clarus