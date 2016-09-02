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

#ifndef CLARUS_FFTW_PLAN_HPP
#define CLARUS_FFTW_PLAN_HPP

#include <fftw3.h>
#include <opencv2/opencv.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Wrapper for a FFTW transform plan.
 *
 * This class implements reference-count management for FFTW plan instances, as
 * well as a simplified interface for setting up and using plans. Single as well as
 * multiple transform plans are supported.
 *
 * FFTW/CV uses FFTW buffers only as temporary containers during transform
 * operations; accordingly, this class supports only in-place transforms, which
 * reduces the number of required FFTW buffers.
 *
 * Moreover, since visual data is usually represented as matrices of integer or
 * floating-point values (possibly stacked in "channels"), only 2D transforms
 * between Real spatial-domain and Complex frequency-domain data are supported. An
 * attempt was made to leave space for future Complex-to-Complex transform
 * implementations, however.
 *
 * Plans are meant to be shareable among Signal instances. Once a plan is computed
 * for a signal of given dimensions, it can be shared with other signals of the same
 * size. For example:
 *
 *     // Creates a signal of given size and computes forward transform plan.
 *     Signal a(size, FORWARD_R2C);
 *
 *     // Creates another signal of same size, shares plan.
 *     Signal b(size, a.plan);
 */
class Plan
{
  /**
   * \brief Wrapper class for a FFTW transform plan instance.
   *
   * Because FFTW plans need to be deallocated using <tt>fftw_destroy_plan()</tt>
   * instead of standard <tt>free()</tt> or \c delete operations, they cannot be
   * wrapped directly in \c cv::Ptr instances. The solution is to wrap the plan
   * instance in a class with a custom destructor, and then wrap heap-allocated
   * instances of that class in \c cv::Ptr instances.
   *
   * Moreover, because FFTW specifies different functions for new-array execution of
   * Real-data forward and backward transforms, some way to select which function to
   * call depending on the case is needed. This is solved by adding a pure virtual
   * method to the wrapper, which is implemented in subclasses responsible for
   * implementing the forward Real-to-Complex and backward Complex-to-Real cases.
   */
  struct Transform
  {
    /** \brief FFTW transform plan instance. */
    fftw_plan plan;

    /**
     * \brief Class destructor.
     */
    virtual ~Transform();

    /**
     * \brief Executes the wrapped plan on the given data buffer.
     *
     * \param buffer Pointer to the data buffer containing the transform data. Its
     * contents will be overwritten with the transform's results.
     */
    virtual void execute(double *buffer) = 0;
  };

  /**
   * \brief Wrapper class for a Real-to-Complex forward transform plan.
   */
  struct ForwardR2C: Transform
  {
    /**
     * \brief Create a new Real-to-Complex forward transform plan.
     *
     * The data buffer must be of size <tt>count * m * (n + 2)</tt> in units of \c double.
     * This is to account for the differences in number and size of Real and Complex
     * values, as explained in the [FFTW reference](http://www.fftw.org/fftw3_doc/Real_002ddata-DFT-Array-Format.html).
     *
     * \param count Number of transforms to plan, equal or greater than 1.
     *
     * \param m Transform data row count.
     *
     * \param n Transform data column count, expected to be even.
     *
     * \param S Transform data buffer pointer.
     */
    ForwardR2C(int count, int m, int n, double *S);

    /** \copydoc Transform::execute(double *buffer) */
    virtual void execute(double *buffer);
  };

  /**
   * \brief Wrapper class for a Complex-to-Real backward transform plan.
   */
  struct BackwardC2R: Transform
  {
    /**
     * \brief Create a new Complex-to-Real backward transform plan.
     *
     * \copydetails ForwardR2C::ForwardR2C(int count, int m, int n, double *S)
     */
    BackwardC2R(int count, int m, int n, double *S);

    /** \copydoc Transform::execute(double *buffer) */
    virtual void execute(double *S);
  };

  /** \brief Reference-counted transform plan. */
  cv::Ptr<Transform> transform;

  /**
   * \brief Create a new plan encapsulating the given transform.
   */
  Plan(int count, int m, int n, Transform *transform);

public:
  /** \brief Number of transforms performed by this plan. */
  int count;

  /** \brief Number of rows in each input. */
  int rows;

  /** \brief Number of columns in each input. */
  int cols;

  /**
   * \brief Default constructor.
   */
  Plan();

  /**
   * \brief Execute the planned transform on the original data buffer.
   */
  void execute();

  /**
   * \brief Execute the planned transform on the given data buffer.
   */
  void execute(double *buffer);

  /** \copydoc ForwardR2C::ForwardR2C(int count, int m, int n, double *S) */
  friend Plan FORWARD_R2C(int count, int m, int n, double *S);

  /** \copydoc BackwardC2R::BackwardC2R(int count, int m, int n, double *S) */
  friend Plan BACKWARD_C2R(int count, int m, int n, double *S);
};

/** \copydoc Plan::ForwardR2C::ForwardR2C(int count, int m, int n, double *S) */
Plan FORWARD_R2C(int count, int m, int n, double *S);

  /** \copydoc Plan::BackwardC2R::BackwardC2R(int count, int m, int n, double *S) */
Plan BACKWARD_C2R(int count, int m, int n, double *S);

/**
 * \brief Pointer class for plan instantiation functions.
 */
typedef Plan (*Planner)(int count, int m, int n, double *S);

} // namespace fftw

} // namespace clarus

#endif
