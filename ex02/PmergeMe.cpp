#include "PmergeMe.hpp"
#include <cstring>

unsigned int Ncompare = 0;

void PmergeMe::ParseInput(const char **args)
{
    if (args[1] == NULL)
        throw std::runtime_error("Error");

    for (int i = 1; args[i]; i++)
    {
        if (args[i][0] == '\0')
            throw std::runtime_error("Error");
        for (int j = 0; args[i][j]; j++)
        {
            if (!std::isdigit(args[i][j]))
                throw std::runtime_error("Error");
        }
        for (int j = i - 1; j >= 0; j--)
        {
            if (i > 1 && std::strcmp(args[i], args[j]) == 0)
                throw std::runtime_error("Error");
        }
    }
}

void PmergeMe::StoreInput(const char **args)
{
    ParseInput(args);

    for (int i = 1; args[i]; i++)
    {
        char *end;
        double value = std::strtod(args[i], &end);
        if (value < 0 || value > INT_MAX)
            throw std::runtime_error("Error");

        pairandBound node(static_cast<int>(value));
        dnumbers.push_back(node);
        vnumbers.push_back(node);
    }
}

template <typename T>
void PrintNumbers(T &container)
{
    for (typename T::iterator it = container.begin(); it != container.end(); ++it)
        std::cout << ' ' << it->value;
    std::cout << std::endl;
}

