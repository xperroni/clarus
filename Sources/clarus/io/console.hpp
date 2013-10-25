#ifndef CLARUS_IO_CONSOLE_HPP
#define CLARUS_IO_CONSOLE_HPP

#include <cstdio>
#include <string>

class Console {
    FILE *pipe;

public:
    Console(const std::string &title);

    virtual ~Console();

    void operator () (const std::string &format, ...);
};

#endif
