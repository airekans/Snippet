#ifndef ALGO_HASHMAP_H_
#define ALGO_HASHMAP_H_

#include "algo/ParamTrait.h"

#include <string>
#include <cstring>
#include <iostream>

namespace snippet {
namespace algo {

namespace detail {

template<typename Key, typename Value>
struct HashMapNode
{
    HashMapNode(typename ParamTrait<const Key>::DeclType k,
         typename ParamTrait<const Value>::DeclType v,
         HashMapNode* n)
    : key(k), value(v), next(n)
    {}

    HashMapNode(typename ParamTrait<const Key>::DeclType k, HashMapNode* n)
    : key(k), value(), next(n)
    {}

    const Key key;
    Value value;
    HashMapNode* next;
};

}  // namespace detail

unsigned int Hash(unsigned int key)
{
    return key;
}

unsigned int Hash(int key)
{
    return static_cast<unsigned int>(key);
}

// This hashing implementation is used by Python
unsigned int Hash(const ::std::string& str)
{
    const unsigned char* p = reinterpret_cast<const unsigned char*>(str.c_str());
    unsigned int result = (*p) << 7;
    const ::std::size_t str_size = str.size();
    for (::std::size_t i = 0; i < str_size; ++i)
    {
        result = (1000003 * result) ^ *p++;
    }
    result ^= str_size;
    return result;
}

template<typename Key, typename Value>
class HashMap
{
    typedef detail::HashMapNode<Key, Value> Node;

    class IteratorBase
    {
        friend bool operator== (const IteratorBase& lhs, const IteratorBase& rhs)
        {
            return lhs.IsEqual(rhs);
        }

        friend bool operator!= (const IteratorBase& lhs, const IteratorBase& rhs)
        {
            return !lhs.IsEqual(rhs);
        }

    public:
        // current_node must in the current_bucket.
        IteratorBase(Node** current_bucket, Node* current_node)
        : m_current_bucket(current_bucket), m_current_node(current_node)
        {
            if (m_current_node == NULL)
            {
                this->IncrementBucket();
            }
        }

        void Next()
        {
            if (m_current_node->next == NULL)
            {
                this->IncrementBucket();
            }
            else
            {
                m_current_node = m_current_node->next;
            }
        }

    protected:
        void IncrementBucket()
        {
            while ((m_current_node = *(++m_current_bucket)) == NULL) ;
        }

        bool IsEqual(const IteratorBase& other) const
        {
            return m_current_bucket == other.m_current_bucket &&
                    m_current_node == other.m_current_node;
        }

        Node** m_current_bucket;
        Node* m_current_node;
    };

public:

    class Iterator : public IteratorBase
    {
    public:
        // current_node must in the current_bucket.
        Iterator(Node** current_bucket, Node* current_node)
        : IteratorBase(current_bucket, current_node)
        {}

        Iterator& operator++()
        {
            this->Next();
            return *this;
        }

        typename ParamTrait<const Key>::DeclType GetKey() const
        {
            return this->m_current_node->key;
        }

        Value& GetValue()
        {
            return this->m_current_node->value;
        }
    };

    class ConstIterator : public IteratorBase
    {
    public:
        // current_node must in the current_bucket.
        ConstIterator(Node** current_bucket, Node* current_node)
        : IteratorBase(current_bucket, current_node)
        {}

        // We can convert a Iterator to ConstIterator
        ConstIterator(const Iterator& it)
        : IteratorBase(it)
        {}

        ConstIterator& operator++()
        {
            this->Next();
            return *this;
        }

        typename ParamTrait<const Key>::DeclType GetKey() const
        {
            return this->m_current_node->key;
        }

        typename ParamTrait<const Value>::DeclType GetValue() const
        {
            return this->m_current_node->value;
        }
    };

    typedef Key KeyType;
    typedef Value ValueType;
    typedef Iterator iterator;
    typedef ConstIterator const_iterator;

    HashMap()
    : m_buckets(new Node*[9977 + 1])
    , m_bucket_count(9977)
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
        m_buckets[m_bucket_count] = reinterpret_cast<Node*>(0x0123);
    }

    // For copy std::map/unordered_map
    template<typename Container>
    explicit HashMap(const Container& c)
    : m_buckets(new Node*[9977+ 1])
    , m_bucket_count(9977)
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
        m_buckets[m_bucket_count] = reinterpret_cast<Node*>(0x0123);
        // TODO: check the size of c and choose next prime to the size
        for (typename Container::const_iterator it = c.begin();
             it != c.end(); ++it)
        {
            Insert(it->first, it->second);
        }
    }

    HashMap(const HashMap& m)
    : m_buckets(new Node*[m.m_bucket_count + 1])
    , m_bucket_count(m.m_bucket_count)
    , m_node_count(0)
    {
        for (::std::size_t i = 0; i < m_bucket_count; ++i)
        {
            if (m.m_buckets[i] == NULL)
            {
                m_buckets[i] = NULL;
            }
            else
            {
                Node* node = m.m_buckets[i];
                Node** prev_node = m_buckets + i;
                while (node != NULL)
                {
                    // TODO: use allocator
                    Node* new_node = new Node(node->key, node->value, NULL);
                    *prev_node = new_node;
                    prev_node = &(new_node->next);
                    node = node->next;
                    ++m_node_count;
                }
            }
        }
        m_buckets[m_bucket_count] = reinterpret_cast<Node*>(0x0123);
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
                typename ParamTrait<const Value>::DeclType value)
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

    void PrintDebugString() const
    {
        for (::std::size_t i = 0; i < size(); ++i)
        {
            ::std::cout << i << "    : ";
            for (Node* node = m_buckets[i]; node != NULL; node = node->next)
            {
                ::std::cout << "(" << node->key << "," << node->value << ") ";
            }
            ::std::cout << ::std::endl;
        }
    }

    ::std::size_t size() const { return m_node_count; }

    Value& operator[] (typename ParamTrait<const Key>::DeclType key)
    {
        return FindAndInsertIfNotPresent(key);
    }

    iterator begin() { return iterator(m_buckets, *m_buckets); }
    const_iterator begin() const { return const_iterator(m_buckets, *m_buckets); }

    iterator end()
    {
        return iterator(m_buckets + m_bucket_count, m_buckets[m_bucket_count]);
    }

    const_iterator end() const
    {
        return const_iterator(m_buckets + m_bucket_count, m_buckets[m_bucket_count]);
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
