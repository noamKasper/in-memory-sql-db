#include <string>
#include <vector>
#include <optional>
#include "tokenizer.h"


struct Condition {
    Token lvalue;
    Token rvalue;
    Token conditionOperator;
    Token* logicalOperator; // NOT, AND, OR ...
    Condition* subCondition;
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
    std::optional<Condition> m_conditions;
public:
    SelectCommand(std::string tableName, std::vector<std::string> columns, std::optional<Condition> conditions = std::nullopt)
        : m_tableName(tableName), m_columns(columns), m_conditions(conditions) {
    }

    std::string Execute() override {
        std::string result = m_tableName + "\n";
        for (const std::string& column : m_columns)
            result += column + ", ";
        if (m_conditions.has_value())
            result += '\n' + m_conditions->lvalue.GetValue() + m_conditions->conditionOperator.GetValue() + m_conditions->rvalue.GetValue();
        return result;
    }
};
