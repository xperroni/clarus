#ifndef CLARUS_CORE_LOOP_HPP
#define CLARUS_CORE_LOOP_HPP

#include <clarus/core/context.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace clarus {
    class Loop;
}

class clarus::Loop: public Context {
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
};

#endif // CLARUS_CORE_LOOP_HPP
