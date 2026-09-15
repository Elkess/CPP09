#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <cstdlib>
# include <list>
# include <string>
# include <vector>

bool parsePositiveIntegers(int argc, char **argv, std::vector<unsigned int> &numbers);
void mergeInsertSortVector(std::vector<unsigned int> &numbers);
void mergeInsertSortList(std::list<unsigned int> &numbers);

#endif
