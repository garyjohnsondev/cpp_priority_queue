#pragma once

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

namespace usu
{
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
        priority_queue(const std::initializer_list<item>& initList);

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

        bool isLeaf(unsigned int index);

        int getParent(unsigned int index);
        int getLeftChild(unsigned int index);
        int getRightChild(unsigned int index);

        void heapifyUp(unsigned int index);
        void heapifyDown(unsigned int index);
    };

    template <typename Value, typename Priority>
    priority_queue<Value, Priority>::priority_queue() :
        m_queue(0),
        m_storageSize(0)
    {
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::priority_queue(const std::initializer_list<item>& initList) :
        priority_queue()
    {
        for (auto i = initList.begin(); i != initList.end(); ++i)
        {
            enqueue((*i).value, (*i).priority);
        }
    }

    template <typename Value, typename Priority>
    inline void priority_queue<Value, Priority>::enqueue(Value value, priority_type priority)
    {
        m_storageSize = static_cast<size_type>(static_cast<double>(m_storageSize) * 1.25 + 1);
        m_queue.push_back({ value, priority });
        //m_queue.resize(m_queue.size() * 1.25 + 1);
        size_type end = m_queue.size() - 1;
        //m_queue[end] = { value, priority };
        heapifyUp(static_cast<unsigned int>(end));
    }

    template <typename Value, typename Priority>
    inline auto priority_queue<Value, Priority>::dequeue()
    {
        try
        {
            if (!empty())
            {
                auto highestPriority = m_queue[0];

                m_queue[0] = m_queue.back();
                m_queue.pop_back();
                heapifyDown(0);

                return highestPriority;
            }

            else
            {
                throw new std::range_error("Cannot remove item from empty queue.");
            }
        }
        catch (std::range_error& e)
        {
            std::cerr << e.what() << std::endl;
            return m_queue[0];
        }
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator priority_queue<Value, Priority>::find(Value value)
    {
        for (auto i = begin(); i != end(); i++)
        {
            auto i2 = *i;

            if ((*i).value == value)
            {
                return i;
            }
        }
        return end();
    }

    template <typename Value, typename Priority>
    inline void priority_queue<Value, Priority>::update(iterator i, priority_type priority)
    {
        (*i).priority = priority;
        auto index = i - begin();
        heapifyUp(static_cast<unsigned int>(index));
        heapifyDown(static_cast<unsigned int>(index));
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::empty()
    {
        return m_queue.empty();
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::size_type priority_queue<Value, Priority>::size()
    {
        if (!empty())
        {
            auto numElements = 0u;
            for (size_type element = 0; element < m_queue.size(); element++)
            {
                if (m_queue[element].priority != 0)
                {
                    numElements++;
                }
            }
            return numElements;
        }
        else
        {
            return 0;
        }
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator priority_queue<Value, Priority>::begin()
    {
        if (empty())
        {
            return end();
        }
        return iterator(this);
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator priority_queue<Value, Priority>::end()
    {
        return iterator(size(), this);
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::reference priority_queue<Value, Priority>::operator[](int index)
    {
        auto i = static_cast<size_type>(index);
        if (i >= 0 && i < size() && size() > 0)
        {
            return m_queue[i];
        }
        return m_queue[0];
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::isLeaf(unsigned int index)
    {
        return (index >= (size() / 2)) && (index < size());
    }

    template <typename Value, typename Priority>
    inline int priority_queue<Value, Priority>::getParent(unsigned int index)
    {
        if (index <= 0)
        {
            return -1;
        }
        return (index - 1) / 2;
    }

    template <typename Value, typename Priority>
    inline int priority_queue<Value, Priority>::getLeftChild(unsigned int index)
    {
        if (index >= size() / 2)
        {
            return -1;
        }
        return (index * 2) + 1;
    }

    template <typename Value, typename Priority>
    inline int priority_queue<Value, Priority>::getRightChild(unsigned int index)
    {
        if (index >= (size() - 1) / 2)
        {
            return -1;
        }
        return (index * 2) + 2;
    }

    template <typename Value, typename Priority>
    inline void priority_queue<Value, Priority>::heapifyUp(unsigned int index)
    {
        while ((index != 0) && (m_queue[index] > m_queue[getParent(index)]))
        {
            std::swap(m_queue[index], m_queue[getParent(index)]);
            index = getParent(index);
        }
    }

    template <typename Value, typename Priority>
    inline void priority_queue<Value, Priority>::heapifyDown(unsigned int index)
    {
        if ((index < 0) || (index >= size()))
        {
            return;
        }
        while (!isLeaf(index))
        {
            unsigned int child = getLeftChild(index);
            if ((child < (size() - 1)) && (m_queue[child] < m_queue[child + 1]))
            {
                child++;
            }
            if (m_queue[index] >= m_queue[child])
            {
                return;
            }
            std::swap(m_queue[index], m_queue[child]);
            index = child;
        }
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::iterator() :
        iterator(nullptr)
    {
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::iterator(queue_pointer ptr) :
        m_pq(ptr),
        m_position(0)
    {
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::iterator(size_type size, queue_pointer ptr) :
        m_pq(ptr),
        m_position(size)
    {
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::iterator(const iterator& iter)
    {
        this->m_pq = iter.m_pq;
        this->m_position = iter.m_position;
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::iterator(iterator&& iter) noexcept
    {
        this->m_pq = iter.m_pq;
        this->m_position = iter.m_position;

        iter.m_pq = nullptr;
        iter.m_position = 0;
    }

    template <typename Value, typename Priority>
    inline priority_queue<Value, Priority>::iterator::~iterator()
    {
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::reference priority_queue<Value, Priority>::iterator::operator*() const
    {
        return (*m_pq)[static_cast<unsigned int>(m_position)];
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::pointer priority_queue<Value, Priority>::iterator::operator->() const
    {
        return &((*m_pq)[static_cast<unsigned int>(m_position)]);
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator& priority_queue<Value, Priority>::iterator::operator=(const iterator& rhs)
    {
        this->m_pq = rhs.m_pq;
        this->m_position = rhs.m_position;

        return *this;
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator& priority_queue<Value, Priority>::iterator::operator=(iterator&& rhs)
    {
        if (this != &rhs)
        {
            std::swap(this->m_pq, rhs.m_pq);
            std::swap(this->m_position, rhs.m_position);
        }

        return *this;
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator priority_queue<Value, Priority>::iterator::operator++()
    {
        ++m_position;
        return *this;
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::iterator priority_queue<Value, Priority>::iterator::operator++(int)
    {
        iterator iter = *this;
        ++m_position;
        return iter;
    }

    template <typename Value, typename Priority>
    typename priority_queue<Value, Priority>::difference_type priority_queue<Value, Priority>::iterator::operator-(iterator const& rhs) const
    {
        return m_position - rhs.m_position;
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::iterator::operator==(const iterator& rhs)
    {
        return this->m_position == rhs.m_position && this->m_pq == rhs.m_pq;
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::iterator::operator!=(const iterator& rhs)
    {
        return !operator==(rhs);
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::item::operator>(const item& i) const
    {
        return (priority > i.priority);
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::item::operator<(const item& i) const
    {
        return (priority < i.priority);
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::item::operator>=(const item& i) const
    {
        return (priority >= i.priority);
    }

    template <typename Value, typename Priority>
    inline bool priority_queue<Value, Priority>::item::operator<=(const item& i) const
    {
        return (priority <= i.priority);
    }
} // namespace usu