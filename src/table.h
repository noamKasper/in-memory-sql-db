#include <iostream>
#include <variant>
#include <vector>
#include <string>

using Value = std::variant<int, std::string>;

struct Column {
    std::string name;
    enum Type { INTEGER, TEXT } type;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Value>> rows;
};

