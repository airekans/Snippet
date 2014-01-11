#ifndef _BTREE_H_
#define _BTREE_H_

#include <string>
#include <utility>
#include <cstring>
#include <iostream>

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


    /// max_key_num should be an odd number
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

    unsigned GetSize() const
    {
        unsigned total_key_num = GetKeyNum();
        if (total_key_num > 0 && !IsLeave())
        {
            for (unsigned i = 0; i <= GetKeyNum(); ++i)
            {
                const BTreeNode* child = GetChild(i);
                if (child)
                {
                    total_key_num += child->GetSize();
                }
            }
        }

        return total_key_num;
    }

    void Dump(const int level) const
    {
        std::cout << '[' << level << ']';
        std::cout << "key_num:" << m_key_num << " elements: ";
        const unsigned key_num = GetKeyNum();
        for (unsigned i = 0; i < key_num; ++i)
        {
            std::cout << GetKey(i) << ' ';
        }
        std::cout << std::endl;

        if (key_num > 0 && !IsLeave())
        {
            for (unsigned i = 0; i <= key_num; ++i)
            {
                const BTreeNode* child = GetChild(i);
                if (child)
                {
                    child->Dump(level + 1);
                }
            }
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

        for (int j = static_cast<int>(GetKeyNum()) - 1;
             j >= static_cast<int>(i); --j)
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
            m_children[i]->InsertNonfull(key, value);
        }
    }

    bool Find(const KeyType key, unsigned* idx) const
    {
        KeyType cur_key;
        for (unsigned i = 0; i < m_key_num; ++i)
        {
            cur_key = GetKey(i);
            if (key <= cur_key)
            {
                *idx = i;
                return key == cur_key;
            }
        }
        *idx = m_key_num;
        return false;
    }

    void GetMaxKeyItem(KeyType* key, ValueType* value) const
    {
        if (IsLeave())
        {
            *key = m_elements[m_key_num - 1].first;
            *value = m_elements[m_key_num - 1].second;
        }
        else
        {
            m_children[m_key_num]->GetMaxKeyItem(key, value);
        }
    }

    void GetMinKeyItem(KeyType* key, ValueType* value) const
    {
        if (IsLeave())
        {
            *key = m_elements[0].first;
            *value = m_elements[0].second;
        }
        else
        {
            m_children[0]->GetMinKeyItem(key, value);
        }
    }

    void Delete(const unsigned idx)
    {
        for (unsigned i = idx + 1; i < m_key_num; ++i)
        {
            m_elements[i - 1] = m_elements[i];
        }
        --m_key_num;
        BTreeNode::DiskWrite(*this);
    }

    BTreeNode* MergeChildren(const unsigned idx)
    {
        BTreeNode* first_child = m_children[idx];
        BTreeNode* second_child = m_children[idx + 1];
        BTreeNode::DiskRead(first_child);
        BTreeNode::DiskRead(second_child);
        const unsigned first_old_key_num = first_child->m_key_num;
        const unsigned second_old_key_num = second_child->m_key_num;

        // merge keys and values
        first_child->m_elements[first_old_key_num] = m_elements[idx];
        for (unsigned i = 0; i < second_old_key_num; ++i)
        {
            first_child->m_elements[first_old_key_num + 1 + i] =
                    second_child->m_elements[i];
        }

        // merge the children
        for (unsigned i = 0; i <= second_old_key_num; ++i)
        {
            first_child->SetChild(first_old_key_num + 1 + i,
                                  second_child->m_children[i]);
        }
        first_child->m_key_num += second_old_key_num + 1;
        BTreeNode::DiskWrite(*first_child);

        // move the key backward in the parent.
        for (unsigned i = idx; i < m_key_num - 1; ++i)
        {
            m_elements[i] = m_elements[i + 1];
        }

        // move the children backward in the parent
        for (unsigned i = idx + 1; i < m_key_num; ++i)
        {
            m_children[i] = m_children[i + 1];
        }
        --m_key_num;
        BTreeNode::DiskWrite(*this);

        second_child->SetKeyNum(0); // we have to reset before deleting
        delete second_child;

        return first_child;
    }

    void LeftShiftKey(const unsigned idx)
    {
        BTreeNode* left_child = m_children[idx];
        BTreeNode* right_child = m_children[idx + 1];
        BTreeNode::DiskRead(left_child);
        BTreeNode::DiskRead(right_child);
        const unsigned left_key_num = left_child->GetKeyNum();
        const unsigned right_key_num = right_child->GetKeyNum();

        left_child->m_elements[left_key_num] = m_elements[idx];
        left_child->m_children[left_key_num + 1] = right_child->m_children[0];
        ++(left_child->m_key_num);
        BTreeNode::DiskWrite(*left_child);

        m_elements[idx] = right_child->m_elements[0];
        BTreeNode::DiskWrite(*this);

        for (unsigned i = 0; i < right_key_num - 1; ++i)
        {
            right_child->m_elements[i] = right_child->m_elements[i + 1];
            right_child->m_children[i] = right_child->m_children[i + 1];
        }
        right_child->m_children[right_key_num - 1] =
                right_child->m_children[right_key_num];
        --(right_child->m_key_num);
        BTreeNode::DiskWrite(*right_child);
    }

    void RightShiftKey(const unsigned idx)
    {
        BTreeNode* left_child = m_children[idx];
        BTreeNode* right_child = m_children[idx + 1];
        BTreeNode::DiskRead(left_child);
        BTreeNode::DiskRead(right_child);
        const unsigned left_key_num = left_child->GetKeyNum();
        const unsigned right_key_num = right_child->GetKeyNum();

        for (unsigned i = right_key_num; i > 0; --i)
        {
            right_child->m_elements[i] = right_child->m_elements[i - 1];
            right_child->m_children[i + 1] = right_child->m_children[i];
        }
        right_child->m_children[1] = right_child->m_children[0];
        right_child->m_elements[0] = m_elements[idx];
        right_child->m_children[0] = left_child->m_children[left_key_num];
        ++(right_child->m_key_num);
        BTreeNode::DiskWrite(*right_child);

        m_elements[idx] = left_child->m_elements[left_key_num - 1];
        BTreeNode::DiskWrite(*this);

        --(left_child->m_key_num);
        BTreeNode::DiskWrite(*left_child);
    }

    void SetKeyNum(const unsigned key_num)
    {
        m_key_num = key_num <= 1024 ? key_num : 1024;
    }

    void SetChild(const unsigned i, BTreeNode* child)
    {
        m_children[i] = child;
    }

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

    unsigned GetSize() const
    {
        return m_root->GetSize();
    }

    void Dump() const
    {
        std::cout << "max_key_num:" << m_root->GetMaxKeyNum() << std::endl;
        m_root->Dump(0);
    }

    bool Find(const int key, BTreeNode** out_node, int* out_index) const
    {
        return Find(*m_root, key, out_node, out_index);
    }

    void Insert(const int key, const std::string& value)
    {
        const unsigned max_key_num = m_root->GetMaxKeyNum();
        if (m_root->GetKeyNum() == max_key_num)
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

    void Delete(const int key)
    {
        if (m_root->GetKeyNum() == 0)
        {
            return;
        }
        else if (m_root->GetKeyNum() == 1 && !m_root->IsLeave())
        {
            BTreeNode::DiskRead(m_root->GetChild(0));
            BTreeNode::DiskRead(m_root->GetChild(1));

            BTreeNode* first_child = m_root->GetChild(0);
            BTreeNode* second_child = m_root->GetChild(1);
            const unsigned min_key_num = m_root->GetMaxKeyNum() / 2;

            if (first_child->GetKeyNum() == min_key_num &&
                second_child->GetKeyNum() == min_key_num)
            {
                first_child = m_root->MergeChildren(0);
                m_root->SetKeyNum(0);
                delete m_root;
                m_root = first_child;
                Delete(*m_root, key);
            }
            else
            {
                Delete(*m_root, key);
            }
        }
        else
        {
            Delete(*m_root, key);
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
            if (out_node)
            {
                *out_node = &node;
                *out_index = i;
            }
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

    void Delete(BTreeNode& node, const int key)
    {
        unsigned key_idx;
        bool is_key_found = node.Find(key, &key_idx);
        if (node.IsLeave()) // case 1
        {
            if (is_key_found)
            {
                node.Delete(key_idx);
            }
            return;
        }
        else if (is_key_found && !node.IsLeave()) // case 2
        {
            const unsigned min_key_num = node.GetMaxKeyNum() / 2;
            BTreeNode* cur_child = node.GetChild(key_idx);
            BTreeNode::DiskRead(cur_child);
            if (cur_child->GetKeyNum() > min_key_num) // case 2a
            {
                int prev_key = 0;
                std::string prev_value;
                cur_child->GetMaxKeyItem(&prev_key, &prev_value);
                Delete(*cur_child, prev_key);
                node.SetItem(key_idx, prev_key, prev_value);
                return;
            }

            BTreeNode* next_child = node.GetChild(key_idx + 1);
            BTreeNode::DiskRead(next_child);
            if (next_child->GetKeyNum() > min_key_num) // case 2b
            {
                int next_key = 0;
                std::string next_value;
                next_child->GetMinKeyItem(&next_key, &next_value);
                Delete(*next_child, next_key);
                node.SetItem(key_idx, next_key, next_value);
                return;
            }

            // case 2c
            (void) node.MergeChildren(key_idx);
            Delete(*cur_child, key);
        }
        else // case 3
        {
            BTreeNode* child = node.GetChild(key_idx);
            BTreeNode::DiskRead(child);
            const unsigned min_key_num = node.GetMaxKeyNum() / 2;
            if (child->GetKeyNum() <= min_key_num)
            {
                BTreeNode* prev_child = NULL;
                BTreeNode* next_child = NULL;
                if (key_idx > 0)
                {
                    prev_child = node.GetChild(key_idx - 1);
                    BTreeNode::DiskRead(prev_child);
                }
                if (key_idx < node.GetKeyNum())
                {
                    next_child = node.GetChild(key_idx + 1);
                    BTreeNode::DiskRead(next_child);
                }

                // case 3a
                if (prev_child != NULL && prev_child->GetKeyNum() > min_key_num)
                {
                    node.RightShiftKey(key_idx - 1);
                }
                // case 3a
                else if (next_child != NULL && next_child->GetKeyNum() > min_key_num)
                {
                    node.LeftShiftKey(key_idx);
                }
                else // case 3b
                {
                    child = node.MergeChildren(prev_child != NULL ?
                                                 key_idx - 1: key_idx);
                }
            }
            Delete(*child, key);
        }
    }

private:
    BTreeNode* m_root;
};

} // algo
} // snippet

#endif /* _BTREE_H_ */
