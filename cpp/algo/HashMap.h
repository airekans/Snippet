#ifndef ALGO_HASHMAP_H_
#define ALGO_HASHMAP_H_

#include "algo/ParamTrait.h"

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <memory>

namespace snippet {
namespace algo {

namespace detail {

template<typename Key, typename Value, bool IsCacheHash>
struct HashMapNode
{
    HashMapNode(typename ParamTrait<const Key>::DeclType k,
         typename ParamTrait<const Value>::DeclType v,
         HashMapNode* n, std::size_t h)
    : key(k), value(v), next(n), cached_hash(h)
    {}

    HashMapNode(typename ParamTrait<const Key>::DeclType k, HashMapNode* n,
                std::size_t h)
    : key(k), value(), next(n), cached_hash(h)
    {}

    HashMapNode(const HashMapNode& other)
    : key(other.key), value(other.value), next(other.next)
    , cached_hash(other.cached_hash)
    {}

    const Key key;
    Value value;
    HashMapNode* next;
    std::size_t cached_hash;
};

// not cache hash code
template<typename Key, typename Value>
struct HashMapNode<Key, Value, false>
{
    HashMapNode(typename ParamTrait<const Key>::DeclType k,
         typename ParamTrait<const Value>::DeclType v,
         HashMapNode* n, std::size_t)
    : key(k), value(v), next(n)
    {}

    HashMapNode(typename ParamTrait<const Key>::DeclType k, HashMapNode* n,
                std::size_t)
    : key(k), value(), next(n)
    {}

    HashMapNode(const HashMapNode& other)
    : key(other.key), value(other.value), next(other.next)
    {}

    const Key key;
    Value value;
    HashMapNode* next;
};

enum { PRIME_NUM = 29 };

template<typename T>
struct HashTablePrimeList
{
    static const T s_prime_list[PRIME_NUM];

    static const T* GetPrimeList()
    {
        return s_prime_list;
    }
};

template<typename T>
const T HashTablePrimeList<T>::s_prime_list[PRIME_NUM] =
{
        5ul,          53ul,         97ul,         193ul,       389ul,
        769ul,        1543ul,       3079ul,       6151ul,      12289ul,
        24593ul,      49157ul,      98317ul,      196613ul,    393241ul,
        786433ul,     1572869ul,    3145739ul,    6291469ul,   12582917ul,
        25165843ul,   50331653ul,   100663319ul,  201326611ul, 402653189ul,
        805306457ul,  1610612741ul, 3221225473ul, 4294967291ul
};


template<typename T, unsigned int TypeSize>
struct HashDouble;

template<typename T>
struct HashDouble<T, 4>
{
    static std::size_t Hash(T* num)
    {
        return *reinterpret_cast<std::size_t*>(num);
    }
};

template<typename T>
struct HashDouble<T, 8>
{
    static std::size_t Hash(T* num)
    {
        // because the following condition can be determined in
        // compile time, compiler will eliminate the dead path.
        if (sizeof(std::size_t) == 4)
        {
            const unsigned char* p = reinterpret_cast<const unsigned char*>(num);
            std::size_t result = (*p) << 7;
            for (::std::size_t i = 0; i < 8; ++i)
            {
                result = (1000003 * result) ^ *p++;
            }
            result ^= 8;
            return result;
        }
        else // sizeof(std::size_t) == 8
        {
            return *reinterpret_cast<std::size_t*>(num);
        }
    }
};

}  // namespace detail

std::size_t Hash(std::size_t key)
{
    return key;
}

std::size_t Hash(unsigned int key)
{
    return static_cast<std::size_t>(key);
}

std::size_t Hash(int key)
{
    return static_cast<std::size_t>(key);
}

std::size_t Hash(float key)
{
    return detail::HashDouble<float, sizeof(float)>::Hash(&key);
}

std::size_t Hash(double key)
{
    return detail::HashDouble<double, sizeof(double)>::Hash(&key);
}

// This hashing implementation is used by Python
std::size_t Hash(const ::std::string& str)
{
    const unsigned char* p = reinterpret_cast<const unsigned char*>(str.c_str());
    std::size_t result = (*p) << 7;
    const ::std::size_t str_size = str.size();
    for (::std::size_t i = 0; i < str_size; ++i)
    {
        result = (1000003 * result) ^ *p++;
    }
    result ^= str_size;
    return result;
}


template<typename Key>
struct DefaultHashMapHashPolicy
{
    static std::size_t DoHash(typename ParamTrait<const Key>::DeclType key)
    {
        return Hash(key);
    }
};

class DefaultHashMapRehashPolicy
{
public:
    typedef detail::HashTablePrimeList<std::size_t> PrimeList;

