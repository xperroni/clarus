#ifndef CLARUS_MODEL_CIRCLE_HPP
#define CLARUS_MODEL_CIRCLE_HPP

#include <string>
#include <vector>

class CircleModel
{
public:
    typedef double Domain;

    typedef std::pair<Domain, Domain> Datum;

    typedef std::vector<Datum> Data;

    typedef Domain Distance;

    Domain x0;

    Domain y0;

    Domain r1;

    Domain r2;

    double fitness;

    CircleModel();

    CircleModel(double x, double y, double r);

    void fit(Data &data);

    Distance distance(const Datum &datum) const;

    Distance distance(const CircleModel &that) const;

    std::string parametric();
};

#endif
