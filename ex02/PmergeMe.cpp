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
                throw std::runtime_error("Error: two same numbers");
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

template <typename T>
bool comparing(const T &a, const T &b)
{
    Ncompare++;
    return a.back().value < b.back().value;
}
bool comparingg(const pairandBound &a, const pairandBound &b)
{
    Ncompare++;
    return a.value < b.value;
}

template <typename T, typename U>
void fullContainer(T &container, U &mainChain)
{
    container.clear();
    for (typename U::iterator it = mainChain.begin(); it != mainChain.end(); ++it)
    {
        container.insert(container.end(), it->begin(), it->end());
    }
    mainChain.clear();
}

template <typename T, typename U>
typename T::iterator findPartner(T &mainChain, U &partnerVal)
{
    if (partnerVal.empty())
        return mainChain.end();
    for (typename T::iterator it = mainChain.begin(); it != mainChain.end(); ++it)
    {
        if (it->back().value == partnerVal.back())
            return it;
    }
    return mainChain.end();
}

template <typename T, typename U>
void pairingSwap(T &container, U &mainChain, U &pendChain, int groupSize)
{
    int half = groupSize / 2;

    if (groupSize <= (int)container.size())
    {
        for (int i = 0; i + groupSize <= (int)container.size(); i += groupSize)
        {
            pairandBound &left = container[i + half - 1];
            pairandBound &right = container[i + groupSize - 1];

            Ncompare++;
            if (left.value > right.value)
            {
                for (int j = 0; j < half; j++)
                    std::swap(container[i + j], container[i + half + j]);
            }
            container[i + half - 1].partner.push_back(container[i + groupSize - 1].value);
        }
    }

    if (groupSize * 2 <= (int)container.size())
        pairingSwap(container, mainChain, pendChain, groupSize * 2);
    typename U::value_type s;
    // if (groupSize < 2)
    //     return;

    mainChain.push_back(
        typename U::value_type(
            container.begin(),
            container.begin() + half));
    mainChain.push_back(
        typename U::value_type(
            container.begin() + half,
            container.begin() + groupSize));
    for (int i = groupSize; i < (int)container.size(); i += groupSize)
    {
        size_t pendBegin = i;
        size_t pendEnd = i + half;
        size_t mainBegin = i + half;
        size_t mainEnd = i + groupSize;
        size_t endOfContainer = container.size();

        if (pendEnd <= container.size())
        {
            pendChain.push_back(
                typename U::value_type(
                    container.begin() + pendBegin,
                    container.begin() + pendEnd));
        }
        else if (i < (int)container.size())
        {
            s = typename U::value_type(
                container.begin() + i,
                container.end());
            break;
        }

        if (mainEnd < endOfContainer)
        {
            mainChain.push_back(
                typename U::value_type(
                    container.begin() + mainBegin,
                    container.begin() + mainEnd));
        }
        else
        {
            s = typename U::value_type(
                container.begin() + mainBegin,
                container.begin() + endOfContainer);
        }
    }
    int prev_jac = 1;
    std::vector<bool> inserted(pendChain.size(), false);

    for (int k = 2;; k++)
    {
        int curr_jac = jacobsthal(k);

        if (prev_jac >= (int)pendChain.size())
            break;

        int last = std::min(curr_jac - 2, (int)pendChain.size() - 1);
        int first = prev_jac - 1;

        for (int idx = last; idx >= first; --idx)
        {
            if (inserted[idx])
                continue;

            typename U::iterator searchEnd =
                findPartner(mainChain, pendChain[idx].back().partner);
            typename U::iterator pos =
                std::upper_bound(mainChain.begin(), searchEnd, pendChain[idx],
                                 comparing<typename U::value_type>);
            mainChain.insert(pos, pendChain[idx]);
            inserted[idx] = true;
        }

        prev_jac = curr_jac;
    }

    for (int idx = (int)pendChain.size() - 1; idx >= 0; --idx)
    {
        if (inserted[idx])
            continue;

        typename U::iterator searchEnd =
            findPartner(mainChain, pendChain[idx].back().partner);
        typename U::iterator pos =
            std::upper_bound(mainChain.begin(), searchEnd, pendChain[idx],
                             comparing<typename U::value_type>);
        mainChain.insert(pos, pendChain[idx]);
    }
    if (!s.empty())
        mainChain.push_back(s);
    fullContainer(container, mainChain);
    pendChain.clear();
}

template <typename T, typename U>
void start(T &container, U &mainChain, U &pendChain)
{
    pairandBound dStraggler;
    bool dOdd = (container.size() % 2 != 0);
    if (dOdd)
    {
        dStraggler = container.back();
        container.pop_back();
    }
    Ncompare = 0;
    pairingSwap(container, mainChain, pendChain, 2);
    if (dOdd)
    {
        typename T::iterator pos =
            std::upper_bound(container.begin(), container.end(), dStraggler, comparingg);
        container.insert(pos, dStraggler);
    }
}
void PmergeMe::run()
{
    {
        std::cout << "Before: ";
        PrintNumbers(vnumbers);
        std::cout << std::endl;
        std::clock_t st = std::clock();
        start(vnumbers, mainChain2, pendChain2); //  run the algorithm on the vector
        std::clock_t end = std::clock();

        double elapsed =
            static_cast<double>(end - st) / CLOCKS_PER_SEC;
        std::cout << "After: ";
        PrintNumbers(vnumbers);
        std::cout << std::endl;

        std::cout
            << "Time to process a range of "
            << vnumbers.size()
            << " elements with std::vector : "
            << elapsed * 1000000
            << " us"
            << std::endl;
    }

    std::clock_t st = std::clock();
    start(dnumbers, mainChain, pendChain); //  run the algorithm on the vector
    std::clock_t end = std::clock();

    double elapsed =
        static_cast<double>(end - st) / CLOCKS_PER_SEC;

    std::cout
        << "Time to process a range of "
        << dnumbers.size()
        << " elements with std::deque : "
        << elapsed * 1000000
        << " us"
        << std::endl;
}
