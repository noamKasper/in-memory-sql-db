#include <string>
#include <vector>
#include "tokenizer.h"

struct SubCondition;

struct Condition {
    SubCondition* subCondition;
    Token conditionOperator;
};

struct SubCondition {
    Token logicalOperator;
    Condition* condition;
};

class CommandInterface {
public:
    virtual std::string Execute() = 0;
    virtual ~CommandInterface() = default;
};

class TablesCommand : public CommandInterface {
public:
    std::string Execute() override {
        return "Tables";
    }
};

class QuitCommand : public CommandInterface {
public:
    std::string Execute() override {
        return "Quit";
    }
};

class SelectCommand : public CommandInterface {
private:
    std::string m_tableName;
    std::vector<std::string> m_columns;
    std::vector<Condition> m_conditions;
public:
    SelectCommand(std::string tableName, std::vector<std::string> columns, std::vector<Condition> conditions)
        : m_tableName(tableName), m_columns(columns), m_conditions(conditions) {
    }

    SelectCommand(std::string tableName, std::vector<std::string> columns)
        : m_tableName(tableName), m_columns(columns), m_conditions() {
    }

    std::string Execute() override {
        return "Yippiee";
    }
};
