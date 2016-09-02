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

#ifndef CLARUS_VISION_INTEGRAL_HPP
#define CLARUS_VISION_INTEGRAL_HPP

#include <opencv2/opencv.hpp>

#include <string>

namespace clarus
{

/**
 * \brief Integral image and common operations.
 */
struct Integral
{
  /** \brief Summed area table of the image. */
  cv::Mat integral;

  /** \brief Summed area table of the element-wise squared image. */
  cv::Mat integral2;

  /**
   * \brief Default constructor.
   */
  Integral();

  /**
   * \brief Create a new integral image from the given `CV_64F` image.
   */
  Integral(const cv::Mat &image);

  /**
   * \brief Create a new integral image from the given `CV_64F` image.
   *
   * This version of the constructor enables either the single or squared integral to be computed separately.
   */
  Integral(const cv::Mat &image, bool compute_integral, bool compute_integral2);

  /**
   * \brief Return whether the integral image is empty.
   */
  bool empty() const;

  /**
   * \brief Return the image magnitude over the given area.
   */
  double magnitude(const cv::Rect roi) const;

  /**
   * \brief Return the image magnitude over the given area.
   */
  double magnitude(int i0, int j0, int rows, int cols) const;

  /**
   * \brief Return the image mean over the given area.
   */
  double mean(const cv::Rect roi) const;

  /**
   * \brief Return the image mean over the given area.
   */
  double mean(int i0, int j0, int rows, int cols) const;

  /**
   * \brief Return the squared image mean over the given area.
   */
  double mean2(const cv::Rect roi) const;

  /**
   * \brief Return the squared image mean over the given area.
   */
  double mean2(int i0, int j0, int rows, int cols) const;

  /**
   * \brief Return the image sum over the given area.
   */
  double sum(const cv::Rect roi) const;

  /**
   * \brief Return the image sum over the given area.
   */
  double sum(int i0, int j0, int rows, int cols) const;

  /**
   * \brief Return the squared image sum over the given area.
   */
  double sum2(const cv::Rect roi) const;

  /**
   * \brief Return the squared image sum over the given area.
   */
  double sum2(int i0, int j0, int rows, int cols) const;

  /**
   * \brief Return the image standard deviation over the given area.
   */
  double standardDeviation(const cv::Rect roi) const;

  /**
   * \brief Return the image standard deviation over the given area.
   */
  double standardDeviation(int i0, int j0, int rows, int cols) const;
};

} // namespace clarus

#endif
