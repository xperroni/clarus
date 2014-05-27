#ifndef CLARUS_MODEL_CLUSTER_HPP
#define CLARUS_MODEL_CLUSTER_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

namespace clarus {
    class Cluster;

    double angle2d(const Cluster &cluster);

    double distance(const Cluster &cluster);

    double distance(const Cluster &c1, const Cluster &c2);

    Point median(const Cluster &cluster, int axis);
}

class clarus::Cluster: public List<Point> {
public:
    /*
    Default constructor.
    */
    Cluster();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Cluster();

    Cluster clone() const;

    Point center() const;

    Point closest() const;

    Point farthest() const;

    double spread() const;
};

#endif

