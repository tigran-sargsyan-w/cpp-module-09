#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <climits>
# include <ctime>
# include <deque>
# include <iostream>
# include <sstream>
# include <stdexcept>
# include <string>
# include <vector>

class PmergeMe
{
  private:
	struct	PairVec
	{
		int	small;
		int	big;
	};

	struct	PairDeq
	{
		int	small;
		int	big;
	};

	std::vector<int> vectorData;
	std::deque<int> dequeData;

  public:
	PmergeMe();
	PmergeMe(char **argv);
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	void process(void);

  private:
	void parseInput(char **argv);
	int parsePositiveInt(const std::string &text);

	void printBefore(void) const;
	void printAfter(const std::vector<int> &sorted) const;

	std::vector<int> fordJohnsonVector(const std::vector<int> &data);
	std::deque<int> fordJohnsonDeque(const std::deque<int> &data);

	std::vector<PairVec> makePairsVector(const std::vector<int> &data, bool &hasOdd, int &oddValue);
	std::deque<PairDeq> makePairsDeque(const std::deque<int> &data, bool &hasOdd, int &oddValue);

	std::vector<PairVec> sortPairsVector(const std::vector<PairVec> &pairs);
	std::deque<PairDeq> sortPairsDeque(const std::deque<PairDeq> &pairs);

	std::vector<int> buildInsertionOrderVector(int lastIndex);
	std::deque<int> buildInsertionOrderDeque(int lastIndex);

	int lowerBoundVector(const std::vector<int> &chain, int end, int value);
	int lowerBoundDeque(const std::deque<int> &chain, int end, int value);

	int findPositionVector(const std::vector<int> &chain, int value);
	int findPositionDeque(const std::deque<int> &chain, int value);

	bool isSortedVector(const std::vector<int> &data) const;
	bool isSortedDeque(const std::deque<int> &data) const;
};

#endif