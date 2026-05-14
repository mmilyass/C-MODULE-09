#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "cannot open file" << std::endl;
        return 1;
    }
    try
    {
        BitcoinExchange btc(av[1]);
        // btc.PrintData();
        btc.ParseInput();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}