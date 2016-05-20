#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <cassert>

namespace sjtu
{

template<class T>
//typedef int T;
class deque
{
protected:
	struct node;
	struct block;

public:
	class const_iterator
	{
		friend class deque;
	public:
		const_iterator() : deq(nullptr), now(nullptr), pos(0) {}

		const_iterator operator+(int n) const
		{
			const_iterator iter = *this;
			iter.move(n);
			return iter;
		}
		const_iterator operator-(int n) const
		{
			const_iterator iter = *this;
			iter.move(-n);
			return iter;
		}
		int operator-(const const_iterator &rhs) const
		{
			if (deq != rhs.deq)
				throw invalid_iterator();
			return pos - rhs.pos;
		}
		const_iterator & operator+=(int n)
		{
			move(n);
			return *this;
		}
		const_iterator & operator-=(int n)
		{
			move(-n);
			return *this;
		}
		const_iterator & operator++()
		{
			if (!now)
				throw runtime_error();
			now = now->next;
			++pos;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator iter = *this;
			++(*this);
			return iter;
		}
		const_iterator & operator--()
		{
			if (!now)
			{
				if (!deq || deq->empty())
					throw runtime_error();
				now = deq->btail->tail;
				pos = deq->ecount - 1;
				return *this;
			}
			now = now->prev;
			--pos;
			if (!now)
				throw runtime_error();
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator iter = *this;
			--(*this);
			return iter;
		}

		const T & operator*() const
		{
			if (!now)
				throw runtime_error();
			return now->val;
		}
		const T * operator->() const noexcept
		{
			return &now->val;
		}

		bool operator==(const const_iterator &rhs) const
		{
			return now == rhs.now && deq == rhs.deq;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return now != rhs.now || deq != rhs.deq;
		}


	protected:
		const deque *deq;
		node *now;
		size_t pos;

	protected:
		const_iterator(const deque *deq, node *now, size_t pos) :
			deq(deq), now(now), pos(pos) 
		{ }

		void move(int offset)
		{
			if (offset == 0)
				return;
			if (!now)
			{
				if (!deq)
					throw invalid_iterator();
				pos = deq->ecount + offset;
				now = deq->find_node(pos);
				return;
			}
			//std::cerr << offset << " ";
			pos += offset;
			if (pos == deq->ecount)
			{
				now = nullptr;
				return;
			}
			if (offset > IterThreshold || -offset < IterThreshold)
			{
				now = deq->find_node(pos);
				return;
			}
			if (offset > 0)
			{
				for (; now && !(now->type & Ttail) && offset > 0; offset--)
					now = now->next;
				if (offset == 0)
					return;
				if (!now)
					throw runtime_error();
				--offset;
				block *cur_block = now->parent->next;
				while (cur_block && offset >= cur_block->elecount)
					offset -= cur_block->elecount, cur_block = cur_block->next;
				if (!cur_block)
				{
					if (offset == 0)
					{
						now = nullptr;
						return;
					}
					throw runtime_error();
				}
				if (offset <= cur_block->elecount - offset - 1)
					now = cur_block->head;
				else
					now = cur_block->tail, offset -= cur_block->elecount - 1;
			}
			else
			{
				for (; offset < 0 && now && !(now->type & Thead); offset++)
					now = now->prev;
				if (!now)
					throw runtime_error();
				if (offset == 0)
					return;
				++offset;
				block *cur_block = now->parent->prev;
				while (cur_block && -offset >= cur_block->elecount)
					offset += cur_block->elecount, cur_block = cur_block->prev;
				if (!cur_block)
					throw runtime_error();
				if (-offset <= cur_block->elecount + offset - 1)
					now = cur_block->tail;
				else
					now = cur_block->head, offset += cur_block->elecount - 1;
			}
			//std::cerr << offset << std::endl;
			if (offset > 0)
			{
				for (; offset > 0 && now; offset--)
					now = now->next;
				if (offset == 0)
					return;
				if (!now)
					throw runtime_error();
			}
			else if (offset < 0)
			{
				for (; offset < 0 && now; offset++)
					now = now->prev;
				if (!now)
					throw runtime_error();
			}
		}
	};
	class iterator : public const_iterator
	{
		friend class deque;
	public:
		iterator() = default;
		/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, the behaviour is **undefined**.
			* as well as operator-
			*/
		iterator operator+(int n) const
		{
			iterator iter = *this;
			iter.move(n);
			return iter;
		}
		iterator operator-(int n) const
		{
			iterator iter = *this;
			iter.move(-n);
			return iter;
		}
		int operator-(const iterator &rhs) const
		{
			if (this->deq != rhs.deq)
				throw invalid_iterator();
			return this->pos - rhs.pos;
		}

