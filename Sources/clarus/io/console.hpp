#ifndef CLARUS_IO_CONSOLE_HPP
#define CLARUS_IO_CONSOLE_HPP

#include <clarus/io/command.hpp>

#include <cstdio>
#include <string>

class Console: public OutputCommand {
public:
    Console(const std::string &title);
};

#endif
