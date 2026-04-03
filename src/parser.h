#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include "tokenizer.h"
#include "engine.h"

CommandInterface* Parser(std::vector<Token> tokens) {
    if (tokens.size() == 0)
        throw NoTokensParserError();
    switch (tokens.at(0).GetType()) {
    case TokenType::KW_SELECT:
        return new SelectCommand("test table", std::vector<std::string>{});
    default:
        throw tokens.at(0).GetInvalidTokenException();
    }
    throw tokens.at(0).GetInvalidTokenException();
}