    DefaultHashMapRehashPolicy(unsigned int load_factor = 2)
    : m_load_factor(load_factor)
    {}

    bool IsRehash(::std::size_t bucket_count, ::std::size_t node_count) const
    {
        return (node_count / bucket_count) >= m_load_factor;
    }

    std::size_t NextBucketCount(::std::size_t hint) const
    {
        const std::size_t* first = PrimeList::GetPrimeList();
        const std::size_t* last = first + detail::PRIME_NUM;
        const std::size_t* pos = ::std::lower_bound(first, last, hint);
        return pos == last? *(last - 1) : *pos;
    }

    std::size_t BucketCountForElements(::std::size_t elements) const
    {
        std::size_t min_bucket = elements / m_load_factor + 1;
        const std::size_t* first = PrimeList::GetPrimeList();
        const std::size_t* last = first + detail::PRIME_NUM;
        const std::size_t* pos = ::std::lower_bound(first, last, min_bucket);
        return pos == last? *(last - 1) : *pos;
    }

private:
    const unsigned int m_load_factor;
};


template<typename Key>
struct DefaultKeyEqual
{
    static bool Equal(typename ParamTrait<const Key>::DeclType lhs,
                      typename ParamTrait<const Key>::DeclType rhs)
    {
        return lhs == rhs;
    }
};


template<typename Key, typename Value, typename HashPolicy,
         bool IsCacheHash>
class RehashBase;

template<typename Key, typename Value, typename HashPolicy>
class RehashBase<Key, Value, HashPolicy, true>
{
    typedef detail::HashMapNode<Key, Value, true> Node;

public:
    static void DoRehash(Node** old_buckets, ::std::size_t old_bucket_count,
                         Node** new_buckets, ::std::size_t new_bucket_count,
                         const HashPolicy&)
    {
        ::std::size_t bucket_index = 0;
        Node* node = NULL;
        Node* next_node = NULL;
        for (::std::size_t i = 0; i < old_bucket_count; ++i)
        {
            for (node = old_buckets[i]; node != NULL; node = next_node)
            {
                bucket_index = node->cached_hash % new_bucket_count;
                next_node = node->next;
                node->next = new_buckets[bucket_index];
                new_buckets[bucket_index] = node;
            }
        }
    }
};

template<typename Key, typename Value, typename HashPolicy>
class RehashBase<Key, Value, HashPolicy, false>
{
    typedef detail::HashMapNode<Key, Value, false> Node;

public:
    static void DoRehash(Node** old_buckets, ::std::size_t old_bucket_count,
                         Node** new_buckets, ::std::size_t new_bucket_count,
                         const HashPolicy& hash_policy)
    {
        ::std::size_t bucket_index = 0;
        Node* node = NULL;
        Node* next_node = NULL;
        for (::std::size_t i = 0; i < old_bucket_count; ++i)
        {
            for (node = old_buckets[i]; node != NULL; node = next_node)
            {
                bucket_index = hash_policy.DoHash(node->key) % new_bucket_count;
                next_node = node->next;
                node->next = new_buckets[bucket_index];
                new_buckets[bucket_index] = node;
            }
        }
    }
};


// KeyEqual and NodeAllocator should not define Equal method at the same time.
template<typename Key, typename Value,
         typename KeyEqual = DefaultKeyEqual<Key>,
         typename HashPolicy = DefaultHashMapHashPolicy<Key>,
         typename RehashPolicy = DefaultHashMapRehashPolicy,
         typename Allocator = ::std::allocator<Key>,
         bool IsCacheHash = true>
class HashMap : public RehashBase<Key, Value, HashPolicy, IsCacheHash>
{
public:
    typedef detail::HashMapNode<Key, Value, IsCacheHash> Node;

private:
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
    typedef typename Allocator::template rebind<Node>::other NodeAllocator;
    typedef typename Allocator::template rebind<Node*>::other BucketAllocator;

    typedef KeyEqual key_equal;
    typedef HashPolicy hash_policy;
    typedef RehashPolicy rehash_policy;


