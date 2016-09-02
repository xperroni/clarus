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

Signal::Domain::C::C():
  Domain_<fftw_complex>()
{
  // Nothing to do.
}

Signal::Domain::C::C(int rows, int cols, Signal *signal):
  Domain_<fftw_complex>(rows, cols, signal)
{
  // Nothing to do.
}

inline void mul_C(fftw_complex &p, const fftw_complex &a, const fftw_complex &b)
{
  p[0] = a[0] * b[0] - a[1] * b[1];
  p[1] = a[0] * b[1] + a[1] * b[0];
}

Signal &Signal::Domain::C::mul(const Signal &a, const Signal &b)
{
  const fftw_complex *A = a.C();
  const fftw_complex *B = b.C();
  fftw_complex *P = (*this)();

  for (const fftw_complex *N = P + rows * cols; P != N;)
    mul_C(*(P++), *(A++), *(B++));

  return *signal;
}

inline void mul_C(fftw_complex &p, const fftw_complex &a, const fftw_complex &b, double c)
{
  p[0] = a[0] * b[0] - c * a[1] * b[1];
  p[1] = a[0] * b[1] + c * a[1] * b[0];
}

Signal &Signal::Domain::C::mul(const Signal &a, const Signal &b, double c)
{
  const fftw_complex *A = a.C();
  const fftw_complex *B = b.C();
  fftw_complex *P = (*this)();

  for (const fftw_complex *N = P + rows * cols; P != N;)
    mul_C(*(P++), *(A++), *(B++), c);

  return *signal;
}

inline void mul_C(fftw_complex &p, const fftw_complex &a, const fftw_complex &b, double c, double s)
{
  p[0] = (a[0] * b[0] - c * a[1] * b[1]) * s;
  p[1] = (a[0] * b[1] + c * a[1] * b[0]) * s;
}

Signal &Signal::Domain::C::mul(const Signal &a, const Signal &b, double c, double s)
{
  const fftw_complex *A = a.C();
  const fftw_complex *B = b.C();
  fftw_complex *P = (*this)();

  for (const fftw_complex *N = A + rows * cols; A != N;)
    mul_C(*(P++), *(A++), *(B++), c, s);

  return *signal;
}

Signal &Signal::Domain::C::mul(const Signal &that)
{
  return mul(*signal, that);
}

Signal &Signal::Domain::C::mul(const Signal &that, double c)
{
  return mul(*signal, that, c);
}

Signal &Signal::Domain::C::mul(const Signal &that, double c, double s)
{
  return mul(*signal, that, c, s);
}

} // namespace fftw

} // namespace clarus
