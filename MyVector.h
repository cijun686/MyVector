#ifndef MYVECTOR_H
// 如果没有定义过 MYVECTOR_H 这个宏，才往下编译
#define MYVECTOR_H

#include <utility>
// 标准库头文件，提供`std::move`移动语义、`std::swap`交换
#include <cstddef>
// C++ 标准头文件，提供`size_t`类型（无符号整数，专门用来表示数组下标、内存大小）
template <typename T>
class MyVector
{
private:
    T *m_data;
    size_t m_size;
    size_t m_cap;
    static constexpr size_t GROW_FACTOR = 2;

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
    // 判断容器是否为空。如果有效元素个数等于 0 返回 true
};

#endif // MYVECTOR_H
