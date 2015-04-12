#include "HashMap.h"

#include <gtest/gtest.h>

#include <memory>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>
#include <iostream>

using snippet::algo::HashMap;
using namespace snippet::algo;
using namespace std;

TEST(HashMap, TestCtor)
{
    HashMap<int, int> hash_map;
    HashMap<int, string> hash_map2;
    HashMap<string, int> hash_map3;
    HashMap<string, string> hash_map4;

    ASSERT_EQ(0, hash_map.size());
}

TEST(HashMap, TestCopyStdMap)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;

    HashMap<int, int> hash_map(std_map);

    ASSERT_EQ(std_map.size(), hash_map.size());
}

TEST(HashMap, TestCopyCtor)
{
    map<string, string> std_map;
    std_map["123"] = "123";
    std_map["abc"] = "abc";
    std_map["456"] = "456";
    HashMap<string, string> hash_map1(std_map);
    ASSERT_EQ(std_map.size(), hash_map1.size());

    HashMap<string, string> hash_map2(hash_map1);
    ASSERT_EQ(std_map.size(), hash_map2.size());

    for (map<string, string>::const_iterator it = std_map.begin();
         it != std_map.end(); ++it)
    {
        string value;
        ASSERT_TRUE(hash_map2.Find(it->first, value));
        ASSERT_EQ(it->second, value);
    }
}

TEST(HashMap, TestFind)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    for (map<int, int>::const_iterator it = std_map.begin();
         it != std_map.end(); ++it)
    {
        int value = 0;
        ASSERT_TRUE(hash_map.Find(it->first, value));
        ASSERT_EQ(it->second, value);
    }
}

TEST(HashMap, TestInsert)
{
    HashMap<int, int> hash_map;
    ASSERT_TRUE(hash_map.Insert(1, 2));
    ASSERT_EQ(1, hash_map.size());

    int value = 0;
    ASSERT_TRUE(hash_map.Find(1, value));
    ASSERT_EQ(2, value);

    ASSERT_FALSE(hash_map.Insert(1, 2));
}

TEST(HashMap, TestInsertConstValue)
{
    HashMap<int, string> hash_map;
    const string const_str = "abc";
    ASSERT_TRUE(hash_map.Insert(1, const_str));
    ASSERT_EQ(1, hash_map.size());
}

namespace {

struct TestObject
{
    TestObject()
    : m_data(0)
    {}

    explicit TestObject(int data)
    : m_data(data)
    {}

    TestObject(const TestObject& obj)
    : m_data(obj.m_data)
    {
        cerr << "TestObject CopyCtor" << endl;
    }

    int m_data;
};

}

TEST(HashMap, TestInsertTestObject)
{
    HashMap<string, TestObject> hash_map;
    ASSERT_TRUE(hash_map.Insert("123", TestObject(1)));
    ASSERT_EQ(1, hash_map.size());

    TestObject value;
    ASSERT_TRUE(hash_map.Find("123", value));
    ASSERT_EQ(1, value.m_data);

    ASSERT_FALSE(hash_map.Insert("123", TestObject(2)));
    ASSERT_EQ(1, hash_map["123"].m_data);
    hash_map["123"].m_data = 3;
    ASSERT_EQ(3, hash_map["123"].m_data);
    ASSERT_EQ(1, hash_map.size());
}

TEST(HashMap, TestDelete)
{
    map<int, int> std_map;
    for (int i = 0; i < 10; ++i)
    {
        std_map[i] = i;
    }
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    std::size_t bucket_count = hash_map.GetBucketCount();
    ASSERT_GT(bucket_count, 0);

    ASSERT_TRUE(hash_map.Delete(1));
    ASSERT_EQ(std_map.size() - 1, hash_map.size());
    int value = 0;
    ASSERT_FALSE(hash_map.Find(1, value));

    ASSERT_FALSE(hash_map.Delete(1));

    ASSERT_TRUE(hash_map.Delete(2));
    ASSERT_TRUE(hash_map.Delete(3));
    ASSERT_EQ(std_map.size() - 3, hash_map.size());

    ASSERT_FALSE(hash_map.Delete(2));
    ASSERT_EQ(std_map.size() - 3, hash_map.size());

    // default rehash policy will not shrink bucket
    ASSERT_EQ(bucket_count, hash_map.GetBucketCount());
}

TEST(HashMap, TestFindAndInsertIfNotPresent)
{
    HashMap<string, string> hash_map;
    ASSERT_EQ(0, hash_map.size());

    hash_map.FindAndInsertIfNotPresent("123") = "123";
    ASSERT_EQ(1, hash_map.size());
    ASSERT_EQ("123", hash_map.FindAndInsertIfNotPresent("123"));
    ASSERT_EQ(1, hash_map.size());

    hash_map.FindAndInsertIfNotPresent("abc") = "abc";
    ASSERT_EQ(2, hash_map.size());
}

