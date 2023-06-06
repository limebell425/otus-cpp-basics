#pragma once
#include <iterator>
#include <memory>
#include <utility>
#include <cstddef>

template<typename T>
class MyForwardList
{
    struct ForwardNode
    {
        ForwardNode(T val_)
            : val(val_)
            , next(nullptr)
        {}

        ForwardNode(T val_, std::unique_ptr<ForwardNode> next_)
            : val(val_)
            , next(std::move(next_))
        {}

        T val = 0;
        std::unique_ptr<ForwardNode> next;
    };

public:
    template<typename ValueType>        
    class Iterator
    {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = ValueType;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using reference = value_type&;
            Iterator() = default;
            Iterator(const Iterator &it)
                : node(it.node)
            {}
            ~Iterator() = default;
            Iterator& operator=(const Iterator &other) = default;
            bool operator==(const Iterator &other) const { return node == other.node; }
            bool operator!=(const Iterator &other) const { return !(*this == other); }
            reference operator*() const { return node->val; }
            pointer operator->() const { return &node->val; }
            Iterator& operator++() { node = node->next.get(); return *this; }
            Iterator operator++(int) { Iterator old = *this; ++(*this); return old; }

        
        private:
            explicit Iterator(ForwardNode *node_)
                : node(node_)
            {}
            ForwardNode *node;
            friend MyForwardList;
    };

public:    
    MyForwardList() = default;
    ~MyForwardList() = default;
    MyForwardList &operator=(const MyForwardList &other)
    {
        if (this == &other)
            return *this;
        ForwardNode *p = other.first.get();
        ForwardNode *prev_node = nullptr;
        while (p != nullptr)
        {
            auto node = std::make_unique<ForwardNode>(p->val);
            ForwardNode *tmp = node.get();
            if (prev_node)
            {
                prev_node->next = std::move(node);
            }
            else
            {
                first = std::move(node);
            }
            prev_node = tmp;
            p = p->next.get();
        }
        size_ = other.size_;
        return *this;
    }

    MyForwardList(const MyForwardList &other)
    {
        *this = other;
    }

    MyForwardList &operator=(MyForwardList &&other)
    {
        if (this == &other)
            return *this;
        first = std::move(other.first);
        size_ = std::exchange(other.size_, 0);
    }

    MyForwardList(MyForwardList &&other)
    {
        *this = std::move(other);
    }

    bool empty() const;
    size_t size() const;
    void erase(const Iterator<T> &from, const Iterator<T> &to);
    void erase(const Iterator<T> &it);
    Iterator<T> begin() { return Iterator<T>{first.get()}; }
    Iterator<T> end() { return Iterator<T>{nullptr}; }
    Iterator<const T> begin() const { return Iterator<const T>{first.get()}; }
    Iterator<const T> end() const { return Iterator<const T>{nullptr}; }
    T &operator[](size_t index);   
    const T &operator[](size_t index) const;

    template<typename U>
    void push_back(U &&value);
    template<typename U>
    void insert(const Iterator<T> &it, U &&value);

private:
    std::unique_ptr<ForwardNode> first;
    size_t size_ = 0;
}; 

template<typename T>
bool MyForwardList<T>::empty() const
{
    return first == nullptr;
}

template<typename T>
size_t MyForwardList<T>::size() const
{
    return size_;
}

template<typename T>
template<typename U>
void MyForwardList<T>::push_back(U &&value)
{
    ++size_;
    std::unique_ptr<ForwardNode> node = std::make_unique<ForwardNode>(std::forward<U>(value));
    if (empty())
    {
        first = std::move(node);
        return;
    }
    ForwardNode *p = first.get();
    while (p->next)
    {
        p = p->next.get(); 
    }   
    p->next = std::move(node);
}

template<typename T>
T &MyForwardList<T>::operator[](size_t index)
{
    return *std::next(begin(), index);
}

template<typename T>
const T &MyForwardList<T>::operator[](size_t index) const
{
    return *std::next(begin(), index);
}


template<typename T>
void MyForwardList<T>::erase(const Iterator<T> &it)
{
    if (empty() || it == end())
    {
        return;
    }
    --size_;
    if (it.node == first.get())
    {
        first = std::move(it.node->next);
        return;
    }
   
   ForwardNode *p1 = first.get();
    while (p1->next.get() != it.node)
    {
        p1 = p1->next.get();
    }
    p1->next = std::move(it.node->next);
}

template<typename T>
void MyForwardList<T>::erase(const Iterator<T> &from, const Iterator<T> &to)
{
    if (empty())
    {
        return;
    }
    if (from.node == first.get())
    {
        ForwardNode *p2 = from.node;
        while ((p2->next.get() != to.node))
        {
            p2 = p2->next.get();
            --size_;
        }
        first = std::move(p2->next);
        return;
    }
    
    ForwardNode *p1 = first.get();
    while (p1->next.get() != from.node)
    {
        p1 = p1->next.get();
    }

    ForwardNode *p2 = from.node;
    while ((p2->next.get() != to.node))
    {
        p2 = p2->next.get();
        --size_;
    }

    p1->next = std::move(p2->next);

}

template<typename T>
template<typename U>
void MyForwardList<T>::insert(const Iterator<T> &it, U &&value)
{
    ++size_;
    if (first.get() == it.node)
    {
        auto node = std::make_unique<ForwardNode>(std::forward<U>(value), std::move(first));
        first = std::move(node);
        return;
    }
    ForwardNode *p1 = first.get();
    while (p1->next.get() != it.node)
    {
        p1 = p1->next.get();
    }
    auto node = std::make_unique<ForwardNode>(std::forward<U>(value), std::move(p1->next));
    p1->next = std::move(node);
    
}
