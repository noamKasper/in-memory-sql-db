#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include "tokenizer.h"
#include "engine.h"

static const std::unordered_set<TokenType> CONDITION_VALUE_TYPES = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::STRING};
enum class State { COLUMNS, TABLE, CONDITION, END };

State& operator++(State& s) {
    if (s == State::END)
        return s;
    return s = static_cast<State>(static_cast<int>(s) + 1);
}

class SelectParser {
private:
    std::vector<Token> m_tokens;
    unsigned int m_idx;
    State m_state;
public:
    static const TokenType BASE_TOKEN = TokenType::KW_SELECT;
    static const unsigned int MINIMUM_SIZE_QUERY = 3;
    static const TokenType ALL_COLUMNS_TOKEN = TokenType::OP_STAR;
private:
    inline bool IsFinished(unsigned int idx) const {
        return idx >= m_tokens.size();
    }

    inline bool IsFinished() const {
        return IsFinished(m_idx);
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

    std::unordered_set<TokenType> StateTerminators() {
        switch (m_state) {
        case State::COLUMNS: return { TokenType::KW_FROM };
        case State::TABLE: return { TokenType::KW_WHERE, TokenType::SEMICOLON };
        default: return { TokenType::SEMICOLON };
        }
    }
    
    std::vector<std::string> ParseColumns() {
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
        } while (!IsFinished() && !Current().IsType(StateTerminators()));
        return columns;
    }

    std::string ParseTable() {
        return Next().AssertType(TokenType::IDENTIFIER).GetValue();
    }
        Token ParseConditionValue() {
            return Current().AssertType(CONDITION_VALUE_TYPES);
    }
    Condition ParseCondition() {
        Token lvalue = ParseConditionValue();

        Token operator
    }
public:
    SelectParser(std::vector<Token> tokens)
        : m_tokens(tokens), m_idx(0), m_state(State::COLUMNS) {
    }

    CommandInterface* Parse() {
        if (m_tokens.size() < MINIMUM_SIZE_QUERY)
            throw Last().GetMissingTokenException();

        Next().AssertType(BASE_TOKEN);
        auto columns = ParseColumns();
        if (columns.size() == 0)
            throw Current().GetMissingTokenException();

        if (IsFinished())
            throw Last().GetMissingTokenException();
        Next().AssertType(StateTerminators());
        if (IsFinished())
            throw Last().GetMissingTokenException();

        ++m_state;
        auto table = ParseTable();
        if (IsFinished())
            throw Last().GetMissingTokenException();
        Next().AssertType(StateTerminators());

        if (IsFinished() || Current().IsType(TokenType::SEMICOLON))
            return new SelectCommand(table, columns);
        return new SelectCommand(table, columns); // debug purposes
    }
};

CommandInterface* Parser(std::vector<Token> tokens) {
    if (tokens.size() == 0)
        throw NoTokensParserError();
    switch (tokens.at(0).GetType()) {
    case TokenType::KW_SELECT:
        return SelectParser(tokens).Parse();
    default:
        throw tokens.at(0).GetInvalidTokenException();
    }
    throw tokens.at(0).GetInvalidTokenException();
}
