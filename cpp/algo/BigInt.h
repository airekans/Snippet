// -*- mode: c++ -*-

#include <cstring>
#include <iostream>

template<typename T = char>
class BigInt
{
public:
    BigInt();
    BigInt(const char* str_num);
    BigInt(int num);
    BigInt(const BigInt& big_int);
    BigInt& operator=(const BigInt& big_int);

    void Dump(std::ostream& ostream = std::cout) const;

    BigInt Add(const BigInt& big_int) const;

    bool IsEqual(const BigInt& big_int) const;
    bool IsGreaterThan(const BigInt& big_int) const;

private:
    int m_length;
#define MAXN 1000
    T m_digits[MAXN];

};

template <typename T>
BigInt<T>::BigInt()
    : m_length(0)
{}

template <typename T>
BigInt<T>::BigInt(const char* str_num)
{
    m_length = strlen(str_num);
    for (int i = 0; i < m_length; ++i)
    {
        m_digits[i] = static_cast<T>(str_num[m_length - i - 1] - '0');
    }
}

template <typename T>
BigInt<T>::BigInt(int num)
    : m_length(0)
{
    while (num > 0)
    {
        m_digits[m_length++] = static_cast<T>(num % 10);
        num /= 10;
    }
}

template <typename T>
BigInt<T>::BigInt(const BigInt& big_int)
{
    m_length = big_int.m_length;
    for (int i = 0; i < m_length; ++i)
    {
        m_digits[i] = big_int.m_digits[i];
    }
}

template <typename T>
BigInt<T>& BigInt<T>::operator=(const BigInt& big_int)
{
    if (this != &big_int)
    {
        m_length = big_int.m_length;
        for (int i = 0; i < m_length; ++i)
        {
            m_digits[i] = big_int.m_digits[i];
        }
    }
    return *this;
}

template <typename T>
void BigInt<T>::Dump(std::ostream& ostream) const
{
    for (int i = 0; i < m_length; ++i)
    {
        std::cout << static_cast<int>(m_digits[m_length - i - 1]);
    }
}

template <typename T>
BigInt<T> BigInt<T>::Add(const BigInt& big_int) const
{
    BigInt result;
    const int size = m_length > big_int.m_length? m_length: big_int.m_length;
    int carry = 0;
    for (int i = 0; i < size; ++i)
    {
        const int lhs = static_cast<int>(i < m_length? m_digits[i]: (T)0);
        const int rhs = static_cast<int>(i < big_int.m_length? big_int.m_digits[i]: (T)0);
        const int tmp = lhs + rhs + carry;
        result.m_digits[i] = static_cast<T>(tmp % 10);
        carry = tmp / 10;
    }

    if (carry > 0)
    {
        result.m_digits[size] = static_cast<T>(carry);
        result.m_length = size + 1;
    }
    else
    {
        result.m_length = size;
    }
    return result;
}

template <typename T>
bool BigInt<T>::IsEqual(const BigInt& big_int) const
{
    if (m_length != big_int.m_length)
    {
        return false;
    }

    for (int i = 0; i < m_length; ++i)
    {
        if (m_digits[i] != big_int.m_digits[i])
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool BigInt<T>::IsGreaterThan(const BigInt& big_int) const
{
    if (m_length > big_int.m_length)
    {
        return true;
    }
    else if (m_length < big_int.m_length)
    {
        return false;
    }
    else
    {
        for (int i = m_length - 1; i >= 0; ++i)
        {
            if (m_digits[i] > big_int.m_digits[i])
            {
                return true;
            }
            else if (m_digits[i] < big_int.m_digits[i])
            {
                return false;
            }
        }
        return false;
    }
}


template <typename T>
BigInt<T> operator+(const BigInt<T>& lhs, const BigInt<T>& rhs)
{
    return lhs.Add(rhs);
}

template <typename T>
bool operator==(const BigInt<T>& lhs, const BigInt<T>& rhs)
{
    return lhs.IsEqual(rhs);
}

template <typename T>
bool operator!=(const BigInt<T>& lhs, const BigInt<T>& rhs)
{
    return !lhs.IsEqual(rhs);
}


template <typename T>
std::ostream& operator<<(std::ostream& ostream, BigInt<T>& big_int)
{
    big_int.Dump(ostream);
    return ostream;
}


