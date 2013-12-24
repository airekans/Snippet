/*
 * BTree.h
 *
 */

#ifndef _BTREE_H_
#define _BTREE_H_

#include <string>
#include <utility>
#include <cstring>

namespace snippet {
namespace algo {

class BTreeNode
{
public:
    typedef int KeyType;
    typedef std::string ValueType;
    typedef std::pair<KeyType, ValueType> Elem;

    BTreeNode(const unsigned max_key_num)
    : m_max_key_num(max_key_num),
      m_key_num(0)
    {
        memset(m_children, 0, 1025 * sizeof(m_children[0]));
    }

    void SetIsLeave(const bool is_leave)
    {
        m_is_leave = is_leave;
    }

    void AddItem(const KeyType key, const ValueType& value)
    {
        // TODO: add implementation
    }

    void SetItem(const unsigned i, const KeyType key, const ValueType& value)
    {
        if (i < m_key_num)
        {
            m_elements[i].first = key;
            m_elements[i].second = value;
        }
    }

    unsigned GetMaxKeyNum() const
    {
        return m_max_key_num;
    }

    unsigned GetKeyNum() const
    {
        return m_key_num;
    }

    void SetKeyNum(const unsigned key_num)
    {
        m_key_num = key_num <= 1024 ? key_num : 1024;
    }

    /// get the key at index i, if i >= m_key_num,
    /// the result is undefined.
    KeyType GetKey(const unsigned i) const
    {
        return m_elements[i].first;
    }

    bool IsLeave() const
    {
        return m_is_leave;
    }

    /// get the child at index i, if i > m_key_num,
    /// the result is NULL;
    BTreeNode* GetChild(const unsigned i) const
    {
        return i <= m_key_num ? m_children[i] : NULL;
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

    bool Find(const int key, BTreeNode** out_node, int* out_index) const
    {
        // TODO: add implementation
        return Find(*m_root, key, out_node, out_index);
    }

private:
    void DiskRead(const BTreeNode* node) const
    {
        // TODO: read the node from disk
    }

    void DiskWrite(const BTreeNode& node) const
    {
        // TODO: write the node to the disk
    }

    bool Find(const BTreeNode& node, const int key,
              BTreeNode** out_node, int* out_index) const
    {
        const unsigned key_num = node.GetKeyNum();
        unsigned i = 0;
        while (i < key_num && key > node.GetKey(i))
        {
            ++i;
        }
        if (i < key_num && key == node.GetKey(i))
        {
            *out_node = &node;
            *out_index = i;
            return true;
        }

        if (node.IsLeave())
        {
            return false;
        }
        else
        {
            const BTreeNode* child_node = node.GetChild(i);
            DiskRead(child_node);
            return Find(*child_node, key, out_node, out_index);
        }
    }

    void SplitNode(BTreeNode& parent, const unsigned i, BTreeNode& node)
    {
        BTreeNode* new_node = new BTreeNode(parent.GetMaxKeyNum());
        new_node->SetIsLeave(node.IsLeave());

    }

private:
    typedef std::pair<int, std::string> Elem;

    BTreeNode* m_root;
};

} // algo
} // snippet

#endif /* _BTREE_H_ */
