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

    // 扩容：分配新容量，拷贝旧元素，释放旧内存
    void reallocate(size_t newCap)
    {
        if (newCap <= m_cap)
        {
            return;
        }
        // 开辟新堆数组
        T *newData = new T[newCap];
        // 拷贝旧元素
        for (size_t i = 0; i < m_size; i++)
        {
            newData[i] = m_data[i];
        }
        // 释放旧内存
        delete[] m_data;
        // 更新指针与容量
        m_data = newData;
        m_cap = newCap;
    }

public:
    // 默认构造
    MyVector() : m_data(nullptr), m_size(0), m_cap(0)
    {
    }
    // 拷贝构造声明
    MyVector(const MyVector<T> &other);
    // 拷贝赋值运算符声明
    MyVector<T> &operator=(const MyVector<T> &other);
    // 移动构造声明
    MyVector(MyVector<T> &&other) noexcept;
    // 移动赋值运算符声明
    MyVector<T> &operator=(MyVector<T> &&other) noexcept;

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
            // 如果 `m_cap == 0`（刚创建空容器，还没分配内存）：新容量 = 1
            // 否则：新容量 = 当前容量 ×2（GROW_FACTOR=2 二倍扩容）
            reallocate(newCap);
        }
        m_data[m_size++] = val;
        // 第一步. m_data[m_size] = val; ：把 val 赋值到下一个空闲位置
        // 第二步. m_size++ ：有效元素数量 + 1
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
    // 运算符重载：[]下标访问

    // operator [] 非 const 版本，可读可写
    // 普通对象调用
    T &operator[](size_t idx)
    {
        return m_data[idx];
    }
    // operator [] const 版本，只读访问
    // const 对象调用
    const T &operator[](size_t idx) const
    {
        return m_data[idx];
    }
    // 迭代器 begin () end ()，支持范围 for
    using iterator = T *;
    using const_iterator = const T *;
    // `using`：C++ 类型别名语法，给类型起外号
    // `iterator` 等价于 `T*`，可读写迭代器；`const_iterator`等价于`const T*`，只读迭代器

    iterator begin()
    {
        return m_data;
    }
    iterator end()
    {
        return m_data + m_size;
    }

    const_iterator begin() const
    {
        return m_data;
    }
    const_iterator end() const
    {
        return m_data + m_size;
    }
    // Tip:end() 返回的是最后一个元素的下一个位置，指向无效内存，不能解引用访问，只能用于范围 for 循环的结束条件

    // 预分配容量，不改变size
    void reserve(size_t newCap);

    // 清空元素，容量不变
    void clear();

    // 修改有效元素个数
    void resize(size_t newSize);

    // 原地构造，可变参数模板
    template <typename... Args>
    void emplace_back(Args &&...args)
    {
        if (m_size >= m_cap)
        {
            size_t newCap = (m_cap == 0) ? 1 : m_cap * GROW_FACTOR;
            reallocate(newCap);
        }
        // placement new 定位new：在已有的内存地址上调用构造，不分配内存
        new (&m_data[m_size]) T(std::forward<Args>(args)...);
        m_size++;
    }
    // Tips:
    // 1. typename... Args : `...`在名字前面：声明包，造袋子
    // 2. args... : `...`在名字后面：展开包，打开袋子，把内容全部拿出来

    // 查询接口
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

// 拷贝构造函数定义
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
// 拷贝赋值运算符定义
template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &other)
{
    // 自赋值判断
    // this：指向调用函数对象的指针。
    // &other：取等号右边对象的地址。
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
// Tips: 拷贝构造函数和拷贝赋值运算符的区别
// 还没有类，正在创建类 → 拷贝构造
// 已经构造完毕，把一个类的内容覆盖给另一个类 → 拷贝赋值

// 移动构造函数定义
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
// 移动赋值运算符定义
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
// Tips:
// && 是右值引用，专门接收临时对象（右值），做资源窃取（浅拷贝）
// noexcept：告诉编译器这个函数不会抛出异常，STL 容器在扩容时会优先调用带noexcept的移动构造

// reserve 预分配容量
template <typename T>
void MyVector<T>::reserve(size_t newCap)
{
    if (newCap > m_cap)
    {
        reallocate(newCap);
    }
}

// clear：清空元素，不释放内存
template <typename T>
void MyVector<T>::clear()
{
    m_size = 0;
}

// resize 修改有效元素数量
template <typename T>
void MyVector<T>::resize(size_t newSize)
{
    if (newSize < m_size)
    {
        // 缩小：析构多余元素
        for (size_t i = newSize; i < m_size; i++)
        {
            m_data[i].~T();
        }
        m_size = newSize;
    }
    else
    {
        // 需要扩容就重分配内存
        if (newSize > m_cap)
        {
            reallocate(newSize);
        }
        // 对新增位置定位new构造T默认对象
        for (size_t i = m_size; i < newSize; ++i)
        {
            new (m_data + i) T{};
        }
        m_size = newSize;
    }
}

#endif // MYVECTOR_H
