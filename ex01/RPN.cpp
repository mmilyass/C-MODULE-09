#include "RPN.hpp"

void RNP::parseTokens(char *tokens)
{
    int check = 0;
    data.clear();
    for (int i = 0; tokens[i]; i++)
    {
        if (tokens[i] == ' ' || tokens[i] == '\t')
        {
            check = 0;
            continue;
        }
        if (tokens[i] == '+' || tokens[i] == '-' || tokens[i] == '*' || tokens[i] == '/' || std::isdigit(tokens[i]))
        {
            if (check == 1)
                throw std::runtime_error("Error");
            check = 1;
            data += tokens[i];
            continue;
        }
        else
        {
            throw std::runtime_error("Error");
        }
    }
}

int RNP::evalRPN(char *tokens)
{
    try
    {
        parseTokens(tokens);
        for (size_t i = 0; i < data.length(); i++)
        {
            if (std::isdigit(data[i]))
            {
                st.push(data[i] - 48);
            }
            else
            {
                if (st.size() < 2)
                    throw std::runtime_error("Error");
                int value2 = st.top();
                st.pop();
                int value1 = st.top();
                st.pop();
                switch (data[i])
                {
                case '+':
                    st.push(value1 + value2);
                    break;
                case '-':
                    st.push(value1 - value2);
                    break;
                case '*':
                    st.push(value1 * value2);
                    break;
                case '/':
                    if (value2 == 0)
                        throw std::runtime_error("Error: division by zero");
                    st.push(value1 / value2);
                    break;
                default:
                    break;
                }
            }
        }
        if (st.size() < 1)
            throw std::runtime_error("Error");
        else
            std::cout << st.top() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}