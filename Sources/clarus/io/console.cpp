#include <clarus/io/console.hpp>

Console::Console(const std::string &title):
    OutputCommand("xterm -title '" + title + "' -e 'cat %s'", true)
{
    // Nothing to do.
}
