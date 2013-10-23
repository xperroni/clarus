#ifndef LINE_HPP
#define LINE_HPP

#include <clarus/fit/samples.hpp>

class LineModel {
public:
    typedef double Domain;

    typedef std::pair<Domain, Domain> Datum;

    typedef Samples<Datum> Data;

    typedef Domain Distance;

    Domain a;

    Domain b;

    void fit(Data &data);

    Distance distance(const Datum &datum);

    Domain eval(const Domain &x);
};

#endif
