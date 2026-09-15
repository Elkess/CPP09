#include "RPN.hpp"

bool isNumberToken(const std::string &token)
{
    if (token.empty())
        return false;

    std::string::size_type i = 0;
    if (token[0] == '-' && token.size() > 1)
        i = 1;

    while (i < token.size())
    {
        if (!std::isdigit(static_cast<unsigned char>(token[i])))
            return false;
        ++i;
    }
    return true;
}

static bool parseToken(const std::string &token, double &value)
{
    std::stringstream stream(token);
    stream >> value;
    if (stream.fail() || !stream.eof())
        return false;
    return true;
}

double applyOperation(double left, double right, const std::string &op)
{
    if (op == "+")
        return left + right;
    if (op == "-")
        return left - right;
    if (op == "*")
        return left * right;
    if (op == "/")
    {
        if (right == 0.0)
            throw std::runtime_error("division by zero");
        return left / right;
    }
    throw std::runtime_error("unknown operator");
}

bool evaluateRPN(const std::string &expression, double &result)
{
    std::stack<double> numbers;
    std::stringstream stream(expression);
    std::string token;

    while (stream >> token)
    {
        if (isNumberToken(token))
        {
            double value = 0.0;
            if (!parseToken(token, value))
                return false;
            numbers.push(value);
            continue;
        }

        if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            if (numbers.size() < 2)
                return false;

            double right = numbers.top();
            numbers.pop();
            double left = numbers.top();
            numbers.pop();

            try
            {
                numbers.push(applyOperation(left, right, token));
            }
            catch (const std::exception &)
            {
                return false;
            }
            continue;
        }

        return false;
    }

    if (numbers.size() != 1)
        return false;

    result = numbers.top();
    return true;
}
