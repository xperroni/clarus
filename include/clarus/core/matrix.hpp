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

#ifndef CLARUS_CORE_MATRIX_HPP
#define CLARUS_CORE_MATRIX_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

namespace clarus
{

/**
 * \brief Merges the given list of single-channel matrices into a single multi-channel matrix.
 */
cv::Mat merge(const List<cv::Mat> &channels);

/**
 * \brief Splits the given multi-channel matrix into a list of single-channel matrices.
 */
List<cv::Mat> split(const cv::Mat &data);

} // namespace clarus

#endif
