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

#include <clarus/vision/images.hpp>

#include <clarus/core/list.hpp>
using clarus::List;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/filters.hpp>

#include <boost/format.hpp>

#include <iostream>
#include <stdexcept>

cv::Mat images::absdiff(const cv::Mat &a, const cv::Mat &b) {
    cv::Mat c;
    cv::absdiff(a, b, c);
    return c;
}

cv::Mat images::convert(const cv::Mat &image, int type) {
    cv::Mat converted;
    image.convertTo(converted, type);
    return converted;
}

inline cv::Mat reduce(const cv::Mat &diff, int type)
{
  cv::Mat out(diff.size(), type, cv::Scalar(0));

  List<cv::Mat> channels = colors::channels(diff);
  for (int i = 0, n = channels.size(); i < n; i++)
    cv::add(out, channels[i], out, cv::noArray(), type);

  return out;
}

template<class T>
inline cv::Mat reduce_(const cv::Mat &diff, int type)
{
  cv::Mat out(diff.size(), type, cv::Scalar(0));

  const cv::Vec3b *u = (const cv::Vec3b*) diff.data;
  T *v = (T*) out.data;
  for (int i = 0, n = diff.rows * diff.cols; i < n; i++, u++, v++)
    *v = (*u)[0] + (*u)[1] + (*u)[2];

  return out;
}

inline cv::Mat reduce_8u(const cv::Mat &diff)
{
  cv::Mat out(diff.size(), CV_8U, cv::Scalar(0));

  const cv::Vec3b *u = (const cv::Vec3b*) diff.data;
  uint8_t *v = (uint8_t*) out.data;
  for (int i = 0, n = diff.rows * diff.cols; i < n; i++, u++, v++)
    *v = std::min((*u)[0] + (*u)[1] + (*u)[2], 255);

  return out;
}

cv::Mat images::difference(const cv::Mat &a, const cv::Mat &b, int type)
{
  cv::Size size1 = a.size();
  cv::Size size2 = b.size();
  if (size1 != size2)
    throw std::runtime_error(
      (boost::format("a.size() %1% != b.size() %2%") % size1 % size2).str()
    );

  int c1 = a.channels();
  int c2 = b.channels();
  if (c1 != c2)
    throw std::runtime_error(
      (boost::format("a.channels() (%1%) != b.channels() (%2%)") % c1 % c2).str()
    );

  cv::Mat diff = images::absdiff(a, b);
  if (c1 == 1)
    return (diff.type() != type ? images::convert(diff, type) : diff);
  else if (c1 != 3)
    return reduce(diff, type);

  switch (type)
  {
    case CV_8U: return reduce_8u(diff);
    case CV_32S: return reduce_<int>(diff, type);
    case CV_32F: return reduce_<float>(diff, type);
    case CV_64F: return reduce_<double>(diff, type);
  }

  return reduce(diff, type);
}

cv::Mat images::integral(const cv::Mat &image)
{
  cv::Mat integrated;
  cv::integral(image, integrated, CV_64F);
  return integrated;
}

double images::sum(const cv::Mat &integral, const cv::Rect &area)
{
  int x1 = area.x;
  int y1 = area.y;
  int x2 = x1 + area.width;
  int y2 = y1 + area.height;

  double s11 = integral.at<double>(y1, x1);
  double s12 = integral.at<double>(y2, x1);
  double s21 = integral.at<double>(y1, x2);
  double s22 = integral.at<double>(y2, x2);

  return s11 + s22 - s12 - s21;
}

cv::Mat images::scale(const cv::Mat &image, int width, int interpolation)
{
  cv::Size original = image.size();
  double factor = ((double) width) / ((double) original.width);
  cv::Size size(width, factor * original.height);
  return scale(image, size, interpolation);
}

cv::Mat images::scale(const cv::Mat &image, const cv::Size &size, int interpolation) {
    cv::Mat scaled;
    cv::resize(image, scaled, size, 0, 0, interpolation);
    return scaled;
}

cv::Mat images::load(const std::string &path) {
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        throw std::ios_base::failure("Could not load \"" + path + "\": file not found");
    }

    return image;
}

cv::Mat images::normalize(const cv::Mat &image) {
    CHANNEL_WISE(images::normalize, image);

    cv::Mat normed;
    cv::normalize(image, normed, 0, 255, cv::NORM_MINMAX, CV_8U);
    return normed;
}

cv::Mat images::load(const std::string &folder, const std::string &name, const std::string &ext) {
    return load(folder + '/' + name + '.' + ext);
}

void images::save(const cv::Mat &data, const std::string &path, bool normalize) {
    cv::Mat pixels = (normalize ? colors::discrete(data) : data);

    cv::imwrite(path, pixels);
}

void images::save(
    const cv::Mat &data,
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    bool normalize
) {
    save(data, folder + '/' + name + '.' + ext, normalize);
}
