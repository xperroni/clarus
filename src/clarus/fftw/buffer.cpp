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

#include <clarus/fftw/buffer.hpp>

#include <fftw3.h>

namespace clarus
{

namespace fftw
{

Buffer::Memory::Memory()
{
  memory = NULL;
}

Buffer::Memory::Memory(int n)
{
  memory = fftw_alloc_real(n);
}

Buffer::Memory::~Memory()
{
  if (memory != NULL)
    fftw_free(memory);
}

Buffer::Buffer()
{
  buffer = new Memory();
}

Buffer::Buffer(int n)
{
  buffer = new Memory(n);
}

Buffer::Buffer(int m, int n)
{
  buffer = new Memory(m * n);
}

Buffer::operator double* ()
{
  return buffer->memory;
}

Buffer::operator const double* () const
{
  return buffer->memory;
}

} // namespace fftw

} // namespace clarus
