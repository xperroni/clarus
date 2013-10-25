#include <clarus/io/console.hpp>

#include <boost/filesystem.hpp>

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdarg>
#include <stdexcept>

namespace fs = boost::filesystem;

Console::Console(const std::string &title) {
    std::string path = (fs::temp_directory_path() / fs::unique_path()).native();

    int status = mkfifo(path.c_str(), 0666);
    if (status != 0) {
        throw std::runtime_error("Could not create named pipe \"" + path + '"');
    }

    std::string command = std::string("xterm -title '") + title + "' -e 'cat " + path + "' &";
    system(command.c_str());

    pipe = fopen(path.c_str(), "w");
}

Console::~Console() {
    fclose(pipe);
}

void Console::operator () (const std::string &format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(pipe, (format + '\n').c_str(), args);
    fflush(pipe);
    va_end(args);
}