		iterator & operator+=(int n)
		{
			this->move(n);
			return *this;
		}
		iterator & operator-=(int n)
		{
			this->move(-n);
			return *this;
		}
		iterator operator++(int)
		{
			iterator iter = *this;
			++(*(const_iterator *)this);
			return iter;
		}
		iterator & operator++()
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
		iterator & operator--()
		{
			--(*(const_iterator *)this);
			return *this;
		}

		T & operator*() const
		{
			if (!this->now)
				throw runtime_error();
			return this->now->val;
		}
		T* operator->() const noexcept
		{
			return &this->now->val;
		}

	protected:
		iterator(const deque *deq, node *now, size_t pos) :
			const_iterator(deq, now, pos)
		{}
	};

	deque() : ecount(0), blocksize(MinBlockSize), bhead(nullptr), btail(nullptr), lastq(new Tlastquery) {}
	deque(const deque &other) : ecount(0), bhead(nullptr), btail(nullptr), lastq(new Tlastquery)
	{
		clone(other);
	}
	~deque()
	{
		clear();
		delete lastq;
	}

	deque &operator=(const deque &other)
	{
		if (this == &other)
			return *this;
		clear();
		clone(other);
		return *this;
	}
	/**
		* access specified element with bounds checking
		* throw index_out_of_bound if out of bound.
		*/
	T & at(const size_t &pos)
	{
		return find_node(pos)->val;
	}
	const T & at(const size_t &pos) const
	{
		return find_node(pos)->val;
	}
	T & operator[](const size_t &pos)
	{
		return find_node(pos)->val;
	}
	const T & operator[](const size_t &pos) const
	{
		return find_node(pos)->val;
	}
	/**
		* access the first element
		* throw container_is_empty when the container is empty.
		*/
	const T & front() const
	{
		if (empty())
			throw container_is_empty();
		return bhead->head->val;
	}
	/**
		* access the last element
		* throw container_is_empty when the container is empty.
		*/
	const T & back() const
	{
		if (empty())
			throw container_is_empty();
		return btail->tail->val;
	}
	/**
		* returns an iterator to the beginning.
		*/
	iterator begin()
	{
		if (empty())
			return end();
		return iterator(this, bhead->head, 0);
	}
	const_iterator cbegin() const
	{
		if (empty())
			return cend();
		return const_iterator(this, bhead->head, 0);
	}
	/**
		* returns an iterator to the end.
		*/
	iterator end()
	{
		return iterator(this, nullptr, ecount);
	}
	const_iterator cend() const 
	{
		return const_iterator(this, nullptr, ecount);
	}
	/**
		* checks whether the container is empty.
		*/
	bool empty() const
	{
		return ecount == 0;
	}
	/**
		* returns the number of elements
		*/
	size_t size() const
	{
		return ecount;
	}
	/**
		* clears the contents
		*/
	void clear()
	{
		if (ecount == 0)
			return;
		node *o = bhead->head;
		while (o)
		{
			//std::cerr << o << std::endl;
			node *tmp = o;
			o = o->next;
			delete tmp;
		}
		block *blk = bhead;
		while (blk)
		{
			block *tmp = blk;
			blk = blk->next;
			delete tmp;
		}
		ecount = 0;
		bhead = btail = nullptr;
		lastq->ans = nullptr;
	}
	/**
		* inserts elements at the specified locat on in the container.
		* inserts value before pos
		* returns an iterator pointing to the inserted value
		*     throw if the iterator is invalid or it point to a wrong place.
		*/
	iterator insert(iterator pos, const T &value)
	{
		if (pos.deq != this)
			throw invalid_iterator();
		if (!pos.now)
		{
			push_back(value);
			return iterator(this, btail->tail, ecount - 1);
		}
		if (pos.now == bhead->head)
		{
			push_front(value);
			return iterator(this, bhead->head, 0);
		}
		node *o = pos.now;
		//assert(o->prev);
		node *tmp = new node(value, o->parent, Tnormal);

		o->prev->next = tmp;
		tmp->prev = o->prev;
		tmp->next = o;
		o->prev = tmp;

		if (o->type & Thead)
		{
			tmp->type = Thead;
			o->type &= ~Thead;
			o->parent->head = tmp;
		}
		
		++o->parent->elecount;
		++ecount;
		update_bsize();

		if (tmp->parent->elecount >= 2 * blocksize)
			split(tmp->parent);

		lastq->ans = nullptr;

		return iterator(this, tmp, pos.pos);
	}
	/**
		* removes specified element at pos.
		* removes the element at pos.
		* returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
		* throw if the container is empty, the iterator is invalid or it points to a wrong place.
		*/
	iterator erase(iterator pos)
	{
		if (pos.deq != this || !pos.now)
			throw invalid_iterator();
		if (empty())
			throw container_is_empty();
		if (pos.now == btail->tail)
		{
			pop_back();
			return end();
		}
		if (pos.now == bhead->head)
		{
			pop_front();
			return begin();
		}
		node *o = pos.now;
		//assert(o->prev && o->next);
		o->prev->next = o->next;
		o->next->prev = o->prev;
		block *parent = o->parent;

		if (parent->elecount > 1)
		{
			if (o->type & Thead)
			{
				parent->head = o->next;
				o->next->type |= Thead;
			}
			else if (o->type & Ttail)
			{
				parent->tail = o->prev;
				o->prev->type |= Ttail;
			}
			--parent->elecount;
			if (parent->prev && parent->prev->elecount + parent->elecount <= blocksize)
				parent = merge(parent->prev);
			if (parent->next && parent->next->elecount + parent->elecount <= blocksize)
				parent = merge(parent);
		}
		else
		{
			//assert(parent->prev && parent->next);
			parent->prev->next = parent->next;
			parent->next->prev = parent->prev;
			if (parent->prev->elecount + parent->next->elecount <= blocksize)
				merge(parent->prev);
			delete parent;
		}
		node *tmp = o->next;
		delete o;
		--ecount;
		update_bsize();
		lastq->ans = nullptr;
		return iterator(this, tmp, pos.pos);
	}
	/**
		* adds an element to the end
		*/
	void push_back(const T &value)
	{
		if (!btail)
		{
			//assert(!bhead);
			//assert(ecount == 0);
			btail = bhead = new block();
			bhead->head = bhead->tail = new node(value, btail, Thead | Ttail);
			bhead->elecount = 1;
		}
		else if (btail->elecount < blocksize)
		{
			node *tmp = new node(value, btail, Ttail);
			btail->tail->type &= ~Ttail;
			btail->tail->next = tmp;
			tmp->prev = btail->tail;
			btail->tail = tmp;
			++btail->elecount;
		}
		else
		{
			block *tmpblock = new block();
			node *tmp = new node(value, tmpblock, Thead | Ttail);
			tmpblock->head = tmpblock->tail = tmp;
			btail->next = tmpblock;
			btail->tail->next = tmp;
			tmp->prev = btail->tail;
			tmpblock->prev = btail;
			btail = tmpblock;
			tmpblock->elecount = 1;
		}
		ecount++;
		update_bsize();
	}
	/**
		* removes the last element
		*     throw when the container is empty.
		*/
	void pop_back()
	{
		if (!btail)
			throw container_is_empty();
		node *k = btail->tail->prev;
		if (btail->elecount > 1)
		{
			//assert(k);
			//assert(k->parent == btail);
			k->next = nullptr;
			k->type |= Ttail;
			delete btail->tail;
			btail->tail = k;
			--btail->elecount;

			if (btail->prev && btail->prev->elecount + btail->elecount <= blocksize)
				merge(btail->prev);
		}
		else
		{
			if (k)
			{
				k->next = nullptr;
				//assert(btail->prev);
				btail->prev->next = nullptr;
			}
			delete btail->tail;
			block *tmp = btail;
			btail = btail->prev;
			if (!btail)
			{
				//assert(ecount == 1);
				bhead = nullptr;
			}
			delete tmp;
		}
		--ecount;
		update_bsize();
		lastq->ans = nullptr;
	}
	/**
		* inserts an element to the beginning.
		*/
	void push_front(const T &value)
	{
		if (!bhead)
		{
			//assert(!btail);
			//assert(ecount == 0);
			bhead = btail = new block();
			bhead->head = bhead->tail = new node(value, bhead, Thead | Ttail);
			bhead->elecount = 1;
		}
		else if (bhead->elecount < blocksize)
		{
			node *tmp = new node(value, bhead, Thead);
			bhead->head->type &= ~Thead;
			bhead->head->prev = tmp;
			tmp->next = bhead->head;
			bhead->head = tmp;
			++bhead->elecount;
		}
		else
		{
			block *tmpblock = new block();
			node *tmp = new node(value, tmpblock, Thead | Ttail);
			tmpblock->head = tmpblock->tail = tmp;
			bhead->prev = tmpblock;
			bhead->head->prev = tmp;
			tmp->next = bhead->head;
			tmpblock->next = bhead;
			bhead = tmpblock;
			tmpblock->elecount = 1;
		}
		++ecount;
		update_bsize();
		lastq->ans = nullptr;
	}
	/**
		* removes the first element.
		*     throw when the container is empty.
		*/
	void pop_front()
	{
		if (!bhead)
			throw container_is_empty();
		node *k = bhead->head->next;
		if (bhead->elecount > 1)
		{
			//assert(k);
			//assert(k->parent == bhead);
			k->prev = nullptr;
			k->type |= Thead;
			delete bhead->head;
			bhead->head = k;
			--bhead->elecount;
		}
		else
		{
			if (k)
			{
				k->prev = nullptr;
				//assert(bhead->next);
				bhead->next->prev = nullptr;
			}
			delete bhead->head;
			block *tmp = bhead;
			bhead = bhead->next;
			if (!bhead)
			{
				//assert(ecount == 1);
				btail = nullptr;
			}
			delete tmp;
		}
		--ecount;
		update_bsize();
		lastq->ans = nullptr;
	}

protected:
	static const int Tnormal = 0;
	static const int Thead = 1;
	static const int Ttail = 2;

