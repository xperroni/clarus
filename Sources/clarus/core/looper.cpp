#include <clarus/core/looper.hpp>

Looper::Looper():
    running(false)
{
    looper.reset();
}

Looper::Looper(const Looper &that):
    running(that.running),
    looper(that.looper)
{
    // Nothing to do.
}

static void body(Loop loop, bool &running) {
    loop(running);
    running = false;
}

void Looper::start(Loop loop) {
    if (running) {
        stop();
    }

    running = true;
    looper.reset(new boost::thread(boost::bind(body, loop, boost::ref(running))));
}

void Looper::stop(bool blocking) {
    if (!running) {
        return;
    }

    running = false;

    if (blocking) {
        looper->join();
    }
}

void Looper::join() {
    looper->join();
}
