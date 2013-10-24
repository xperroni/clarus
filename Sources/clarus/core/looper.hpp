#ifndef CLARUS_CORE_LOOPER_HPP
#define CLARUS_CORE_LOOPER_HPP

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

typedef boost::function<void(bool&)> Loop;

class Looper
{
    bool running;

    boost::shared_ptr<boost::thread> looper;

public:
    Looper();

    Looper(const Looper &that);

    void start(Loop loop);

    void stop(bool blocking = true);

    void join();
};

#endif
