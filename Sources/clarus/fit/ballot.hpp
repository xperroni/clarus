#ifndef CLARUS_FIT_BALLOT_HPP
#define CLARUS_FIT_BALLOT_HPP

#include <map>

template<class X> class Ballot {
    typedef std::map<X, int> Votes;

    Votes votes;

public:
    void vote(const X &x);

    X winner();
};

template<class X> void Ballot<X>::vote(const X &x) {
    if (votes.count(x) == 0) {
        votes[x] = 1;
    }
    else {
        votes[x] += 1;
    }
}

template<class X> X Ballot<X>::winner() {
    X x = votes.begin()->first;
    int count = 0;

    for (typename Votes::iterator i = votes.begin(), n = votes.end(); i != n; ++i) {
        int voting = i->second;
        if (voting > count) {
            count = voting;
            x = i->first;
        }
    }

    return x;
};

#endif // CLARUS_FIT_BALLOT_HPP
