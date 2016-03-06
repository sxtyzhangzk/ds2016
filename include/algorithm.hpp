#ifndef SJTU_ALGORITHM_HPP
#define SJTU_ALGORITHM_HPP

#include "vector.hpp"
#include "list.hpp"

namespace sjtu {

template<class VecIter>
void sort(VecIter beg, VecIter end) {}

template<class VecIter, class Compare>
void sort(VecIter beg, VecIter end, Compare comp) {}

template<class ListIter>
void list_sort(ListIter beg, ListIter end) {}

template<class ListIter, class Compare>
void list_sort(ListIter beg, ListIter end, Compare comp) {}

template<class VecIter>
void make_heap(VecIter beg, VecIter end) {}

template<class VecIter, class Compare>
void make_heap(VecIter beg, VecIter end, Compare comp) {}

}

#endif