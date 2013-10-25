#ifndef CLARUS_CORE_LOOPER_HPP
#define CLARUS_CORE_LOOPER_HPP

#include <clarus/core/locker.hpp>
#include <clarus/core/table.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

class Looper {
    typedef boost::function<void(Looper&, bool&)> Body;

    struct Loop {
        bool running;

        boost::shared_ptr<boost::thread> thread;

        Loop(Body body, Looper &looper);

        void run(Body body, Looper &looper);
    };

    typedef boost::shared_ptr<Loop> LoopP;

    typedef std::deque<LoopP> Loops;

    Table table;

    Loops loops;

    Loop &peek();

    void pop();

    size_t size();

    void wipe();

public:
    // Locker object, can be used for synchronizing across tasks.
    Locker locker;

    void start(Body body);

    void stop(bool blocking = false);

    void join();

    bool has(const std::string &name);

    template<typename T> T &get(const std::string &name);

    template<typename T> T &set(const std::string &name, const T &value);

    template<typename T> T &set(const std::string &name, T *value);
};

template<typename T> T &Looper::get(const std::string &name) {
    return locker.read(table)->get<T>(name);
}

template<typename T> T &Looper::set(const std::string &name, const T &value) {
    return locker.write(table)->set<T>(name, value);
}

template<typename T> T &Looper::set(const std::string &name, T *value) {
    return locker.write(table)->set<T>(name, value);
}

#endif
