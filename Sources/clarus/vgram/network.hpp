#ifndef CLARUS_VGRAM_NETWORK_HPP
#define CLARUS_VGRAM_NETWORK_HPP

#include <clarus/vgram/neuron.hpp>

namespace vgram {
    template<class W> class network;
}

template <class W> class vgram::network {
    typedef typename W::X X;

    typedef typename W::Y Y;

    typedef typename W::B B;

    typedef typename W::G G;

    typedef typename W::J J;

    typedef typename W::L L;

    typedef typename W::N N;

    typedef typename W::O O;

    typedef typename W::P P;

    typedef typename W::alpha alpha;

    typedef typename W::beta beta;

    typedef typename W::gamma gamma;

    typedef typename W::delta delta;

    typedef typename W::omega omega;

    P p;

    N N_z;

public:
    network(const P &p);

    O get(const X &x) const;

    void set(const X &x, const Y &y);
};

template<class W> vgram::network<W>::network(const P &_p):
    p(_p)
{
    // Nothing to do.
}

template<class W> typename W::O vgram::network<W>::get(const X &x) const {
    alpha a(p, x);
    delta d(p, N_z, x);
    omega w(p);
    for (typename delta::const_iterator di = d.begin(), dn = d.end(); di != dn; ++di) {
        const neuron<B, G> &nl = di->second;
        const L &l = di->first;
        gamma c(p, l);
        for (typename gamma::iterator ci = c.begin(), cn = c.end(); ci != cn; ++ci) {
            const J &j = *ci;
            typename neuron<B, G>::response r = nl.get(a(l, j));
            w.add(l, j, r.g, r.d);
        }
    }

    return w();
}

template<class W> void vgram::network<W>::set(const X &x, const Y &y) {
    alpha a(p, x);
    beta b(p, y);
    delta d(p, N_z, x);
    for (typename delta::iterator di = d.begin(), dn = d.end(); di != dn; ++di) {
        const L &l = di->first;
        neuron<B, G> &nl = di->second;
        gamma c(p, l);
        for (typename gamma::iterator ci = c.begin(), cn = c.end(); ci != cn; ++ci) {
            const J &j = *ci;
            nl.set(a(l, j), b(j));
        }
    }
}

#endif
