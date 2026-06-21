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
        // std::cout << "here" << value << std::endl;

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
        std::cout << '[' << *it << ']';
    }
    std::cout << std::endl;
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
bool comparing(int value, int element) {
    Ncompare++;
    std::cout << "Comparing " << value << " and " << element << std::endl;
    return value < element; // Or your custom comparison logic
}



template <typename T, typename B>
void pairingSwap(T &container, int sizeOfpairs, T &mainChain, T &pendChain, B &bounds)
{
    if (sizeOfpairs > (int)container.size())
        return;

    for (size_t i = 0; i + sizeOfpairs <= container.size(); i += sizeOfpairs)
    {
        Ncompare++;
        if (container[i + (sizeOfpairs / 2) - 1] > container[i + sizeOfpairs - 1])
        {
            std::swap_ranges(container.begin() + i,
                             container.begin() + i + sizeOfpairs / 2,
                             container.begin() + i + sizeOfpairs / 2);
        }
    }

    pairingSwap(container, sizeOfpairs * 2, mainChain, pendChain, bounds);

    if (sizeOfpairs < 2)
        return;

    mainChain.insert(mainChain.end(),
                     container.begin(),
                     container.begin() + sizeOfpairs);

    size_t i;
    for (i = sizeOfpairs; i + sizeOfpairs <= container.size(); i += sizeOfpairs)
    {
        size_t firstHalfStart = i;
        size_t firstHalfEnd = i + (sizeOfpairs / 2);
        size_t secondHalfStart = i + (sizeOfpairs / 2);
        size_t secondHalfEnd = i + sizeOfpairs;

        // The partner block starts here in mainChain BEFORE insertion
        size_t partnerBaseIndex = mainChain.size();

        pendChain.insert(pendChain.end(),
                         container.begin() + firstHalfStart,
                         container.begin() + firstHalfEnd);

        // Store partner positions, not partner values
        for (size_t j = 0; j < (size_t)(sizeOfpairs / 2); ++j)
            bounds.push_back(partnerBaseIndex + j);

        mainChain.insert(mainChain.end(),
                         container.begin() + secondHalfStart,
                         container.begin() + secondHalfEnd);
    }

    if (i < container.size())
    {
        pendChain.insert(pendChain.end(), container.begin() + i, container.end());

        // No partner for leftovers: search the whole chain
        for (size_t j = i; j < container.size(); ++j)
            bounds.push_back(mainChain.size());
    }

    int prev_jac = 1;
    int inserted = 0;

    for (int k = 2; true; k++)
    {
        int curr_jac = GenerateJacobsthalNumber(k);
        int jac_diff = curr_jac - prev_jac;

        if (jac_diff > (int)pendChain.size())
            break;

        for (int idx = jac_diff - 1; idx >= 0 && !pendChain.empty(); --idx)
        {
            typename T::iterator pend_it = pendChain.begin() + idx;

            size_t bound_index = bounds[idx];
            if (bound_index > mainChain.size())
                bound_index = mainChain.size();

            // Search only up to the partner position
            typename T::iterator bound_it = mainChain.begin() + bound_index;

            typename T::iterator pos =
                std::upper_bound(mainChain.begin(), bound_it, *pend_it, comparing);

            size_t insertedPos = std::distance(mainChain.begin(), pos);

            mainChain.insert(pos, *pend_it);

            // Remove current pend element and its bound
            pendChain.erase(pend_it);
            bounds.erase(bounds.begin() + idx);

            // Shift every remaining bound that is to the right
            for (typename B::iterator it = bounds.begin();
                 it != bounds.end();
                 ++it)
            {
                if (*it >= (int)insertedPos)
                    ++(*it);
            }
        }

        prev_jac = curr_jac;
        inserted += jac_diff;
    }

    while (!pendChain.empty())
    {
        typename T::iterator pend_it = pendChain.end() - 1;
        typename T::iterator pos = std::upper_bound(mainChain.begin(), mainChain.end(), *pend_it, comparing);

        mainChain.insert(pos, *pend_it);
        pendChain.pop_back();
    }

    container.clear();
    container = mainChain;
    mainChain.clear();
    pendChain.clear();
    bounds.clear();
}

void PmergeMe::run()
{
    std::deque<int> pendChain;
    std::deque<int> mainChain;
    std::deque<int> bounds;
    pairingSwap(dnumbers, 2, mainChain, pendChain, bounds);
    PrintNumbers(dnumbers);
    std::cout << "Number of comparisons: " << Ncompare << std::endl;
}
 