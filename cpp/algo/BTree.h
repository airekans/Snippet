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

template<unsigned MAX_KEY_NUM>
class BTreeNode
{
public:


private:
    typedef std::pair<int, std::string> Elem;

    Elem m_elements[1024];
    BTreeNode* m_children[1025];
    unsigned m_key_num;
    bool m_is_leave;
};

class BTree
{
public:
    BTree();
    ~BTree();

private:
    typedef std::pair<int, std::string> Elem;

    BTreeNode* m_root;
};

} // algo
} // snippet

#endif /* _BTREE_H_ */
