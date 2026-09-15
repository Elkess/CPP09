#include "RPN.hpp"

int main(int argc, char **argv)
{
    double result;

    if (argc != 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    if (!evaluateRPN(argv[1], result))
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    std::cout << result << std::endl;
    return 0;
}
