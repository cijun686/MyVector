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

    // 扩容：分配新容量，拷贝旧元素，释放旧内存
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
    // 默认构造
    MyVector() : m_data(nullptr), m_size(0), m_cap(0)
    {
    }

    // 析构
    ~MyVector()
    {
        delete[] m_data;
    }
    // 添加元素到末尾
    void push_back(const T &val)
    {
        if (m_size >= m_cap)
        {
            size_t newCap = (m_cap == 0) ? 1 : m_cap * GROW_FACTOR;
            reallocate(newCap);
        }
        m_data[m_size++] = val;
    }

    // 删除末尾元素
    void pop_back()
    {

        if (m_size > 0)
        {
            m_data[m_size - 1].~T();
            --m_size;
        }
    }
    // 运算符重载：下标访问

    // operator [] 非 const 版本
    T &operator[](size_t idx)
    {
        return m_data[idx];
    }
    // operator [] const 版本
    const T &operator[](size_t idx) const
    {
        return m_data[idx];
    }

    // 查询接口 const
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

#endif // MYVECTOR_H
