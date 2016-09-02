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

#include <clarus/fftw/signals.hpp>

namespace clarus
{

namespace fftw
{

Signals::Signals()
{
  // Nothing to do.
}

Signals::Signals(int count, const cv::Size &size, Planner planner)
{
  int m = optimalRowSize(size.height);
  int n = optimalColSize(size.width);

  buffer = Buffer(count * m * (n + 2));
  plan = planner(count, m, n, buffer);

  for (int i = 0; i < count; i++)
    signals.push_back(Signal(size, buffer, i));
}

Signals::Signals(Plan plan)
{
  this->plan = plan;
  int count = plan.count;
  int m = plan.rows;
  int n = plan.cols;

  buffer = Buffer(count * m * (n + 2));
  for (int i = 0; i < count; i++)
    signals.push_back(Signal(m, n, buffer, i));
}

Signal &Signals::operator [] (int index)
{
  return signals.at(index);
}

const Signal &Signals::operator [] (int index) const
{
  return signals.at(index);
}

Signals &Signals::transform()
{
  plan.execute();
  return *this;
}

size_t Signals::size() const
{
  return signals.size();
}

} // namespace fftw

} // namespace clarus
