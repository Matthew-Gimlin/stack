// Created by Matthew Gimlin on July 25, 2022.

// This class is a stack data structure.

#pragma once

template<typename T>
class Stack
{
public:
    using SizeType       = std::size_t;
    using ValueType      = T;
    using ConstReference = const T&;

    /**
     * @brief Default constructor.
     * 
     * Creates an empty stack.
     */
    Stack()
        : m_Container( (T*) ::operator new(INITIAL_CAPACITY * sizeof(T)) ),
          m_Size(0),
          m_Capacity(INITIAL_CAPACITY)
    { }

    /**
     * @brief Copy constructor.
     * @param other The stack to copy.
     * 
     * Creates a stack by copying the contents of another.
     */
    Stack(const Stack& other)
        : m_Container( (T*) ::operator new(other.m_Capacity * sizeof(T)) ),
          m_Size(other.m_Size),
          m_Capacity(other.m_Capacity)
    {
        for (SizeType i = 0; i < m_Size; ++i)
            new (&m_Container[i]) ValueType(other.m_Container[i]);
    }

    /**
     * @brief Move constructor.
     * @param other The stack to move.
     * 
     * Creates a stack by moving the contents of another.
     */
    Stack(Stack&& other)
        : m_Container(other.m_Capacity),
          m_Size(other.m_Size),
          m_Capacity(other.m_Capacity)
    {
        other.m_Capacity = ::operator new(INITIAL_CAPACITY * sizeof(T));
        other.m_Size = 0;
        other.m_Capacity = INITIAL_CAPACITY;
    }

    ~Stack() { Clear(); }

    /**
     * @brief Copy assignment operator.
     * @param other The stack to copy.
     * 
     * Clears the stack, and recreates it by copying the contents of another.
     */
    Stack& operator=(const Stack& other)
    {
        if (this == &other) return *this;

        Clear();
        ::operator delete(m_Container, m_Capacity * sizeof(T));

        m_Container = (T*) ::operator new(other.m_Capacity * sizeof(T));
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        for (SizeType i = 0; i < m_Size; ++i)
            new (&m_Container[i]) ValueType(other.m_Container[i]);
        
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The stack to move.
     * 
     * Clears the stack, and recreates it by moving the contents of another.
     */
    Stack& operator=(Stack&& other)
    {
        if (this == &other) return *this;

        m_Container = other.m_Capacity;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        other.m_Capacity = (T*) ::operator new(INITIAL_CAPACITY * sizeof(T));
        other.m_Size = 0;
        other.m_Capacity = INITIAL_CAPACITY;
        
        return *this;
    }

    // private member data getters
    bool Empty() const { return m_Size == 0; }
    SizeType Size() const { return m_Size; }
    SizeType Capacity() const { return m_Capacity; }

    // get the top of the stack
    ConstReference Peak() const { return m_Container[m_Size - 1]; }

    /**
     * @brief Pushes onto the stack.
     * @param value The value to push.
     * 
     * Adds value to the top of the stack by copying it.
     */
    void Push(const ValueType& value)
    {
        if (m_Size >= m_Capacity)
            Reallocate(m_Capacity * 2);
        
        new (&m_Container[m_Size]) ValueType(value);
        ++m_Size;
    }

    /**
     * @brief Pushes onto the stack.
     * @param value The value to push.
     * 
     * Adds value to the top of the stack by moving it.
     */
    void Push(ValueType&& value)
    {
        if (m_Size >= m_Capacity)
            Reallocate(m_Capacity * 2);
        
        new (&m_Container[m_Size]) ValueType( std::move(value) );
        ++m_Size;
    }

    /**
     * @brief Pops the stack.
     * 
     * Destructs the top element on the stack.
     */
    void Pop()
    {
        if (m_Size == 0) return;

        --m_Size;
        m_Container[m_Size].~ValueType();
    }

    /**
     * @brief Clears the container.
     * 
     * Destructs each element of the container.
     */
    void Clear()
    {
        for (SizeType i = 0; i < m_Size; ++i)
            m_Container[i].~ValueType();
        
        m_Size = 0;
    }

private:
    T* m_Container;
    SizeType m_Size;
    SizeType m_Capacity;

    static const SizeType INITIAL_CAPACITY = 1;

    /**
     * @brief Grows the container.
     * @param newCapacity The new capacity of the container.
     * 
     * Creates a new larger container, and moves the contents over.
     */
    void Reallocate(SizeType newCapacity)
    {
        if (newCapacity <= m_Capacity)
            throw std::invalid_argument("The new capacity must be larger than the current.");

        T* newContainer = (T*) ::operator new(newCapacity * sizeof(T));

        // move contents and call destructors
        for (SizeType i = 0; i < m_Size; ++i)
        {
            new (&newContainer[i]) ValueType( std::move(m_Container[i]) );
            m_Container[i].~ValueType();
        }

        // reassign member data
        ::operator delete(m_Container, m_Capacity * sizeof(T));
        m_Container = newContainer;
        m_Capacity = newCapacity;
    }
};
