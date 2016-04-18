#include <iostream>
#include <stack>
#include "stack.hpp"
#include "../common/class-bint.hpp"
#include "list.hpp"

sjtu::stack<int, sjtu::list> stackInt;
sjtu::stack<Util::Bint, sjtu::list> stackBint;

int main()
{
	for (int i = 1; i <= 100; ++i) {
		stackInt.push(i);
		stackBint.push(Util::Bint(i));
	}
	sjtu::stack<Util::Bint, sjtu::list> duplicateStackBint(stackBint);
	sjtu::stack<int, sjtu::list> duplicateStackInt;
	duplicateStackInt = stackInt;
	std::cout << "a stack for int" << std::endl; 
	while (!stackInt.empty()) {
		std::cout << stackInt.top() << " ";
		stackInt.pop();
	}
	std::cout << std::endl;
	std::cout << "a stack for bint" << std::endl;
	while (!stackBint.empty()) {
		std::cout << stackBint.top() << " ";
		stackBint.pop();
	}
	std::cout << std::endl;
	std::cout << "a stack for int" << std::endl; 
	while (!duplicateStackInt.empty()) {
		std::cout << duplicateStackInt.top() << " ";
		duplicateStackInt.pop();
	}
	std::cout << std::endl;
	std::cout << "a stack for bint" << std::endl;
	while (!duplicateStackBint.empty()) {
		std::cout << duplicateStackBint.top() << " ";
		duplicateStackBint.pop();
	}
	std::cout << std::endl;
}