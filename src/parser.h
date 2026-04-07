#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <type_traits>
#include "tokenizer.h"
#include "engine.h"

static const std::unordered_set<TokenType> CONDITION_VALUE_TYPES = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::STRING};
static const std::unordered_set<TokenType> CONDITION_OPERATOR_TYPES = {
    TokenType::OP_EQUAL,
    TokenType::OP_NOT_EQUAL,
    TokenType::OP_LESS,
    TokenType::OP_GREATER,
    TokenType::OP_LESS_EQUAL,
    TokenType::OP_GREATER_EQUAL
};

class NoTemplateState {
public:
    virtual ~NoTemplateState() = default;
};

template <class T> 
class State : public NoTemplateState{
private:
    const std::vector<Token>& m_tokens;
    unsigned int& m_idx;
public:
    static const TokenType BASE_TOKEN = TokenType::KW_SELECT;
    static const unsigned int MINIMUM_SIZE_QUERY = 3;
    static const TokenType ALL_COLUMNS_TOKEN = TokenType::OP_STAR;
protected:
    inline bool IsFinished(unsigned int idx) const {
        return idx >= m_tokens.size();
    }

    inline bool IsFinished() const {
        return IsFinished(m_idx);
    }

    virtual State<T>& AssertFinished() {
        if (!IsFinished())
            throw Last().GetMissingTokenException();
        return *this;
    }

    virtual State<T>& AssertNotFinished() {
        if (IsFinished())
            throw Last().GetMissingTokenException();
        return *this;
    }

    inline Token Next() {
        Token prev = Current();
        m_idx++;
        return prev;
    }

    inline Token Current() const {
        return m_tokens.at(m_idx);
    }

    inline Token Last() const {
        return m_tokens.at(m_tokens.size() - 1);
    }

    inline bool IsNextType(TokenType type) const {
        return !IsFinished(m_idx + 1) && m_tokens.at(m_idx + 1).IsType(type);
    }

public:
    using resultType = T;
    State(const std::vector<Token>& tokens, unsigned int& idx) 
        : m_tokens(tokens), m_idx(idx) {}
    
    
    virtual ~State() = default;
    virtual T Parse() = 0;
};

class ColumnsState: public State<std::vector<std::string>> {
private:
    const TokenType STATE_TERMINATOR = TokenType::KW_FROM;
public:
    using State::State;
    std::vector<std::string> Parse() {
        if (Current().IsType(ALL_COLUMNS_TOKEN)) {
            Next();
            return { toString(ALL_COLUMNS_TOKEN) };
        }

        std::vector<std::string> columns;
        bool wasLastComma = true;
        do {
            Token token = Next();
            if (wasLastComma) {
                token.AssertType(TokenType::IDENTIFIER, "non-identifier token");
                columns.push_back(token.GetValue());
                wasLastComma = false;
                continue;
            }
            token.AssertType(TokenType::COMMA);
            wasLastComma = true;
        } while (!IsFinished() && !Current().IsType(STATE_TERMINATOR));
        return columns;
    }
};

class TableState : public State<std::string> {
public:
    using State::State;
    std::string Parse() {
        Next().AssertType(TokenType::KW_FROM);
        AssertNotFinished();
        return Next().AssertType(TokenType::IDENTIFIER).GetValue();
    }
};

class ConditionState : public State<Condition> {
private:
    inline Token ParseConditionValue() {
        return Current().AssertType(CONDITION_VALUE_TYPES);
    }
    inline Token ParseOperatorValue() {
        return Current().AssertType(CONDITION_OPERATOR_TYPES);
    }
public:
    using State::State;
    
    Condition Parse() {
        Next().AssertType(TokenType::KW_WHERE);
        AssertNotFinished();
        Token lvalue = ParseConditionValue();
        Next();
        AssertNotFinished();
        Token op = ParseOperatorValue();
        Next();
        AssertNotFinished();
        Token rvalue = ParseConditionValue();
        Next();
        return Condition{lvalue, rvalue, op, nullptr, nullptr};
    }
};


template <typename... States>
class Parser {
    static_assert((std::is_convertible_v<States*, NoTemplateState*> && ...),
              "All parser types must be State or publicly derive from State");
private:
    const std::vector<Token>& m_tokens;
    unsigned int m_idx;
public:
    Parser(const std::vector<Token>& tokens)
        : m_tokens(tokens), m_idx(0) {}

    auto Parse (){
        m_idx++;
        return std::tuple<typename States::resultType...>{
            States(m_tokens, m_idx).Parse()...
        };
    }
};

using SelectParser = Parser<ColumnsState, TableState, ConditionState>;

CommandInterface* Parse(std::vector<Token> tokens) {
    if (tokens.size() == 0)
        throw NoTokensParserError();
    switch (tokens.at(0).GetType()) {
    case TokenType::KW_SELECT: {
        auto [columns, table, condition] = SelectParser(tokens).Parse();
        return new SelectCommand(table, columns, condition);
    }
    default:
        throw tokens.at(0).GetInvalidTokenException();
    }
    throw tokens.at(0).GetInvalidTokenException();
}
