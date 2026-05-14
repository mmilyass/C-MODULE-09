#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <climits>
class BitcoinExchange
{
private:
    std::map<std::string, double> data;
    std::string dataFile;
    std::string inputFile;

public:
    BitcoinExchange(std::string input) :  dataFile("data.csv"), inputFile(input) { StoreData();};
    BitcoinExchange(const BitcoinExchange &other) : data(other.data), dataFile(other.dataFile) {};
    BitcoinExchange &operator=(const BitcoinExchange &other)
    {
        data = other.data;
        dataFile = other.dataFile;
        inputFile = other.inputFile;
        return *this;
    }
    void StoreData();
    void PrintData();
    void ParseInput();
    ~BitcoinExchange(){};
};


#endif