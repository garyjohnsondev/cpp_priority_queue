#pragma once

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

namespace usu {
	template <typename Value, typename Priority = unsigned int>
    class priority_queue
    {
      public:
        struct item
        {
            Value value;
            Priority priority;

            bool operator>(const item& i) const;
            bool operator<(const item& i) const;
            bool operator>=(const item& i) const;
            bool operator<=(const item& i) const;
        };

        using value_type = item;
        using priority_type = Priority;
        using queue_pointer = priority_queue<Value, Priority>*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = item*;
        using reference = item&;

        class iterator : public std::iterator<std::forward_iterator_tag, priority_queue*>
        {
          public:
            using iterator_category = std::forward_iterator_tag;

            iterator(); // Default Constructor
            iterator(queue_pointer ptr);
            iterator(size_type size, queue_pointer ptr);
            iterator(const iterator& iter);     // Copy Constructor
            iterator(iterator&& iter) noexcept; // Move Constructor
            ~iterator();

            reference operator*() const; // Dereference Operator
            pointer operator->() const;

            iterator& operator=(const iterator& rhs); // Copy Assignment
            iterator& operator=(iterator&& rhs);      // Move Assignment
            iterator operator++();                    // ++increment
            iterator operator++(int);                 // increment++

            difference_type operator-(iterator const& rhs) const; // Difference Operator to find distance between 2 iterators

            bool operator==(const iterator& rhs);
            bool operator!=(const iterator& rhs);

          private:
            queue_pointer m_pq;
            size_type m_position;
        };

        priority_queue();
        priority_queue(const std::initializer_list<item>& intiList);

        void enqueue(Value value, priority_type priority);
        auto dequeue();
        iterator find(Value value);
        void update(iterator i, priority_type priority);
        bool empty();
        size_type size();

        iterator begin();
        iterator end();

        reference operator[](int index);

      private:
        std::vector<item> m_queue;
        size_type m_storageSize;
        
        //heap functionality
        bool isLeaf(unsigned int index);
        int getParent(unsigned int index);
        int getLeftChild(unsigned int index);
        int getRightChild(unsigned int index);
        void heapifyUp(unsigned int index);
        void heapifyDown(unsigned int index);
    };
}