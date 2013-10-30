#ifndef GNUPLOT_HPP
#define GNUPLOT_HPP

#include <clarus/io/command.hpp>

#include <cstdio>
#include <string>

class Gnuplot: public OutputCommand {
public:
    Gnuplot();
};

void plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1);

/*
Set some sensible defaults.
*/
void set_defaults(Gnuplot &gnuplot);

#endif // GNUPLOT_HPP
