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

#ifndef CLARUS_MODEL_BALLOT_DISCRETE_HPP
#define CLARUS_MODEL_BALLOT_DISCRETE_HPP

#include <map>

namespace ballot {
    template<class X> class discrete;
}

template<class X> class ballot::discrete {
    typedef std::map<X, int> Votes;

    Votes votes;

public:
    void vote(const X &x);

    X winner();
};

template<class X> void ballot::discrete<X>::vote(const X &x) {
    if (votes.count(x) == 0) {
        votes[x] = 1;
    }
    else {
        votes[x] += 1;
    }
}

template<class X> X ballot::discrete<X>::winner() {
    X x = votes.begin()->first;
    int count = 0;

    for (typename Votes::iterator i = votes.begin(), n = votes.end(); i != n; ++i) {
        int voting = i->second;
        if (voting > count) {
            count = voting;
            x = i->first;
        }
    }

    return x;
}

#endif
