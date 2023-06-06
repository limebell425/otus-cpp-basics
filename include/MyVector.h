#pragma once
#include <iterator>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>

template<typename T>
class MyVector
{
public:
    template<typename ValueType>
    class Iterator 
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        Iterator() = default;
        Iterator(const Iterator &it)
            : p(it.p)
        {}
        ~Iterator() = default;
        Iterator& operator=(const Iterator& other) = default;
        bool operator==(const Iterator &other) const { return p == other.p; }
        bool operator!=(const Iterator &other) const { return !(*this == other); }
        reference operator*() const { return *p; }
        pointer operator->() const { return p; }
        Iterator& operator++() { ++p; return *this; }
        Iterator operator++(int) { Iterator old = *this; ++(*this); return old; }
        Iterator& operator--() { --p; return *this; }
        Iterator operator--(int) { Iterator old = *this; --(*this); return old; }
        Iterator& operator+=(difference_type n) { p += n; return *this; }
        Iterator& operator-=(difference_type n) {  p -= n;  return *this; }
        Iterator operator-(difference_type n) const { return Iterator{p - n}; }
        difference_type operator-(const Iterator &other) const { return p - other.p; }
        value_type operator[](difference_type n) const { return p[n]; }
        bool operator<(const Iterator &other) const { return p < other.p; }
        bool operator>(const Iterator &other) const { return p > other.p; }
        bool operator<=(const Iterator &other) const { return !(*this > other); }
        bool operator>=(const Iterator &other) const { return !(*this < other); }

        friend Iterator operator+(const Iterator &lhs, difference_type rhs) 
        {
            return Iterator{lhs.p + rhs};
        }

        friend Iterator operator+(difference_type lhs, const Iterator &rhs) 
        {
            return Iterator{lhs + rhs.p};
        }

        friend class MyVector;
 
    private:
        explicit Iterator(ValueType *p_) 
            : p(p_)
        {}
        ValueType *p;
    };
    
    MyVector() = default;
    MyVector(int count);
    ~MyVector()
    {
        delete[] memory;
    }
    
    MyVector& operator=(const MyVector &vec)
    {
        if (this == &vec)
            return *this;
        size_ = vec.size_;
        capacity_ = vec.capacity_;
        delete[] memory;
        memory = new T[capacity_];
        std::copy(vec.begin(), vec.end(), memory);
        return *this;
    }

    MyVector(const MyVector &vec)
    {
        *this = vec;
    }

    MyVector& operator=(MyVector &&vec)
    {
        if (this == &vec)
            return *this;
        size_ = std::exchange(vec.size_, 0);
        capacity_ = std::exchange(vec.capacity_, 0);
        memory = std::exchange(vec.memory, nullptr);
        return *this;
    }

    MyVector(MyVector &&vec)
    {
        *this = std::move(vec);
    }

    bool operator==(const MyVector &other) const
    {
        if (size() != other.size())
        {
            return false;
        }
        return (std::mismatch(begin(), end(), other.begin(), other.end()) == std::pair{end(), other.end()});
    }
    
    bool operator!=(const MyVector &other) const
    {
        return !(*this == other);
    }

    Iterator<T> begin() { return Iterator<T>{memory}; }
    Iterator<T> end() { return Iterator<T>{memory + size_}; }
    Iterator<const T> begin() const { return Iterator<const T>{memory}; }
    Iterator<const T> end() const { return Iterator<const T>{memory + size_}; }
    Iterator<const T> cbegin() const { return Iterator<const T>{memory}; }
    Iterator<const T> cend() const { return Iterator<const T>{memory + size_}; }
    T &operator[](size_t index);
    const T &operator[](size_t index) const;
    int size() const;
    int empty() const;
    int capacity() const;    
    void erase(const Iterator<T> &it);
    void erase(const Iterator<T> &from, const Iterator<T> &to);

    template<typename U>
    void push_back(U &&value);
    template<typename U>
    void insert(const Iterator<T> &it, U &&value);

private:
    int size_ = 0;
    int capacity_ = 0;
    T *memory = nullptr;
    static constexpr float size_modifier = 1.5f;

public:
    using value_type = T;
};

template<typename T>
MyVector<T>::MyVector(int count)
{
    capacity_ = static_cast<int>(count * size_modifier);
    memory = new T[capacity_];
}

template<typename T>
template<typename U>
void MyVector<T>::push_back(U &&value)
{
    if (size_ >= capacity_) {
        capacity_ = static_cast<int>((size_ + 1) * size_modifier);
        T *new_region = new T[capacity_];
        std::move(begin(), end(), new_region);
        new_region[size_] = std::forward<U>(value);
        delete[] memory;
        memory = new_region;
    } 
    else {
        memory[size_] = std::forward<U>(value);
    }
    ++size_;
}

template<typename T>
T &MyVector<T>::operator[](size_t index) 
{
    if (empty())
    {
        throw std::out_of_range("Out of range in MyVector operator[]");
    }

    if (index > (size()-1))
    {
        throw std::out_of_range("Out of range in MyVector operator[]");
    }

    return memory[index];
}

template<typename T>
const T &MyVector<T>::operator[](size_t index) const
{
    if (empty())
    {
        throw std::out_of_range("Out of range in MyVector operator[]");
    }

    if (index > (size()-1))
    {
        throw std::out_of_range("Out of range in MyVector operator[]");
    }

    return memory[index];
}

template<typename T>
int MyVector<T>::size() const
{
    return size_;
}

template<typename T>
int MyVector<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
int MyVector<T>::capacity() const
{
    return capacity_;
}


template<typename T>
template<typename U>
void MyVector<T>::insert(const Iterator<T> &it, U &&value)
{
    if (it > end())
    {
        throw(std::out_of_range("Out of range in MyVector insert"));
    }
    
    if (size_ >= capacity_)
    {
        capacity_ = static_cast<int>((size_+1) * size_modifier);
        T *new_region = new T[capacity_];
        std::move(begin(), it, new_region);
        std::move(it, end(), new_region+(it-begin()) + 1);
        auto idx = it - begin();
        *(new_region + idx) = std::forward<U>(value);
        delete[] memory;
        memory = new_region;
    }
    else
    {
        for (T* i = memory + size_; i > it.p; --i)
        {
            *i = *(i - 1);
        }
        *it.p = std::forward<U>(value);
    }
    ++size_;
}

template <typename T>
void MyVector<T>::erase(const Iterator<T> &it)
{
    if (empty())
    {
        throw(std::out_of_range("Out Of Range in MyVector Erase"));
    }

    if (it > end())
    {
        throw(std::out_of_range("Out Of Range in MyVector Erase"));
    }

    if (it == end())
    {
        --size_;
        return;
    }
    std::move(it + 1, end(), it);
    --size_;
}

template <typename T>
void MyVector<T>::erase(const Iterator<T> &from, const Iterator<T> &to)
{
    std::move(to, end(), from);
    size_ -= (to - from);
}