#ifndef CLARUS_CORE_TABLE_HPP
#define CLARUS_CORE_TABLE_HPP

#include <boost/shared_ptr.hpp>

#include <map>
#include <stdexcept>
#include <string>

struct Entry {
    // Added a virtual destructor just to ensure the class is polymorphic.
    virtual ~Entry() {
        // Do nothing.
    }
};

template<typename T> struct Record: public Entry {
    boost::shared_ptr<T> value;

    Record(T *pointer)
    {
        value.reset(pointer);
    };
};

class Table {
    std::map<std::string, boost::shared_ptr<Entry> > entries;

public:
    bool has(const std::string &name);

    template<typename T> T &get(const std::string &name);

    template<typename T> T &set(const std::string &name, const T &value);

    template<typename T> T &set(const std::string &name, T *value);
};

template<typename T> T &Table::get(const std::string &name) {
    Record<T> *record = dynamic_cast<Record<T>*>(entries[name].get());
    if (record == NULL) {
        throw std::runtime_error(std::string("Record \"") + name + "\" not found");
    }

    return *(record->value);
}

template<typename T> T &Table::set(const std::string &name, const T &value) {
    return set<T>(name, new T(value));
}

template<typename T> T &Table::set(const std::string &name, T *value) {
    Record<T> *record = new Record<T>(value);
    entries[name].reset(record);
    return *(record->value);
}

#endif
