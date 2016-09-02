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

#ifndef CLARUS_FFTW_SIGNAL_HPP
#define CLARUS_FFTW_SIGNAL_HPP

#include <clarus/core/math.hpp>
#include <clarus/fftw/buffer.hpp>
#include <clarus/fftw/plan.hpp>

namespace clarus
{

namespace fftw
{

/**
 * \brief Wrapper combining a DFT transform and a buffer for temporary data storage.
 *
 * The Signal class provides a simple interface between OpenCV and FFTW transform
 * calls. It combines an encapsulated 16-bit aligned FFTW buffer and convenient
 * methods for writing
 * <a href="http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat">Mat</a>
 * data to it, performing in-place transforms, and writing results back to
 * <tt>Mat</tt> objects.
 */
class Signal
{
  /**
   * \brief Information about Signal input data.
   */
  struct Inputs
  {
    /** \brief Input data row count. */
    int rows;

    /** \brief Input data column count. */
    int cols;

    /**
     * \brief Default constructor.
     */
    Inputs()
    {
      rows = 0;
      cols = 0;
    }

    /**
     * \brief Create a new specification for an input of given dimensions.
     */
    Inputs(int rows, int cols)
    {
      this->rows = rows;
      this->cols = cols;
    }
  };

  /**
   * \brief %Domain-dependent Signal data manipulation facilities.
   *
   * Signal data can be stored as either Real or Complex values. Domain subclasses
   * provide a convenient API for accessing and manipulating data in either format.
   */
  struct Domain
  {
    /**
     * \brief Basic numeric domain superclass.
     *
     * This class provides the groundwork for presenting Signal data in different
     * numeric domains. Subclasses provide the template parameter and convenient
     * operations.
     */
    template<class T>
    struct Domain_
    {
      /** \brief Row count in the target domain. */
      int rows;

      /** \brief Column count in the target domain. */
      int cols;

      /**
       * \brief Default constructor.
       */
      Domain_();

      /**
       * \brief Create a new domain interface for the given signal and target domain dimensions.
       */
      Domain_(int rows, int cols, Signal *signal);

      /**
       * \brief Pointer to buffer data typecast to the target domain.
       */
      const T *operator () () const;

      /**
       * \brief Non-const version of <tt>operator ()</tt>.
       */
      T *operator () ();

    protected:
      /** \brief Pointer to the interfaced Signal object. */
      Signal *signal;
    };

    /**
     * \brief Real domain interface class.
     *
     * This class provides access to Signal data in the Real domain as \c double
     * values.
     */
    struct R: Domain_<double>
    {
      /**
       * \brief Default constructor.
       */
      R();

      /**
       * \brief Create a new Real domain interface for the given signal and data dimensions.
       */
      R(int rows, int cols, Signal *signal);

      /**
       * \brief Perform element-wise product between the given signals in the Real domain.
       *
       * The product result is stored in the encapsulated Signal object.
       */
      Signal &mul(const Signal &a, const Signal &b);

      /**
       * \brief Perform element-wise product between the encapsulated and the given signals.
       *
       * The product result is stored in the encapsulated Signal object.
       */
      Signal &mul(const Signal &that);
    };

    /**
     * \brief Complex domain interface class.
     *
     * This class provides access to Signal data in the Complex domain as \c fftw_complex
     * values.
     */
    struct C: Domain_<fftw_complex>
    {
      /**
       * \brief Default constructor.
       */
      C();

      /**
       * \brief Create a new Complex domain interface for the given signal and data dimensions.
       */
      C(int rows, int cols, Signal *signal);

      /**
       * \brief Perform element-wise product between the given signals in the Complex domain.
       *
       * The product result is stored in the encapsulated Signal object.
       */
      Signal &mul(const Signal &a, const Signal &b);

      /**
       * \copydoc mul(const Signal&, const Signal&)
       *
       * To compute the element-wise product of the complex conjugate of \c a by \c b,
       * set \c c to \c -1.
       */
      Signal &mul(const Signal &a, const Signal &b, double c);

      /**
       * \copydoc mul(const Signal&, const Signal&, double)
       *
       * The multiplication factor \c s is applied to every element in the result. When
       * computing a element-wise product in the frequency domain it can be used to
       * scale the result.
       */
      Signal &mul(const Signal &a, const Signal &b, double c, double s);

      /**
       * \brief Perform element-wise product between the encapsulated and the given signals in the Complex domain.
       *
       * The encapsulated Signal object's buffer is overwritten with the product result.
       */
      Signal &mul(const Signal &that);

      /**
       * \copydoc mul(const Signal&)
       *
       * To compute the element-wise product of the complex conjugate of the
       * encapsulated Signal by \c that, set \c c to \c -1.
       */
      Signal &mul(const Signal &that, double c);

      /**
       * \copydoc mul(const Signal&, double)
       *
       * The multiplication factor \c s is applied to every element in the result. When
       * computing a element-wise product in the frequency domain it can be used to
       * scale the result.
       */
      Signal &mul(const Signal &that, double c, double s);
    };
  };

  /** \brief Reference-counted FFTW buffer instance. */
  Buffer buffer;

  /**
   * \brief Initialize this Signal with data from another Signal.
   *
   * No data is copied, only references.
   */
  void init(const Signal &that);

  /**
   * \brief Initialize this Signal with the given arguments.
   *
   * If the Buffer is empty, a new Buffer is allocated.
   */
  void init(int rows, int cols, Buffer buffer = Buffer(), int offset = 0);

public:
  /** \brief Matrix object holding the signal's data. */
  cv::Mat data;

  /** \brief Input data specs. */
  Inputs I;

