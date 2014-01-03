/*
 * BTreeTest.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: airekans
 */

#include "BTree.h"
#include <gtest/gtest.h>

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
    btree.Dump();

    ASSERT_TRUE(btree.Find(1, NULL, NULL));
    ASSERT_TRUE(btree.Find(2, NULL, NULL));
    ASSERT_EQ(static_cast<unsigned>(2), btree.GetSize());
}

TEST(BTree, TestInsertWithLeave)
{
    BTree btree(5);

    btree.Insert(1, "a");
    for (int i = 0; i < 10; ++i)
    {
        btree.Insert(i, "a");
    }
    btree.Dump();

    ASSERT_EQ(static_cast<unsigned>(10), btree.GetSize());
    for (int i = 0; i < 10; ++i)
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
