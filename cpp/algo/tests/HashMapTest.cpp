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
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    ASSERT_TRUE(hash_map.Delete(1));
    ASSERT_EQ(std_map.size() - 1, hash_map.size());
    int value = 0;
    ASSERT_FALSE(hash_map.Find(1, value));

    ASSERT_FALSE(hash_map.Delete(1));

    ASSERT_TRUE(hash_map.Delete(2));
    ASSERT_TRUE(hash_map.Delete(3));
    ASSERT_EQ(0, hash_map.size());

    ASSERT_FALSE(hash_map.Delete(2));
    ASSERT_EQ(0, hash_map.size());
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

namespace {

static unsigned int BM_SEQ_INSERT_TEST_TIMES = 100000;

}

TEST(HashMap, BenchmarkHashMapSeqInsert)
{
    HashMap<unsigned int, unsigned int> hash_map;
    for (unsigned int i = 0; i < BM_SEQ_INSERT_TEST_TIMES; ++i)
    {
        hash_map.Insert(i, i);
    }
}

TEST(HashMap, BenchmarkStdMapSeqInsert)
{
    map<unsigned int, unsigned int> std_map;
    for (unsigned int i = 0; i < BM_SEQ_INSERT_TEST_TIMES; ++i)
    {
        std_map.insert(make_pair(i, i));
    }
}



