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

#include <clarus/vgram/intstring.hpp>

#include <clarus/core/list.hpp>
using clarus::List;
using clarus::ListIteratorConst;

#include <clarus/core/types.hpp>

#include <stdexcept>

vgram::intstring::intstring():
    List<long>()
{
    // Nothing to do.
}

vgram::intstring::intstring(size_t size):
    List<long>(size)
{
    // Nothing to do.
}

size_t vgram::intstring::distance(const intstring &that) const {
    if (size() != that.size()) {
        throw std::runtime_error(
            "List sizes differ: " + types::to_string(size()) + " != " + types::to_string(that.size())
        );
    }

    size_t d = 0;
    for (ListIteratorConst<long> i(*this), j(that); i.more();) {
        long a = i.next();
        long b = j.next();
        d += labs(a - b);
    }

    return d;
}
