#include "PmergeMe.hpp"

/**
 * @brief Constructs an empty sorter.
 */
PmergeMe::PmergeMe() {}

/**
 * @brief Constructs sorter and parses input values.
 * @param argv Null-terminated array of input strings.
 */
PmergeMe::PmergeMe(char **argv)
{
	parseInput(argv);
}

/**
 * @brief Copy-constructs a sorter.
 * @param other Source object to copy from.
 */
PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

/**
 * @brief Assigns sorter data from another instance.
 * @param other Source object to assign from.
 * @return Reference to this object.
 */
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		vectorData = other.vectorData;
		dequeData = other.dequeData;
	}
	return (*this);
}

/**
 * @brief Destroys the sorter object.
 */
PmergeMe::~PmergeMe() {}

/**
 * @brief Parses one positive integer from text.
 * @param text Input text.
 * @return Parsed integer value.
 */
int	PmergeMe::parsePositiveInt(const std::string &text)
{
	long value;
	char extra;
	std::stringstream stream;

	if (text.empty())
		throw std::runtime_error("Error");
	stream.str(text);
	stream >> value;
	if (stream.fail() || stream.get(extra))
		throw std::runtime_error("Error");
	if (value <= 0 || value > INT_MAX)
		throw std::runtime_error("Error");
	return (static_cast<int>(value));
}

/**
 * @brief Parses all arguments and fills internal containers.
 * @param argv Null-terminated array of numeric strings.
 */
void PmergeMe::parseInput(char **argv)
{
	int	value;
	int	index;

	index = 0;
	while (argv[index] != NULL)
	{
		value = parsePositiveInt(argv[index]);
		vectorData.push_back(value);
		dequeData.push_back(value);
		index++;
	}
}

/**
 * @brief Prints input sequence before sorting.
 */
void PmergeMe::printBefore(void) const
{
	std::size_t	index;

	std::cout << "Before: ";
	index = 0;
	while (index < vectorData.size())
	{
		std::cout << vectorData[index];
		if (index + 1 < vectorData.size())
			std::cout << " ";
		index++;
	}
	std::cout << std::endl;
}

/**
 * @brief Prints resulting sorted sequence.
 * @param sorted Sorted vector to print.
 */
void PmergeMe::printAfter(const std::vector<int> &sorted) const
{
	std::size_t	index;

	std::cout << "After: ";
	index = 0;
	while (index < sorted.size())
	{
		std::cout << sorted[index];
		if (index + 1 < sorted.size())
			std::cout << " ";
		index++;
	}
	std::cout << std::endl;
}

/**
 * @brief Finds lower-bound position in vector prefix.
 * @param chain Sorted chain.
 * @param end Exclusive search end index.
 * @param value Value to insert.
 * @return Insertion position.
 */
int	PmergeMe::lowerBoundVector(const std::vector<int> &chain, int end, int value)
{
	int	left;
	int	right;
	int	mid;

	left = 0;
	right = end;
	while (left < right)
	{
		mid = left + (right - left) / 2;
		if (chain[mid] < value)
			left = mid + 1;
		else
			right = mid;
	}
	return (left);
}

/**
 * @brief Finds lower-bound position in deque prefix.
 * @param chain Sorted chain.
 * @param end Exclusive search end index.
 * @param value Value to insert.
 * @return Insertion position.
 */
int	PmergeMe::lowerBoundDeque(const std::deque<int> &chain, int end, int value)
{
	int	left;
	int	right;
	int	mid;

	left = 0;
	right = end;
	while (left < right)
	{
		mid = left + (right - left) / 2;
		if (chain[mid] < value)
			left = mid + 1;
		else
			right = mid;
	}
	return (left);
}

/**
 * @brief Finds position of a value in vector chain.
 * @param chain Data sequence.
 * @param value Value to search.
 * @return Found index or -1.
 */
int	PmergeMe::findPositionVector(const std::vector<int> &chain, int value)
{
	std::size_t	index;

	index = 0;
	while (index < chain.size())
	{
		if (chain[index] == value)
			return (static_cast<int>(index));
		index++;
	}
	return (-1);
}

/**
 * @brief Finds position of a value in deque chain.
 * @param chain Data sequence.
 * @param value Value to search.
 * @return Found index or -1.
 */
