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
private:
    bool IsEndLine() const {
        return m_currentIdx >= m_query.length();
    }

    static bool IsSymbol(std::string symbol) {
        return SYMBOL_TOKEN_MAP.count(symbol);
    }

    bool IsSymbol() const {
        return IsSymbol(std::string{Peek()});
    }

    bool IsSymbol(unsigned int start){
        return IsSymbol(start, m_currentIdx);
    }

    bool IsSymbol(unsigned int start, unsigned int end){
        return IsSymbol(m_query.substr(start, end-start)) && (IsEndLine() || IsSymbol(m_query.substr(start, end-start+1)));
    }

    bool IsWhiteSpace() const {
        return isspace(Peek());
    }

    bool IsStringBoundry() const {
        return Peek() == STRING_ENCAPSULATOR;
    }
    public:
    Tokenizer(std::string& query)
        : m_query(query), m_currentIdx(0)
    {

    }

    char Peek() const {
        if (IsEndLine())
            return '\0';
        return m_query[m_currentIdx];
    }

    std::string PeekAhead() const {
        if ( m_currentIdx + 1 >= m_query.length())
            return "\0";
        return std::string {m_query[m_currentIdx + 1]};
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
        bool isInString = IsStringBoundry();
        bool finishedStringBoundry = false;
        do{
            Advance();
            if (IsStringBoundry()) {
                finishedStringBoundry = true;
                Advance();
            }
        }
        while (!IsSymbol(startIdx) && !IsEndLine() && !IsSymbol(m_currentIdx, m_currentIdx+1) && !(IsWhiteSpace() && !isInString) && !(isInString && finishedStringBoundry));
        
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
