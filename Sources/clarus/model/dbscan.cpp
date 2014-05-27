#include <clarus/model/dbscan.hpp>
using clarus::Cloud;
using clarus::Cluster;
using clarus::List;
using clarus::Point;

#include <vector>
using std::vector;

static List<int> query(const List<Point> &points, const Point &p, double e2) {
    clarus::List<int> keys;
    for (int i = 0, n = points.size(); i < n; i++) {
        const Point &q = points[i];
        double d = clarus::distance2(p, q);
        if(d <= e2) {
            keys.append(i);
        }
    }

    return keys;
}

Cloud dbscan::run(const clarus::List<Point> &points, float e, size_t b) {
    Cloud cloud;
    int n = points.size();
    vector<bool> clustered(n, false);
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
        clustered.push_back(false);
        visited.push_back(false);
    }

    // for each unvisted point P in dataset keypoints
    double e2 = e * e;
    for (int i = 0; i < n; i++) {
        if(visited[i]) {
            continue;
        }

        const Point &p = points[i];
        visited[i] = true; // mark point as visited
        clarus::List<int> neighbors = query(points, p, e2);
        if(neighbors.size() < b) {
            // Point is noise, ignore it
            continue;
        }

        //expand cluster
        Cluster &c = cloud.append();
        c.append(p); // add P to cluster c

        //for each point P' in neighborPts
        for (size_t j = 0; j < neighbors.size(); j++) {
            //if P' is not visited
            int k = neighbors[j];
            const Point &q = points[k];
            if (!visited[k]) {
                //Mark P' as visited
                visited[k] = true;
                clarus::List<int> farther = query(points, q, e2);
                if(farther.size() >= b) {
                    neighbors.extend(farther);
                }
            }

            // if q is not yet a member of any cluster
            // add q to cluster c
            if (!clustered[k]) {
                clustered[k] = true;
                c.append(q);
            }
        }
    }

    return cloud;
}
