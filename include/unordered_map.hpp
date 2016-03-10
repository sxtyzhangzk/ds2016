#ifndef SJTU_UNORDERED_MAP_HPP
#define SJTU_UNORDERED_MAP_HPP

#include <functional>
#include <cstddef>
#include "map.hpp"

namespace sjtu {

template<
	class Key, 
	class T,
	class Hash = std::hash<Key>,
	class KeyEqual = std::equal_to<Key>
> class unordered_map {
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 */
	class value_type {

	};
	/**
	 * see BidirectionalIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		T& operator*() const{}
		bool operator==(const iterator &rhs) {}
		bool operator==(const const_iterator &rhs) {}
	};
	class const_iterator {

	};
	/**
	 * iterators for a certain hash value elements, behave like list::iterator
	 */
	class local_iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		T& operator*() const{}
		bool operator==(const iterator &rhs) {}
		bool operator==(const const_iterator &rhs) {}	
	};
	class const_local_iterator {

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
	/**
	 * checks whether the conatiner is empty.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {}
	/**
	 * clears the contents.
	 */
	void clear() {}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {}
	/**
	 * erase the element at pos.
	 */
	void erase(iterator pos) {}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count() {}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {}
	const_iterator find(const Key &key) const {}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {}
	const T & at(const Key &key) const {}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {}
	/**
	 * bucket interfaces
	 */
	/**
	 * returns an iterator to the beginning of the specified bucket.
	 */
	local_iterator begin(const size_t &n) {}
	const_local_iterator cbegin(const size_t &n) const {}
	/**
	 * returns an iterator to the end of the specified bucket.
	 */
	local_iterator end(const size_t &n) {}
	const_local_iterator cend(const size_t &n) const {}
	/**
	 * returns the number of buckets.
	 */
	size_t bucket_count() const {}
	/**
	 * returns the number of elements in specific bucket.
	 */
	size_t bucket_size() const {}
	/**
	 * returns average number of elements per buckets.
	 */
	size_t load_factor() const {}
};
}

#endif