#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, double> rates;

		bool	isLeapYear(int year) const;
		bool	isValidDate(const std::string &date) const;
		bool	isValidValue(const std::string &valueText, double &value) const;
		std::string	trim(const std::string &text) const;
		double	getRateForDate(const std::string &date) const;

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase(const std::string &filename);
		void	processInputFile(const std::string &filename) const;
};

#endif