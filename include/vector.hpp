#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T> class vector {
  public:
    /**
     * TODO
     * a type for actions of the elements of a vector, and you should write
     *   a class named const_iterator with same interfaces.
     */
    /**
     * you can see RandomAccessIterator at CppReference for help.
     */
    class const_iterator;
    class iterator {
        // The following code is written for the C++ type_traits library.
        // Type traits is a C++ feature for describing certain properties of a
        // type. For instance, for an iterator, iterator::value_type is the type
        // that the iterator points to. STL algorithms and containers may use
        // these type_traits (e.g. the following typedef) to work properly. In
        // particular, without the following code,
        // @code{std::sort(iter, iter1);} would not compile.
        // See these websites for more information:
        // https://en.cppreference.com/w/cpp/header/type_traits
        // About value_type:
        // https://blog.csdn.net/u014299153/article/details/72419713 About
        // iterator_category: https://en.cppreference.com/w/cpp/iterator
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::output_iterator_tag;
        iterator(pointer _ptr = nullptr, const vector<T> *_owner = nullptr)
            : ptr(_ptr), owner(_owner) {}

      private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        pointer ptr;
        const vector<T> *owner;

      public:
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            // TODO
            return iterator(ptr + n, owner);
        }
        iterator operator-(const int &n) const {
            // TODO
            return iterator(ptr - n, owner);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw
        // invaild_iterator.
        int operator-(const iterator &rhs) const {
            // TODO
            if (owner != rhs.owner)
                throw invalid_iterator();
            return ptr - rhs.ptr;
        }
        iterator &operator+=(const int &n) {
            // TODO
            ptr += n;
            return *this;
        }
        iterator &operator-=(const int &n) {
            // TODO
            ptr -= n;
            return *this;
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            ptr++;
            return iterator(ptr - 1, owner);
        }
        /**
         * TODO ++iter
         */
        iterator &operator++() {
            ptr++;
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            ptr--;
            return iterator(ptr + 1, owner);
        }
        /**
         * TODO --iter
         */
        iterator &operator--() {
            ptr--;
            return *this;
        }
        /**
         * TODO *it
         */
        T &operator*() const { return *ptr; }
        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory address).
         */
        bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     */
    class const_iterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::output_iterator_tag;
        const_iterator(pointer _ptr = nullptr,
                       const vector<T> *_owner = nullptr)
            : ptr(_ptr), owner(_owner) {}
        const_iterator(iterator it) : ptr(it.ptr), owner(it.owner) {}

      private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        pointer ptr;
        const vector<T> *owner;

      public:
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            // TODO
            return const_iterator(ptr + n, owner);
        }
        const_iterator operator-(const int &n) const {
            // TODO
            return const_iterator(ptr - n, owner);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw
        // invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            // TODO
            if (owner != rhs.owner)
                throw invalid_iterator();
            return ptr - rhs.ptr;
        }
        const_iterator &operator+=(const int &n) {
            // TODO
            ptr += n;
            return *this;
        }
        const_iterator &operator-=(const int &n) {
            // TODO
            ptr -= n;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            ptr++;
            return const_iterator(ptr - 1, owner);
        }
        /**
         * TODO ++iter
         */
        const_iterator &operator++() {
            ptr++;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            ptr--;
            return const_iterator(ptr + 1, owner);
        }
        /**
         * TODO --iter
         */
        const_iterator &operator--() {
            ptr--;
            return *this;
        }
        /**
         * TODO *it
         */
        const T operator*() const { return *ptr; }
        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory address).
         */
        bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    /**
     * TODO Constructs
     * At least two: default constructor, copy constructor
     */
    int siz, cap;
    T *start;
    T *allocate(int n) {
        T *it = static_cast<T *>(malloc(sizeof(T) * n));
        return it;
    }
    void deallocate(T *ptr) {
        free(ptr);
        return;
    }
    void check_memory() {
        if (siz != cap)
            return;
        if (!siz)
            start = allocate(cap = 1);
        else {
            T *it = allocate(cap *= 2);
            for (int i = 0; i < siz; i++)
                new (it + i) T(start[i]), start[i].~T();
            deallocate(start);
            start = it;
        }
        return;
    }
    vector() : siz(0), cap(0), start(nullptr) {}
    vector(const vector &other) {
        siz = other.siz, cap = other.cap;
        start = allocate(cap);
        for (int i = 0; i < siz; i++)
            new (start + i) T(other[i]);
    }
    /**
     * TODO Destructor
     */
    ~vector() {
        for (int i = 0; i < siz; i++)
            start[i].~T();
        deallocate(start);
    }
    /**
     * TODO Assignment operator
     */
    vector &operator=(const vector &other) {
        if (this == &other)
            return *this;
        for (int i = 0; i < siz; i++)
            start[i].~T();
        deallocate(start);
        siz = other.siz, cap = other.cap;
        start = allocate(cap);
        for (int i = 0; i < siz; i++)
            new (start + i) T(other[i]);
        return *this;
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T &at(const size_t &pos) {
        if (pos < 0 || pos >= siz)
            throw index_out_of_bound();
        return start[pos];
    }
    const T &at(const size_t &pos) const {
        if (pos < 0 || pos >= siz)
            throw index_out_of_bound();
        return start[pos];
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
    T &operator[](const size_t &pos) { return at(pos); }
    const T &operator[](const size_t &pos) const { return at(pos); }
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T &front() const {
        if (siz == 0)
            throw container_is_empty();
        return start[0];
    }
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T &back() const {
        if (siz == 0)
            throw container_is_empty();
        return start[siz - 1];
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() { return iterator(start, this); }
    const_iterator begin() const { return const_iterator(start, this); }
    const_iterator cbegin() const { return const_iterator(start, this); }
    /**
     * returns an iterator to the end.
     */
    iterator end() { return iterator(start + siz, this); }
    const_iterator end() const { return const_iterator(start + siz, this); }
    const_iterator cend() const { return const_iterator(start + siz, this); }
    /**
     * checks whether the container is empty
     */
    bool empty() const { return siz == 0; }
    /**
     * returns the number of elements
     */
    size_t size() const { return siz; }
    /**
     * clears the contents
     */
    void clear() {
        for (int i = 0; i < siz; i++)
            start[i].~T();
        deallocate(start);
        siz = cap = 0, start = nullptr;
        return;
    }
    /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        int ind = pos - begin();
        check_memory();
        pos = iterator(start + ind, this);
        if (pos == end()) {
            new (&(*pos)) T(value);
            siz++;
            return pos;
        }
        if (siz)
            new (&(*end())) T(*(end() - 1));
        for (iterator it = end() - 1; it != pos; it--)
            *it = *(it - 1);
        *pos = value;
        siz++;
        return pos;
    }
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size
     * because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) {
        if (ind > siz)
            throw index_out_of_bound();
        check_memory();
        return insert(iterator(start + ind, this), value);
    }
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is
     * returned.
     */
    iterator erase(iterator pos) {
        for (iterator it = pos; it != end() - 1; it++)
            *it = *(it + 1);
        siz--;
        start[siz].~T();
        return pos;
    }
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) {
        if (ind >= siz)
            throw index_out_of_bound();
        return erase(iterator(start + ind, this));
    }
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) {
        insert(siz, value);
        return;
    }
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() {
        if (!siz)
            throw container_is_empty();
        erase(siz - 1);
        return;
    }
};

} // namespace sjtu

#endif
