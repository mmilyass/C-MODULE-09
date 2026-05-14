#include "PmergeMe.hpp"

unsigned int Ncompare = 0;

void PmergeMe::ParseInput(const char **args)
{
    if (args[1] == NULL)
        return;
    for (int i = 1; args[i]; i++)
    {
        if (args[i][0] == '\0')
            throw std::runtime_error("Error");
        for (int j = 0; args[i][j]; j++)
        {
            if (!std::isdigit(args[i][j]))
                throw std::runtime_error("Error");
        }
    }
    std::cout << "Parsing Done! no error found." << std::endl;
}

void PmergeMe::StoreInput(const char **args)
{
    char *end;
    ParseInput(args);

    for (int i = 1; args[i]; i++)
    {
        double value = std::strtod(args[i], &end);
        if (value < 0 || value > INT_MAX)
            throw std::runtime_error("Error");
        dnumbers.push_back(static_cast<int>(value));
        std::cout << "here" << value << std::endl;

        vnumbers.push_back(static_cast<int>(value));
    }
}

template <typename T>
void PrintNumbers(T &container)
{
    if (container.size() < 1)
    {
        std::cout << "no data stored yet ?" << std::endl;
        return;
    }
    std::cout << "numbers stored: " << std::endl;
    for (typename T::iterator it = container.begin(); it != container.end(); ++it)
    {
        std::cout << '[' << *it << ']' << std::endl;
    }
}

size_t GenerateJacobsthalNumber(size_t k)
{
    // 0 and 1 are the first two numbers in the Jacobsthal sequence
    if (k == 0)
        return 0;
    if (k == 1)
        return 1;
    size_t prev2 = 0, prev1 = 1; // * to store the two previous numbers in the sequence
    for (size_t i = 2; i <= k; i++)
    {
        size_t result = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = result;
    }
    return prev1;
}

template <typename T>
void pairingSwap(T &container, int sizeOfpairs, T &mainChain, T &pendChain)
{
    if (sizeOfpairs > (int)container.size())
        return;
    for (size_t i = 0; i + sizeOfpairs <= container.size(); i += sizeOfpairs)
    {
        if (container[i + (sizeOfpairs / 2) - 1] > container[i + sizeOfpairs - 1])
        {
            Ncompare++;
            std::swap_ranges(container.begin() + i, container.begin() + i + sizeOfpairs / 2, container.begin() + i + sizeOfpairs / 2);
        }
    }
    pairingSwap(container, sizeOfpairs * 2, mainChain, pendChain);
    if (sizeOfpairs < 2)
        return;
    bool is_odd = (container.size() % sizeOfpairs != 0);
    mainChain.insert(mainChain.end(),
                     container.begin(),
                     container.begin() + sizeOfpairs);
    size_t i;
    for (i = sizeOfpairs; i < container.size(); i += sizeOfpairs)
    {
        size_t firstHalfStart = i;
        size_t firstHalfEnd = i + (sizeOfpairs / 2);
        size_t secondHalfStart = i + (sizeOfpairs / 2);
        size_t secondHalfEnd = i + sizeOfpairs;

        pendChain.insert(pendChain.end(), container.begin() + firstHalfStart, container.begin() + firstHalfEnd);
        mainChain.insert(mainChain.end(), container.begin() + secondHalfStart, container.begin() + secondHalfEnd);
    }
    if (is_odd)
    {
        pendChain.insert(pendChain.end(), container.begin() + i, container.begin() + i + (sizeOfpairs / 2));
    }
    // PrintNumbers(pendChain);
    // PrintNumbers(mainChain);
    int prev_jac = 0;
    int inserted = 0 ;
    for (int k = 0; true; k++)
    {
        int curr_jac = GenerateJacobsthalNumber(k);
        int jac_diff = curr_jac - prev_jac;

        if (jac_diff > (int)pendChain.size())
            break;

        for (int i = jac_diff - 1; i >= 0 && !pendChain.empty(); i--)
        {
            typename T::iterator pend_it = pendChain.begin() + i;
            typename T::iterator bound_it = mainChain.begin() + std::min((int)mainChain.size(), curr_jac + inserted);
            typename T::iterator pos = std::upper_bound(mainChain.begin(), bound_it, *pend_it);
            mainChain.insert(pos, *pend_it);
            pendChain.erase(pend_it);
        }
        prev_jac = curr_jac;
        inserted += jac_diff;
    }
    while (!pendChain.empty())
    {
        typename T::iterator pend_it = pendChain.end() - 1;
        typename T::iterator pos = std::upper_bound(mainChain.begin(), mainChain.end(), *pend_it);

        mainChain.insert(pos, *pend_it);
        pendChain.pop_back();
    }

    // pendChain.clear();
    container.clear();
    container = mainChain;
    mainChain.clear();
}

void PmergeMe::run()
{
    std::deque<int> pendChain;
    std::deque<int> mainChain;
    pairingSwap(dnumbers, 2, mainChain, pendChain);
    PrintNumbers(dnumbers);

}
