/*
 * BTree.h
 *
 */

#ifndef _BTREE_H_
#define _BTREE_H_

#include <string>
#include <utility>

namespace snippet {
namespace algo {

class BTreeNode
{
    typedef int KeyType;
    typedef std::string ValueType;
    typedef std::pair<KeyType, ValueType> Elem;

public:
    BTreeNode(const unsigned max_key_num)
    : m_max_key_num(max_key_num),
      m_key_num(0)
    {
        // TODO: memset(m_children, 0)
    }

    void SetIsLeave(const bool is_leave)
    {
        m_is_leave = is_leave;
    }

    void AddItem(const KeyType key, const ValueType& value)
    {
        // TODO: add implementation
    }

private:
    const unsigned m_max_key_num;
    Elem m_elements[1024];
    BTreeNode* m_children[1025];
    unsigned m_key_num;
    bool m_is_leave;
};

class BTree
{
public:
    BTree(const unsigned max_key_num)
    {
        m_root = new BTreeNode(max_key_num);
        m_root->SetIsLeave(true);
        DiskWrite(*m_root);
    }

    ~BTree()
    {
        delete m_root;
    }

    void Find(const int key, BTreeNode** out_node, int* out_index)
    {
        // TODO: add implementation
    }

private:
    void DiskWrite(const BTreeNode& node)
    {
        // TODO: write the node to the disk
    }

private:
    typedef std::pair<int, std::string> Elem;

    BTreeNode* m_root;
};

} // algo
} // snippet

#endif /* _BTREE_H_ */