	static const size_t MinBlockSize = 32;
	static const size_t IterThreshold = 100;

	struct node
	{
		T val;
		node *next, *prev;
		block *parent;
		int type;

		node(const T &val, block *parent, int type) : 
			val(val), next(nullptr), prev(nullptr), parent(parent), type(type) {}
	};
	struct block
	{
		size_t elecount;
		node *head, *tail;
		block *next, *prev;

		block() : elecount(0), head(nullptr), tail(nullptr), next(nullptr), prev(nullptr) {}
	};

	size_t ecount;
	size_t blocksize;
	block *bhead, *btail;

	struct Tlastquery
	{
		size_t query;
		node *ans;
		Tlastquery() : ans(nullptr) {}
	};
	Tlastquery *lastq;

protected:
	block * merge(block *o)
	{
		block *k = o->next;
		//assert(k);
		o->tail->type &= ~Ttail;
		k->head->type &= ~Thead;
		if (o->elecount >= k->elecount)
		{
			o->tail = k->tail;
			for (node *i = k->head, *j = k->tail->next; i != j; i = i->next)
				i->parent = o;
			o->next = k->next;
			if (k->next)
				k->next->prev = o;
			else
			{
				//assert(k == btail);
				btail = o;
			}
			o->elecount += k->elecount;
			delete k;
			return o;
		}
		else
		{
			//assert(o->tail->next == k->head);
			k->head = o->head;
			for (node *i = o->head, *j = o->tail->next; i != j; i = i->next)
				i->parent = k;
			k->prev = o->prev;
			if (o->prev)
				o->prev->next = k;
			else
			{
				//assert(o == bhead);
				bhead = k;
			}
			k->elecount += o->elecount;
			delete o;
			return k;
		}
	}

