#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "token.h"

class Tokenizer
{
private:
    std::string m_query;
    unsigned int m_currentIdx;
    bool m_isInString;
private:
    bool IsEndLine() const {
        return m_currentIdx >= m_query.length();
    }

    bool IsEndLine(unsigned int idx) {
        return idx >= m_query.length();
    }

    static bool IsSymbol(std::string symbol) {
        return SYMBOL_TOKEN_MAP.count(symbol);
    }

    bool IsSymbol() const {
        return IsSymbol(std::string{ Peek() });
    }

    bool IsSymbol(unsigned int start, unsigned int length) {
        return !IsEndLine(start + length - 1) && IsSymbol(m_query.substr(start, length));
    }

    bool IsSymbolFrom(unsigned int start) {
        switch (m_currentIdx - start) {
        case 1:
            return IsSymbol(start, 1) && !IsSymbol(start, 2);
        case 2:
            return IsSymbol(start, 2);
        default:
            return false;
        }
    }

    bool IsNextSymbol() {
        return IsSymbol(m_currentIdx, 1) || IsSymbol(m_currentIdx, 2);
    }

    bool IsWhiteSpace() const {
        return isspace(Peek());
    }

    bool IsStringBoundry() const {
        return Peek() == STRING_ENCAPSULATOR;
    }


    bool IsToken(unsigned int start) {
        if (IsEndLine())
            return true;
        if (m_isInString)
            return false;
        if (IsSymbolFrom(start))
            return true;
        if (IsSymbol(m_currentIdx - 1, 2))
            return false;
        if (IsNextSymbol())
            return true;
        if (IsWhiteSpace())
            return true;
        return false;
    }
public:
    Tokenizer(std::string& query)
        : m_query(query), m_currentIdx(0), m_isInString(false) {
    }

    char Peek() const {
        if (IsEndLine())
            return '\0';
        return m_query[m_currentIdx];
    }

    std::string PeekAhead() const {
        if (m_currentIdx + 1 >= m_query.length())
            return "\0";
        return std::string{ m_query[m_currentIdx + 1] };
    }

    char Advance() {
        char curr = Peek();
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
        m_isInString = IsStringBoundry();
        bool finishedStringBoundry = false;
        do {
            Advance();
            if (m_isInString && IsStringBoundry()) {
                finishedStringBoundry = true;
                m_isInString = false;
                Advance();
            }
        } while (!IsToken(startIdx) && !finishedStringBoundry);

        return m_query.substr(startIdx, (m_currentIdx - startIdx));
    }

    std::vector<Token> Tokenize() {
        std::vector<Token> tokens;
        while (!IsEndLine()) {
            SkipWhiteSpace();
            unsigned int idx = m_currentIdx;
            std::string word = ScanWord();
            tokens.push_back(Token(word, idx));
        }
        return tokens;
    }
};
