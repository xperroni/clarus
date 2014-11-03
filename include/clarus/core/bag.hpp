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

#ifndef CLARUS_CORE_BAG_HPP
#define CLARUS_CORE_BAG_HPP

#include <boost/shared_ptr.hpp>

#include <map>
#include <stdexcept>
#include <string>

namespace clarus {
    class Bag;
}

/**
\brief An heterogeneous container.

A bag is a collection of string-indexed values. Contrary to most containers, values
are not restricted to belonging to the same class, not even to a common superclass.
Template methods enable values of arbitrary types to be stored to and retrieved from
the collection; the user is required to remember which type is associated to each
string key and use the proper template parameters when retrieving stored values,
though.
*/
class clarus::Bag {
    /**
    \brief Abstract base class for structures that encapsulate stored values.
    */
    struct Entry {
        /** \brief Reference-counted pointer type alias. */
        typedef boost::shared_ptr<Entry> P;

        /**
        \brief Virtual destructor. Enforces polymorphism. Do not remove.
        */
        virtual ~Entry() {
            // Do nothing.
        }

        /**
        \brief Returns a void pointer to the value encapsulated in this entry.
        */
        virtual const void *get() const = 0;
    };

    /**
    \brief Template structure for encapsulating stored values of arbitrary type.
    */
    template<class T> struct Record: public Entry {
        /** \brief Reference-counted pointer to stored value. */
        boost::shared_ptr<T> value;

        /**
        \brief Encapsulates the given pointer.

        The pointer is stored as a reference-counted smart pointer. When the reference
        count goes down to zero, the pointed object is deleted.
        */
        Record(T *pointer)
        {
            value.reset(pointer);
        }

        /**
        \brief Encapsulates the given smart pointer.

        A new smart pointer to the given object is created and stored inside this object.
        */
        Record(boost::shared_ptr<T> _value):
            value(_value)
        {
            // Nothing to do.
        }

        // See Entry::get()
        virtual const void *get() const {
            return value.get();
        }
    };

    /** \brief Map of string-indexed stored values. */
    std::map<std::string, Entry::P> entries;

public:
    /**
    \brief Default constructor.
    */
    Bag();

    /**
    \brief Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Bag();

    /**
    \brief Returns a reference to the value stored by the given name. If the name is not known,
    throws an exception.

    Be aware that this method is not thread-safe. If the record is overwritten or deleted
    by another thread, the reference returned by this method will become invalid, likely
    resulting in an application crash.
    */
    template<class T> T &fetch(const std::string &name);

    /**
    \brief Returns a reference to the value stored by the given name. If the name is not known,
    associate it to the fall-back value, the return it.

    Be aware that this method is not thread-safe. If the record is overwritten or deleted
    by another thread, the reference returned by this method will become invalid, likely
    resulting in an application crash.
    */
    template<class T> T &fetch(const std::string &name, const T &fallback);

    /**
    \brief Returns whether a value indexed by the given name is stored at this collection.
    */
    bool has(const std::string &name) const;

    /**
    \brief Returns a shared pointer to the vale indexed by the given name.

    If no value indexed by the given name is found, a runtime error is thrown.
    */
    template<class T> boost::shared_ptr<T> get(const std::string &name);

    /**
    \brief Returns a shared pointer to the vale indexed by the given name. If no value
    indexed by the given name is found, the given fall back value is stored and a smart
    pointer to it is returned.
    */
    template<class T> boost::shared_ptr<T> get(const std::string &name, const T &fallback);

    /**
    \brief Stores a copy of the given value, indexing it by the given name.

    If there was already a stored value indexed by the given name, it is discarded.
    */
    template<class T> boost::shared_ptr<T> set(const std::string &name, const T &value);

    /**
    \brief Stores the given value pointer, indexing it by the given name.

    If there was already a stored value indexed by the given name, it is discarded.
    The given pointer is stored as a reference-counted smart pointer, which means the
    pointed object will be deleted once the reference count goes to zero (e.g. if the
    value is overwritten, or this bag is deleted).
    */
    template<class T> boost::shared_ptr<T> set(const std::string &name, T *value);

    /**
    \brief Stores the given smart pointer, indexing it by the given name.

    If there was already a stored value indexed by the given name, it is discarded.
    */
    template<class T> boost::shared_ptr<T> set(const std::string &name, boost::shared_ptr<T> value);
};

template<class T> T &clarus::Bag::fetch(const std::string &name) {
    return *(get<T>(name));
}

template<class T> T &clarus::Bag::fetch(const std::string &name, const T &fallback) {
    return *(get<T>(name, fallback));
}

template<class T> boost::shared_ptr<T> clarus::Bag::get(const std::string &name) {
    Record<T> *record = dynamic_cast<Record<T>*>(entries[name].get());
    if (record == NULL) {
        throw std::runtime_error(std::string("Record \"") + name + "\" not found");
    }

    return record->value;
}

template<class T> boost::shared_ptr<T> clarus::Bag::get(const std::string &name, const T &fallback) {
    return (has(name) ? get<T>(name) : set<T>(name, fallback));
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, const T &value) {
    return set<T>(name, new T(value));
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, T *value) {
    Record<T> *record = new Record<T>(value);
    entries[name].reset(record);
    return record->value;
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, boost::shared_ptr<T> value) {
    Record<T> *record = new Record<T>(value);
    entries[name].reset(record);
    return record->value;
}

#endif
