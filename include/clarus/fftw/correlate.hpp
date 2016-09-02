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

#ifndef CLARUS_FFTW_CORRELATE_HPP
#define CLARUS_FFTW_CORRELATE_HPP

#include <clarus/fftw/signals.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Circular cross-correlation operator.
 *
 * This class implements the circular cross-correlation operator for a "filtered"
 * input of a given size. The "filter" input can be of any size up to that of the
 * "filtered" input.
 */
class Correlate
{
  /** \brief Input data. */
  Signals in;

  /** \brief Output data. */
  Signal out;

public:
  /**
   * \brief Create a new operator for a "filtered" input of given dimensions.
   */
  Correlate(const cv::Size &size);

  /**
   * \brief Apply the operator to the given matrices.
   *
   * \param a "Filter" input.
   *
   * \param b "Filtered" input
   *
   * \return Reference to the Signal instance holding the result of the cross-correlation of b by a.
   */
  Signal &operator () (const cv::Mat &a, const cv::Mat &b);
};

} // namespace fftw

} // namespace clarus

#endif
