#ifndef GNUPLOT_HPP
#define GNUPLOT_HPP

#include <boost/shared_ptr.hpp>

#include <cstdio>
#include <string>

class Gnuplot {
    boost::shared_ptr<FILE> pipe;

public:
    Gnuplot();

    void operator () (const std::string &command, ...);

    void flush();
};

void plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1);

/*
Set some sensible defaults.
*/
void set_defaults(Gnuplot &gnuplot);

#endif // GNUPLOT_HPP
