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

#ifndef CLARUS_FFTW_SIGNALS_HPP
#define CLARUS_FFTW_SIGNALS_HPP

#include <clarus/fftw/signal.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Interface for performing multiple DFT's in a single call.
 *
 * This class provides simplified access to the FFTW API's for perforning multiple
 * DFT's in a single call. This is usually faster than performing several DFT's
 * separately.
 *
 * At construction time, a number of Signal objects are created, all pointing to
 * different offsets within a single FFTW buffer. These instances are used to fill
 * in input data and access transform outputs.
 */
class Signals
{
  /** \brief Reference-counted FFTW buffer instance. */
  Buffer buffer;

  /** \brief Vector of Signal wrappers pointing to contiguous offsets on the FFTW buffer. */
  std::vector<Signal> signals;

public:
  /** \brief DFT transform plan. */
  Plan plan;

  /**
   * \brief Default constructor.
   */
  Signals();

  /**
   * \brief Create a new batch transform wrapper.
   *
   * \param count Number of Signal objects to instantiate.
   *
   * \param size Dimensions of each individual Signal instance.
   *
   * \param planner Planner used to compute a transform plan for the Signal set.
   */
  Signals(int count, const cv::Size &size, Planner planner);

  /**
   * \brief Create a new batch transform wrapper.
   *
   * \param plan Transform plan for the Signal set.
   */
  Signals(Plan plan);

  /**
   * \brief Return a reference to the <tt>index</tt>-th Signal instance.
   */
  const Signal &operator [] (int index) const;

  /**
   * \brief Non-const version of <tt>operator []</tt>.
   */
  Signal &operator [] (int index);

  /**
   * \brief Perform the planned transform on the encapsulated data.
   */
  Signals &transform();

  /**
   * \brief Return the number of signals.
   */
  size_t size() const;
};

} // namespace fftw

} // namespace clarus

#endif
