#ifndef CLARUS_MODEL_POINT_HPP
#define CLARUS_MODEL_POINT_HPP

#include <clarus/core/list.hpp>
#include <clarus/core/math.hpp>

#include <opencv2/opencv.hpp>

#include <iostream>

namespace clarus {
    class Point;

    /*
    Computes the angle of the line segment passing through the given point and the origin.
    */
    double angle2d(const Point &point);

    /*
    Computes the angle of the line segment passing through two points.
    */
    double angle2d(const Point &p0, const Point &p1);

    /*
    Returns the rectangle bounds of a point list.
    */
    cv::Rect bounds2d(const List<Point> &points);

    /*
    Computes the point's distance from the origin.
    */
    double distance(const Point &p0);

    /*
    Computes the distance between two points.
    */
    double distance(const Point &p0, const Point &p1);

    /*
    Computes the point's squared distance from the origin.
    */
    double distance2(const Point &p0);

    /*
    Computes the squared distance between two points.
    */
    double distance2(const Point &p0, const Point &p1);

    bool inrange(const Point &pt, const Point &p0, const Point &pn);

    Point mean(const List<Point> &points);

    /*
    Creates a new point at the origin of the space of given dimension.
    */
    Point Point0(size_t dimension);

    /*
    Creates a new 2D point.
    */
    Point Point2D(double x0, double x1);

    /*
    Creates a new 2D point.
    */
    Point Point2D(const cv::Point2f &p);

    /*
    Creates a new 2D point.
    */
    List<Point> Point2D(const List<cv::Point2f> &p);

    /*
    Creates a new 3D point.
    */
    Point Point3D(double x0, double x1, double x2);

    /*
    Converts a 2D point to an OpenCV point.
    */
    cv::Point2f PointCV(const Point &p);

    /*
    Applies the given operation to each value pair of the two input points,
    producing the output point.
    */
    Point vectorize(DoubleOp op, const Point &a, const Point &b);
}

/*
A point in n-dimensional cartesian space.
*/
class clarus::Point {
    /* Point coordinates. */
    List<double> point;

public:
    /*
    Creates a new point of dimension 0.
    */
    Point();

    /*
    Custom copy-constructor.
    */
    Point(const Point &that);

    /*
    Creates a new point of given dimension.
    */
    Point(size_t dimensions, ...);

    /*
    Creates a new point fromthe given list.
    */
    Point(const List<double> &point);

    /*
    Creates a new point at the average position between two others.
    */
    Point(const Point &p0, const Point &p1);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Point();

    const List<double> &operator * () const;

    double &operator [] (int index);

    const double &operator [] (int index) const;

    bool operator == (const Point &that) const;

    bool operator != (const Point &that) const;

    Point &operator = (Point that);

    Point &operator += (const Point &that);

    Point operator + (const Point &that) const;

    Point operator - (const Point &that) const;

    Point operator * (const Point &that) const;

    Point operator * (double v) const;

    Point operator / (const Point &that) const;

    Point operator / (double v) const;

    size_t dimension() const;

    friend Point vectorize(DoubleOp op, const Point &a, const Point &b);
};

std::ostream &operator << (std::ostream &out, const clarus::Point &point);

#endif