int	PmergeMe::findPositionDeque(const std::deque<int> &chain, int value)
{
	std::size_t	index;

	index = 0;
	while (index < chain.size())
	{
		if (chain[index] == value)
			return (static_cast<int>(index));
		index++;
	}
	return (-1);
}

/**
 * @brief Builds ordered pairs from vector input.
 * @param data Source data.
 * @param hasOdd Set to true if one value is unpaired.
 * @param oddValue Stores the unpaired value.
 * @return Container of normalized pairs.
 */
std::vector<PmergeMe::Pair> PmergeMe::makePairsVector(const std::vector<int> &data, bool &hasOdd, int &oddValue)
{
	std::vector<Pair> pairs;
	Pair pair;
	std::size_t	index;

	hasOdd = false;
	oddValue = 0;
	index = 0;
	while (index + 1 < data.size())
	{
		if (data[index] <= data[index + 1])
		{
			pair.small = data[index];
			pair.big = data[index + 1];
		}
		else
		{
			pair.small = data[index + 1];
			pair.big = data[index];
		}
		pairs.push_back(pair);
		index += 2;
	}
	if (index < data.size())
	{
		hasOdd = true;
		oddValue = data[index];
	}
	return (pairs);
}

/**
 * @brief Builds ordered pairs from deque input.
 * @param data Source data.
 * @param hasOdd Set to true if one value is unpaired.
 * @param oddValue Stores the unpaired value.
 * @return Container of normalized pairs.
 */
std::deque<PmergeMe::Pair> PmergeMe::makePairsDeque(const std::deque<int> &data, bool &hasOdd, int &oddValue)
{
	std::deque<Pair> pairs;
	Pair pair;
	std::size_t	index;

	hasOdd = false;
	oddValue = 0;
	index = 0;
	while (index + 1 < data.size())
	{
		if (data[index] <= data[index + 1])
		{
			pair.small = data[index];
			pair.big = data[index + 1];
		}
		else
		{
			pair.small = data[index + 1];
			pair.big = data[index];
		}
		pairs.push_back(pair);
		index += 2;
	}
	if (index < data.size())
	{
		hasOdd = true;
		oddValue = data[index];
	}
	return (pairs);
}

/**
 * @brief Sorts vector pairs by their larger element.
 * @param pairs Input pair sequence.
 * @return Sorted pair sequence.
 */
std::vector<PmergeMe::Pair> PmergeMe::sortPairsVector(const std::vector<Pair> &pairs)
{
	std::vector<Pair> left;
	std::vector<Pair> right;
	std::vector<Pair> result;
	std::size_t	mid;
	std::size_t	index;
	std::size_t	leftIndex;
	std::size_t	rightIndex;

	if (pairs.size() <= 1)
		return (pairs);
	mid = pairs.size() / 2;
	index = 0;
	while (index < mid)
	{
		left.push_back(pairs[index]);
		index++;
	}
	while (index < pairs.size())
	{
		right.push_back(pairs[index]);
		index++;
	}
	left = sortPairsVector(left);
	right = sortPairsVector(right);
	leftIndex = 0;
	rightIndex = 0;
	while (leftIndex < left.size() && rightIndex < right.size())
	{
		if (left[leftIndex].big <= right[rightIndex].big)
		{
			result.push_back(left[leftIndex]);
			leftIndex++;
		}
		else
		{
			result.push_back(right[rightIndex]);
			rightIndex++;
		}
	}
	while (leftIndex < left.size())
	{
		result.push_back(left[leftIndex]);
		leftIndex++;
	}
	while (rightIndex < right.size())
	{
		result.push_back(right[rightIndex]);
		rightIndex++;
	}
	return (result);
}

/**
 * @brief Sorts deque pairs by their larger element.
 * @param pairs Input pair sequence.
 * @return Sorted pair sequence.
 */
