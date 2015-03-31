#ifndef ALGO_HASHMAP_H_
#define ALGO_HASHMAP_H_

#include "algo/ParamTrait.h"

#include <string>
#include <cstring>
#include <iostream>

namespace snippet {
namespace algo {

unsigned int Hash(unsigned int key)
{
    return key;
}

unsigned int Hash(int key)
{
    return static_cast<unsigned int>(key);
}

template<typename Key, typename Value>
class HashMap
{
    struct Node
    {
        Node(typename ParamTrait<const Key>::DeclType k,
             typename ParamTrait<Value>::DeclType v,
             Node* n)
        : key(k), value(v), next(n)
        {}

        Node(typename ParamTrait<const Key>::DeclType k, Node* n)
        : key(k), value(), next(n)
        {}

        const Key key;
        Value value;
        Node* next;
    };

public:

    class Iterator
    {
    public:
        Iterator(Node** current_bucket, Node* current_node)
        : m_current_bucket(current_bucket), m_current_node(current_node)
        {}

        void operator++()
        {
            if (m_current_node->next == NULL)
            {
                m_current_node = *(++m_current_bucket);
            }
            else
            {
                m_current_node = m_current_node->next;
            }
        }

        Value* operator->()
        {
            return &(m_current_node->value);
        }

        Value& operator*()
        {
            return m_current_node->value;
        }

    private:
        Node** m_current_bucket;
        Node* m_current_node;
    };

    typedef Key KeyType;
    typedef Value ValueType;
    typedef Iterator iterator;

    HashMap()
    : m_buckets(new Node*[17])
    , m_bucket_count(17)
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
    }

    // For copy std::map/unordered_map
    template<typename Container>
    explicit HashMap(const Container& c)
    : m_buckets(new Node*[17])
    , m_bucket_count(17)
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
        // TODO: check the size of c and choose next prime to the size
        for (typename Container::const_iterator it = c.begin();
             it != c.end(); ++it)
        {
            Insert(it->first, it->second);
        }
    }

    HashMap(const HashMap& m)
    : m_buckets(new Node*[m.m_bucket_count])
    , m_bucket_count(m.m_bucket_count)
    , m_node_count(0)
    {
        // TODO: implement this
        std::cerr << "this?" << std::endl;
    }

    // Do NOT derive from this class
    ~HashMap()
    {
        Node* node = NULL;
        Node* next = NULL;
        for (std::size_t i = 0; i < m_bucket_count; ++i)
        {
            // TODO: use allocator
            node = m_buckets[i];
            while (node != NULL)
            {
                next = node->next;
                delete node;
                node = next;
            }
            m_buckets[i] = NULL;
        }

        delete [] m_buckets;
    }

    bool Insert(typename ParamTrait<const Key>::DeclType key,
                typename ParamTrait<Value>::DeclType value)
    {
        // TODO: rehash
        const unsigned int hash_code = Hash(key);
        const ::std::size_t bucket_index = hash_code % m_bucket_count;
        if (FindInBucket(m_buckets + bucket_index, key) != NULL)
        {
            return false;
        }

        // TODO: use allocator
        Node** bucket = m_buckets + bucket_index;
        Node* new_node = new Node(key, value, *bucket);
        *bucket = new_node;
        ++m_node_count;
        return true;
    }

    bool Find(typename ParamTrait<const Key>::DeclType key, Value& value) const
    {
        const unsigned int hash_code = Hash(key);
        const ::std::size_t bucket_index = hash_code % m_bucket_count;
        if (Node* node = FindInBucket(m_buckets + bucket_index, key))
        {
            value = node->value;
            return true;
        }
        else
        {
            return false;
        }
    }

    Value& FindAndInsertIfNotPresent(typename ParamTrait<const Key>::DeclType key)
    {
        const unsigned int hash_code = Hash(key);
        const ::std::size_t bucket_index = hash_code % m_bucket_count;
        if (Node* node = FindInBucket(m_buckets + bucket_index, key))
        {
            return node->value;
        }

        // TODO: use allocator
        Node** bucket = m_buckets + bucket_index;
        Node* new_node = new Node(key, *bucket);
        *bucket = new_node;
        ++m_node_count;
        return new_node->value;
    }

    bool Delete(typename ParamTrait<const Key>::DeclType key)
    {
        const unsigned int hash_code = Hash(key);
        const ::std::size_t bucket_index = hash_code % m_bucket_count;
        Node** prev_node = m_buckets + bucket_index;
        Node* cur_node = *prev_node;
        while (cur_node != NULL)
        {
            // TODO: Use Equal
            if (cur_node->key == key)
            {
                *prev_node = cur_node->next;
                // TODO: use allocator
                delete cur_node;
                --m_node_count;
                return true;
            }

            prev_node = &(cur_node->next);
            cur_node = cur_node->next;
        }

        return false;
    }

    ::std::size_t size() const { return m_node_count; }

    Value& operator[] (typename ParamTrait<const Key>::DeclType key)
    {
        return FindAndInsertIfNotPresent(key);
    }

private:
    Node* FindInBucket(Node** bucket, typename ParamTrait<const Key>::DeclType key) const
    {
        for (Node* node = *bucket; node != NULL; node = node->next)
        {
            // TODO: use Equal
            if (key == node->key)
            {
                return node;
            }
        }
        return NULL;
    }

    Node** m_buckets;
    ::std::size_t m_bucket_count;
    ::std::size_t m_node_count;
};

}  // namespace algo
}  // namespace snippet



#endif /* ALGO_HASHMAP_H_ */
