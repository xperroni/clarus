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

#include <clarus/fftw/signal.hpp>

namespace clarus
{

namespace fftw
{

Signal::Domain::R::R():
  Domain_<double>()
{
  // Nothing to do.
}

Signal::Domain::R::R(int rows, int cols, Signal *signal):
  Domain_<double>(rows, cols, signal)
{
  // Nothing to do.
}

Signal &Signal::Domain::R::mul(const Signal &a, const Signal &b)
{
  const double *a_R = a.R();
  const double *b_R = b.R();
  double *c_R = (*this)();

  int rows_I = signal->I.rows;
  int cols_I = signal->I.cols;
  int cols = signal->data.cols;
  for (int i = 0; i < rows_I; i++)
  {
    for (int j = 0; j < cols_I; j++)
    {
      int k = (i * cols) + j;
      c_R[k] = a_R[k] * b_R[k];
    }
  }

  return *signal;
}

Signal &Signal::Domain::R::mul(const Signal &that)
{
  return mul(*signal, that);
}

} // namespace fftw

} // namespace clarus
