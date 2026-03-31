#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN& other) : numbers(other.numbers) {}

RPN&	RPN::operator=(const RPN& other)
{
	if (this != &other)
		numbers = other.numbers;
	return (*this);
}

RPN::~RPN() {}

bool	RPN::isOperator(const std::string& token) const
{
	return (token == "+" || token == "-" || token == "*"
		|| token == "/");
}

bool	RPN::isDigitToken(const std::string& token) const
{
	return (token.length() == 1 && std::isdigit(token[0]));
}

int	RPN::applyOperation(int left, int right, const std::string& op) const
{
	if (op == "+")
		return (left + right);
	if (op == "-")
		return (left - right);
	if (op == "*")
		return (left * right);
	if (op == "/")
	{
		if (right == 0)
			throw std::runtime_error("division by zero");
		return (left / right);
	}
	throw std::runtime_error("invalid operator");
}

int	RPN::evaluate(const std::string& expression)
{
	std::istringstream	stream(expression);
	std::string			token;
	int					left;
	int					right;
	int					result;

	while (stream >> token)
	{
		if (isDigitToken(token))
			numbers.push(token[0] - '0');
		else if (isOperator(token))
		{
			if (numbers.size() < 2)
				throw std::runtime_error("not enough operands");
			right = numbers.top();
			numbers.pop();
			left = numbers.top();
			numbers.pop();
			result = applyOperation(left, right, token);
			numbers.push(result);
		}
		else
			throw std::runtime_error("invalid token");
	}
	if (numbers.size() != 1)
		throw std::runtime_error("invalid expression");
	return (numbers.top());
}