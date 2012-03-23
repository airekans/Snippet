#include "foreach.h"
#include <gtest/gtest.h>

#include <map>
#include <vector>
#include <list>
#include <utility>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/list.hpp>

using namespace std;
using namespace boost::assign;


TEST(TestSuite, map_foreach_test_map)
{
  typedef std::map<std::string, int> TestMap;
  TestMap m = map_list_of("abc", 1)("map", 2)("key", 3);

  TestMap m2;
  MAP_FOREACH(key, value, m)
    {
      m2[key] = value;
    }

  EXPECT_EQ(m.size(), m2.size());
  BOOST_FOREACH(TestMap::value_type p, m)
    {
      EXPECT_EQ(p.second, m2[p.first]);
    }
}

TEST(TestSuite, map_foreach_test_empty)
{
  map<int, int> emptyMap;
  EXPECT_EQ(0, emptyMap.size());

  int loopCount = 0;
  MAP_FOREACH(key, value, emptyMap)
    {
      ++loopCount;
    }

  EXPECT_EQ(0, loopCount);
}

TEST(TestSuite, map_foreach_test_vector)
{
  using namespace boost::assign;
  typedef pair<string, int> StrIntPair;
  typedef vector<StrIntPair> Pairs;
  Pairs pairs;
  pairs += StrIntPair("abc", 1), StrIntPair("map", 2),
    StrIntPair("key", 3);

  Pairs pairs2;
  MAP_FOREACH(key, value, pairs)
    pairs2 += StrIntPair(key, value);
	
  EXPECT_EQ(pairs.size(), pairs2.size());
  for (size_t i = 0; i < pairs.size(); ++i)
    {
      EXPECT_EQ(pairs[i].first, pairs2[i].first);
      EXPECT_EQ(pairs[i].second, pairs2[i].second);
    }
}

TEST(TestSuite, map_foreach_test_break)
{
  typedef std::map<std::string, int> TestMap;
  TestMap m = map_list_of("abc", 1)("map", 2)("key", 3);

  int loopCount = 0;
  TestMap m2; 
  MAP_FOREACH(key, value, m)
    {
      if (++loopCount == 2)
	{
	  break;
	}

      m2[key] = value;
    }

  EXPECT_EQ(2, loopCount);
  EXPECT_EQ(1, m2.size());
	
  for (BOOST_AUTO(i, m2.begin()); i != m2.end(); ++i)
    {
      BOOST_AUTO(key, i->first);
      BOOST_AUTO(value, i->second);

      EXPECT_EQ(m[key], value);
    }
}

TEST(TestSuite, map_foreach_test_continue)
{
  typedef std::map<std::string, int> TestMap;
  TestMap m = map_list_of("abc", 1)("map", 2)("key", 3);

  int loopCount = 0;
  std::string skipKey;
  int skipValue = 0;
  TestMap m2; 
  MAP_FOREACH(key, value, m)
    {
      if (++loopCount == 2)
	{
	  skipKey = key;
	  skipValue = value;
	  continue;
	}
		
      m2[key] = value;
    }

  EXPECT_EQ(2, m2.size());
  EXPECT_TRUE(m2.find(skipKey) == m2.end());
  BOOST_FOREACH(TestMap::value_type& i, m2)
    {
      BOOST_AUTO(key, i.first);
      BOOST_AUTO(value, i.second);

      EXPECT_EQ(m[key], value);
    }
}

TEST(TestSuite, map_foreach_test_nested)
{
  typedef std::map<std::string, int> TestMap;
  typedef std::map<std::string, TestMap> NestedTestMap;
  TestMap m1 = map_list_of("abc", 1)("map", 2)("key", 3);
  TestMap m2 = map_list_of("ab", 4)("ma", 5)("ke", 6);
  NestedTestMap nestedMap = map_list_of("nested1", m1)("nested2", m2);

  NestedTestMap nestedMap2;
  MAP_FOREACH(key, value, nestedMap)
    {
      TestMap m;
      MAP_FOREACH(key2, value2, value)
	{
	  m[key2] = value2;
	}

      nestedMap2[key] = m;
    }

  EXPECT_EQ(nestedMap.size(), nestedMap2.size());
  BOOST_FOREACH(NestedTestMap::value_type& i, nestedMap2)
    {
      BOOST_AUTO(key, i.first);
      BOOST_AUTO(value, i.second);

      EXPECT_EQ(nestedMap[key].size(), value.size());
      BOOST_FOREACH(TestMap::value_type& j, value)
	{
	  BOOST_AUTO(key2, j.first);
	  BOOST_AUTO(value2, j.second);

	  EXPECT_EQ(nestedMap[key][key2], value2);
	}
    }
}

TEST(TestSuite, map_foreach_test_change_value)
{
  typedef std::map<int, int> TestMap;
  TestMap m = map_list_of(1, 1)(2, 2)(3, 3);

  TestMap m2;
  MAP_FOREACH(key, value, m)
    {
      m2[key] = value + 1;
      ++value;
    }

  EXPECT_EQ(m.size(), m2.size());
  BOOST_FOREACH(TestMap::value_type& i, m2)
    {
      BOOST_AUTO(key, i.first);
      BOOST_AUTO(value, i.second);

      EXPECT_EQ(m2[key], value);
    }
}

TEST(TestSuite, list_foreach_test_list)
{
  typedef list<int> TestList;
  TestList l;
  l += 3, 2, 1;
 
  TestList l2;
  LIST_FOREACH(i, l)
    {
      l2.push_back(i);
    }

  EXPECT_EQ(l.size(), l2.size());
  BOOST_AUTO(it, l.begin());
  BOOST_AUTO(it2, l2.begin());
  for ( ; it != l.end(); ++it, ++it2)
    {
      EXPECT_EQ(*it, *it2);
    }
}

TEST(TestSuite, list_foreach_test_vector)
{
  typedef vector<int> TestVector;
  TestVector v;
  v += 3, 2, 1;

  TestVector v2;
  LIST_FOREACH(i, v)
    {
      v2 += i;
    }

  EXPECT_EQ(v.size(), v2.size());
  for (int i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v[i], v2[i]);
    }
}

