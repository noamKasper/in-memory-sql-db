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

class Cursor {
private:
    const std::vector<Token>& m_tokens;
    unsigned int m_idx;
public:
    static const TokenType BASE_TOKEN = TokenType::KW_SELECT;
    static const unsigned int MINIMUM_SIZE_QUERY = 3;
public:
    inline bool IsFinished(unsigned int idx) const {
        return idx >= m_tokens.size();
    }

    inline bool IsFinished() const {
        return IsFinished(m_idx);
    }

    Cursor& AssertFinished() {
        if (!IsFinished())
            throw Last().GetMissingTokenException();
        return *this;
    }

    Cursor& AssertNotFinished() {
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
    Cursor(const std::vector<Token>& tokens, unsigned int idx) 
        : m_tokens(tokens), m_idx(idx) {}
};

class ColumnsState {
private:
    Cursor& m_cursor;
    static const TokenType STATE_TERMINATOR = TokenType::KW_FROM;
    static const TokenType ALL_COLUMNS_TOKEN = TokenType::OP_STAR;
public:
    ColumnsState(Cursor& cursor)
        : m_cursor(cursor) {}

    std::vector<std::string> Parse() {
        if (m_cursor.Current().IsType(ALL_COLUMNS_TOKEN)) {
            m_cursor.Next();
            return { toString(ALL_COLUMNS_TOKEN) };
        }

        std::vector<std::string> columns;
        bool wasLastComma = true;
        do {
            Token token = m_cursor.Next();
            if (wasLastComma) {
                token.AssertType(TokenType::IDENTIFIER, "non-identifier token");
                columns.push_back(token.GetValue());
                wasLastComma = false;
                continue;
            }
            token.AssertType(TokenType::COMMA);
            wasLastComma = true;
        } while (!m_cursor.IsFinished() && !m_cursor.Current().IsType(STATE_TERMINATOR));
        return columns;
    }
};

class TableState {
private:
    Cursor& m_cursor;
public:
    TableState(Cursor& cursor) 
        : m_cursor(cursor) {}

    std::string Parse() {
        m_cursor.Next().AssertType(TokenType::KW_FROM);
        m_cursor.AssertNotFinished();
        return m_cursor.Next().AssertType(TokenType::IDENTIFIER).GetValue();
    }
};

class ConditionState {
private:
    Cursor& m_cursor;
private:
    inline Token ParseConditionValue() {
        return m_cursor.Current().AssertType(CONDITION_VALUE_TYPES);
    }
    inline Token ParseOperatorValue() {
        return m_cursor.Current().AssertType(CONDITION_OPERATOR_TYPES);
    }
public:
    ConditionState(Cursor& cursor) 
        : m_cursor(cursor) {}

    Condition Parse() {
        m_cursor.Next().AssertType(TokenType::KW_WHERE);
        m_cursor.AssertNotFinished();
        Token lvalue = ParseConditionValue();
        m_cursor.Next();
        m_cursor.AssertNotFinished();
        Token op = ParseOperatorValue();
        m_cursor.Next();
        m_cursor.AssertNotFinished();
        Token rvalue = ParseConditionValue();
        m_cursor.Next();
        return Condition{lvalue, rvalue, op, nullptr, nullptr};
    }
};


template <typename, typename = void>
struct has_parse : std::false_type {};

template <typename T>
struct has_parse<T, std::void_t<decltype(std::declval<T&>().Parse())>>
    : std::true_type {};

template <typename... States>
class Parser {
    static_assert((has_parse<States>::value && ...),  "All types must provide a Parse() member function");
private:
    Cursor m_cursor;
public:
    Parser(const std::vector<Token>& tokens)
        : m_cursor(tokens, 1) {}

    auto Parse (){
        return std::tuple{
            States(m_cursor).Parse()...
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
