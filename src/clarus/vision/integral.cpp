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

#include <clarus/vision/integral.hpp>

namespace clarus
{

Integral::Integral()
{
  // Nothing to do.
}

Integral::Integral(const cv::Mat &image)
{
  cv::integral(image, integral, CV_64F);
  cv::integral(image.mul(image), integral2, CV_64F);
}

Integral::Integral(const cv::Mat &image, bool compute_integral, bool compute_integral2)
{
  if (compute_integral)
    cv::integral(image, integral, CV_64F);

  if (compute_integral2)
    cv::integral(image.mul(image), integral2, CV_64F);
}

bool Integral::empty() const
{
  return (integral.empty() && integral2.empty());
}

double Integral::magnitude(const cv::Rect roi) const
{
  return ::sqrt(sum2(roi));
}

double Integral::magnitude(int i0, int j0, int rows, int cols) const
{
  return ::sqrt(sum2(i0, j0, rows, cols));
}

double Integral::mean(const cv::Rect roi) const
{
  double n = roi.width * roi.height;
  return sum(roi) / n;
}

double Integral::mean(int i0, int j0, int rows, int cols) const
{
  double n = rows * cols;
  return sum(i0, j0, rows, cols) / n;
}

double Integral::mean2(const cv::Rect roi) const
{
  double n = roi.width * roi.height;
  return sum2(roi) / n;
}

double Integral::mean2(int i0, int j0, int rows, int cols) const
{
  double n = rows * cols;
  return sum2(i0, j0, rows, cols) / n;
}

double Integral::sum(const cv::Rect roi) const
{
  int i0 = roi.y;
  int j0 = roi.x;
  int in = i0 + roi.height;
  int jn = j0 + roi.height;

  return
    integral.at<double>(i0, j0) + integral.at<double>(in, jn) -
    integral.at<double>(i0, jn) - integral.at<double>(in, j0);
}

double Integral::sum(int i0, int j0, int rows, int cols) const
{
  int in = i0 + rows;
  int jn = j0 + cols;

  return
    integral.at<double>(i0, j0) + integral.at<double>(in, jn) -
    integral.at<double>(i0, jn) - integral.at<double>(in, j0);
}

double Integral::sum2(const cv::Rect roi) const
{
  int i0 = roi.y;
  int j0 = roi.x;
  int in = i0 + roi.height;
  int jn = j0 + roi.height;

  return
    integral2.at<double>(i0, j0) + integral2.at<double>(in, jn) -
    integral2.at<double>(i0, jn) - integral2.at<double>(in, j0);
}

double Integral::sum2(int i0, int j0, int rows, int cols) const
{
  int in = i0 + rows;
  int jn = j0 + cols;

  return
    integral2.at<double>(i0, j0) + integral2.at<double>(in, jn) -
    integral2.at<double>(i0, jn) - integral2.at<double>(in, j0);
}

double Integral::standardDeviation(const cv::Rect roi) const
{
  double m = mean(roi);
  double m2 = mean2(roi);
  return ::sqrt(m2 - m * m);
}

double Integral::standardDeviation(int i0, int j0, int rows, int cols) const
{
  double m = mean(i0, j0, rows, cols);
  double m2 = mean2(i0, j0, rows, cols);
  return ::sqrt(m2 - m * m);
}

} // namespace clarus
