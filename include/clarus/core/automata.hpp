/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_CORE_AUTOMATA_HPP
#define CLARUS_CORE_AUTOMATA_HPP

#include <clarus/core/bag.hpp>

#include <boost/function.hpp>

#include <map>
#include <stdexcept>
#include <string>

namespace clarus
{

/**
 * \brief A discrete state in the state machine.
 *
 * This is a solution to the function-that-returns-its-own type as described here:
 * http://stackoverflow.com/questions/12304438/how-can-i-get-a-boostfunction-or-other-generic-function-wrapper-that-returns
 */
struct State: boost::function<State(Bag&)>
{
  /** \brief A convenient alias for the base type. */
  typedef boost::function<State(Bag&)> functor;

  /**
   * \brief Default constructor.
   *
   * Required by C++'s local value initialization machinery.
   */
  State();

  /**
   * \brief Wraps a custom function into a State object.
   */
  template<class F> State(F f);
};

template<class F> State::State(F f):
  functor(f)
{
  // Nothing to do.
}

/**
 * \brief Run a state machine from the given start state, with the given context.
 */
void run(State start, Bag &context);

/**
 * \brief Special state used to signal the end of a program.
 */
State FINISH(Bag &context);

} // namespace clarus

#endif
