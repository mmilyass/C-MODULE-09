#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <cstring>

extern unsigned int Ncompare;

typedef struct pairandBound
{
    int value;
    std::vector<int> partner; // stores the VALUE of the winner this pend element is paired with
    std::vector<pairandBound> friends;

    pairandBound() : value(0) {}
    pairandBound(int v) : value(v) {}

    bool operator==(const pairandBound &other) const { return value == other.value; }
} pairandBound;

inline std::ostream &operator<<(std::ostream &os, const pairandBound &p)
{
    os << p.value;
    return os;
}

class PmergeMe
{
private:
    std::vector<pairandBound> vnumbers;
    std::deque<pairandBound> dnumbers;
    bool hasstraggler;
    int straggler;

public:
    PmergeMe() : hasstraggler(false), straggler(0) {}
    PmergeMe(const char **args) : hasstraggler(false), straggler(0) { StoreInput(args); }
    PmergeMe(const PmergeMe &o) : vnumbers(o.vnumbers), dnumbers(o.dnumbers),
                                  hasstraggler(o.hasstraggler), straggler(o.straggler) {}
    PmergeMe &operator=(const PmergeMe &o)
    {
        vnumbers = o.vnumbers;
        dnumbers = o.dnumbers;
        hasstraggler = o.hasstraggler;
        straggler = o.straggler;
        return *this;
    }
    ~PmergeMe() {}

    void ParseInput(const char **args);
    void StoreInput(const char **args);
    void run();
};

#endif