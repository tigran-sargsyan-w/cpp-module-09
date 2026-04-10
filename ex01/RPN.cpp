#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

/**
 * @brief Constructs an empty RPN evaluator.
 */
RPN::RPN() {}

/**
 * @brief Copy-constructs an RPN evaluator.
 * @param other Source object to copy from.
 */
RPN::RPN(const RPN& other) : numbers(other.numbers) {}

/**
 * @brief Assigns evaluator state from another object.
 * @param other Source object to assign from.
 * @return Reference to this object.
 */
RPN&	RPN::operator=(const RPN& other)
{
	if (this != &other)
		numbers = other.numbers;
	return (*this);
}

/**
 * @brief Destroys the RPN evaluator.
 */
RPN::~RPN() {}

/**
 * @brief Checks whether a token is a supported operator.
 * @param token Token to check.
 * @return True if token is an operator, otherwise false.
 */
bool	RPN::isOperator(const std::string& token) const
{
	return (token == "+" || token == "-" || token == "*"
		|| token == "/");
}

/**
 * @brief Checks whether a token is a single digit.
 * @param token Token to check.
 * @return True if token is a single decimal digit.
 */
bool	RPN::isDigitToken(const std::string& token) const
{
	return (token.length() == 1 && std::isdigit(token[0]));
}

/**
 * @brief Applies an arithmetic operation to two operands.
 * @param left Left operand.
 * @param right Right operand.
 * @param op Operator token.
 * @return Result of the operation.
 */
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

/**
 * @brief Evaluates an expression written in reverse polish notation.
 * @param expression Input RPN expression.
 * @return Final evaluation result.
 */
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