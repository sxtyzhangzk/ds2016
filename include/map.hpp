/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

 // only for std::less<T>
#include <functional>
#include <algorithm>
#include <cstddef>
#include <cassert>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu
{
	template<
		class Key,
		class T,
		class Compare = std::less<Key>
	>
	/*typedef int Key;
	typedef int T;
	typedef std::less<Key> Compare;*/
	class map
	{
		friend class const_iterator;
	protected:
		class node;
	public:
		/**
			* the internal type of data.
			* it should have a default constructor, a copy constructor.
			* You can use sjtu::map as value_type by typedef.
			*/
		typedef pair<const Key, T> value_type;
		/**
			* see BidirectionalIterator at CppReference for help.
			*
			* if there is anything wrong throw invalid_iterator.
			*     like it = map.begin(); --it;
			*       or it = map.end(); ++end();
			*/
		class const_iterator
		{
			friend class map;
		protected:
			const map *parent;
			node *cur;
		public:
			const_iterator() : parent(nullptr), cur(nullptr) {}
			const_iterator(const const_iterator &other) = default;
			const_iterator(const map *parent, node *cur) : parent(parent), cur(cur) {}

			/**
			 * return a new iterator which pointer n-next elements
			 *   even if there are not enough elements, just return the answer.
			 * as well as operator-
			 */
			/**
			 * iter++
			 */
			const_iterator operator++(int)
			{
				if (!cur)
					throw invalid_iterator();
				const_iterator iter = *this;
				cur = parent->find_next(cur->val.first, parent->root);
				return iter;
			}
			/**
			 * ++iter
			 */
			const_iterator& operator++()
			{
				if (!cur)
					throw invalid_iterator();
				cur = parent->find_next(cur->val.first, parent->root);
				return *this;
			}
			/**
			 * iter--
			 */
			const_iterator operator--(int)
			{
				const_iterator iter = *this;
				if (cur)
					cur = parent->find_prev(cur->val.first, parent->root);
				else
					cur = parent->find_tail(parent->root);
				if (!cur)
					throw invalid_iterator();
				return iter;
			}
			/**
			 * --iter
			 */
			const_iterator& operator--()
			{
				if (cur)
					cur = parent->find_prev(cur->val.first, parent->root);
				else
					cur = parent->find_tail(parent->root);
				if (!cur)
					throw invalid_iterator();
				return *this;
			}
			
			const value_type& operator*() const
			{
				return cur->val;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			bool operator==(const const_iterator &rhs) const
			{
				return cur == rhs.cur && parent == rhs.parent;
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return cur != rhs.cur || parent != rhs.parent;
			}

			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			const value_type* operator->() const noexcept
			{
				return &cur->val;
			}
		};
		class iterator : public const_iterator
		{
		public:
			iterator() {}
			iterator(map *parent, node *cur) : const_iterator(parent, cur) {}
			iterator operator++(int)
			{
				iterator iter = *this;
				++(*(const_iterator *)this);
				return iter;
			}
			iterator operator++()
			{
				++(*(const_iterator *)this);
				return *this;
			}
			iterator operator--(int)
			{
				iterator iter = *this;
				--(*(const_iterator *)this);
				return iter;
			}
			iterator operator--()
			{
				--(*(const_iterator *)this);
				return *this;
			}

			value_type & operator*() const
			{
				return this->cur->val;
			}
			value_type * operator->() const noexcept
			{
				return &this->cur->val;
			}
		};

	public:
		/**
		 * TODO two constructors
		 */
		map() : root(nullptr), elecount(0) {}
		map(const map &other) : elecount(other.elecount), root(nullptr)
		{
			root = clone_tree(other.root);
		}
		/**
		 * TODO assignment operator
		 */
		map &operator=(const map &other)
		{
			if (&other == this)
				return *this;
			clear();
			root = clone_tree(other.root);
			elecount = other.elecount;
			return *this;
		}
		/**
		 * TODO Destructors
		 */
		~map()
		{
			clear();
		}
		/**
		 * TODO
		 * access specified element with bounds checking
		 * Returns a reference to the mapped value of the element with key equivalent to key.
		 * If no such element exists, an exception of type `index_out_of_bound'
		 */
		T & at(const Key &key)
		{
			node *o = find_node(key, root);
			if (!o)
				throw index_out_of_bound();
			return o->val.second;
		}
		const T & at(const Key &key) const
		{
			node *o = find_node(key, root);
			if (!o)
				throw index_out_of_bound();
			return o->val.second;
		}
		/**
			* TODO
			* access specified element
			* Returns a reference to the value that is mapped to a key equivalent to key,
			*   performing an insertion if such key does not already exist.
			*/
		T & operator[](const Key &key)
		{
			node *o = find_node(key, root);
			if (!o)
				return insert(pair<Key, T>(key, T())).first->second;
			return o->val.second;
		}
		/**
			* return a iterator to the beginning
			*/
		iterator begin()
		{
			return iterator(this, find_head(root));
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, find_head(root));
		}
		/**
			* return a iterator to the end
			* in fact, it returns past-the-end.
			*/
		iterator end()
		{
			return iterator(this, nullptr);
		}
		const_iterator cend() const
		{
			return const_iterator(this, nullptr);
		}
		/**
			* checks whether the container is empty
			* return true if empty, otherwise false.
			*/
		bool empty() const { return !root; }
		/**
			* returns the number of elements.
			*/
		size_t size() const { return elecount; }
		/**
			* clears the contents
			*/
		void clear()
		{
			if (root)
			{
				delete_tree(root);
				root = nullptr;
				elecount = 0;
			}
		}
		/**
			* insert an element.
			* return a pair, the first of the pair is
			*   the iterator to the new element (or the element that prevented the insertion),
			*   the second one is true if insert successfully, or false.
			*/
		pair<iterator, bool> insert(const value_type &value)
		{
			node *o = find_node(value.first, root);
			if (o)
				return pair<iterator, bool>(iterator(this, o), false);
			o = new node(value);
			insert_node(o, root);
			++elecount;
			return pair<iterator, bool>(iterator(this, o), true);
		}
		/**
			* erase the element at pos.
			*
			* throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
			*/
		void erase(iterator pos)
		{
			if (pos.parent != this || !pos.cur)
				throw invalid_iterator();
			remove_node(pos.cur, root);
			delete pos.cur;
			--elecount;
		}
		/**
			* Returns the number of elements with key
			*   that compares equivalent to the specified argument,
			*   which is either 1 or 0
			*     since this container does not allow duplicates.
			* The default method of check the equivalence is !(a < b || b > a)
			*/
		size_t count(const Key &key) const
		{
			if (find_node(key, root))
				return 1;
			return 0;
		}
		/**
			* Finds an element with key equivalent to key.
			* key value of the element to search for.
			* Iterator to an element with key equivalent to key.
			*   If no such element is found, past-the-end (see end()) iterator is returned.
			*/
		iterator find(const Key &key)
		{
			node *o = find_node(key, root);
			if (!o)
				return end();
			return iterator(this, o);
		}
		const_iterator find(const Key &key) const
		{
			node *o = find_node(key, root);
			if (!o)
				return cend();
			return const_iterator(this, o);
		}

	protected:
		struct node
		{
			value_type val;
			node *lchild, *rchild;
			size_t height;

			node(const value_type& val) : 
				val(val), 
				lchild(nullptr), rchild(nullptr),  
				height(0) {}

			void maintain()
			{
				int hl = lchild ? lchild->height : -1;
				int hr = rchild ? rchild->height : -1;
				height = std::max(hl, hr) + 1;
			}
			int bval()
			{
				int hl = lchild ? lchild->height : -1;
				int hr = rchild ? rchild->height : -1;
				return hl - hr;
			}
		};

	protected:
		node *root;
		size_t elecount;

	protected:
		node * find_node(const Key &key, node *o) const
		{
			static Compare cmp;
			if (!o)
				return nullptr;
			if (cmp(o->val.first, key))
				return find_node(key, o->rchild);
			if (cmp(key, o->val.first))
				return find_node(key, o->lchild);
			return o;
		}

		static int height(node *o)
		{
			if (!o)
				return -1;
			o->maintain();
			return o->height;
		}

		void insert_node(node *new_node, node *&o)
		{
			static Compare cmp;
			if (!o)
			{
				o = new_node;
				return;
			}
			if (cmp(new_node->val.first, o->val.first))
			{
				insert_node(new_node, o->lchild);
				if (height(o->lchild) - height(o->rchild) == 2)
				{
					if (cmp(new_node->val.first, o->lchild->val.first))
						LL(o);
					else
						LR(o);
				}
			}
			else
			{
				insert_node(new_node, o->rchild);
				if (height(o->rchild) - height(o->lchild) == 2)
				{
					if (cmp(new_node->val.first, o->rchild->val.first))
						RL(o);
					else
						RR(o);
				}
			}
			o->maintain();
		}
		
		void remove_node(node *target, node *&o)
		{
			static Compare cmp;
			assert(o);
			if (o == target)
			{
				if (o->lchild && o->rchild)
				{
					node *k = find_next(target->val.first, o->rchild);
					assert(k);
					remove_node(k, o->rchild);
					k->lchild = o->lchild;
					k->rchild = o->rchild;
					o = k;
					o->maintain();
				}
				else
				{
					if (o->rchild)
						o = o->rchild;
					else if (o->lchild)
						o = o->lchild;
					else
						o = nullptr;
					return;
				}
			}
			else if (cmp(target->val.first, o->val.first))
				remove_node(target, o->lchild);
			else
				remove_node(target, o->rchild);
			o->maintain();
			int hl = height(o->lchild), hr = height(o->rchild);
			if (hl - hr == 2)
			{
				if (o->lchild->bval() >= 0)
					rotate_r(o);
				else
					LR(o);
			}
			else if (hr - hl == 2)
			{
				if (o->rchild->bval() <= 0)
					rotate_l(o);
				else
					RL(o);
			}
			o->maintain();
		}

		
		void delete_tree(node *o)
		{
			assert(o);
			if (o->lchild)
				delete_tree(o->lchild);
			if (o->rchild)
				delete_tree(o->rchild);
			delete o;
		}
		static node * clone_tree(node *o)
		{
			if (!o)
				return nullptr;
			node *k = new node(o->val);
			k->lchild = clone_tree(o->lchild);
			k->rchild = clone_tree(o->rchild);
			return k;
		}
		node * find_prev(const Key &key, node *o) const
		{
			static Compare cmp;
			if (!o)
				return nullptr;
			if (cmp(o->val.first, key))
			{
				node *k = find_prev(key, o->rchild);
				if (!k)
					return o;
				return k;
			}
			else
				return find_prev(key, o->lchild);
		}
		node * find_next(const Key &key, node *o) const
		{
			static Compare cmp;
			if (!o)
				return nullptr;
			if (cmp(key, o->val.first))
			{
				node *k = find_next(key, o->lchild);
				if (!k)
					return o;
				return k;
			}
			else
				return find_next(key, o->rchild);
		}
		void rotate_r(node *&o)
		{
			node *k = o->lchild;
			o->lchild = k->rchild;
			k->rchild = o;
			o->maintain();
			k->maintain();
			o = k;
		}
		void rotate_l(node *&o)
		{
			node *k = o->rchild;
			o->rchild = k->lchild;
			k->lchild = o;
			o->maintain();
			k->maintain();
			o = k;
		}
		void LL(node *&o) { rotate_r(o); }
		void RR(node *&o) { rotate_l(o); }
		void LR(node *&o) { rotate_l(o->lchild); rotate_r(o); }
		void RL(node *&o) { rotate_r(o->rchild); rotate_l(o); }

		node * find_head(node *o) const 
		{
			if (!o)
				return nullptr;
			while (o->lchild)
				o = o->lchild;
			return o;
		}
		node * find_tail(node *o) const
		{
			if (!o)
				return nullptr;
			while (o->rchild)
				o = o->rchild;
			return o;
		}
	};

}

#endif
