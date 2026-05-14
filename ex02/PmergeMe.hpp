#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <cstdlib>
#include <climits>
#include <algorithm>

class PmergeMe
{
private:
    std::vector<int> vnumbers;
    std::deque<int> dnumbers;
    bool hasstraggler;
    int straggler;

public:
    PmergeMe() : vnumbers(), dnumbers(), hasstraggler(false), straggler(0) {};
    PmergeMe(const char **args) : vnumbers(), dnumbers(), hasstraggler(false), straggler(0) { StoreInput(args); };
    PmergeMe(const PmergeMe &other) : vnumbers(other.vnumbers), dnumbers(other.dnumbers), hasstraggler(other.hasstraggler), straggler(other.straggler) {};
    PmergeMe &operator=(const PmergeMe &other)
    {
        this->vnumbers = other.vnumbers;
        this->dnumbers = other.dnumbers;
        this->straggler = other.straggler;
        this->hasstraggler = other.hasstraggler;
        return *this;
    }
    void ParseInput(const char **args);
    void StoreInput(const char **args);
    void run();
    ~PmergeMe() {};
};

#endif