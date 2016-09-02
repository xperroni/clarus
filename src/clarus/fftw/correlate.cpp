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

#include <clarus/fftw/correlate.hpp>

namespace clarus
{

namespace fftw
{

Correlate::Correlate(const cv::Size &size):
  in(2, size, FORWARD_R2C),
  out(size, BACKWARD_C2R)
{
  // Nothing to do.
}

Signal &Correlate::operator () (const cv::Mat &a, const cv::Mat &b)
{
  in[0].set(a);
  in[1].set(b);
  in.transform();
  out.C.mul(in[0], in[1], -1.0).transform();
  return out;
}

} // namespace fftw

} // namespace clarus