std::deque<PmergeMe::Pair> PmergeMe::sortPairsDeque(const std::deque<Pair> &pairs)
{
	std::deque<Pair> left;
	std::deque<Pair> right;
	std::deque<Pair> result;
	std::size_t	mid;
	std::size_t	index;
	std::size_t	leftIndex;
	std::size_t	rightIndex;

	if (pairs.size() <= 1)
		return (pairs);
	mid = pairs.size() / 2;
	index = 0;
	while (index < mid)
	{
		left.push_back(pairs[index]);
		index++;
	}
	while (index < pairs.size())
	{
		right.push_back(pairs[index]);
		index++;
	}
	left = sortPairsDeque(left);
	right = sortPairsDeque(right);
	leftIndex = 0;
	rightIndex = 0;
	while (leftIndex < left.size() && rightIndex < right.size())
	{
		if (left[leftIndex].big <= right[rightIndex].big)
		{
			result.push_back(left[leftIndex]);
			leftIndex++;
		}
		else
		{
			result.push_back(right[rightIndex]);
			rightIndex++;
		}
	}
	while (leftIndex < left.size())
	{
		result.push_back(left[leftIndex]);
		leftIndex++;
	}
	while (rightIndex < right.size())
	{
		result.push_back(right[rightIndex]);
		rightIndex++;
	}
	return (result);
}

/**
 * @brief Builds Jacobsthal-based insertion order for vector mode.
 * @param lastIndex Last pair index to insert.
 * @return Sequence of pair indices.
 */
std::vector<int> PmergeMe::buildInsertionOrderVector(int lastIndex)
{
	std::vector<int> order;
	std::vector<int> jacob;
	int	next;
	int	groupStart;
	int	groupEnd;
	int	value;
	std::size_t	index;

	if (lastIndex < 1)
		return (order);
	jacob.push_back(1);
	jacob.push_back(3);
	while (jacob.back() < lastIndex)
	{
		next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
		jacob.push_back(next);
	}
	groupStart = 1;
	index = 1;
	while (index < jacob.size())
	{
		groupEnd = jacob[index];
		if (groupEnd > lastIndex)
			groupEnd = lastIndex;
		value = groupEnd;
		while (value >= groupStart)
		{
			order.push_back(value);
			value--;
		}
		groupStart = jacob[index] + 1;
		index++;
	}
	if (groupStart <= lastIndex)
	{
		value = lastIndex;
		while (value >= groupStart)
		{
			order.push_back(value);
			value--;
		}
	}
	return (order);
}

/**
 * @brief Builds Jacobsthal-based insertion order for deque mode.
 * @param lastIndex Last pair index to insert.
 * @return Sequence of pair indices.
 */
std::deque<int>	PmergeMe::buildInsertionOrderDeque(int lastIndex)
{
	std::deque<int>	order;
	std::deque<int>	jacob;
	int	next;
	int	groupStart;
	int	groupEnd;
	int	value;
	std::size_t	index;

	if (lastIndex < 1)
		return (order);
	jacob.push_back(1);
	jacob.push_back(3);
	while (jacob.back() < lastIndex)
	{
		next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
		jacob.push_back(next);
	}
	groupStart = 1;
	index = 1;
	while (index < jacob.size())
	{
		groupEnd = jacob[index];
		if (groupEnd > lastIndex)
			groupEnd = lastIndex;
		value = groupEnd;
		while (value >= groupStart)
		{
			order.push_back(value);
			value--;
		}
		groupStart = jacob[index] + 1;
		index++;
	}
	if (groupStart <= lastIndex)
	{
		value = lastIndex;
		while (value >= groupStart)
		{
			order.push_back(value);
			value--;
		}
	}
	return (order);
}

/**
 * @brief Sorts data with Ford-Johnson algorithm using vector.
 * @param data Input sequence.
 * @return Sorted sequence.
 */
std::vector<int> PmergeMe::fordJohnsonVector(const std::vector<int> &data)
{
	std::vector<Pair> pairs;
	std::vector<int> order;
	std::vector<int> chain;
	bool hasOdd;
	int	oddValue;
	int	lastIndex;
	std::size_t	index;
	int	pairIndex;
	int	bigPos;
	int	insertPos;

	if (data.size() <= 1)
		return (data);
	pairs = makePairsVector(data, hasOdd, oddValue);
	pairs = sortPairsVector(pairs);
	chain.push_back(pairs[0].small);
	chain.push_back(pairs[0].big);
	index = 1;
	while (index < pairs.size())
	{
		chain.push_back(pairs[index].big);
		index++;
	}
	lastIndex = static_cast<int>(pairs.size()) - 1;
	order = buildInsertionOrderVector(lastIndex);
	index = 0;
	while (index < order.size())
	{
		pairIndex = order[index];
		bigPos = findPositionVector(chain, pairs[pairIndex].big);
		if (bigPos < 0)
			throw std::runtime_error("Error");
		insertPos = lowerBoundVector(chain, bigPos, pairs[pairIndex].small);
		chain.insert(chain.begin() + insertPos, pairs[pairIndex].small);
		index++;
	}
	if (hasOdd)
	{
		insertPos = lowerBoundVector(
				chain,
				static_cast<int>(chain.size()),
				oddValue);
		chain.insert(chain.begin() + insertPos, oddValue);
	}
	return (chain);
}

