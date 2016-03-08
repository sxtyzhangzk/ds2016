#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <cstddef>

namespace sjtu { 

template<class T>
class deque {
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
		iterator operator+(const int &n) {
			//TODO
		}
		iterator operator-(const int &n) {
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
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
		 * TODO *it
		 */
		T& operator*() const{}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) {}
		bool operator==(const const_iterator &rhs) {}
	};
public:
	/**
	 * TODO Constructors
	 */
	deque() {}
	deque(const deque<T> &other) {}
	/**
	 * TODO Deconstructor
	 */
	~deque() {}
	/**
	 * TODO assignment operator
	 */
	deque<T> &operator=(const deque<T> &other) {}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {}
	const T & at(const size_t &pos) const {}
	T & operator[](const size_t &pos) {}
	const T & operator[](const size_t &pos) const {}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	void clear() {}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 */
	iterator insert(iterator pos, const T &value) {}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 */
	iterator erase(iterator pos) {}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {}
	/**
	 * removes the last element
	 */
	void pop_back() {}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {}
	/**
	 * removes the first element.
	 */
	void pop_front() {}
};

}

#endif