	void split(block *o)
	{
		node *mid = o->tail;
		block *k = new block();
		for (size_t i = 0; i < o->elecount / 2; i++)
			mid->parent = k, mid = mid->prev;
		//assert(mid->next && mid->next->parent == k && mid->parent == o);
		k->head = mid->next;
		k->tail = o->tail;
		o->tail = mid;
		mid->next->type |= Thead;
		mid->type |= Ttail;

		k->prev = o;
		k->next = o->next;
		if (o->next)
			o->next->prev = k;
		else
		{
			//assert(o == btail);
			btail = k;
		}
		o->next = k;
		k->elecount = o->elecount / 2;
		o->elecount -= k->elecount;
	}

	node * find_node(size_t pos) const
	{
		if (empty())
			throw container_is_empty();
		if (pos >= ecount || pos < 0)
			throw index_out_of_bound();

		//static int total = 0, hit = 0;
		//total++;
		if (lastq->ans)
		{
			//hit++;
			if (pos == lastq->query + 1)
				return lastq->query = pos, lastq->ans = lastq->ans->next;
			else if (pos == lastq->query - 1)
				return lastq->query = pos, lastq->ans = lastq->ans->prev;
			else if (pos == lastq->query)
				return lastq->ans;
			//hit--;
		}
		//std::cerr << hit << " / " << total << std::endl;
		lastq->query = pos;

		block *blk;
		size_t idx = 0;
		
		if (pos <= ecount / 2)
		{
			blk = bhead;
			while (idx + blk->elecount <= pos)
				idx += blk->elecount, blk = blk->next;
		}
		else
		{
			blk = btail;
			idx = ecount - btail->elecount;
			while (idx > pos)
				blk = blk->prev, idx -= blk->elecount;
		}
		if (pos - idx <= idx + blk->elecount - 1 - pos)
		{
			node *o = blk->head;
			while (idx < pos)
				o = o->next, ++idx;
			lastq->ans = o;
			return o;
		}
		else
		{
			node *o = blk->tail;
			idx += blk->elecount - 1;
			while (idx > pos)
				o = o->prev, --idx;
			lastq->ans = o;
			return o;
		}
	}

