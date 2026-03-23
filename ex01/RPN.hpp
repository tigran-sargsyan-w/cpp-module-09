#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
	private:
		std::stack<int> numbers;

		bool	isOperator(const std::string& token) const;
		bool	isDigitToken(const std::string& token) const;
		int		applyOperation(int left, int right, const std::string& op) const;

	public:
		RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();

		int		evaluate(const std::string& expression);
};

#endif