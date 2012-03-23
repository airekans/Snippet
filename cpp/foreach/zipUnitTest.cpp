#include "zip.h"
#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <string>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/list.hpp>

using namespace std;
using namespace boost::assign;
using namespace zip_foreach;
using namespace zip_foreach::__impl;

class ZipTestSuite : public ::testing::Test
{
public:
  typedef vector<string> TestVector;
  typedef list<double> TestList;
  typedef ZipContainer<TestVector, TestList> ZC;

protected:
  TestVector v;
  TestList l;

  
  virtual void SetUp()
  {
    v += "abc", "123", "bbb";
    l += 1.2, 2.3, 3.4;
  }
};


TEST_F(ZipTestSuite, test_syntax)
{
  ZC z(v, l);
  ZC::iterator i = z.begin();
  string vBegin = i->first();
  double lBegin = i->second();

  EXPECT_EQ("abc", vBegin);
  EXPECT_EQ(1.2, lBegin);
  
  string vBegin2 = (*i).first();
  double lBegin2 = (*i).second();

  EXPECT_EQ("abc", vBegin);
  EXPECT_EQ(1.2, lBegin);
}

TEST_F(ZipTestSuite, test_zip_function)
{
  BOOST_AUTO(zc, zip(v, l));
  BOOST_AUTO(i, zc.begin());

  string vBegin = i->first();
  double lBegin = i->second();

  EXPECT_EQ("abc", vBegin);
  EXPECT_EQ(1.2, lBegin);
}

TEST_F(ZipTestSuite, test_simple_for)
{
  BOOST_AUTO(zc, zip(v, l));
  
  TestVector v2;
  TestList l2;
  for (BOOST_AUTO(it, zc.begin()); it != zc.end(); ++it)
    {
      v2 += it->first();
      l2 += it->second();
    }

  EXPECT_EQ(v.size(), v2.size());
  EXPECT_EQ(l.size(), l2.size());

  for (int i = 0; i < v.size(); ++i)
    {
      EXPECT_EQ(v[i], v2[i]);
    }

  BOOST_AUTO(it1, l.begin());
  BOOST_AUTO(it2, l2.begin());
  for ( ; it1 != l.end() && it2 != l2.end(); ++it1, ++it2)
    {
      EXPECT_EQ(*it1, *it2);
    }
}

TEST_F(ZipTestSuite, test_not_same_length_container)
{
  v += "add";

  EXPECT_GT(v.size(), l.size());

  BOOST_AUTO(zc, zip(v, l));
  
  TestVector v2;
  TestList l2;
  for (BOOST_AUTO(it, zc.begin()); it != zc.end(); ++it)
    {
      v2 += it->first();
      l2 += it->second();
    }

  EXPECT_NE(v.size(), v2.size());
  EXPECT_EQ(l.size(), v2.size());
  EXPECT_EQ(l.size(), l2.size());

  for (int i = 0; i < v2.size(); ++i)
    {
      EXPECT_EQ(v[i], v2[i]);
    }

  BOOST_AUTO(it1, l.begin());
  BOOST_AUTO(it2, l2.begin());
  for ( ; it1 != l.end() && it2 != l2.end(); ++it1, ++it2)
    {
      EXPECT_EQ(*it1, *it2);
    }
  
}

TEST_F(ZipTestSuite, test_not_same_length_zip_2_foreach)
{
  v += "add";

  EXPECT_GT(v.size(), l.size());

  TestVector v2;
  TestList l2;
  ZIP_2_FOREACH(i1, i2, zip(v, l))
    {
      v2 += i1;
      l2 += i2;
    }

  EXPECT_NE(v.size(), v2.size());
  EXPECT_EQ(l.size(), v2.size());
  EXPECT_EQ(l.size(), l2.size());

  for (int i = 0; i < v2.size(); ++i)
    {
      EXPECT_EQ(v[i], v2[i]);
    }

  BOOST_AUTO(it1, l.begin());
  BOOST_AUTO(it2, l2.begin());
  for ( ; it1 != l.end() && it2 != l2.end(); ++it1, ++it2)
    {
      EXPECT_EQ(*it1, *it2);
    }
  
}

