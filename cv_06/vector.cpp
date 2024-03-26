#include <cstring>
#include <iostream>
#include <__nullptr>

template <typename T>

class CVector
{
private:
    T * m_Data;
    size_t m_Size;
    size_t m_Capacity;

    void realloc()
    {
        T * tmp = new T [m_Capacity];
        for( size_t i = 0 ; i < m_Size ; i++)
        {
            tmp[i] = m_Data[i];
        }
        delete [] m_Data;
        m_Capacity *= 2;
        m_Data = new T [m_Capacity];
        m_Data = tmp;
    }


public:
    CVector<T>()
    : m_Size(0),
      m_Capacity(5)
    {
     m_Data = new T [m_Capacity];
    }
    CVector(const CVector & a)
    : m_Capacity(a.m_Capacity),
      m_Size(a.m_Size)
    {
        m_Data = new T [m_Capacity];
        for ( size_t i = 0 ; i < a.m_Size ; i++)
        {
            m_Data[i] = a.m_Data[i];
        }
    }
    ~CVector()
    {
        delete [] m_Data;
    }

    CVector *& operator= (CVector a)
    {
        this->m_Size = a.m_Size;
        this->m_Capacity = a.m_Capacity;
        swap( &a);
    }
    void swap(CVector * a)
    {
        T * ptr = m_Data;
        size_t thisSize = m_Size;
        size_t thisCapacity = m_Capacity;

        m_Data = a->m_Data;
        a->m_Data = ptr;
        m_Capacity = a->m_Capacity;
        m_Size = a->m_Capacity;
        a->m_Capacity = thisCapacity;
        a->m_Size = thisSize;
    }
    void pushBack(T value)
    {
        if ( m_Size == m_Capacity)
            realloc();
        m_Data[m_Size] = value;
        m_Size ++;
    }
    void printVector()
    {
        for ( size_t i = 0; i < m_Size ; i++)
        {
            std::cout << m_Data[i] << " ";
        }
        std::cout << std::endl;
    }
    void insert(size_t index)
    {



    }
};

int main (void)
{
    CVector<int> intVector;
    intVector.pushBack(5);
    intVector.pushBack(5);
    intVector.pushBack(5);
    intVector.pushBack(5);
    intVector.pushBack(5);
    intVector.pushBack(5);
    intVector.printVector();

    return 0;
}
