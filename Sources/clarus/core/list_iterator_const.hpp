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

#ifndef CLARUS_CORE_LIST_ITERATOR_CONST_HPP
#define CLARUS_CORE_LIST_ITERATOR_CONST_HPP

template<class T> class ListIteratorConst
{
    typename std::vector<T>::const_iterator i;

    typename std::vector<T>::const_iterator n;

public:
    ListIteratorConst();

    ListIteratorConst(const List<T> &list);

    ListIteratorConst(const std::vector<T> &list);

    bool operator == (const ListIteratorConst &that) const;

    bool operator != (const ListIteratorConst &that) const;

    const T &operator * () const;

    const T *operator -> () const;

    bool more() const;

    const T &current() const;

    const T &next();
};

template<class T> ListIteratorConst<T>::ListIteratorConst() {
    // Nothing to do.
}

template<class T> ListIteratorConst<T>::ListIteratorConst(const List<T> &list):
    i(list->begin()),
    n(list->end())
{
    // Nothing to do.
}

template<class T> ListIteratorConst<T>::ListIteratorConst(const std::vector<T> &list):
    i(list.begin()),
    n(list.end())
{
    // Nothing to do.
}

template<class T> bool ListIteratorConst<T>::operator == (const ListIteratorConst &that) const {
    return (this->i == that.i);
}

template<class T> bool ListIteratorConst<T>::operator != (const ListIteratorConst &that) const {
    return (this->i != that.i);
}

template<class T> const T &ListIteratorConst<T>::operator * () const {
    return *i;
}

template<class T> const T *ListIteratorConst<T>::operator -> () const {
    return i.operator->();
}

template<class T> bool ListIteratorConst<T>::more() const {
    return (i != n);
}

template<class T> const T &ListIteratorConst<T>::current() const {
    return *i;
}

template<class T> const T &ListIteratorConst<T>::next() {
    const T &value = *i;
    i++;
    return value;
}

#endif