	void update_bsize()
	{
		if ((unsigned long long)blocksize * blocksize > ecount && blocksize >= 2 * MinBlockSize)
		{
			blocksize /= 2;
			for (block *i = bhead; i; i = i->next)
				if (i->elecount > 2 * blocksize)
					split(i);
		}
		else if ((unsigned long long)blocksize * blocksize < ecount / 16)
		{
			blocksize *= 2;
			block *i = bhead;
			while (i)
			{
				if (i->next && i->elecount + i->next->elecount <= blocksize)
					i = merge(i);
				i = i->next;
			}
		}
	}

	void clone(const deque &other)
	{
		//assert(empty());
		if (other.empty())
			return;
		blocksize = other.blocksize;
		ecount = other.ecount;
		block *tmpblock = new block();
		bhead = btail = tmpblock;
		node *now = other.bhead->head;
		node *last = nullptr;
		int i = 0;
		while (now)
		{
			node *tmp = new node(now->val, tmpblock, Tnormal);
			if (last)
			{
				last->next = tmp;
				tmp->prev = last;
			}
			++tmpblock->elecount;
			if (i == 0)
			{
				tmp->type |= Thead;
				tmpblock->head = tmp;
			}
			if (i == blocksize - 1 || !now->next)
			{
				tmp->type |= Ttail;
				tmpblock->tail = tmp;
				if (now->next)
				{
					tmpblock = new block();
					btail->next = tmpblock;
					tmpblock->prev = btail;
					btail = tmpblock;
				}
			}
			++i;
			if (i == blocksize)
				i = 0;
			last = tmp;
			now = now->next;
		}
	}
};

}

#endif