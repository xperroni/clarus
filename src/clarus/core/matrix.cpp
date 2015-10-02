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

#include <clarus/core/matrix.hpp>

namespace clarus
{

cv::Mat merge(const List<cv::Mat> &channels)
{
  cv::Mat merged;
  cv::merge(*channels, merged);
  return merged;
}

List<cv::Mat> split(const cv::Mat &data)
{
  List<cv::Mat> channels;
  cv::split(data, *channels);
  return channels;
}

} // namespace clarus
