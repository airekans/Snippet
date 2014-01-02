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

    static void DiskRead(const BTreeNode* node)
    {
        // TODO: read the node from disk
    }

    static void DiskWrite(const BTreeNode& node)
    {
        // TODO: write the node to the disk
    }


    BTreeNode(const unsigned max_key_num)
    : m_max_key_num(max_key_num),
      m_key_num(0)
    {
        memset(m_children, 0, 1025 * sizeof(m_children[0]));
    }

    ~BTreeNode()
    {
        if (m_key_num > 0)
        {
            for (unsigned i = 0; i <= m_key_num; ++i)
            {
                delete m_children[i];
            }
        }
    }

    void SetIsLeave(const bool is_leave)
    {
        m_is_leave = is_leave;
    }

    void AddItem(const KeyType key, const ValueType& value)
    {
        // TODO: add implementation
    }

    unsigned GetMaxKeyNum() const
    {
        return m_max_key_num;
    }

    unsigned GetKeyNum() const
    {
        return m_key_num;
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

    void SplitChild(const unsigned i, BTreeNode& child)
    {
        const unsigned max_key_num = m_max_key_num;
        const unsigned min_key_num = max_key_num / 2;
        BTreeNode* new_child = new BTreeNode(max_key_num);
        new_child->SetIsLeave(child.IsLeave());

        new_child->SetKeyNum(min_key_num);
        for (unsigned j = 0; j < min_key_num; ++j)
        {
            const Elem& e = child.m_elements[min_key_num + 1 + j];
            new_child->SetItem(j, e.first, e.second);
        }

        if (!child.IsLeave())
        {
            for (unsigned j = 0; j <= min_key_num; ++j)
            {
                new_child->m_children[j] = child.m_children[min_key_num + 1 + j];
            }
        }

        child.SetKeyNum(min_key_num);
        for (unsigned j = GetKeyNum(); j > i; --j)
        {
            m_children[j + 1] = m_children[j];
        }
        m_children[i + 1] = new_child;

        for (unsigned j = GetKeyNum() - 1; j >= i; --j)
        {
            m_elements[j + 1] = m_elements[j];
        }
        m_elements[i] = child.m_elements[min_key_num];
        ++m_key_num;

        DiskWrite(child);
        DiskWrite(*new_child);
        DiskWrite(*this);
    }

    void InsertNonfull(const KeyType key, const ValueType& value)
    {
        int i = static_cast<int>(GetKeyNum()) - 1;
        if (IsLeave())
        {
            while (i >= 0 && key < GetKey(static_cast<unsigned>(i)))
            {
                m_elements[i + 1] = m_elements[i];
                --i;
            }
            SetItem(static_cast<unsigned>(i) + 1, key, value);
            ++m_key_num;
            DiskWrite(*this);
        }
        else
        {
            while (i >= 0 && key < GetKey(static_cast<unsigned>(i)))
            {
                --i;
            }
            ++i;
            BTreeNode* child = m_children[i];
            DiskRead(child);
            if (child->GetKeyNum() == m_max_key_num)
            {
                SplitChild(static_cast<unsigned>(i), *child);
                if (key > GetKey(static_cast<unsigned>(i)))
                {
                    ++i;
                }
            }
            child->InsertNonfull(key, value);
        }
    }

    void SetKeyNum(const unsigned key_num)
    {
        m_key_num = key_num <= 1024 ? key_num : 1024;
    }

    void SetChild(const unsigned i, BTreeNode* child)
    {
        m_children[i] = child;
    }

private:
    void SetItem(const unsigned i, const KeyType key, const ValueType& value)
    {
        m_elements[i].first = key;
        m_elements[i].second = value;
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
        BTreeNode::DiskWrite(*m_root);
    }

    ~BTree()
    {
        delete m_root;
    }

    bool Find(const int key, BTreeNode** out_node, int* out_index) const
    {
        return Find(*m_root, key, out_node, out_index);
    }

    void Insert(const int key, const std::string& value)
    {
        const unsigned max_key_num = m_root->GetMaxKeyNum();
        const unsigned min_key_num = max_key_num / 2;
        if (m_root->GetKeyNum() == min_key_num)
        {
            BTreeNode* old_root = m_root;
            m_root = new BTreeNode(max_key_num);
            m_root->SetIsLeave(false);
            m_root->SetKeyNum(0);
            m_root->SetChild(0, old_root);
            m_root->SplitChild(0, *old_root);
            m_root->InsertNonfull(key, value);
        }
        else
        {
            m_root->InsertNonfull(key, value);
        }
    }

private:
    bool Find(BTreeNode& node, const int key,
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
            BTreeNode* child_node = node.GetChild(i);
            BTreeNode::DiskRead(child_node);
            return Find(*child_node, key, out_node, out_index);
        }
    }

private:
    typedef std::pair<int, std::string> Elem;

    BTreeNode* m_root;
};

} // algo
} // snippet

#endif /* _BTREE_H_ */
