#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


enum class TokenType {
    LITERAL,
    OP_EQUAL,          // =
    OP_NOT_EQUAL,      // !=
    OP_PLUS,           // +
    OP_MINUS,          // -
    OP_STAR,           // *
    OP_SLASH,          // /
    OP_LESS,           // <
    OP_GREATER,        // >
    OP_LESS_EQUAL,     // <=
    OP_GREATER_EQUAL,  // >=
    OP_CONCAT,         // ||
    OP_DOT,            // .
    OP_PERCENT,        // %
    COMMA,             // ,
    LEFT_PAREN,        // (
    RIGHT_PAREN,       // )
    SEMICOLON,         // ;
    KW_QUIT,           // .quit
    KW_TABLES,         // .tables
    KW_SELECT,
    KW_FROM,
    KW_WHERE,
    KW_INSERT,
    KW_INTO,
    KW_VALUES,
    KW_UPDATE,
    KW_SET,
    KW_DELETE,
    KW_CREATE,
    KW_TABLE,
    KW_AND,
    KW_OR,
    KW_NOT,
    KW_NULL,
    KW_AS

};

const std::unordered_map<std::string, TokenType> SYMBOL_TOKEN_MAP = {
    {"=",  TokenType::OP_EQUAL},
    {"!=", TokenType::OP_NOT_EQUAL},
    {"+",  TokenType::OP_PLUS},
    {"-",  TokenType::OP_MINUS},
    {"*",  TokenType::OP_STAR},
    {"/",  TokenType::OP_SLASH},
    {"<",  TokenType::OP_LESS},
    {">",  TokenType::OP_GREATER},
    {"<=", TokenType::OP_LESS_EQUAL},
    {">=", TokenType::OP_GREATER_EQUAL},
    {"||", TokenType::OP_CONCAT},
    {".",  TokenType::OP_DOT},
    {"%",  TokenType::OP_PERCENT},
    {",", TokenType::COMMA},
    {"(", TokenType::LEFT_PAREN},
    {")", TokenType::RIGHT_PAREN},
    {";", TokenType::SEMICOLON},
};

const std::unordered_map<std::string, TokenType> KEYWORD_TOKEN_MAP = {
    {".quit", TokenType::KW_QUIT},
    {".table", TokenType::KW_TABLE},
    {"SELECT", TokenType::KW_SELECT},
    {"FROM",   TokenType::KW_FROM},
    {"WHERE",  TokenType::KW_WHERE},
    {"INSERT", TokenType::KW_INSERT},
    {"INTO",   TokenType::KW_INTO},
    {"VALUES", TokenType::KW_VALUES},
    {"UPDATE", TokenType::KW_UPDATE},
    {"SET",    TokenType::KW_SET},
    {"DELETE", TokenType::KW_DELETE},
    {"CREATE", TokenType::KW_CREATE},
    {"TABLE",  TokenType::KW_TABLE},
    {"AND",    TokenType::KW_AND},
    {"OR",     TokenType::KW_OR},
    {"NOT",    TokenType::KW_NOT},
    {"NULL",   TokenType::KW_NULL},
    {"AS",     TokenType::KW_AS}
};


struct Token
{
    const unsigned int idx;
    const TokenType type;
    const std::string value;

    static TokenType GetWordTokenType(std::string word){
        if (SYMBOL_TOKEN_MAP.count(word))
            return SYMBOL_TOKEN_MAP.at(word);
        if (KEYWORD_TOKEN_MAP.count(word))
            return KEYWORD_TOKEN_MAP.at(word);
        return TokenType::LITERAL;
    }
};

class Tokenizer
{
private:
    std::string m_query;
    unsigned int m_currentIdx;
private:
    bool IsEndLine() const {
        return m_currentIdx >= m_query.length();
    }

    static bool IsSymbol(std::string symbol) {
        return SYMBOL_TOKEN_MAP.count(symbol);
    }

    bool IsSymbol() const {
        return IsSymbol(Peek());
    }

    bool IsWhiteSpace() const {
        return isspace(Peek()[0]);
    }
public:
    Tokenizer(std::string& query)
        : m_query(query), m_currentIdx(0)
    {

    }

    std::string Peek() const {
        if (IsEndLine())
            return "\0";
        return std::string{ m_query[m_currentIdx] };
    }

    std::string PeekAhead() const {
        if ( m_currentIdx + 1 >= m_query.length())
            return "\0";
        return std::string {m_query[m_currentIdx + 1]};
    }

    std::string Advance() {
        std::string curr = Peek();
        m_currentIdx++;
        return curr;
    }

    void SkipWhiteSpace() {
        while (IsWhiteSpace()) {
            Advance();
        }
    }


    std::string ScanWord() {
        unsigned int startIdx = m_currentIdx;
        if (IsSymbol())
            return Advance();
        while (!IsSymbol() && !IsEndLine() && !IsWhiteSpace())
            Advance();
        
        return m_query.substr(startIdx, (m_currentIdx - startIdx));
    }

    std::vector<Token> Tokenize() {
        std::vector<Token> tokens;
        while (!IsEndLine()) {
            SkipWhiteSpace();
            std::string word = ScanWord();
            tokens.push_back(Token{ m_currentIdx, Token::GetWordTokenType(word), word});
        }
        return tokens;
    }
};
