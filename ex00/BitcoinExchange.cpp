#include "BitcoinExchange.hpp"

void BitcoinExchange::StoreData()
{
    char *end;
    std::ifstream file(dataFile.c_str());
    if (!file)
        throw std::runtime_error("Error ! cannot open file.");
    std::string line;
    // skip the first line but you need to check whether it is having value | data
    std::getline(file, line);
    while (std::getline(file, line))
    {
        size_t comas = line.find(',');
        if (comas == std::string::npos)
            throw std::runtime_error("data file not correct.");
        this->data[line.substr(0, comas)] = std::strtod(line.substr(comas + 1).c_str(), &end);
        if (*end != '\0')
            throw std::runtime_error("data file not correct.");
    }
}

void BitcoinExchange::PrintData()
{
    for (std::map<std::string, double>::iterator it = data.begin(); it != data.end(); ++it)
    {
        std::cout << it->first << ", " << it->second << std::endl;
    }
}

void checkIsDigit(std::string year, std::string mmonth, std::string dday, std::string key)
{
    for (int i = 0; i < 4; i++)
    {
        if (year[i] < '0' || year[i] > '9')
            throw std::runtime_error("Error! bad input => " + key);
    }
    for (int i = 0; i < 2; i++)
    {
        if (mmonth[i] < '0' || mmonth[i] > '9' || dday[i] < '0' || dday[i] > '9')
            throw std::runtime_error("Error! bad input => " + key);
    }
}

void parseKey(std::string &key)
{
    key.erase(remove(key.begin(), key.end(), ' '), key.end());
    if (key.length() != 10)
        throw std::runtime_error("Error: bad input => " + key);
    std::string yyear = key.substr(0, 4).c_str();
    std::string mmonth = key.substr(5, 2).c_str();
    std::string dday = key.substr(8, 2).c_str();

    checkIsDigit(yyear, mmonth, dday, key); // check whether they are valid degits
    int year = std::atoi(yyear.c_str());
    int month = std::atoi(mmonth.c_str());
    int day = std::atoi(dday.c_str());
    if (key[4] != '-' || key[7] != '-' || year < 0 || month < 1 || month > 12)
        throw std::runtime_error("Error: bad input => " + key);
    if (month == 2)
    {
        if (((year % 4 == 0 && year % 100 != 0) || (year % 400) == 0) && day > 29)
            throw std::runtime_error("Error: Invalid date: February has at most 29 days => " + key);
        else if (day > 28)
            throw std::runtime_error("Error: Invalid date: February has at most 28 days => " + key);
    }
    if (day < 1 || day > 31)
        throw std::runtime_error("Error: bad input => " + dday);
}

double parseValue(std::string &value)
{
    char *whatHappend;
    double var = std::strtod(value.c_str(), &whatHappend);
    while (std::isspace(*whatHappend))
        ++whatHappend;
    if (*whatHappend != '\0')
        throw std::runtime_error("Error: invalid/not-found value.");
    if (var > 1000)
        throw std::runtime_error("Error: too large value.");
    if (var < 0)
        throw std::runtime_error("Error: not a positive value.");
    value.erase(remove(value.begin(), value.end(), ' '), value.end());
    return var;
}

void parsFirstLine(std::string &line)
{
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    if (line != "date|value")
        throw std::runtime_error("first line does not match : date | value");
}

void BitcoinExchange::ParseInput()
{
    std::ifstream file(inputFile.c_str());
    if (!file)
    {
        throw std::runtime_error("Error: cannot open file: " + inputFile);
    }
    std::string key;
    std::string value;
    double dvalue;
    double exchange_rate;
    std::string line;
    std::getline(file, line); // to skip the first line
    parsFirstLine(line);
    while (std::getline(file, line))
    {
        try
        {
            if (line.empty())
                continue;
            size_t index = line.find('|');
            if (index == std::string::npos)
                throw std::runtime_error("Error: bad input => " + line);
            key = line.substr(0, index);
            value = line.substr(index + 1);

            parseKey(key);
            dvalue = parseValue(value);
            std::map<std::string, double>::iterator it = data.lower_bound(key);
            if (it != data.end() && it->first == key) // if the lower bound find the key it sets the iterator to the exact match
            {
                exchange_rate = it->second;
            }
            else if (it == data.begin()) // if it does not find the key it sets the iterator to the begin.
            {
                std::cout << "Error: no previous matching date" << std::endl;
                continue;
            }
            else
            {
                --it;
                exchange_rate = it->second;
            }
            std::cout << key << " => " << value << " = " << dvalue * exchange_rate << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }
    }
}