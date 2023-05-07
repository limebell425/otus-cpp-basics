#pragma once
#include <iterator>

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
        Iterator& operator--() { --p; return *this }
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
    Iterator<T> begin() { return Iterator<T>{memory}; }
    Iterator<T> end() { return Iterator<T>{memory + size_}; }
    Iterator<const T> begin() const { return Iterator<const T>{memory}; }
    Iterator<const T> end() const { return Iterator<const T>{memory + size_}; }
    Iterator<const T> cbegin() const { return Iterator<const T>{memory}; }
    Iterator<const T> cend() const { return Iterator<const T>{memory + size_}; }
    void push_back(T value);
    T operator[](size_t index);
    int size();
    void insert(const Iterator<T> &it, T value);
    void erase(const Iterator<T> &it);
    void erase(const Iterator<T> &from, const Iterator<T> &to);
    //void reserve(int n);

private:
    int size_ = 0;
    int capacity = 0;
    T *memory = nullptr;
    static constexpr float size_modifier = 1.5f;
};

template<typename T>
MyVector<T>::MyVector(int count)
{
    capacity = count * size_modifier;
    memory = new T[capacity];
}

template<typename T>
void MyVector<T>::push_back(T value)
{
    ++size_;
    if (size_ > capacity) {
        capacity = size_ * size_modifier;
        T *new_region = new T[capacity];
        for (size_t i = 0; i < size_ - 1; ++i) {
            new_region[i] = memory[i];
        }
        new_region[size_-1] = value;
        delete memory;
        memory = new_region;
    } 
    else {
        memory[size_ - 1] = value;
    }
}

template<typename T>
T MyVector<T>::operator[](size_t index)
{
    return memory[index];
}

template<typename T>
int MyVector<T>::size()
{
    return size_;
}

template<typename T>
void MyVector<T>::insert(const Iterator<T> &it, T value)
{
    ++size_;
    if (size_ > capacity)
    {
        capacity = size_ * size_modifier;
        T *new_region = new T[capacity];
        T *j = new_region;  
        int help = 0; 
        for (T* i  = memory; i < it.p; ++i, ++j)
        {
            *j = *i;
        }
        *j++ = value;
        for (T* i = it.p; i < memory + size_ - 1; ++i, ++j)
        {
            *j = *i;
        }
        delete memory;
        memory = new_region;
    }
    else
    {
        for (T* i = memory + size_-1; i > it.p; --i)
        {
            *i = *(i - 1);
        }
        *it.p = value;
    }
}

template <typename T>
void MyVector<T>::erase(const Iterator<T> &it)
{
    for (T *i = it.p; i < memory + size_ - 1; ++i)
    {
        *i = *(i + 1);
    }
    --size_;
}

template <typename T>
void MyVector<T>::erase(const Iterator<T> &from, const Iterator<T> &to)
{
    T * to_ = memory + size_ - (to - from) - 1;
    for (T *i = from.p; i < to_; ++i){
        *i = *(i + (to - from));
    }
    size_ -= (to - from);
}