#ifndef CLARUS_CORE_LOOP_HPP
#define CLARUS_CORE_LOOP_HPP

#include <clarus/core/context.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace clarus {
    class Loop;
}

class clarus::Loop {
    Context proxy;

    boost::shared_ptr<boost::thread> thread;

    bool on;

public:
    typedef boost::function<void(Loop &loop)> F;

    typedef boost::shared_ptr<Loop> P;

    Loop(F body, Context *parent);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Loop();

    void join();

    void stop(bool blocking = false);

    bool running() const;

    template<class T> T &get(const std::string &name);

    template<class T> T &get(const std::string &name, const T &fallback);
};

template<class T> T &clarus::Loop::get(const std::string &name) {
    boost::shared_ptr<T> value = proxy.get<T>(name);
    return *value;
}

template<class T> T &clarus::Loop::get(const std::string &name, const T &fallback) {
    boost::shared_ptr<T> value = proxy.get<T>(name, fallback);
    return *value;
}

#endif
