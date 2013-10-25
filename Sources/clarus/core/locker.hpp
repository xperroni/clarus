#ifndef CLARUS_CORE_LOCKER_HPP
#define CLARUS_CORE_LOCKER_HPP

#include <boost/thread.hpp>

template<typename T> struct locked_ptr: public boost::shared_ptr<T> {
    template<typename F, typename G> locked_ptr(T *pointer, F lock, G unlock):
        boost::shared_ptr<T>(pointer, unlock)
    {
        lock();
    };
};

class Locker {
    boost::shared_mutex locker;

    boost::condition_variable notification;

    boost::mutex waiter;

    int tally;

public:
    Locker();

    void lock();

    template<typename T> locked_ptr<T> read(T &value);

    template<typename T> locked_ptr<T> write(T &value);

    void unlock();

    void wait();

    void notify();
};

template<typename T> locked_ptr<T> Locker::read(T &value) {
    return locked_ptr<T>(
        &value,
        boost::bind(&boost::shared_mutex::lock_shared, &locker),
        boost::bind(&boost::shared_mutex::unlock_shared, &locker)
    );
}

template<typename T> locked_ptr<T> Locker::write(T &value) {
    return locked_ptr<T>(
        &value,
        boost::bind(&boost::shared_mutex::lock, &locker),
        boost::bind(&boost::shared_mutex::unlock, &locker)
    );
}

#endif
