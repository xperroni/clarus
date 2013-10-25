#include <clarus/core/table.hpp>

bool Table::has(const std::string &name) {
    return (entries.count(name) > 0);
}
