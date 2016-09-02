/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of FFTW/CV.

FFTW/CV is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FFTW/CV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FFTW/CV. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_FFTW_COSINE_SEARCH_HPP
#define CLARUS_FFTW_COSINE_SEARCH_HPP

#include <clarus/fftw/correlate.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Template search operator using cosine similarity metric.
 */
class CosineSearch
{
  /** \brief Cross-correlation operator. */
  Correlate c;

  /** \brief Cross-correlation operator for computing the normalization factors. */
  Correlate d;

  /** \brief Normalization window. */
  cv::Mat o;

public:
  /**
   * \brief Create a new template search operator.
   *
   * \param size_a Dimensions of the template.
   *
   * \param size_b Dimensions of the searched image.
   */
  CosineSearch(const cv::Size &size_a, const cv::Size &size_b);

  /**
   * \brief Search for template `a` in image `b`.
   *
   * \return A tuple `(x, y, z)` where `x` and `y` are the top-left coordinates of
   *         the match, and `z` the matching strength as a real number in the range
   *         `[0, 1]`.
   */
  cv::Point3f operator () (const cv::Mat &a, const cv::Mat &b);
};

} // namespace fftw

} // namespace clarus

#endif
