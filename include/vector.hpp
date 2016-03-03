#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <climits>
#include <cstddef>

namespace sjtu{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
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
		iterator operator+(const int &n) {
			//TODO
		}
		iterator operator-(const int &n) {
			//TODO
		}
		int operator-(const iterator &rhs) {
			//TODO
		}
		iterator operator+=(const int &n) {
			//TODO
		}
		iterator operator-=(const int &n) {
			//TODO
		}
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
		 * TODO *T
		 */
		T& operator*() const{}
	};
	/**
	 * TODO
	 */
	class const_iterator {

	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector() {}
	/**
	 * TODO Destructor
	 */
	~vector() {}

	/**
	 * TODO Assignment operator
	 */
	vector& operator=(const vector<T> &x) {}
	/**
	 * TODO Appends the specified element to the end of this list.
	 * throw runtime_error if failed.
	 */
	void push_back(const T &e) {}
	/**
	 * TODO Appends n specified elements to the end.
	 * @param e specified elements
	 * @param n the number you should append
	 */
	void push_back(const T &e, const size_t &n) {}
	/**
	 * TODO insert an element and the element inserted will 
	 *   become the index-th element after inserting.
	 * the index is zero-basec, with range [0, size]
	 * throw if index is invalid
	 */
	void insert(size_t index, const T &e) {}
	/**
	 * TODO Removes all of the elements from this list.
	 */
	void clear() {}
	/**
	 * TODO Returns the index if this list contains the specified element,
	 *   Otherwise returns INT_MAX
	 */
	size_t find(const T& e) const {}
	/**
	 * TODO Returns true if this list contains no elements.
	 */
	bool empty() const {}
	/**
	 * TODO Returns a const reference to the element at the specified position in this list.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	T& at(const size_t &index) {}
	const T& at(const size_t &index) const {}
	/**
	 * TODO Remove a element at index ind, i.e., a[ind] = a[ind + 1], a[ind + 1] = a[ind + 2],...
	 */
	void erase(const size_t &ind) {}
	/**
	 * TODO Remove the element which the iterator point to
	 * throw invaild_iterator if the iterator points to a invaild place.
	 */
	void erase(const iterator &it) {}
	/**
	 * some functions
	 * It should have the same meanings as STL
	 */
	iterator begin() {}
	iterator end() {}
	const_iterator begin() const {}
	const_iterator end() const {}
	/**
	 * TODO 
	 */
	T& operator[](const size_t &index) {}
	const T& operator[](const size_t &index) const {}
};


}

#endif