    HashMap(std::size_t size_hint = 0,
            const KeyEqual& key_equal = KeyEqual(),
            const HashPolicy& hash_policy = HashPolicy(),
            const RehashPolicy& rehash_policy = RehashPolicy(),
            const NodeAllocator& node_alloc = NodeAllocator(),
            const BucketAllocator& bucket_alloc = BucketAllocator())
    : m_hash_impl(node_alloc, key_equal, hash_policy)
    , m_rehash_impl(bucket_alloc, rehash_policy)
    , m_bucket_count(rehash_policy.NextBucketCount(size_hint))
    , m_buckets(m_rehash_impl.allocate(m_bucket_count + 1))
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
        m_buckets[m_bucket_count] = reinterpret_cast<Node*>(0x0123);
    }

    // For copy std::map/unordered_map
    template<typename Container>
    HashMap(const Container& c,
            const KeyEqual& key_equal = KeyEqual(),
            const HashPolicy& hash_policy = HashPolicy(),
            const RehashPolicy& rehash_policy = RehashPolicy(),
            const NodeAllocator& node_alloc = NodeAllocator(),
            const BucketAllocator& bucket_alloc = BucketAllocator())
    : m_hash_impl(node_alloc, key_equal, hash_policy)
    , m_rehash_impl(bucket_alloc, rehash_policy)
    , m_bucket_count(m_rehash_impl.rehash_policy.BucketCountForElements(c.size()))
    , m_buckets(m_rehash_impl.allocate(m_bucket_count + 1))
    , m_node_count(0)
    {
        memset(m_buckets, 0, sizeof(Node*) * m_bucket_count);
        m_buckets[m_bucket_count] = reinterpret_cast<Node*>(0x0123);
        for (typename Container::const_iterator it = c.begin();
             it != c.end(); ++it)
        {
            Insert(it->first, it->second);
        }
    }

    HashMap(const HashMap& m)
    : m_hash_impl(m.m_hash_impl)
    , m_rehash_impl(m.m_rehash_impl)
    , m_bucket_count(m.m_bucket_count)
    , m_buckets(m_rehash_impl.allocate(m.m_bucket_count + 1))
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
                    Node* new_node = m_hash_impl.allocate(1);
                    (void) new (new_node) Node(*node);
                    new_node->next = NULL;
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
        Clear();
        m_rehash_impl.deallocate(m_buckets, m_bucket_count + 1);
    }

    bool Insert(typename ParamTrait<const Key>::DeclType key,
                typename ParamTrait<const Value>::DeclType value)
    {
        const std::size_t hash_code = m_hash_impl.hash_policy.DoHash(key);
        ::std::size_t bucket_index = hash_code % m_bucket_count;
        if (FindInBucket(m_buckets + bucket_index, key) != NULL)
        {
            return false;
        }

        if (m_rehash_impl.rehash_policy.IsRehash(m_bucket_count, m_node_count + 1))
        {
            std::size_t new_bucket_count =
                    m_rehash_impl.rehash_policy.BucketCountForElements(m_node_count + 1);
            RehashImpl(new_bucket_count);
            bucket_index = hash_code % m_bucket_count;
        }

        Node** bucket = m_buckets + bucket_index;
        Node* new_node = m_hash_impl.allocate(1);
        (void) new (new_node) Node(key, value, *bucket, hash_code);
        *bucket = new_node;
        ++m_node_count;
        return true;
    }

    bool Find(typename ParamTrait<const Key>::DeclType key, Value& value) const
    {
        const std::size_t hash_code = m_hash_impl.hash_policy.DoHash(key);
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
        const std::size_t hash_code = m_hash_impl.hash_policy.DoHash(key);
        ::std::size_t bucket_index = hash_code % m_bucket_count;
        if (Node* node = FindInBucket(m_buckets + bucket_index, key))
        {
            return node->value;
        }

        if (m_rehash_impl.rehash_policy.IsRehash(m_bucket_count, m_node_count + 1))
        {
            std::size_t new_bucket_count =
                    m_rehash_impl.rehash_policy.BucketCountForElements(m_node_count + 1);
            RehashImpl(new_bucket_count);
            bucket_index = hash_code % m_bucket_count;
        }

        Node** bucket = m_buckets + bucket_index;
        Node* new_node = m_hash_impl.allocate(1);
        (void) new (new_node) Node(key, *bucket, hash_code);
        *bucket = new_node;
        ++m_node_count;
        return new_node->value;
    }

