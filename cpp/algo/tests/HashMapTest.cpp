#include "HashMap.h"
#include <gtest/gtest.h>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>

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

TEST(HashMap, TestDelete)
{
    map<int, int> std_map;
    std_map[1] = 2;
    std_map[2] = 3;
    std_map[3] = 4;
    HashMap<int, int> hash_map(std_map);
    ASSERT_EQ(std_map.size(), hash_map.size());

    hash_map.Delete(1);
    ASSERT_EQ(std_map.size() - 1, hash_map.size());
    int value = 0;
    ASSERT_FALSE(hash_map.Find(1, value));
}


