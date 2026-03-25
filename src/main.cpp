#include <iostream>
#include <string>
#include <vector>
#include "tokenizer.h"

const char* const EXIT_STRING = ".quit";

void PrintTokens(std::vector<Token> tokens) {
    for (const Token& token : tokens) {
        std::cout << '[' << token.idx << "] " << static_cast<int>(token.type) << ": " << token.value << std::endl;
    }
}

int main(int, char**)
{

    std::string test = "test";
    std::cout << test.substr(1,2) << '\n';
    std::cout << "Welcome to TinySQL\n";
    std::string query;

    std::cout << ">>>  ";
    std::getline(std::cin, query);
    Tokenizer tokenzier = Tokenizer(query);
    auto tokens = tokenzier.Tokenize();

    PrintTokens(tokens);
}
