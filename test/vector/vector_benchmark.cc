#include "../../include/vector.hpp"

#include <iostream>

#include <vector>

int main()
{
	sjtu::vector<long long> v;
	for (long long i = 0; i < 1LL << 30; ++i) {
		v.push_back(i);
	}
	std::cout << v.back() << std::endl;
	return 0;
}