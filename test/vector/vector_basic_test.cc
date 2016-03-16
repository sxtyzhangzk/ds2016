#include <iostream>
#include <iomanip>
#include <vector>

int main(int argc, char const *argv[])
{
	/**
	 * test default constructor.
	 */
	std::vector<int> v;
	/**
	 * test push_back method.
	 */
	for (int i = 1; i <= 10; ++i) {
		v.push_back(i);
	}
	/**
	 * iterate all elements by index.
	 */
	for (size_t i = 0; i < v.size(); ++i) {
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
	/**
	 * iterate all elements by iterator.
	 */
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	/**
	 * test copy constructor.
	 */
	const std::vector<int> vb(v);
	/**
	 * test const_iterator for a constant object.
	 */
	for (std::vector<int>::const_iterator it = vb.cbegin(); it != vb.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	/**
	 * test boundary checking.
	 */
	try {
		std::cout << v.at(20) << std::endl;
	} catch (...) {
		std::cout << "Throw correctly" << std::endl;
	}
	return 0;
}