  /** \brief Data interface for the Real domain. */
  Domain::R R;

  /** \brief Data interface for the Complex domain. */
  Domain::C C;

  /** \brief DFT transform plan. */
  Plan plan;

  /**
   * \brief Default constructor.
   */
  Signal();

  /**
   * \brief Copy constructor.
   */
  Signal(const Signal &that);

  /**
   * \brief Creates a new signal of given input size and computes a new transform plan for it.
   *
   * The internal buffer is filled with zeros on initialization.
   *
   * Because FFTW handles some buffer sizes better than others (see the
   * <a href="http://www.fftw.org/fftw3_doc/Real_002ddata-DFTs.html">reference</a>),
   * Signal may allocate a buffer larger than the given size.
   */
  Signal(const cv::Size &size, Planner planner);

  /**
   * \brief Creates a new signal of given input size and assigns it the given transform plan.
   *
   * \copydetails Signal(const cv::Size &size, Planner planner)
   */
  Signal(const cv::Size &size, Plan plan);

  /**
   * \brief Creates a new signal with the given input and computes a new transform plan for it.
   *
   * Because FFTW handles some buffer sizes better than others (see the
   * <a href="http://www.fftw.org/fftw3_doc/Real_002ddata-DFTs.html">reference</a>),
   * Signal may allocate a buffer larger than the given size.
   *
   * During initialization the buffer is set to the contents of the given
   * <a href="http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat">Mat</a>
   * object. If the buffer is larger than the object's dimensions, the remaining
   * space is filled with zeros.
   */
  Signal(const cv::Mat &data, Planner planner);

  /**
   * \brief Creates a new signal with the given input and assigns it the given transform plan.
   *
   * \copydetails Signal(const cv::Mat &data, Planner planner)
   */
  Signal(const cv::Mat &data, Plan plan);

  /**
   * \brief Creates a new signal of given input size, assigns it the given data, and computes a new transform plan for it.
   *
   * The input size is expected to be equal or larger than the assigned data.
   *
   * \copydetails Signal(const cv::Mat &data, Planner planner)
   *
   */
  Signal(const cv::Size &size, const cv::Mat &data, Planner planner);

  /**
   * \brief Creates a new signal of given input size and assigns it the given data and transform plan.
   *
   * \copydetails Signal(const cv::Size &size, const cv::Mat &data, Planner planner)
   */
  Signal(const cv::Size &size, const cv::Mat &data, Plan plan);

  /**
   * \brief Creates a new signal of given input size, wrapping the given buffer and pointing at the given offset.
   *
   * This constructor is used when sharing a single FFTW buffer across several Signal
   * wrappers is required. It's used in the Signals class, which simplifies the use of
   * the "multiple transform" API's provided by FFTW, which make possible to perform
   * multiple DFT's of equal dimensions in a single call, which is faster than
   * performing each transform separately.
   */
  Signal(int m, int n, Buffer buffer, int offset);

  /**
   * \brief Creates a new signal of given input size, wrapping the given buffer and pointing at the given offset.
   *
   * This constructor is used when sharing a single FFTW buffer across several Signal
   * wrappers is required. It's used in the Signals class, which simplifies the use of
   * the "multiple transform" API's provided by FFTW, which make possible to perform
   * multiple DFT's of equal dimensions in a single call, which is faster than
   * performing each transform separately.
   */
  Signal(const cv::Size &size, Buffer buffer, int offset);

  /**
   * \brief Assignment operator.
   */
  Signal &operator = (const Signal &that);

  /**
   * \brief Return the signal data wrapped in a \c Mat object.
   *
   * If <tt>copy == true</tt> the data is copied to a \c Mat object with its own
   * (reference-counted) buffer; otherwise, it merely points to the Signal object's
   * internal buffer. Clearly, the latter is faster, while the former is safer.
   */
  cv::Mat toMat(bool copy = false);

  /**
   * \brief Execute the planned Fourier transform.
   */
  Signal &transform();

  /**
   * \brief Set the internal buffer to the contents of the given matrix.
   *
   * If the FFTW buffer is larger than the matrix, the remaining cells are filled
   * with zeros.
   *
   * This function does not update the Signal's input dimensions, even though the
   * dimensions of the given matrix may differ from the input specification's.
   */
  Signal &set(const cv::Mat &values);

  /**
   * \brief Fills the internal buffer with the given value.
   */
  Signal &set(const cv::Scalar &value);
};

template<class T>
Signal::Domain::Domain_<T>::Domain_():
  rows(0),
  cols(0),
  signal(NULL)
{
  // Nothing to do.
}

template<class T>
Signal::Domain::Domain_<T>::Domain_(int rows, int cols, Signal *signal)
{
  this->rows = rows;
  this->cols = cols;
  this->signal = signal;
}

template<class T>
const T *Signal::Domain::Domain_<T>::operator () () const
{
  return (const T*) signal->data.data;
}

template<class T>
T *Signal::Domain::Domain_<T>::operator () ()
{
  return (T*) signal->data.data;
}

/**
  * \brief Compute the optimal column count for the FFT transform.
  *
  * Because signal objects are used to store data in both real and complex domains,
  * it's important that an even column count is enforced. This function calls
  * `cv::getOptimalDFTSize()` and checks if the returned number is even; if it's not,
  * the function is called again for the next optimal value.
  */
int optimalColSize(int cols);

/**
 * \brief Compute the optimal row count for the FFT transform.
 *
 * This is just an alias for `cv::getOptimalDFTSize()`.
 */
inline int optimalRowSize(int rows)
{
  return cv::getOptimalDFTSize(rows);
}

} // namespace fftw

} // namespace clarus

#endif
