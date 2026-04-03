#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "exception.h"
#include "tokenizer.h"
#include "parser.h"

const char* const EXIT_STRING = ".quit";

void PrintTokens(std::vector<Token> tokens) {
    for (const Token& token : tokens) {
        token.Print();
    }
}

int main(int, char**) {
    std::string test = "test";
    std::cout << test.substr(1, 2) << '\n';
    std::cout << "Welcome to TinySQL\n";
    std::string query;

    std::cout << ">>>  ";
    std::getline(std::cin, query);

    try {
        Tokenizer tokenzier = Tokenizer(query);
        auto tokens = tokenzier.Tokenize();
        
        PrintTokens(tokens);
        auto output = Parser(tokens)->Execute();
        std::cout << output << std::endl;
    }
    catch (GeneralException& e) {
        std::cout << query << "\n";
        std::cout << std::string(e.GetExceptionIdx(), ' ') << '^' << "\n";
        std::cout << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
