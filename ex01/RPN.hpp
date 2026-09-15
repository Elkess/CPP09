#ifndef RPN_HPP
# define RPN_HPP

# include <cctype>
# include <cstdlib>
# include <iostream>
# include <sstream>
# include <stack>
# include <stdexcept>
# include <string>

bool isNumberToken(const std::string &token);
bool evaluateRPN(const std::string &expression, double &result);
double applyOperation(double left, double right, const std::string &op);

#endif
