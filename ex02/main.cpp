#include "PmergeMe.hpp"

int main(int ac, const char **av)
{
    if (ac < 2)
    {
        std::cout << "enter numbers" << std::endl;
    }
    else
        try
        {
            PmergeMe pm(av);
            pm.run();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
}