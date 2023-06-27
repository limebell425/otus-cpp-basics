#pragma once
#include <iterator>
#include <memory>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>

template <typename T>
class MyList
{
    struct Node 
    {
        Node(T val_)
            : val(val_)
            , next(nullptr)
            , prev(nullptr)
        {}

        static std::unique_ptr<Node> create_fict(Node *prev_)
        {
            auto node = std::unique_ptr<Node>(new Node());
            node->prev = prev_;
            return node;
        }

        T val;
        std::unique_ptr<Node> next;
        Node *prev;

        private:
            Node() = default;
    };

public:
    template <typename ValueType>
    class Iterator
    {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = ValueType;
            using difference_type = std::ptrdiff_t; 
            using pointer = ValueType*;
            using reference = ValueType&;
            Iterator() = default;
            Iterator(const Iterator &it)
                : node(it.node)
            {}
            ~Iterator() = default;
            Iterator &operator=(const Iterator &other) = default;
            bool operator==(const Iterator &other) const { return node == other.node; }
            bool operator!=(const Iterator &other) const { return !(*this == other); }
            reference operator*() const { return node->val; }
            pointer operator->() const { return &node->val; }
            Iterator &operator++() { node = node->next.get(); return *this; }
            Iterator operator++(int) { Iterator old = *this; ++(*this); return old; }
            Iterator &operator--() { node = node->prev; return *this; }
            Iterator operator--(int) { Iterator old = *this; --(*this); return old; }

        private:
            explicit Iterator(Node *node_)
                :node(node_)
            {}
            Node *node;
            friend MyList;
    };

    public:
        MyList() = default;
        ~MyList() = default;
        MyList &operator=(const MyList &other)
        {
            if (this == &other)
                return *this;
            
            if (other.empty())
            {
                size_ = 0;
                first = nullptr;
                return *this;
            }

            Node *p = other.first.get();
            Node *prev_node = nullptr;
            while (p != nullptr)
            {
                auto node = std::make_unique<Node>(p->val);
                Node *tmp = node.get();
                if (prev_node)
                {
                    node->prev = prev_node;
                    prev_node->next = std::move(node);
                }
                else
                {
                    first = std::move(node);
                }
                prev_node = tmp;
                p = p->next.get();
            }
            last = prev_node;
            size_ = other.size_;

            return *this;
        }

        MyList(const MyList &other)
        {
            *this = other;
        }

        MyList &operator=(MyList &&other)
        {
            if (this == &other)
                return *this;
            first = std::move(other.first);
            last = std::exchange(other.last, nullptr);
            size_ = std::exchange(other.size_, 0);
            return *this;
        }

        MyList(MyList &&other)
        {
            *this = std::move(other);
        }

        bool operator==(const MyList &other) const
        {
            if (size() != other.size())
            {
                return false;
            }
            return (std::mismatch(begin(), end(), other.begin(), other.end()) == std::pair{end(), other.end()});
        }
    
        bool operator!=(const MyList &other) const
        {
            return !(*this == other);
        }

        bool empty() const;
        size_t size() const;
        void erase(const Iterator<T> &it);
        Iterator<T> begin() {return Iterator<T>{first.get()}; }
        Iterator<T> end() {return Iterator<T>{last}; }
        Iterator<T> begin() const {return Iterator<T>{first.get()}; }
        Iterator<T> end() const {return Iterator<T>{last}; }
        T &operator[](size_t index);
        const T &operator[](size_t) const;

        template<typename U>
        void push_back(U &&value);
        template<typename U>
        void insert(const Iterator<T> &it, U &&value);

        using value_type = T;
       
    private:
        std::unique_ptr<Node> first;
        Node *last = nullptr;
        size_t size_ = 0;
};

template<typename T>
bool MyList<T>::empty() const
{
    return first == nullptr;
}

template<typename T>
size_t MyList<T>::size() const
{
    return size_;
}

template<typename T>
template <typename U>
void MyList<T>::push_back(U &&value)
{
    ++size_;
    std::unique_ptr<Node> node = std::make_unique<Node>(std::forward<U>(value));
    if (empty())
    {
        first = std::move(node);
        first->next = Node::create_fict(first.get());
        last = first->next.get();
        return;
    }
    Node *p = last->prev;
    node->prev = p;
    node->next = std::move(p->next);
    p->next = std::move(node);
    last->prev = p->next.get();
}

template <typename T>
T &MyList<T>::operator[](size_t index)
{
    if (empty())
    {
        throw std::out_of_range("Out of range in MyList operator[]");
    }

    if (index > (size()-1))
    {
        throw std::out_of_range("Out of range in MyList operator[]");
    }

    return *std::next(begin(), index);
}

template <typename T>
const T &MyList<T>::operator[](size_t index) const
{
    if (empty())
    {
        throw std::out_of_range("Out of range in MyList operator[]");
    }

    if (index > (size()-1))
    {
        throw std::out_of_range("Out of range in MyList operator[]");
    }

    return *std::next(begin(), index);
}

template <typename T>
void MyList<T>::erase(const Iterator<T> &it)
{
    if (empty())
    {
        throw(std::out_of_range("Out Of Range in MyList Erase"));
    }

    if (size() == 1)
    {
        --size_;
        first = nullptr;
        return;
    }


    --size_;
    if (it == begin())
    {
        first = std::move(it.node->next);
        first->prev = nullptr;
        return;
    }
    if (it == end())
    {
        Iterator<T> it2 = end();
        --it2;
        Node *p1 = it2.node->prev;
        std::unique_ptr<Node> p2 = std::move(it2.node->next);
        p2->prev = p1;
        p1->next = std::move(p2);
        return;
    }
    Node *p1 = it.node->prev;
    std::unique_ptr<Node> p2 = std::move(it.node->next);
    p2->prev = p1;
    p1->next = std::move(p2);
}

template <typename T>
template <typename U>
void MyList<T>::insert(const Iterator<T> &it, U &&value)
{
    if (empty())
    {
        push_back(std::forward<U>(value));
        return;
    }
    ++size_;
    auto node = std::make_unique<Node>(std::forward<U>(value));
    if (it == begin())
    {
        it.node->prev = node.get();
        node->next = std::move(first);
        first = std::move(node);
        return;
    }
    Node *p = it.node->prev;
    node->prev = p;
    node->next = std::move(p->next);
    p->next = std::move(node);
    it.node->prev = p->next.get();
}