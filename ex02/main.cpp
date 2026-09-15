#include "PmergeMe.hpp"

#include <ctime>
#include <iostream>

int main(int argc, char **argv)
{
    std::vector<unsigned int> numbers;
    if (!parsePositiveIntegers(argc, argv, numbers))
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::vector<unsigned int> vectorCopy = numbers;
    std::list<unsigned int> listCopy;
    for (std::size_t i = 0; i < numbers.size(); ++i)
        listCopy.push_back(numbers[i]);

    std::clock_t vectorStart = std::clock();
    mergeInsertSortVector(vectorCopy);
    std::clock_t vectorEnd = std::clock();

    std::clock_t listStart = std::clock();
    mergeInsertSortList(listCopy);
    std::clock_t listEnd = std::clock();

    std::cout << "Before:";
    for (std::size_t i = 0; i < numbers.size(); ++i)
        std::cout << ' ' << numbers[i];
    std::cout << std::endl;

    std::cout << "After:";
    for (std::size_t i = 0; i < vectorCopy.size(); ++i)
        std::cout << ' ' << vectorCopy[i];
    std::cout << std::endl;

    std::cout << "Time to process a range of " << numbers.size()
              << " elements with std::vector : "
              << static_cast<double>(vectorEnd - vectorStart) / CLOCKS_PER_SEC * 1000000.0
              << " us" << std::endl;
    std::cout << "Time to process a range of " << numbers.size()
              << " elements with std::list : "
              << static_cast<double>(listEnd - listStart) / CLOCKS_PER_SEC * 1000000.0
              << " us" << std::endl;

    return 0;
}
