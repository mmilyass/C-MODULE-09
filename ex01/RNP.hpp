#ifndef RNP_HPP
#define RNP_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

class RNP
{
private:
    std::stack<int> st;
    std::string data;
public:
    RNP(){};
    RNP(const RNP &other) : st(other.st) {};
    RNP &operator=(const RNP &other)
    {
        st = other.st;
        return *this;
    }
    int evalRPN(char *tokens);
    void parseTokens(char *tokens);
    ~RNP(){};
};


#endif