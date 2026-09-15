#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <cstdlib>
# include <fstream>
# include <iomanip>
# include <iostream>
# include <map>
# include <sstream>
# include <string>

class BitcoinExchange
{
    public:
        BitcoinExchange();
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);

        void loadDatabase(const std::string &path);
        bool findRate(const std::string &date, double &rate) const;
        bool exchangeValue(const std::string &date, double value, double &result) const;

        static bool isValidDate(const std::string &date);
        static bool parseValue(const std::string &rawValue, double &value);
        static std::string trim(const std::string &input);

    private:
        std::map<std::string, double> _database;
};

#endif