TEST(HashMap, TestIterator)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    unsigned int iter_count = 0;
    for (HashMap<int, int>::iterator it = hash_map.begin();
         it != hash_map.end(); ++it, ++iter_count)
    {
        ASSERT_TRUE(std_map.find(it.GetKey()) != std_map.end());
        ASSERT_EQ(std_map.find(it.GetKey())->second, it.GetValue());
    }

    ASSERT_EQ(hash_map.size(), iter_count);
}

TEST(HashMap, TestIteratorAndAssign)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    unsigned int iter_count = 0;
    for (HashMap<int, int>::iterator it = hash_map.begin();
         it != hash_map.end(); ++it, ++iter_count)
    {
        ASSERT_TRUE(std_map.find(it.GetKey()) != std_map.end());
        ASSERT_EQ(std_map.find(it.GetKey())->second, it.GetValue());
        it.GetValue() = 8;
    }
    ASSERT_EQ(std_map.size(), iter_count);

    iter_count = 0;
    for (HashMap<int, int>::iterator it = hash_map.begin();
         it != hash_map.end(); ++it, ++iter_count)
    {
        ASSERT_TRUE(std_map.find(it.GetKey()) != std_map.end());
        ASSERT_EQ(8, it.GetValue());
    }
    ASSERT_EQ(std_map.size(), iter_count);
}

TEST(HashMap, TestConstIterator)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    unsigned int iter_count = 0;
    for (HashMap<int, int>::const_iterator it = hash_map.begin();
         it != hash_map.end(); ++it, ++iter_count)
    {
        ASSERT_TRUE(std_map.find(it.GetKey()) != std_map.end());
        ASSERT_EQ(std_map.find(it.GetKey())->second, it.GetValue());
    }

    ASSERT_EQ(hash_map.size(), iter_count);
}

TEST(HashMap, TestConstHashMap)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    const HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    unsigned int iter_count = 0;
    for (HashMap<int, int>::const_iterator it = hash_map.begin();
         it != hash_map.end(); ++it, ++iter_count)
    {
        ASSERT_TRUE(std_map.find(it.GetKey()) != std_map.end());
        ASSERT_EQ(std_map.find(it.GetKey())->second, it.GetValue());

        int value = 0;
        ASSERT_TRUE(hash_map.Find(it.GetKey(), value));
        ASSERT_EQ(it.GetValue(), value);
    }

    ASSERT_EQ(hash_map.size(), iter_count);
}

TEST(HashMap, TestBucketCount)
{
    HashMap<int, int> hash_map;
    typedef typename HashMap<int, int>::rehash_policy::PrimeList DefaultPrimeList;

    ASSERT_EQ(DefaultPrimeList::GetPrimeList()[0], hash_map.GetBucketCount());
    ASSERT_EQ(0, hash_map.size());

    std::size_t grow_num = DefaultPrimeList::GetPrimeList()[0] * 2;
    for (std::size_t i = 0; i < grow_num - 1; ++i)
    {
        hash_map[i] = i;
        ASSERT_EQ(i + 1, hash_map.size());
        ASSERT_EQ(DefaultPrimeList::GetPrimeList()[0], hash_map.GetBucketCount());
    }

    hash_map[grow_num] = grow_num;
    ASSERT_EQ(grow_num, hash_map.size());
    ASSERT_EQ(DefaultPrimeList::GetPrimeList()[1], hash_map.GetBucketCount());
}

namespace {

unsigned int Hash(const TestObject& obj)
{
    return static_cast<unsigned int>(obj.m_data);
}

bool operator== (const TestObject& lhs, const TestObject& rhs)
{
    return lhs.m_data == rhs.m_data;
}

struct TestObjectHashPolicy
{
    static unsigned int DoHash(const TestObject& key)
    {
        return static_cast<unsigned int>(key.m_data);
    }
};

struct TestObjectEqual
{
    static bool Equal(const TestObject& lhs, const TestObject& rhs)
    {
        return lhs == rhs;
    }
};

}

TEST(HashMap, TestCustomHash)
{
    HashMap<TestObject, int> hash_map;
    TestObject obj;
    hash_map[obj] = 1;

    HashMap<TestObject, int, TestObjectEqual, TestObjectHashPolicy> hash_map1;
    hash_map1[obj] = 1;
}

TEST(HashMap, TestRehash)
{
    HashMap<int, int> hash_map;
    std::size_t bucket_count = hash_map.GetBucketCount();
    ASSERT_GT(hash_map.GetBucketCount(), 0);

    std::size_t new_bucket_count = hash_map.Rehash(hash_map.GetBucketCount() + 1);
    ASSERT_GT(new_bucket_count, bucket_count);
    ASSERT_EQ(new_bucket_count, hash_map.GetBucketCount());

    new_bucket_count = hash_map.Rehash();
    ASSERT_EQ(bucket_count, hash_map.GetBucketCount());

    new_bucket_count = hash_map.Rehash();
    ASSERT_EQ(bucket_count, hash_map.GetBucketCount());
}

