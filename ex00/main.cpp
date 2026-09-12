#include "BitcoinExchange.hpp"
#include <ios>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <iomanip>
/*

Error: not a positive number.
Error: too large a number.
Error: bad input => 2001-42-42


*/
void	err_printer(std::string err)
{
	std::cerr << "Error: " << err << std::endl;
}

void	fill_db(std::map<std::string, double> &db)
{
	std::ifstream data_file("data.csv");
	if (!data_file.is_open())
		err_printer("could not open DB file.");

	std::string line;
	while (std::getline(data_file, line))
	{
		if (line == "date,exchange_rate")
			continue ;
		size_t pos = line.find(',');
		std::string date = line.substr(0, pos); 
		std::string exchange_rate_str = line.substr(pos + 1, line.size());
		std::stringstream ss(exchange_rate_str);
		double exchange_rate;
		ss >> exchange_rate;
		db[date] = exchange_rate;
	}

	// std::map<std::string, double>::iterator it = db.begin();
	// while (it != db.end())
	// {
	// 	std::cout << std::fixed <<std::setprecision(2) << it->first << " " << it->second << std::endl;
	// 	it++;
	// }
}

void	bitcoinexchange(std::ifstream &file)
{
	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() != 10)
			err_printer(" bad input => " + line);
		size_t pos = line.find('|');
		if (pos == std::string::npos || 
			line[pos +1] != ' ' && line[pos -1])
		{
			err_printer("Invalid line: follow exact format plz 'date | value'");
			continue ;
		}
		std::string date = line.substr(0, pos -1);
		std::string val_str = line.substr(pos +1, line.size());
		// khassni n parssi date o val
	}
}


int	main(int ac, char **av)
{
	if (ac == 2)
	{
		std::ifstream file(av[1]);
		if (!file.is_open())
			return err_printer("could not open file."),1;
		std::map<std::string, double> db;
		fill_db(db);
		bitcoinexchange(file);
	}
	else
		return err_printer("could not open file."), 1;
} 
