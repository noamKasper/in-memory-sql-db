#include <iostream>
#include <string>
#include "myfunctions.h"

class Query
{
public:
    static constexpr char EXIT_CHAR = 'q';
private:
    std::string m_query{};
public:
    void ReadLine()
    {
        std::cout << ">>  ";
        std::getline(std::cin, m_query);
    }

    void Echo()
    {
        std::cout << "Your input was: " << m_query << '\n';
    }

    bool IsExitQuery()
    {
        return m_query.length() == 1 && m_query[0] == Query::EXIT_CHAR;
    }

};

int main(int, char**)
{
    std::cout << "Welcome to TinySQL\n";
    Query query = Query();
    do
    {
        query.ReadLine();
        query.Echo();
    } while (!query.IsExitQuery());
}
