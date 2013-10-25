#include "gnuplot.hpp"

#include <cstdarg>
#include <fstream>

Gnuplot::Gnuplot():
    pipe(popen("gnuplot", "w"), pclose)
{
    // Nothing to do.
}

void Gnuplot::operator () (const std::string &command, ...) {
    va_list args;
    va_start(args, command);
    vfprintf(pipe.get(), (command + '\n').c_str(), args);
    va_end(args);
}

void Gnuplot::flush() {
    fflush(pipe.get());
}

void set_defaults(Gnuplot &gnuplot) {
    gnuplot("set terminal x11");   /* drawing destination */
    gnuplot("set grid");  /* draw grid */
    gnuplot("set mouse");  /* use mouse */
    gnuplot("set xlabel \"x\"");  /* label of x-axis */
    gnuplot("set ylabel \"y\"");  /* label of y-axis */
    gnuplot("set trange [-10:10]");  /* range of function parameter t */
    gnuplot("set xrange [-10:10]");  /* range of x-axis */
    gnuplot("set yrange [-10:10]");  /* range of y-axis */
    gnuplot("set size ratio -1");  /* aspect ratio */
    gnuplot("unset key");  /* hide graph legends */
}

void plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1) {
    gnuplot("plot '-' using %d:%d with points notitle", c0, c1);

    std::ifstream file(path.c_str());
    for (std::string line; std::getline(file, line) && line != "";) {
        gnuplot(line);
    }

    gnuplot("e");
    gnuplot.flush();
}
