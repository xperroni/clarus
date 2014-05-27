#ifndef CLARUS_MODEL_DBSCAN_HPP
#define CLARUS_MODEL_DBSCAN_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/cloud.hpp>

namespace dbscan {
    clarus::Cloud run(const clarus::List<clarus::Point> &points, float e, size_t b);
};

#endif
