#pragma once
#include <exception>
#include <string>

class GeneralException: public std::exception {
private:
    std::string m_message;
    unsigned int m_idx;
public:
    GeneralException(std::string message, unsigned int idx)
        : m_message(message), m_idx(idx) {}

    unsigned int GetExceptionIdx() const {
        return m_idx;
    }

    virtual const char* what() const noexcept override {
        return m_message.c_str();
    }
};

static std::string Quote(std::string str) {
    return '"' + str + '"';
}

class NoTokensParserError : public GeneralException {
public:
    NoTokensParserError()
        : GeneralException("The command contains no tokens", 0) {}
};

class NotEnoughTokensException : public GeneralException {
public:
    NotEnoughTokensException()
        : GeneralException("The command doesn't contain tokens", 0) {}
};
static std::string ToListOptionsString(std::vector<std::string> v) {
    switch (v.size()) {
        case 0: return Quote("");
        case 1: return Quote(v.at(0));
    }
    
    std::string result;
    for (auto it = v.begin(); it != std::prev(v.end(), 2); ++it)
        result += Quote((*it)) + ", ";

    return result + Quote(v.at(v.size() - 2)) + " or " + Quote(v.at(v.size() - 1));       
}
class UnexpectedTokenException : public GeneralException {
public:
    UnexpectedTokenException(std::string token, std::string expected, unsigned int idx)
        : GeneralException("Unexpected token " + Quote(token) + " at position " + std::to_string(idx) + ", expected " + Quote(expected), idx) {}

    UnexpectedTokenException(std::string token, std::string expected, std::string message, unsigned int idx)
        : GeneralException("Unexpected token " + Quote(token) + " at position " + std::to_string(idx) + ", expected " + Quote(expected) + ": " + message, idx) {}

    UnexpectedTokenException(std::string token, std::vector<std::string> expected, unsigned int idx)
        : GeneralException("Unexpected token " + Quote(token) + " at position " + std::to_string(idx) + ", expected " + ToListOptionsString(expected), idx) {}

    UnexpectedTokenException(std::string token, std::vector<std::string> expected, std::string message, unsigned int idx)
        : GeneralException("Unexpected token " + Quote(token) + " at position " + std::to_string(idx) + ", expected " + ToListOptionsString(expected) + ": " + message, idx) {}
};

class MissingTokenException : public GeneralException {
public:
    MissingTokenException(unsigned int idx)
        : GeneralException("Missing token at position: " + std::to_string(idx), idx) {}

    MissingTokenException(std::string token, unsigned int idx)
        : GeneralException("Missing token " + Quote(token) + " at position " + std::to_string(idx), idx) {}
};

class InvalidTokenException : public GeneralException {
public:
    InvalidTokenException(std::string token, std::string message, unsigned int idx)
        : GeneralException("Invalid token " + Quote(token) + " at position " + std::to_string(idx) + ": " + message, idx) {
    }
};

