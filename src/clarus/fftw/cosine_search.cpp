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

#include <clarus/fftw/cosine_search.hpp>

namespace clarus
{

namespace fftw
{

CosineSearch::CosineSearch(const cv::Size &size_a, const cv::Size &size_b):
  c(size_b),
  d(size_b),
  o(size_a, CV_64F, ONE)
{
  // Nothing to do.
}

cv::Point3f CosineSearch::operator () (const cv::Mat &a, const cv::Mat &b)
{
  Signal &ab = c(a, b);
  double *ab_R = ab.R();

  Signal &ob = d(o, b.mul(b));
  double *ob_R = ob.R();

  int i_s, j_s;
  float r = 0;
  int rows_s = b.rows - a.rows + 1;
  int cols_s = b.cols - a.cols + 1;
  int cols = ab.data.cols;
  for (int i = 0; i < rows_s; i++)
  {
    for (int j = 0; j < cols_s; j++)
    {
      int ij = i * cols + j;
      float v = ab_R[ij] / sqrt(ob_R[ij]);
      if (r < v)
      {
        r = v;
        i_s = i;
        j_s = j;
      }
    }
  }

  return cv::Point3f(j_s, i_s, r);
}

} // namespace fftw

} // namespace clarus
