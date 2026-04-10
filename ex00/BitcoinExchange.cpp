#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Constructs an empty BitcoinExchange object.
 */
BitcoinExchange::BitcoinExchange() {}

/**
 * @brief Copy-constructs a BitcoinExchange object.
 * @param other Source object to copy from.
 */
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : rates(other.rates) {}

/**
 * @brief Assigns exchange rate data from another object.
 * @param other Source object to assign from.
 * @return Reference to this object.
 */
BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		rates = other.rates;
	return (*this);
}

/**
 * @brief Destroys the BitcoinExchange object.
 */
BitcoinExchange::~BitcoinExchange() {}

/**
 * @brief Removes leading and trailing whitespace from a string.
 * @param text Input string to trim.
 * @return Trimmed string.
 */
std::string	BitcoinExchange::trim(const std::string &text) const
{
	std::string::size_type	start;
	std::string::size_type	end;

	start = 0;
	while (start < text.length() && std::isspace(text[start]))
		start++;
	end = text.length();
	while (end > start && std::isspace(text[end - 1]))
		end--;
	return (text.substr(start, end - start));
}

/**
 * @brief Checks whether a year is leap.
 * @param year Year value.
 * @return True if leap year, otherwise false.
 */
bool	BitcoinExchange::isLeapYear(int year) const
{
	if (year % 400 == 0)
		return (true);
	if (year % 100 == 0)
		return (false);
	return (year % 4 == 0);
}

/**
 * @brief Validates a date string in YYYY-MM-DD format.
 * @param date Date string to validate.
 * @return True if date is valid, otherwise false.
 */
bool	BitcoinExchange::isValidDate(const std::string &date) const
{
	int					year;
	int					month;
	int					day;
	int					daysInMonth[12];
	std::istringstream	stream;
	char				dash1;
	char				dash2;

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	stream.str(date);
	stream >> year >> dash1 >> month >> dash2 >> day;
	if (stream.fail() || !stream.eof())
		return (false);
	if (dash1 != '-' || dash2 != '-')
		return (false);
	if (month < 1 || month > 12)
		return (false);
	daysInMonth[0] = 31;
	daysInMonth[1] = 28;
	daysInMonth[2] = 31;
	daysInMonth[3] = 30;
	daysInMonth[4] = 31;
	daysInMonth[5] = 30;
	daysInMonth[6] = 31;
	daysInMonth[7] = 31;
	daysInMonth[8] = 30;
	daysInMonth[9] = 31;
	daysInMonth[10] = 30;
	daysInMonth[11] = 31;
	if (isLeapYear(year))
		daysInMonth[1] = 29;
	if (day < 1 || day > daysInMonth[month - 1])
		return (false);
	return (true);
}

/**
 * @brief Parses and validates a numeric value string.
 * @param valueText Text containing the value.
 * @param value Parsed output value.
 * @return True if parsing succeeded, otherwise false.
 */
bool	BitcoinExchange::isValidValue(const std::string &valueText, double &value) const
{
	char		*end;
	std::string	text;

	text = trim(valueText);
	if (text.empty())
		return (false);
	value = std::strtod(text.c_str(), &end);
	if (*end != '\0')
		return (false);
	return (true);
}

/**
 * @brief Loads exchange rates from a CSV database file.
 * @param filename Path to the database file.
 */
void	BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream		file(filename.c_str());
	std::string			line;
	std::string			date;
	std::string			valueText;
	std::string::size_type	commaPos;
	double				value;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");
	std::getline(file, line);
	if (line != "date,exchange_rate")
		throw std::runtime_error("Error: invalid database header.");
	while (std::getline(file, line))
	{
		commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;
		date = trim(line.substr(0, commaPos));
		valueText = trim(line.substr(commaPos + 1));
		if (!isValidDate(date))
			continue;
		if (!isValidValue(valueText, value))
			continue;
		rates[date] = value;
	}
	if (rates.empty())
		throw std::runtime_error("Error: database is empty.");
}

/**
 * @brief Returns the closest exchange rate for a given date.
 * @param date Date key to search for.
 * @return Exchange rate for the exact date or closest previous date.
 */
double	BitcoinExchange::getRateForDate(const std::string &date) const
{
	std::map<std::string, double>::const_iterator	it;

	if (rates.empty())
		throw std::runtime_error("Error: database is empty.");
	it = rates.lower_bound(date);
	if (it != rates.end() && it->first == date)
		return (it->second);
	if (it == rates.begin())
		throw std::runtime_error("Error: bad input => " + date);
	it--;
	return (it->second);
}

/**
 * @brief Processes an input file and prints converted values.
 * @param filename Path to the input file.
 */
void	BitcoinExchange::processInputFile(const std::string &filename) const
{
	std::ifstream			file(filename.c_str());
	std::string				line;
	std::string				date;
	std::string				valueText;
	std::string::size_type	pipePos;
	double					value;
	double					rate;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");
	std::getline(file, line);
	if (line != "date | value")
		throw std::runtime_error("Error: invalid input header.");
	while (std::getline(file, line))
	{
		pipePos = line.find('|');
		if (pipePos == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = trim(line.substr(0, pipePos));
		valueText = trim(line.substr(pipePos + 1));
		if (!isValidDate(date))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (!isValidValue(valueText, value))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		try
		{
			rate = getRateForDate(date);
			std::cout << date << " => " << value << " = " << value * rate
				<< std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}