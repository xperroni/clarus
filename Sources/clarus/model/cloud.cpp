#include <clarus/model/cloud.hpp>
using clarus::distance2;
using clarus::Cloud;
using clarus::Cluster;
using clarus::ListIteratorConst;

Cloud::Cloud():
    List<Cluster>()
{
    // Nothing to do.
}

Cloud::~Cloud() {
    // Nothing to do.
}

const Cluster &Cloud::closest() const {
    double d = std::numeric_limits<double>::max();
    double j = 0;
    for (int i = 0, n = size(); i < n; i++) {
        double e = distance(at(i));
        if (e < d) {
            d = e;
            j = i;
        }
    }

    return at(j);
}

static bool no_close_matches(const clarus::Point &p, const clarus::Cloud &b, double t2) {
    for (ListIteratorConst<Cluster> j(b); j.more();) {
        const Cluster &v = j.next();
        if (distance2(p, v.center()) <= t2) {
            return false;
        }
    }

    return true;
}

Cloud clarus::difference(const Cloud &a, const Cloud &b, double t) {
    Cloud c;
    double t2 = t * t;
    for (ListIteratorConst<Cluster> i(a); i.more();) {
        const Cluster &u = i.next();
        if (no_close_matches(u.center(), b, t2)) {
            c.append(u);
        }
    }

    return c;
}
