#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _database(other._database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        _database = other._database;
    return *this;
}

std::string BitcoinExchange::trim(const std::string &input)
{
    std::string::size_type start = 0;
    std::string::size_type end = input.size();

    while (start < end && (input[start] == ' ' || input[start] == '\t' || input[start] == '\r' || input[start] == '\n'))
        ++start;
    while (end > start && (input[end - 1] == ' ' || input[end - 1] == '\t' || input[end - 1] == '\r' || input[end - 1] == '\n'))
        --end;
    return input.substr(start, end - start);
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (std::string::size_type i = 0; i < date.size(); ++i)
    {
        if ((i == 4) || (i == 7))
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    int maxDay = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        maxDay = 30;
    else if (month == 2)
    {
        bool leapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
        maxDay = leapYear ? 29 : 28;
    }

    if (day > maxDay)
        return false;
    return true;
}

bool BitcoinExchange::parseValue(const std::string &rawValue, double &value)
{
    std::string cleaned = trim(rawValue);
    if (cleaned.empty())
        return false;

    char *endPtr = 0;
    value = std::strtod(cleaned.c_str(), &endPtr);
    if (endPtr == cleaned.c_str())
        return false;

    while (*endPtr == ' ' || *endPtr == '\t')
        ++endPtr;
    if (*endPtr != '\0')
        return false;
    return true;
}

void BitcoinExchange::loadDatabase(const std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::string::size_type commaPos = line.find(',');
        if (commaPos == std::string::npos)
            continue;

        std::string date = trim(line.substr(0, commaPos));
        std::string rateText = trim(line.substr(commaPos + 1));

        if (date == "date" && rateText == "exchange_rate")
            continue;
        if (!isValidDate(date))
            continue;

        char *endPtr = 0;
        double rate = std::strtod(rateText.c_str(), &endPtr);
        while (*endPtr == ' ' || *endPtr == '\t')
            ++endPtr;
        if (*endPtr != '\0')
            continue;
        _database[date] = rate;
    }
}

bool BitcoinExchange::findRate(const std::string &date, double &rate) const
{
    if (_database.empty())
        return false;

    std::map<std::string, double>::const_iterator it = _database.lower_bound(date);
    if (it == _database.end())
    {
        --it;
    }
    else if (it != _database.begin() && it->first > date)
    {
        --it;
    }
    else if (it->first > date)
    {
        return false;
    }

    rate = it->second;
    return true;
}

bool BitcoinExchange::exchangeValue(const std::string &date, double value, double &result) const
{
    if (value < 0.0 || value > 1000.0)
        return false;

    double rate = 0.0;
    if (!findRate(date, rate))
        return false;

    result = value * rate;
    return true;
}