TEST(HashMap, TestClear)
{
    HashMap<int, int> hash_map;
    ASSERT_TRUE(hash_map.empty());
    ASSERT_GT(hash_map.GetBucketCount(), 0);

    hash_map.Clear();
    ASSERT_TRUE(hash_map.empty());
    ASSERT_GT(hash_map.GetBucketCount(), 0);

    for (int i = 0; i < 20; ++i)
    {
        hash_map[i] = i;
    }
    ASSERT_FALSE(hash_map.empty());
    ASSERT_EQ(20, hash_map.size());
    ASSERT_GT(hash_map.GetBucketCount(), 0);

    hash_map.Clear();
    ASSERT_TRUE(hash_map.empty());
    ASSERT_GT(hash_map.GetBucketCount(), 0);
}

namespace {

template<typename T>
struct MemRecordAllocator
{
    template<typename Other>
    struct rebind
    {
        typedef MemRecordAllocator<Other> other;
    };

    typedef T Elem;

    MemRecordAllocator() : allocated_bytes(0) {}

    T* allocate(size_t n, const void* = 0)
    {
        allocated_bytes += sizeof(T) * n;
        return reinterpret_cast<T*>(::malloc(sizeof(T) * n));
    }

    void deallocate(T* p, size_t n)
    {
        ::free(p);
        allocated_bytes -= sizeof(T) * n;
    }

    size_t allocated_bytes;
};

}

TEST(HashMap, TestNotCachedHash)
{
    typedef HashMap<int, int, DefaultKeyEqual<int>, DefaultHashMapHashPolicy<int>,
                    DefaultHashMapRehashPolicy, MemRecordAllocator<int>, false> HashMapNotCacheKey;
    typedef HashMap<int, int, DefaultKeyEqual<int>, DefaultHashMapHashPolicy<int>,
            DefaultHashMapRehashPolicy, MemRecordAllocator<int> > HashMapCacheKey;

    ASSERT_GT(sizeof(HashMapCacheKey::NodeAllocator::Elem),
              sizeof(HashMapNotCacheKey::NodeAllocator::Elem));

    HashMapNotCacheKey hash_map(static_cast<std::size_t>(0),
                                HashMapNotCacheKey::key_equal(), HashMapNotCacheKey::hash_policy(),
                                HashMapNotCacheKey::rehash_policy());
    HashMapNotCacheKey::NodeAllocator& node_allocator = hash_map.GetNodeAllocator();
    HashMapNotCacheKey::BucketAllocator& bucket_allocator = hash_map.GetBucketAllocator();

    HashMapCacheKey hash_map1(static_cast<std::size_t>(0),
                              HashMapCacheKey::key_equal(), HashMapCacheKey::hash_policy(),
                              HashMapCacheKey::rehash_policy());
    HashMapCacheKey::NodeAllocator& node_allocator1 = hash_map1.GetNodeAllocator();
    HashMapCacheKey::BucketAllocator& bucket_allocator1 = hash_map1.GetBucketAllocator();

    for (int i = 0; i < 100; ++i)
    {
        hash_map[i] = i;
        hash_map1[i] = i;
    }

    ASSERT_EQ(100, hash_map1.size());
    ASSERT_EQ(hash_map1.size(), hash_map.size());
    ASSERT_EQ(hash_map1.GetBucketCount(), hash_map.GetBucketCount());

    ASSERT_GT(node_allocator1.allocated_bytes, node_allocator.allocated_bytes);
    ASSERT_EQ(bucket_allocator.allocated_bytes, bucket_allocator1.allocated_bytes);

    ASSERT_EQ(100 * sizeof(HashMapNotCacheKey::Node), node_allocator.allocated_bytes);
    ASSERT_EQ(100 * sizeof(HashMapCacheKey::Node), node_allocator1.allocated_bytes);

    ASSERT_EQ((hash_map.GetBucketCount() + 1)* sizeof(HashMapNotCacheKey::Node*),
              bucket_allocator.allocated_bytes);
    ASSERT_EQ((hash_map1.GetBucketCount() + 1) * sizeof(HashMapCacheKey::Node*),
              bucket_allocator1.allocated_bytes);

    hash_map.Clear();
    hash_map1.Clear();
    ASSERT_TRUE(hash_map.empty());

    ASSERT_EQ(0, node_allocator.allocated_bytes);
    ASSERT_EQ(0, node_allocator1.allocated_bytes);

    ASSERT_EQ((hash_map.GetBucketCount() + 1)* sizeof(HashMapNotCacheKey::Node*),
              bucket_allocator.allocated_bytes);
    ASSERT_EQ((hash_map1.GetBucketCount() + 1) * sizeof(HashMapCacheKey::Node*),
              bucket_allocator1.allocated_bytes);
}