/**
 * @brief Sorts data with Ford-Johnson algorithm using deque.
 * @param data Input sequence.
 * @return Sorted sequence.
 */
std::deque<int>	PmergeMe::fordJohnsonDeque(const std::deque<int> &data)
{
	std::deque<Pair> pairs;
	std::deque<int>	order;
	std::deque<int>	chain;
	bool hasOdd;
	int	oddValue;
	int	lastIndex;
	std::size_t	index;
	int	pairIndex;
	int	bigPos;
	int	insertPos;

	if (data.size() <= 1)
		return (data);
	pairs = makePairsDeque(data, hasOdd, oddValue);
	pairs = sortPairsDeque(pairs);
	chain.push_back(pairs[0].small);
	chain.push_back(pairs[0].big);
	index = 1;
	while (index < pairs.size())
	{
		chain.push_back(pairs[index].big);
		index++;
	}
	lastIndex = static_cast<int>(pairs.size()) - 1;
	order = buildInsertionOrderDeque(lastIndex);
	index = 0;
	while (index < order.size())
	{
		pairIndex = order[index];
		bigPos = findPositionDeque(chain, pairs[pairIndex].big);
		if (bigPos < 0)
			throw std::runtime_error("Error");
		insertPos = lowerBoundDeque(chain, bigPos, pairs[pairIndex].small);
		chain.insert(chain.begin() + insertPos, pairs[pairIndex].small);
		index++;
	}
	if (hasOdd)
	{
		insertPos = lowerBoundDeque(
				chain,
				static_cast<int>(chain.size()),
				oddValue);
		chain.insert(chain.begin() + insertPos, oddValue);
	}
	return (chain);
}

/**
 * @brief Checks whether vector data is nondecreasing.
 * @param data Sequence to validate.
 * @return True if sorted, otherwise false.
 */
bool PmergeMe::isSortedVector(const std::vector<int> &data) const
{
	std::size_t	index;

	if (data.size() <= 1)
		return (true);
	index = 1;
	while (index < data.size())
	{
		if (data[index - 1] > data[index])
			return (false);
		index++;
	}
	return (true);
}

/**
 * @brief Checks whether deque data is nondecreasing.
 * @param data Sequence to validate.
 * @return True if sorted, otherwise false.
 */
bool PmergeMe::isSortedDeque(const std::deque<int> &data) const
{
	std::size_t	index;

	if (data.size() <= 1)
		return (true);
	index = 1;
	while (index < data.size())
	{
		if (data[index - 1] > data[index])
			return (false);
		index++;
	}
	return (true);
}

/**
 * @brief Runs sorting for both containers and prints timing.
 */
void PmergeMe::process(void)
{
	std::vector<int> sortedVector;
	std::deque<int>	sortedDeque;
	clock_t	startVec;
	clock_t	endVec;
	clock_t	startDeq;
	clock_t	endDeq;
	double timeVec;
	double timeDeq;

	printBefore();
	startVec = clock();
	sortedVector = fordJohnsonVector(vectorData);
	endVec = clock();
	startDeq = clock();
	sortedDeque = fordJohnsonDeque(dequeData);
	endDeq = clock();
	if (!isSortedVector(sortedVector) || !isSortedDeque(sortedDeque))
		throw std::runtime_error("Error");
	printAfter(sortedVector);
	timeVec = static_cast<double>(endVec - startVec)
		/ CLOCKS_PER_SEC * 1000000.0;
	timeDeq = static_cast<double>(endDeq - startDeq)
		/ CLOCKS_PER_SEC * 1000000.0;
	std::cout << "Time to process a range of "
		<< vectorData.size()
		<< " elements with std::vector : "
		<< timeVec << " us" << std::endl;
	std::cout << "Time to process a range of "
		<< dequeData.size()
		<< " elements with std::deque : "
		<< timeDeq << " us" << std::endl;
}