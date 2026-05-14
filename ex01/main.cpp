#include "RNP.hpp"


int main(int ac, char **av){
    if (ac < 2)
    {
        std::cerr << "need arguments ." << std::endl;
        return 1;
    }
    try
    {
        RNP rnp;
        rnp.evalRPN(av[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}