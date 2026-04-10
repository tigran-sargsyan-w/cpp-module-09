#include "PmergeMe.hpp"

/**
 * @brief Entry point for the PmergeMe program.
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument values.
 * @return Exit status code.
 */
int	main(int argc, char **argv)
{
	try
	{
		if (argc < 2)
			throw std::runtime_error("Error");
		PmergeMe sorter(argv + 1);
		sorter.process();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}