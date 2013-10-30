#ifndef CLARUS_IO_COMMAND_HPP
#define CLARUS_IO_COMMAND_HPP

#include <boost/shared_ptr.hpp>

#include <cstdio>
#include <string>

class Command: public boost::shared_ptr<FILE> {
public:
    Command(const std::string &command, const std::string &mode);
};

class InputCommand: private Command {
public:
    InputCommand(const std::string &command);

    bool operator () (std::string &line);
};

class OutputCommand: private Command {
    bool autoflush;

public:
    OutputCommand(const std::string &command, bool autoflush = false);

    void operator () (const std::string &format, ...);

    void flush();
};

#endif // PIPE_HPP
