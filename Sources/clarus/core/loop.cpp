#include <clarus/core/loop.hpp>
using clarus::Loop;

static void run(Loop::F body, Loop &loop, bool &on) {
    body(loop);
    on = false;
}

Loop::Loop(F body, Context *parent):
    Context(parent),
    on(true)
{
    thread.reset(
        new boost::thread(run, body, boost::ref(*this), boost::ref(this->on))
    );
}

Loop::~Loop() {
    // Nothing to do.
}

void Loop::join() {
    thread->join();
}

void Loop::stop(bool blocking) {
    if (!on) {
        return;
    }

    on = false;
    if (blocking) {
        thread->join();
    }
}

bool Loop::running() const {
    return on;
}
