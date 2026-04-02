#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include "tokenizer.h"
#include "engine.h"

class NoTokensParserError : public std::exception {
public:
    const char* what() const noexcept override {
        return "The command contains no tokens";
    }
};

class InvalidTokenException : public std::exception {
private:
    Token m_token;
    std::string m_message;
public:
    InvalidTokenException(Token token)
        : m_token(token), m_message("Token \"" + m_token.value +
            "\" at position " + std::to_string(m_token.idx) +
            " is invalid") {
    }
    const char* what() const noexcept override {
        return m_message.c_str();
    }
};

CommandInterface* Parser(std::vector<Token> tokens) {
    if (tokens.size() == 0)
        throw NoTokensParserError();
    switch (tokens.at(0).type) {
    case TokenType::KW_SELECT:
        return new SelectCommand("test table", std::vector<std::string>{});
    default:
        throw InvalidTokenException(tokens.at(0));
    }
    throw InvalidTokenException(tokens.at(0));
}
