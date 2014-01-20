#ifndef CLARUS_MODEL_QUADTREE_HPP
#define CLARUS_MODEL_QUADTREE_HPP

#include <clarus/model/point.hpp>

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <stdexcept>
#include <string>

namespace quadtree {
    template<class T> class points;
}

template<class T> class quadtree::points {
    struct node {
        typedef std::vector<node*> Children;

        typedef std::pair<point, T> Value;

        typedef List<Value> Values;

        point::range bounds;

        Children children;

        Values values;

        node(const point::range &range);

        virtual ~node();

        bool add(const point &p, const T& value);

        void query(const point::range &range, Values &result) const;
    };

    node root;

public:
    typedef typename node::Values values;

    points(size_t in, size_t jn);

    void add(size_t i, size_t j, const T &value);

    void add(const point &p, const T &value);

    void add(const cv::Point &p, const T &value);

    List<std::pair<point, T> > query(const point::range &sought) const;
};

template<class T> quadtree::points<T>::node::node(const point::range &range):
    bounds(range)
{
    // Nothing to do.
}

template<class T> quadtree::points<T>::node::~node() {
    for (typename Children::iterator i = children.begin(), n = children.end(); i != n; ++i) {
        delete *i;
    }
}

template<class T> bool quadtree::points<T>::node::add(const point &p, const T &value) {
    if (!bounds.contains(p)) {
        return false;
    }

    if (values.size() < 4) {
        values.append(std::make_pair(p, value));
        return true;
    }

    if (children.size() == 0) {
        const point &p0 = bounds.p0;
        const point &pn = bounds.pn;
        point pk((pn[0] + p0[0]) / 2, (pn[1] + p0[1]) / 2);
        children.push_back(new node(point::range(p0[0], p0[1], pk[0], pk[1])));
        children.push_back(new node(point::range(p0[0], pk[1], pk[0], pn[1])));
        children.push_back(new node(point::range(pk[0], p0[1], pn[0], pk[1])));
        children.push_back(new node(point::range(pk[0], pk[1], pn[0], pn[1])));
    }

    for (typename Children::iterator k = children.begin(), n = children.end(); k != n; ++k) {
        node *child = *k;
        if (child->add(p, value)) {
            return true;
        }
    }

    throw std::runtime_error("Could not add value to tree at point " + (std::string) p);
}

template<class T> void quadtree::points<T>::node::query(const point::range &range, Values &result) const {
    if (!bounds.intersects(range)) {
        return;
    }

    for (ListIteratorConst<Value> i(values); i.more(); i.next()) {
        const point &p = i->first;
        if (range.contains(p)) {
            result.append(*i);
        }
    }

    for (typename Children::const_iterator i = children.begin(), n = children.end(); i != n; ++i) {
        node *child = *i;
        child->query(range, result);
    }
}

template<class T> quadtree::points<T>::points(size_t in, size_t jn):
    root(point::range(0, 0, in, jn))
{
    // Nothing to do.
}

template<class T> void quadtree::points<T>::add(size_t i, size_t j, const T&value) {
    root.add(point(i, j), value);
}

template<class T> void quadtree::points<T>::add(const point &p, const T&value) {
    root.add(p, value);
}

template<class T> void quadtree::points<T>::add(const cv::Point &p, const T &value) {
    add(p.y, p.x, value);
}

template<class T> List<std::pair<point, T> > quadtree::points<T>::query(const point::range &sought) const {
    values result;
    root.query(sought, result);
    return result;
}

#endif
