#ifndef SJTU_UNORDERED_MAP_HPP
#define SJTU_UNORDERED_MAP_HPP

#include <functional>
#include <cstddef>

namespace sjtu {

template<
	class Key, 
	class T,
	class Hash = std::hash<Key>,
	class KeyEqual = std::equal_to<Key>
> class unordered_map {
	class const_iterator;
	class iterator {

	};
	class const_iterator {

	};
public:
	/**
	 * TODO Constructor
	 */
	unordered_map() {}
	unordered_map(const unordered_map<Key, T, Hash, KeyEqual> &other) {}
	/**
	 * TODO Destructor
	 */
	~unordered_map() {}
	/**
	 * TODO assignment operator
	 */
	unordered_map<Key, T, Hash, KeyEqual> &operator=(const unordered_map<Key, T, Hash, KeyEqual> &other) {}
	/**
	 * returns an iterator to the beginning
	 */
	iterator begin() {}
	const_iterator cbegin() {}
	/**
	 * returns an iterator to the end
	 */
	iterator end() {}
	const_iterator cend() {}
};
}

#endif