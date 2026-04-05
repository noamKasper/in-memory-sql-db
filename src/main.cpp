#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "exception.h"
#include "tokenizer.h"
#include "parser.h"

void PrintTokens(std::vector<Token> tokens) {
    for (const Token& token : tokens) {
        token.Print();
    }
}

int main(int, char**) {
    std::cout << "Welcome to TinySQL\n";
    std::cout << ">>>  ";
    std::string query;
    std::getline(std::cin, query);

    try {
        Tokenizer tokenzier = Tokenizer(query);
        auto tokens = tokenzier.Tokenize();
        
        PrintTokens(tokens); // debug function
        auto output = Parser(tokens)->Execute();
        std::cout << output << std::endl;
    }
    catch (GeneralException& e) {
        std::cerr << query << "\n";
        std::cerr << std::string(e.GetExceptionIdx(), ' ') << '^' << "\n";
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

