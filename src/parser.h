#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include "tokenizer.h"
#include "engine.h"

/* Condition ParseCondition(std::vector<Token> tokens) {

}
 */
std::vector<std::string> ParseColumns(std::vector<Token> tokens) {
    if (tokens.size() < 3)
        throw NotEnoughTokensException();
    if (tokens.at(0).GetType() != TokenType::KW_SELECT)
        throw tokens.at(0).GetInvalidTokenException("Trying to parse for SELECT while token isn't SELECT");
    
    if (tokens.at(1).IsType(TokenType::OP_STAR)) {
        if (!tokens.at(2).IsType(TokenType::KW_FROM))
            throw tokens.at(2).GetInvalidTokenException("Unexpected Token after *, expecting FROM"); // replace with unexpected token error
        return std::vector<std::string> {"*"};
    }
    std::vector<std::string> columns;
    bool wasLastComma = true;
    bool reachedFrom = false;
    for (auto it = tokens.begin()+1; it != tokens.end(); it++) {
        Token token = *it;
        if (wasLastComma) {
            if (!token.IsType(TokenType::IDENTIFIER))
                throw token.GetInvalidTokenException("Non identifier token before FROM");  // replace with unexpected token error
            columns.push_back(token.GetValue());
            wasLastComma = false;
            continue;
        }
        if (token.IsType(TokenType::COMMA)) {
            wasLastComma = true;
            continue;
        }

        if (token.IsType(TokenType::KW_FROM)) { 
            reachedFrom = true;
            break; 
        }
        throw token.GetInvalidTokenException("Token doesn't belong here, looking for either a comma or a FROM"); // replace with unexpected token error
    }
    if (!reachedFrom)
        throw NotEnoughTokensException();
    return columns;
}

CommandInterface* Parser(std::vector<Token> tokens) {
    if (tokens.size() == 0)
        throw NoTokensParserError();
    switch (tokens.at(0).GetType()) {
    case TokenType::KW_SELECT:
        return new SelectCommand("test table", ParseColumns(tokens));
    default:
        throw tokens.at(0).GetInvalidTokenException();
    }
    throw tokens.at(0).GetInvalidTokenException();
}
