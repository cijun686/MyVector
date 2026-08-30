#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <utility>
#include <cstddef>

template <typename T>
class MyVector
{
private:
    T *m_data;
    size_t m_size;
    size_t m_cap;
    static constexpr size_t GROW_FACTOR = 2;

    void reallocate(size_t newCap)
    {
        if (newCap <= m_cap)
        {
            return;
        }
        T *newData = new T[newCap];
        for (size_t i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        delete[] m_data;
        m_data = newData;
        m_cap = newCap;
    }

public:
    MyVector() : m_data(nullptr), m_size(0), m_cap(0)
    {
    }
    MyVector(const MyVector<T> &other);
    MyVector<T> &operator=(const MyVector<T> &other);
    MyVector(MyVector<T> &&other) noexcept;
    MyVector<T> &operator=(MyVector<T> &&other) noexcept;

    ~MyVector()
    {
        delete[] m_data;
    }
    void push_back(const T &val)
    {
        if (m_size >= m_cap)
        {
            size_t newCap = (m_cap == 0) ? 1 : m_cap * GROW_FACTOR;
            reallocate(newCap);
        }
        m_data[m_size++] = val;
    }

    void pop_back()
    {

        if (m_size > 0)
        {
            m_data[m_size - 1].~T();
            --m_size;
        }
    }

    T &operator[](size_t idx)
    {
        return m_data[idx];
    }

    const T &operator[](size_t idx) const
    {
        return m_data[idx];
    }

    size_t size() const
    {
        return m_size;
    }
    size_t capacity() const
    {
        return m_cap;
    }
    bool empty() const
    {
        return m_size == 0;
    }
};
template <typename T>
MyVector<T>::MyVector(const MyVector<T> &other)
{
    m_size = other.size();
    m_cap = other.capacity();
    m_data = new T[m_cap];
    for (size_t i = 0; i < m_size; i++)
    {
        m_data[i] = other.m_data[i];
    }
}

template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &other)
{
    if (this == &other)
    {
        return *this;
    }
    delete[] m_data;
    m_size = other.m_size;
    m_cap = other.m_cap;
    m_data = new T[m_cap];
    for (size_t i = 0; i < m_size; i++)
    {
        m_data[i] = other.m_data[i];
    }
    return *this;
}

template <typename T>
MyVector<T>::MyVector(MyVector<T> &&other) noexcept
{
    m_data = other.m_data;
    m_size = other.m_size;
    m_cap = other.m_cap;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_cap = 0;
}

template <typename T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    delete[] m_data;
    m_data = other.m_data;
    m_size = other.m_size;
    m_cap = other.m_cap;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_cap = 0;

    return *this;
}

#endif // MYVECTOR_H
