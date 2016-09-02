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

#ifndef CLARUS_CORE_BUCKET_SORT_HPP
#define CLARUS_CORE_BUCKET_SORT_HPP

#include <clarus/core/list.hpp>

#include <boost/function.hpp>

namespace clarus
{

namespace bucket_sort
{

template<class T>
class Buckets
{
  int size_;

  List<List<T> > buckets_;

public:
  Buckets(int size);

  List<T> &operator [] (int index);

  void add(int key, const T &value);

  void clear(int index);

  void clearAll();

  void clearAll(int index);

  int size();
};

template<class T>
Buckets<T>::Buckets(int size):
  size_(size),
  buckets_(size)
{
  // Nothing to do.
}

template<class T>
List<T> &Buckets<T>::operator [] (int index)
{
  return buckets_[index];
}

template<class T>
void Buckets<T>::add(int key, const T &value)
{
  if (key == 0)
    return;

  int k = size_ - key;
  buckets_[k].append(value);
}

template<class T>
void Buckets<T>::clear(int index)
{
  buckets_[index].clear();
}

template<class T>
void Buckets<T>::clearAll()
{
  clearAll(0);
}

template<class T>
void Buckets<T>::clearAll(int index)
{
  for(int i = index, n = size();i < n; i++)
    buckets_[i].clear();
}

template<class T>
int Buckets<T>::size()
{
  return size_;
}

template<class T>
class Sink
{
  Buckets<T> buckets_;

  int i_;

  int j_;

public:
  Sink(Buckets<T> &buckets);

  virtual ~Sink();

  const T &operator () ();

  /**
   * \brief Move to the next valid element.
   *
   * Returns whether a valid element was found.
   */
  bool drain();
};

template<class T>
Sink<T>::Sink(Buckets<T> &buckets):
  buckets_(buckets),
  i_(0),
  j_(-1)
{
  // Nothing to do.
}

template<class T>
Sink<T>::~Sink()
{
  buckets_.clearAll(i_);
}

template<class T>
const T &Sink<T>::operator () ()
{
  return buckets_[i_][j_];
}

template<class T>
bool Sink<T>::drain()
{
  j_++;

  // Skips empty buckets
  for (int n = buckets_.size(); i_ < n; i_++)
  {
    List<T> &bucket = buckets_[i_];
    if (j_ < bucket.size())
      return true;

    bucket.clear();
    j_ = 0;
  }

  return false;
}

} // namespace bucket_sort


} // namespace clarus

#endif
