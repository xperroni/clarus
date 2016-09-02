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

#ifndef CLARUS_FFTW_BUFFER_HPP
#define CLARUS_FFTW_BUFFER_HPP

#include <opencv2/opencv.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Reference-counted FFTW memory buffer.
 *
 * This class encapsulates a 16-bit aligned memory buffer allocated using the FFTW
 * API. It keeps track of references to the buffer, automatically deallocating it
 * as the last reference is erased.
 */
class Buffer
{
  /**
   * \brief Wrapper class for a FFTW memory buffer.
   *
   * Because FFTW buffers need to be deallocated using <tt>fftw_free()</tt> instead of
   * standard functions, they cannot be wrapped directly in \c cv::Ptr instances. The
   * solution is to wrap the buffer pointer in a class with a custom destructor, and
   * then wrap heap-allocated instances of that class in \c cv::Ptr instances.
   */
  struct Memory
  {
    /** \brief Pointer to allocated memory buffer. */
    double *memory;

    /**
     * \brief Default class constructor.
     */
    Memory();

    /**
     * \brief Creates a new memory buffer of given length.
     *
     * \param n Length of the buffer, measured in units of \c double.
     */
    Memory(int n);

    /**
     * \brief Class destructor.
     */
    ~Memory();
  };

  /** \brief Reference-counted memory buffer. */
  cv::Ptr<Memory> buffer;

public:
  /**
   * \brief Default class constructor.
   */
  Buffer();

  /**
   * \brief Creates a new buffer of given length.
   *
   * \param n Buffer length, measured in units of \c double.
   */
  Buffer(int n);

  /**
   * \brief Creates a new buffer of given dimensions.
   *
   * The buffer will have a total length of <tt>(m * n)</tt> in units of \c double.
   *
   * \param m Buffer row count, measured in units of \c double.
   *
   * \param n Buffer column count, measured in units of \c double.
   */
  Buffer(int m, int n);

  /**
   * \brief Returns a pointer to the wrapped FFTW buffer.
   *
   * Care must be taken when using this operator, for the buffer it points to may be
   * deallocated once the \c Buffer object from which it was obtained falls out of
   * scope. in general it's a bad idea to keep it beyond a calling context &ndash;
   * copy the \c Buffer wrapper instead.
   */
  operator const double* () const;

  /** \brief Non-const version of the \c double* type conversion operator. */
  operator double* ();
};

} // namespace fftw

} // namespace clarus

#endif
