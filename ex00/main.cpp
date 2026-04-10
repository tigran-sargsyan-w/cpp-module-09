#include "BitcoinExchange.hpp"
#include <iostream>

/**
 * @brief Entry point for the BitcoinExchange program.
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument values.
 * @return Exit status code.
 */
int	main(int argc, char **argv)
{
	BitcoinExchange	exchange;

	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		exchange.loadDatabase("data.csv");
		exchange.processInputFile(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}