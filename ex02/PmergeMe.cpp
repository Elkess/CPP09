#include "PmergeMe.hpp"

#include <algorithm>

static bool isUnsignedInteger(const std::string &token)
{
    if (token.empty())
        return false;

    std::string::size_type i = 0;
    while (i < token.size())
    {
        if (token[i] < '0' || token[i] > '9')
            return false;
        ++i;
    }
    return true;
}

bool parsePositiveIntegers(int argc, char **argv, std::vector<unsigned int> &numbers)
{
    numbers.clear();
    for (int i = 1; i < argc; ++i)
    {
        std::string token(argv[i]);
        if (!isUnsignedInteger(token))
            return false;

        unsigned long value = std::strtoul(token.c_str(), 0, 10);
        if (value > 4294967295UL)
            return false;
        numbers.push_back(static_cast<unsigned int>(value));
    }
    return argc > 1;
}

static std::vector<std::size_t> buildJacobsthal(std::size_t limit)
{
    std::vector<std::size_t> sequence;
    if (limit == 0)
        return sequence;

    std::size_t a = 1;
    std::size_t b = 1;
    sequence.push_back(1);

    while (b < limit)
    {
        std::size_t next = b + 2 * a;
        a = b;
        b = next;
        if (b < limit)
            sequence.push_back(b);
    }
    return sequence;
}

static std::vector<unsigned int> pairSort(const std::vector<unsigned int> &source)
{
    std::vector<unsigned int> smaller;
    std::vector<unsigned int> larger;

    for (std::size_t i = 0; i + 1 < source.size(); i += 2)
    {
        unsigned int left = source[i];
        unsigned int right = source[i + 1];
        if (left > right)
        {
            unsigned int tmp = left;
            left = right;
            right = tmp;
        }
        smaller.push_back(left);
        larger.push_back(right);
    }

    if (source.size() % 2 != 0)
        smaller.push_back(source[source.size() - 1]);

    std::sort(smaller.begin(), smaller.end());
    std::sort(larger.begin(), larger.end());

    std::vector<unsigned int> result;
    if (!larger.empty())
        result = larger;
    else
        result = smaller;

    std::vector<std::size_t> jacobsthal = buildJacobsthal(smaller.size());
    std::vector<bool> inserted(smaller.size(), false);

    for (std::size_t i = 0; i < jacobsthal.size(); ++i)
    {
        std::size_t index = jacobsthal[i];
        if (index >= smaller.size())
            continue;
        if (inserted[index])
            continue;

        std::vector<unsigned int>::iterator insertPos = std::lower_bound(
            result.begin(), result.end(), smaller[index]);
        result.insert(insertPos, smaller[index]);
        inserted[index] = true;
    }

    for (std::size_t i = 0; i < smaller.size(); ++i)
    {
        if (inserted[i])
            continue;
        std::vector<unsigned int>::iterator insertPos = std::lower_bound(
            result.begin(), result.end(), smaller[i]);
        result.insert(insertPos, smaller[i]);
    }

    return result;
}

void mergeInsertSortVector(std::vector<unsigned int> &numbers)
{
    if (numbers.size() < 2)
        return;
    numbers = pairSort(numbers);
}

void mergeInsertSortList(std::list<unsigned int> &numbers)
{
    if (numbers.size() < 2)
        return;

    std::vector<unsigned int> temp;
    for (std::list<unsigned int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
        temp.push_back(*it);

    temp = pairSort(temp);

    numbers.clear();
    for (std::size_t i = 0; i < temp.size(); ++i)
        numbers.push_back(temp[i]);
}
