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

Signal::Signal():
  buffer(),
  data(),
  I(0, 0),
  R(0, 0, this),
  C(0, 0, this),
  plan()
{
  // Nothing to do.
}

Signal::Signal(const Signal &that)
{
  init(that);
}

Signal::Signal(const cv::Size &size, Planner planner)
{
  init(size.height, size.width);
  plan = planner(1, R.rows, R.cols, R());
  set(ZERO);
}

Signal::Signal(const cv::Size &size, Plan plan)
{
  init(size.height, size.width);
  this->plan = plan;
  set(ZERO);
}

Signal::Signal(const cv::Mat &data, Planner planner)
{
  init(data.rows, data.cols);
  plan = planner(1, R.rows, R.cols, R());
  set(data);
}

Signal::Signal(const cv::Mat &data, Plan plan)
{
  init(data.rows, data.cols);
  this->plan = plan;
  set(data);
}

Signal::Signal(const cv::Size &size, const cv::Mat &data, Planner planner)
{
  init(size.height, size.width);
  plan = planner(1, R.rows, R.cols, R());
  set(data);
}

Signal::Signal(const cv::Size &size, const cv::Mat &data, Plan plan)
{
  init(size.height, size.width);
  this->plan = plan;
  set(data);
}

Signal::Signal(int m, int n, Buffer buffer, int offset)
{
  init(m, n, buffer, offset);
  set(ZERO);
}

Signal::Signal(const cv::Size &size, Buffer buffer, int offset)
{
  init(size.height, size.width, buffer, offset);
  set(ZERO);
}

Signal &Signal::operator = (const Signal &that)
{
  init(that);
  return *this;
}

void Signal::init(const Signal &that)
{
  buffer = that.buffer;
  data = that.data;
  I = that.I;
  R = Domain::R(that.R.rows, that.R.cols, this);
  C = Domain::C(that.C.rows, that.C.cols, this);
  plan = that.plan;
}

void Signal::init(int rows, int cols, Buffer buffer, int offset)
{
  int m = optimalRowSize(rows);
  int n = optimalColSize(cols) + 2;
  if (buffer == NULL)
    buffer = Buffer(m, n);

  data = cv::Mat(m, n, CV_64F, buffer + offset * m * n);
  this->buffer = buffer;
  I.rows = rows;
  I.cols = cols;

  R = Domain::R(m, n - 2, this);
  C = Domain::C(m, n / 2, this);
}

cv::Mat Signal::toMat(bool copy)
{
  cv::Rect roi(0, 0, I.cols, I.rows);
  cv::Mat I(data, roi);
  if (!copy)
    return I;

  cv::Mat out(I.rows, I.cols, CV_64F);
  I.copyTo(out);
  return out;
}

Signal &Signal::transform()
{
  plan.execute(R());
  return *this;
}

Signal &Signal::set(const cv::Mat &values)
{
  int m = data.rows - values.rows;
  int n = data.cols - values.cols;

  CV_Assert(values.type() == CV_64F);
  CV_Assert(m >= 0 && n >= 0);

  cv::copyMakeBorder(values, data, 0, m, 0, n, cv::BORDER_CONSTANT, ZERO);

  return *this;
}

Signal &Signal::set(const cv::Scalar &value)
{
  data = value;
  return *this;
}

int optimalColSize(int cols)
{
  for (int optimal = cols;; optimal++)
  {
    optimal = cv::getOptimalDFTSize(optimal);
    if (optimal % 2 == 0)
      return optimal;
  }
}

} // namespace fftw

} // namespace clarus
