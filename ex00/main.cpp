#include "BitcoinExchange.hpp"

static std::string formatOutput(double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(10) << value;
    std::string result = oss.str();

    while (result.size() > 1 && result[result.size() - 1] == '0')
        result.erase(result.size() - 1);
    if (!result.empty() && result[result.size() - 1] == '.')
        result.erase(result.size() - 1);
    return result;
}

static bool extractRow(const std::string &line, std::string &date, std::string &value)
{
    std::string trimmed = BitcoinExchange::trim(line);
    if (trimmed.empty() || trimmed == "date | value")
        return false;

    std::string::size_type pipePos = trimmed.find('|');
    if (pipePos == std::string::npos)
        return false;
    if (pipePos == 0 || pipePos == trimmed.size() - 1)
        return false;
    if (trimmed[pipePos - 1] != ' ' || trimmed[pipePos + 1] != ' ')
        return false;

    date = BitcoinExchange::trim(trimmed.substr(0, pipePos));
    value = BitcoinExchange::trim(trimmed.substr(pipePos + 1));
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    BitcoinExchange exchange;
    exchange.loadDatabase("data.csv");

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::string date;
        std::string valueText;

        if (!extractRow(line, date, valueText))
        {
            std::string cleaned = BitcoinExchange::trim(line);
            if (!cleaned.empty() && cleaned != "date | value")
                std::cerr << "Error: bad input => " << cleaned << std::endl;
            continue;
        }

        if (!BitcoinExchange::isValidDate(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }

        double value = 0.0;
        if (!BitcoinExchange::parseValue(valueText, value))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }
        if (value < 0.0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000.0)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        double result = 0.0;
        if (!exchange.exchangeValue(date, value, result))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }

        std::cout << date << " => " << formatOutput(value) << " = " << formatOutput(result) << std::endl;
    }

    return 0;
}