int jacobsthal(int k)
{
    if (k == 0)
        return 0;
    if (k == 1)
        return 1;

    int prev2 = 0;
    int prev1 = 1;
    for (int i = 2; i <= k; i++)
    {
        int curr = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

bool comparing(const pairandBound &a, const pairandBound &b)
{
    Ncompare++;
    return a.value < b.value;
}

template <typename T>
typename T::iterator findPartner(T &mainChain, int partnerVal)
{
    if (partnerVal < 0)
        return mainChain.end();
    for (typename T::iterator it = mainChain.begin(); it != mainChain.end() && partnerVal > 0; ++it)
    {
        if (it->value == partnerVal)
            return it;
    }
    return mainChain.end();
}

// ─────────────────────────────────────────────────────────────────────────────
// pairingSwap  –  the heart of Ford-Johnson
//
// WHAT IT DOES (think of groupSize=2 on first call):
//
//   container:  [ 5 | 3 | 8 | 1 | 4 | 7 ]
//                 ^---^   ^---^   ^---^       <-- pairs of size groupSize
//                left right left right ...
//
//   Step 1 – compare each pair, swap so the BIGGER one is always on the RIGHT
//             also record: left.partner = right.value  (loser remembers winner)
//
//   Step 2 – recurse with groupSize*2  (this sorts only the right-block winners)
//
//   Step 3 – after recursion returns, build two chains:
//             mainChain  = winners (right blocks) + a1 (very first left block)
//             pendChain  = losers  (left blocks, everything else)
//
//   Step 4 – insert every pend element into mainChain with binary search,
//             but only search UP TO its partner (we know pend <= partner,
//             so the answer is definitely to the LEFT of partner)
//             Use Jacobsthal order so the search range grows as slowly as possible
// ─────────────────────────────────────────────────────────────────────────────

template <typename T>
void pairingSwap(T &container, int groupSize)
{
    int half = groupSize / 2;

    if (groupSize > (int)container.size())
        return;

    for (int i = 0; i + groupSize <= (int)container.size(); i += groupSize)
    {
        pairandBound &left = container[i + half - 1];
        pairandBound &right = container[i + groupSize - 1];

        Ncompare++;
        if (left.value > right.value)
        {
            for (int k = 0; k < half; k++)
                std::swap(container[i + k], container[i + half + k]);
        }
        container[i + half - 1].partner.push_back(container[i + groupSize - 1].value);
    }

    pairingSwap(container, groupSize * 2);
    std::cout << "after pairing" << std::endl;
    PrintNumbers(container);
    std::cout << "groupSize=" << groupSize << std::endl;
    if (groupSize <= 2)
        return;
    T pendChain, mainChain;

    int fullPairs = (container.size() / groupSize) * groupSize;
    mainChain.insert(mainChain.end(),
                     container.begin() + half,
                     container.begin() + groupSize);
    mainChain.insert(mainChain.begin(),
                     container.begin(),
                     container.begin() + half);
    int i;
    for (i = groupSize; i + groupSize <= (int)container.size(); i += groupSize)
    {
        container[i + half - 1].friends.insert(container[i + half - 1].friends.end(),
                                               container.begin() + i, container.begin() + (i + half - 1));
        pendChain.push_back(container[i + half - 1]);

        mainChain.insert(mainChain.end(),
                         container.begin() + i + half,
                         container.begin() + i + groupSize);
    }
    int x = container.size() - fullPairs;
    if (x < groupSize / 2)
    {
        mainChain.insert(mainChain.end(),
                         container.begin() + fullPairs,
                         container.end());
    }
    else
    {
        std::cout << "=====================" << i << "=====================" << std::endl;
        std::cout << "=====================" << x << "=====================" << std::endl;
        container.back().friends.insert(container.back().friends.end(),
                                        container.begin() + i, container.end() - 1);
        std::cout << "container.back().friends: ";
        PrintNumbers(container.back().friends);
        pendChain.push_back(container.back());
    }
    std::cout << "maiChain: ";
    PrintNumbers(mainChain);
    std::cout << "pendChain: ";
    PrintNumbers(pendChain);
    // pairingSwap(pendChain, 2);
    int prev_jac = 1;
    for (int k = 2;; k++)
    {
        int curr_jac = jacobsthal(k);
        int jac_diff = curr_jac - prev_jac;
        if (jac_diff > (int)pendChain.size())
            break;
        for (int idx = jac_diff - 1; idx >= 0 && !pendChain.empty(); idx--)
        {
            pairandBound pend = pendChain[idx];
            typename T::iterator partnerIt = findPartner(mainChain, pend.partner.empty() ? -1 : pend.partner.back());
            // std::cout << "partnerIt: ";
            // if (partnerIt != mainChain.end())
            //     std::cout << partnerIt->value << std::endl;
            // else
            //     std::cout << "not found" << std::endl;
            typename T::iterator searchEnd = (partnerIt == mainChain.end())
                                                 ? mainChain.end()
                                                 : partnerIt + 1;
            typename T::iterator pos =
                std::upper_bound(mainChain.begin(), searchEnd, pend, comparing);
            !pend.partner.empty() ? pend.partner.pop_back() : void(); // remove the partner value after using it

            std::cout << "pos value: " << pos->value << std::endl;
            mainChain.insert(pos, pend);
            if (!pend.friends.empty())
            {
                pos = pos - 1;
                std::cout << "inserting friends of " << pend.value << ": ";
                PrintNumbers(pend.friends);
                std::cout << "pos value: " << pos->value << std::endl;
                mainChain.insert(pos, pend.friends.begin(), pend.friends.end());
                pend.friends.clear();
            }
            pendChain.erase(pendChain.begin() + idx);
        }

        prev_jac = curr_jac;
    }

    while (!pendChain.empty())
    {
        pairandBound pend = pendChain.back();

        typename T::iterator partnerIt = findPartner(mainChain, pend.partner.empty() ? -1 : pend.partner.back());
        typename T::iterator searchEnd = (partnerIt == mainChain.end())
                                             ? mainChain.end()
                                             : partnerIt + 1;
        typename T::iterator pos =
            std::upper_bound(mainChain.begin(), searchEnd, pend, comparing);
        !pend.partner.empty() ? pend.partner.pop_back() : void(); // remove the partner value after using it
        mainChain.insert(pos, pend);
        if (!pend.friends.empty())
        {
            mainChain.insert(pos, pend.friends.begin(), pend.friends.end());
            pend.friends.clear();
        }
        pendChain.pop_back();
    }
    container = mainChain;
}

void PmergeMe::run()
{
    pairandBound dStraggler;
    bool dOdd = (dnumbers.size() % 2 != 0);
    if (dOdd)
    {
        dStraggler = dnumbers.back();
        dnumbers.pop_back();
    }
    Ncompare = 0;
    pairingSwap(dnumbers, 2);
    if (dOdd)
    {
        std::deque<pairandBound>::iterator pos =
            std::upper_bound(dnumbers.begin(), dnumbers.end(), dStraggler, comparing);
        dnumbers.insert(pos, dStraggler);
    }
    PrintNumbers(dnumbers);
}