    bool Delete(typename ParamTrait<const Key>::DeclType key)
    {
        const std::size_t hash_code = m_hash_impl.hash_policy.DoHash(key);
        const ::std::size_t bucket_index = hash_code % m_bucket_count;
        Node** prev_node = m_buckets + bucket_index;
        Node* cur_node = *prev_node;
        while (cur_node != NULL)
        {
            if (m_hash_impl.Equal(cur_node->key, key))
            {
                *prev_node = cur_node->next;
                cur_node->~Node();
                m_hash_impl.deallocate(cur_node, 1);
                --m_node_count;

                // Try to rehash
                // But the default rehash policy will not rehash after deleting
                if (m_rehash_impl.rehash_policy.IsRehash(m_bucket_count, m_node_count))
                {
                    std::size_t new_bucket_count =
                            m_rehash_impl.rehash_policy.BucketCountForElements(m_node_count);
                    RehashImpl(new_bucket_count);
                }

                return true;
            }

            prev_node = &(cur_node->next);
            cur_node = cur_node->next;
        }

        return false;
    }

    void Clear()
    {
        Node* node = NULL;
        Node* next = NULL;
        for (std::size_t i = 0; i < m_bucket_count; ++i)
        {
            node = m_buckets[i];
            while (node != NULL)
            {
                next = node->next;
                node->~Node();
                m_hash_impl.deallocate(node, 1);
                node = next;
            }
            m_buckets[i] = NULL;
        }

        m_node_count = 0;
        if (m_rehash_impl.rehash_policy.IsRehash(m_bucket_count, m_node_count))
        {
            std::size_t new_bucket_count =
                    m_rehash_impl.rehash_policy.BucketCountForElements(m_node_count);
            RehashImpl(new_bucket_count);
        }
    }

    // if hint is 0, then try to rehash to fit the current node_count;
    // returns the new bucket_count
    ::std::size_t Rehash(std::size_t size_hint = 0)
    {
        if (size_hint == 0)
        {
            size_hint = m_rehash_impl.rehash_policy.BucketCountForElements(m_node_count);
        }
        else
        {
            size_hint = m_rehash_impl.rehash_policy.NextBucketCount(size_hint);
        }

        if (size_hint != m_bucket_count)
        {
            RehashImpl(size_hint);
        }
        return m_bucket_count;
    }

    ::std::size_t GetBucketCount() const { return m_bucket_count; }

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

    NodeAllocator& GetNodeAllocator() { return m_hash_impl; }
    const NodeAllocator& GetNodeAllocator() const { return m_hash_impl; }
    BucketAllocator& GetBucketAllocator() { return m_rehash_impl; }
    const BucketAllocator& GetBucketAllocator() const { return m_rehash_impl; }

    // STL compatible methods
    ::std::size_t size() const { return m_node_count; }
    bool empty() const { return m_node_count == 0; }
    void clear() { Clear(); }

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
            if (m_hash_impl.Equal(key, node->key))
            {
                return node;
            }
        }
        return NULL;
    }

    void RehashImpl(std::size_t new_bucket_count)
    {
        Node** new_buckets = m_rehash_impl.allocate(new_bucket_count + 1);
        memset(new_buckets, 0, sizeof(Node*) * new_bucket_count);
        new_buckets[new_bucket_count] = reinterpret_cast<Node*>(0x0123);

        this->DoRehash(m_buckets, m_bucket_count, new_buckets, new_bucket_count,
                       m_hash_impl.hash_policy);

        m_rehash_impl.deallocate(m_buckets, m_bucket_count + 1);
        m_buckets = new_buckets;
        m_bucket_count = new_bucket_count;
    }


    struct HashPolicyAndNodeAllocator : public NodeAllocator, public KeyEqual
    {
        HashPolicyAndNodeAllocator(const NodeAllocator& alloc,
                                   const KeyEqual& key_equal,
                                   const HashPolicy& policy)
        : NodeAllocator(alloc), KeyEqual(key_equal), hash_policy(policy)
        {}

        HashPolicy hash_policy;
    };

    HashPolicyAndNodeAllocator m_hash_impl;

    struct RehashPolicyAndBucketAllocator : public BucketAllocator
    {
        RehashPolicyAndBucketAllocator(const BucketAllocator& alloc,
                                       const RehashPolicy& policy)
        : BucketAllocator(alloc), rehash_policy(policy)
        {}

        RehashPolicy rehash_policy;
    };

    RehashPolicyAndBucketAllocator m_rehash_impl;

    ::std::size_t m_bucket_count;
    Node** m_buckets;
    ::std::size_t m_node_count;
};

}  // namespace algo
}  // namespace snippet



#endif /* ALGO_HASHMAP_H_ */
