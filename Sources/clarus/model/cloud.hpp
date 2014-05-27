#ifndef CLARUS_MODEL_CLOUD_HPP
#define CLARUS_MODEL_CLOUD_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/cluster.hpp>

namespace clarus {
    class Cloud;

    /*
    Returns the list of clusters in A whose closest cluster in B is at a
    minimum length T.
    */
    Cloud difference(const Cloud &a, const Cloud &b, double t);
}

class clarus::Cloud: public List<Cluster> {
public:
    /*
    Default constructor.
    */
    Cloud();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Cloud();

    const Cluster &closest() const;
};

#endif
