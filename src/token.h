#include <string>
#include <exception>
#include <unordered_map>
#include <unordered_set>
#include "exception.h"


enum class TokenType {
    STRING,
    INTEGER,
    IDENTIFIER,
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

const char STRING_ENCAPSULATOR = '\'';

std::string toString(TokenType type) {
    switch (type) {
        case TokenType::STRING: return "STRING";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";

        case TokenType::OP_EQUAL: return "=";
        case TokenType::OP_NOT_EQUAL: return "!=";
        case TokenType::OP_PLUS: return "+";
        case TokenType::OP_MINUS: return "-";
        case TokenType::OP_STAR: return "*";
        case TokenType::OP_SLASH: return "/";
        case TokenType::OP_LESS: return "<";
        case TokenType::OP_GREATER: return ">";
        case TokenType::OP_LESS_EQUAL: return "<=";
        case TokenType::OP_GREATER_EQUAL: return ">=";
        case TokenType::OP_CONCAT: return "||";
        case TokenType::OP_DOT: return ".";
        case TokenType::OP_PERCENT: return "%";

        case TokenType::COMMA: return ",";
        case TokenType::LEFT_PAREN: return "(";
        case TokenType::RIGHT_PAREN: return ")";
        case TokenType::SEMICOLON: return ";";

        case TokenType::KW_QUIT: return ".quit";
        case TokenType::KW_TABLES: return ".tables";

        case TokenType::KW_SELECT: return "SELECT";
        case TokenType::KW_FROM: return "FROM";
        case TokenType::KW_WHERE: return "WHERE";
        case TokenType::KW_INSERT: return "INSERT";
        case TokenType::KW_INTO: return "INTO";
        case TokenType::KW_VALUES: return "VALUES";
        case TokenType::KW_UPDATE: return "UPDATE";
        case TokenType::KW_SET: return "SET";
        case TokenType::KW_DELETE: return "DELETE";
        case TokenType::KW_CREATE: return "CREATE";
        case TokenType::KW_TABLE: return "TABLE";
        case TokenType::KW_AND: return "AND";
        case TokenType::KW_OR: return "OR";
        case TokenType::KW_NOT: return "NOT";
        case TokenType::KW_NULL: return "NULL";
        case TokenType::KW_AS: return "AS";

        default: return "UNKNOWN";
    }
}

bool IsDigit(char chr) {
    return chr >= '0' && chr <= '9';
}

bool HasNonDigits(std::string& s) {
    return s.find_first_not_of("0123456789") != std::string::npos;
}

class Token
{
private:
    const std::string m_value;
    const unsigned int m_idx;
    const TokenType m_type;
public:
    Token(std::string word, unsigned int idx) : m_value(word), m_idx(idx), m_type(GetTokenType(word, idx)) {}

    Token(std::string value, unsigned int idx, TokenType type) : m_value(value), m_idx(idx), m_type(type) {}

    std::string GetValue() const {
        return m_value;
    }
    
    TokenType GetType() const {
        return m_type;
    }
    
    unsigned int GetIdxAfterWord() const {
        return m_idx + static_cast<unsigned int>(m_value.length());
    }

    bool IsType(TokenType type) const {
        return m_type == type;
    }

    bool IsType(std::unordered_set<TokenType> types) const {
        return types.find(m_type) != types.end();
    }

    inline const Token AssertType(TokenType expected) const {
        if (!IsType(expected))
            throw GetUnexpectedTokenException(expected);
        return *this;
    } 

    inline const Token AssertType(TokenType expected, std::string message) const {
        if (!IsType(expected))
            throw GetUnexpectedTokenException(expected, message);
        return *this;
    } 

    inline const Token AssertType(std::unordered_set<TokenType> types) const {
        if (!IsType(types))
            throw GetUnexpectedTokenException(types);
        return *this;
    } 

    void Print() const {
        std::cout << '[' << m_idx << "-" << m_idx + m_value.length() << "] " << static_cast<int>(m_type) << ": " << m_value << std::endl;
    }

    InvalidTokenException GetInvalidTokenException() {
        return InvalidTokenException(m_value, "", m_idx);
    }

    InvalidTokenException GetInvalidTokenException(std::string message) const {
        return InvalidTokenException(m_value, message, m_idx);
    }
    
    UnexpectedTokenException GetUnexpectedTokenException(TokenType expected) const {
        return UnexpectedTokenException(m_value, toString(expected), m_idx);
    }

    UnexpectedTokenException GetUnexpectedTokenException(TokenType expected, std::string message) const {
        return UnexpectedTokenException(m_value, toString(expected), message, m_idx);
    }

    UnexpectedTokenException GetUnexpectedTokenException(std::unordered_set<TokenType> expected) const {
        std::vector<std::string> expectedStr;
        for (const TokenType& type : expected)
            expectedStr.push_back(toString(type));
        return UnexpectedTokenException(m_value, expectedStr, m_idx);
    }

    UnexpectedTokenException GetUnexpectedTokenException(std::unordered_set<TokenType> expected, std::string message) const {
        std::vector<std::string> expectedStr;
        for (const TokenType& type : expected)
            expectedStr.push_back(toString(type));
        return UnexpectedTokenException(m_value, expectedStr, message, m_idx);
    }

    UnexpectedTokenException GetUnexpectedTokenException(std::vector<TokenType> expected) const {
        std::vector<std::string> expectedStr;
        for (TokenType& type : expected)
            expectedStr.push_back(toString(type));
        return UnexpectedTokenException(m_value, expectedStr, m_idx);
    }

    UnexpectedTokenException GetUnexpectedTokenException(std::vector<TokenType> expected, std::string message) const {
        std::vector<std::string> expectedStr;
        for (TokenType& type : expected)
            expectedStr.push_back(toString(type));
        return UnexpectedTokenException(m_value, expectedStr, message, m_idx);
    }

    MissingTokenException GetMissingTokenException() const {
        return MissingTokenException(GetIdxAfterWord() + 1);
    }

    MissingTokenException GetMissingTokenException(TokenType requiredToken) const {
        return MissingTokenException(toString(requiredToken), GetIdxAfterWord() + 1);
    }

    static TokenType GetTokenType(std::string word, unsigned int idx) {
        if (word.length() == 0)
            throw InvalidTokenException(word, "A valid token must be of length greater than 0", idx);

        if (SYMBOL_TOKEN_MAP.count(word))
            return SYMBOL_TOKEN_MAP.at(word);

        if (KEYWORD_TOKEN_MAP.count(word))
            return KEYWORD_TOKEN_MAP.at(word);

        if (word.length() > 1 && word.at(0) == STRING_ENCAPSULATOR && word.at(word.length() - 1) == STRING_ENCAPSULATOR)
            return TokenType::STRING;

        if (word.find_first_of(STRING_ENCAPSULATOR) != std::string::npos)
            throw InvalidTokenException(word, std::string("A token must not contain \"") + STRING_ENCAPSULATOR + '\"', idx);

        if (IsDigit(word.at(0))) {
            if (HasNonDigits(word))
                throw InvalidTokenException(word, "Token starts with a digit but contains non-digit characters", idx);
            return TokenType::INTEGER;
        }

        return TokenType::IDENTIFIER;
    }

};
