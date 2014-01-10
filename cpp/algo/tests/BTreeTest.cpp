/*
 * BTreeTest.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: airekans
 */

#include "BTree.h"
#include <gtest/gtest.h>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using snippet::algo::BTree;

TEST(BTree, TestCtor)
{
    BTree btree(9);

    ASSERT_EQ(static_cast<unsigned>(0), btree.GetSize());
}

TEST(BTree, TestInsert)
{
    BTree btree(9);

    btree.Insert(1, "a");

    ASSERT_TRUE(btree.Find(1, NULL, NULL));
    ASSERT_EQ(static_cast<unsigned>(1), btree.GetSize());

    btree.Insert(2, "b");

    ASSERT_TRUE(btree.Find(1, NULL, NULL));
    ASSERT_TRUE(btree.Find(2, NULL, NULL));
    ASSERT_EQ(static_cast<unsigned>(2), btree.GetSize());
}

class BTreeValueTest : public ::testing::TestWithParam<int>
{
public:
    static void SetUpTestCase()
    {
        srand(unsigned(time(NULL)));
    }
};

TEST_P(BTreeValueTest, TestInsertWithLeave)
{
    const int times = GetParam();
    BTree btree(5);

    for (int i = 0; i < times; ++i)
    {
        btree.Insert(i, "a");
    }

    ASSERT_EQ(static_cast<unsigned>(times), btree.GetSize());
    for (int i = 0; i < times; ++i)
    {
        EXPECT_TRUE(btree.Find(i, NULL, NULL)) << "i: " << i;
    }
}

TEST_P(BTreeValueTest, TestInsertWithReverseOrder)
{
    const int times = GetParam();
    BTree btree(5);

    for (int i = times; i > 0; --i)
    {
        btree.Insert(i, "a");
    }

    ASSERT_EQ(static_cast<unsigned>(times), btree.GetSize());
    for (int i = times; i > 0; --i)
    {
        EXPECT_TRUE(btree.Find(i, NULL, NULL)) << "i: " << i;
    }
}

namespace {
    template<typename T>
    std::string VectorToString(const std::vector<T>& v)
    {
        if (v.empty())
        {
            return "";
        }

        std::ostringstream oss;
        for (unsigned i = 0; i < v.size() - 1; ++i)
        {
            oss << v[i] << ' ';
        }
        oss << v.back();
        return oss.str();
    }
}

TEST_P(BTreeValueTest, TestInsertWithRandomOrder)
{
    const int times = GetParam();
    BTree btree(5);

    // generate a vector<int> with size times
    std::vector<int> numbers;
    for (int i = 0; i < times; ++i)
    {
        numbers.push_back(i);
    }
    std::random_shuffle(numbers.begin(), numbers.end());

    for (int i = 0; i < times; ++i)
    {
        btree.Insert(numbers[i], "a");
    }

    ASSERT_EQ(static_cast<unsigned>(times), btree.GetSize())
        << VectorToString(numbers);
    for (int i = 0; i < times; ++i)
    {
        EXPECT_TRUE(btree.Find(i, NULL, NULL)) << "i: " << i;
    }
}

TEST(BTree, TestFind)
{
    BTree btree(9);

    btree.Insert(1, "a");
    btree.Insert(2, "b");

    ASSERT_TRUE(btree.Find(1, NULL, NULL));
    ASSERT_TRUE(btree.Find(2, NULL, NULL));
    ASSERT_FALSE(btree.Find(3, NULL, NULL));
}

TEST(BTree, TestFindWithEmptyTree)
{
    BTree btree(7);

    ASSERT_FALSE(btree.Find(1, NULL, NULL));
    ASSERT_FALSE(btree.Find(2, NULL, NULL));
    ASSERT_FALSE(btree.Find(3, NULL, NULL));
}

TEST(BTree, TestDeleteInRoot)
{
    BTree btree(5);

    btree.Insert(1, "a");
    ASSERT_EQ(static_cast<unsigned>(1), btree.GetSize());
    ASSERT_TRUE(btree.Find(1, NULL, NULL));

    btree.Delete(1);
    ASSERT_EQ(static_cast<unsigned>(0), btree.GetSize());
    ASSERT_FALSE(btree.Find(1, NULL, NULL));
}

TEST(BTree, TestDeleteRoot2)
{
    BTree btree(5);

    btree.Insert(1, "a");
    btree.Insert(2, "a");
    ASSERT_EQ(static_cast<unsigned>(2), btree.GetSize());
    ASSERT_TRUE(btree.Find(1, NULL, NULL));
    ASSERT_TRUE(btree.Find(2, NULL, NULL));

    btree.Delete(1);
    ASSERT_EQ(static_cast<unsigned>(1), btree.GetSize());
    ASSERT_FALSE(btree.Find(1, NULL, NULL));
    ASSERT_TRUE(btree.Find(2, NULL, NULL));
}

TEST(BTree, TestDeleteInLeaveSimple)
{
    BTree btree(5);

    for (unsigned i = 0; i < 8; ++i)
    {
        btree.Insert(i, "a");
    }
    ASSERT_EQ(static_cast<unsigned>(8), btree.GetSize());

    btree.Delete(0);
    ASSERT_EQ(static_cast<unsigned>(7), btree.GetSize());
    ASSERT_FALSE(btree.Find(0, NULL, NULL));

    for (unsigned i = 1; i < 8; ++i)
    {
        ASSERT_TRUE(btree.Find(i, NULL, NULL));
    }
}

namespace {
void DoTestDeleteElem(const unsigned times, const unsigned del_elem)
{
    BTree btree(5);

    for (unsigned i = 0; i < times; ++i)
    {
        btree.Insert(i, "a");
    }
    ASSERT_EQ(static_cast<unsigned>(times), btree.GetSize());
    btree.Dump();

    btree.Delete(del_elem);

    std::ostringstream oss;
    oss << "times: " << times << " del_elem: " << del_elem;
    SCOPED_TRACE(oss.str());
    EXPECT_EQ(static_cast<unsigned>(times - 1),
            btree.GetSize());
    for (unsigned i = 0; i < times; ++i)
    {
        if (i == del_elem)
        {
            EXPECT_FALSE(btree.Find(i, NULL, NULL));
        }
        else
        {
            EXPECT_TRUE(btree.Find(i, NULL, NULL));
        }
    }
}
}

TEST_P(BTreeValueTest, TestDeleteFirst)
{
    DoTestDeleteElem(GetParam(), 0);
}

TEST_P(BTreeValueTest, TestDeleteLast)
{
    DoTestDeleteElem(GetParam(), GetParam() - 1);
}

TEST_P(BTreeValueTest, TestDeleteRandom)
{
    DoTestDeleteElem(GetParam(), rand() % GetParam());
}

INSTANTIATE_TEST_CASE_P(TestBTreeInsert,
                        BTreeValueTest,
                        ::testing::Range(3, 31));
