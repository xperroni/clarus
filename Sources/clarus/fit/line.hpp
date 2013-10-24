#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <vector>

class LineModel {
public:
    typedef double Domain;

    typedef std::pair<Domain, Domain> Datum;

    typedef std::vector<Datum> Data;

    typedef Domain Distance;

    Domain a;

    Domain b;

    Domain min_xd;

    double fitness;

    LineModel();

    LineModel(const Domain &min_xd);

    void fit(Data &data);

    Distance distance(const Datum &datum) const;

    Distance distance(const LineModel &that) const;

    Domain eval(const Domain &x) const;

    std::string cartesian();

    std::string parametric();
};

#endif
