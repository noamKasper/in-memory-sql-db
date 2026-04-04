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

class InvalidTokenException : public GeneralException {
public:
    InvalidTokenException(std::string token, std::string message, unsigned int idx)
        : GeneralException("Invalid token \"" + token + "\" at position " + std::to_string(idx) + ": " + message, idx) {
    }
};

