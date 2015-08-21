#ifndef ALGO_INTRUSIVELIST_H_
#define ALGO_INTRUSIVELIST_H_

#include <cstddef>

namespace snippet {
namespace algo {

namespace detail {

template<typename Node, bool STORE_SIZE> struct IntrusiveListStorage;

template<typename Node>
struct IntrusiveListStorage<Node, true>
{
    Node head;
    ::std::size_t size;

    void SetSize(::std::size_t s) { size = s; }
    ::std::size_t GetSize() const { return size; }  // O(1)

    void Add(Node* node, Node* prev, Node* next)
    {
        node->prev = prev;
        node->next = next;
        prev->next = node;
        next->prev = node;
        ++size;
    }

    // used to delete one element
    void Delete(Node* prev, Node* next)
    {
        prev->next = next;
        next->prev = prev;
        --size;
    }

    void DeleteRange(Node* prev, Node* next)  // O(n)
    {
        std::size_t remove_size = 0;
        for (Node* cur = prev->next; cur != next; cur = cur->next, ++remove_size) ;

        prev->next = next;
        next->prev = prev;
        size -= remove_size;
    }

    void Clear()
    {
        head.next = head.prev = &head;
        size = 0;
    }
};

template<typename Node>
struct IntrusiveListStorage<Node, false>
{
    Node head;

    void SetSize(::std::size_t s) {}
    ::std::size_t GetSize() const  // O(n)
    {
        ::std::size_t size = 0;
        for (const Node* cur = head.next, *end = &head;
                cur != end; cur = cur->next) {
            ++size;
        }

        return size;
    }

    static void Add(Node* node, Node* prev, Node* next)
    {
        node->prev = prev;
        node->next = next;
        prev->next = node;
        next->prev = node;
    }

    static void Delete(Node* prev, Node* next)
    {
        prev->next = next;
        next->prev = prev;
    }

    static void DeleteRange(Node* prev, Node* next)  // O(1)
    {
        Delete(prev, next);
    }

    void Clear() { head.next = head.prev = &head; }
};

}  // namespace detail

struct ListNode
{
    ListNode* prev;
    ListNode* next;
};

// T should be a POD type
// And the element added to this list will not be deleted,
// it's user's responsibility of release the node.
template<typename T, typename Node = ListNode,
        Node T::*MemberOffset = &T::list_node, bool STORE_SIZE = true>
class IntrusiveList
{
public:
    inline static T* NodeToElem(Node* node)
    {
        // The following trick only works on POD types
        T* ptr = reinterpret_cast<T*>(0x1000);
        ::std::size_t offset = reinterpret_cast<std::size_t>(&(ptr->*MemberOffset)) - 0x1000;
        return reinterpret_cast<T*>(reinterpret_cast<char*>(node) - offset);
    }

    inline static const T* NodeToElem(const Node* node)
    {
        // The following trick only works on POD types
        T* ptr = reinterpret_cast<T*>(0x1000);
        ::std::size_t offset = reinterpret_cast<std::size_t>(&(ptr->*MemberOffset)) - 0x1000;
        return reinterpret_cast<const T*>(reinterpret_cast<const char*>(node) - offset);
    }

private:
    class IteratorBase
    {
        inline friend bool operator==(const IteratorBase& lhs, const IteratorBase& rhs)
        {
            return lhs.m_node == rhs.m_node;
        }

        inline friend bool operator!=(const IteratorBase& lhs, const IteratorBase& rhs)
        {
            return lhs.m_node != rhs.m_node;
        }

    protected:
        explicit IteratorBase(Node* node) : m_node(node) {}
        IteratorBase(const IteratorBase& it);
        IteratorBase& operator=(const IteratorBase& it);  // NOLINT(whitespace/operators)

    public:
        Node* m_node;
    };

    class Iterator : public IteratorBase
    {
        friend class IntrusiveList;

        explicit Iterator(Node* node) : IteratorBase(node) {}
        IteratorBase::m_node;

    public:
        Iterator(const Iterator& it) : IteratorBase(it.m_node) {}
        Iterator& operator=(const Iterator& it)  // NOLINT(whitespace/operators)
        {
            m_node = it.m_node; return *this;
        }

        T& operator*() const { return *IntrusiveList::NodeToElem(m_node); }
        T* operator->() const { return IntrusiveList::NodeToElem(m_node); }
        Iterator& operator++() { m_node = m_node->next; return *this; }
        Iterator operator++(int) { Iterator it((*this)); m_node = m_node->next; return it; }
        Iterator& operator--() { m_node = m_node->prev; return *this; }
        Iterator operator--(int) { Iterator it((*this)); m_node = m_node->prev; return it; }

        Node* GetNode() const { return m_node; }
    };

    class ConstIterator : public IteratorBase
    {
        friend class IntrusiveList;

        IteratorBase::m_node;

        explicit ConstIterator(const Node* node)
        : IteratorBase(const_cast<Node*>(node))
        {}

    public:
        ConstIterator(const Iterator& it)  // NOLINT(runtime/explicit)
        : IteratorBase(it.GetNode())
        {}

        ConstIterator(const ConstIterator& it) : IteratorBase(it.m_node) {}
        ConstIterator& operator=(const ConstIterator& it)  // NOLINT(whitespace/operators)
        {
            m_node = it.m_node;
            return *this;
        }

        ConstIterator& operator=(const Iterator& it)  // NOLINT(whitespace/operators)
        {
            m_node = it.GetNode();
            return *this;
        }

        const T& operator*() const { return *IntrusiveList::NodeToElem(m_node); }
        const T* operator->() const { return IntrusiveList::NodeToElem(m_node); }
        ConstIterator& operator++() { m_node = m_node->next; return *this; }

        ConstIterator operator++(int)
        {
            ConstIterator it((*this));
            m_node = m_node->next;
            return it;
        }

        ConstIterator& operator--() { m_node = m_node->prev; return *this; }

        ConstIterator operator--(int)
        {
            ConstIterator it((*this));
            m_node = m_node->prev;
            return it;
        }

        const Node* GetNode() const { return m_node; }
    };

public:
    typedef T value_type;
    typedef T* pointer_type;
    typedef T& reference_type;
    typedef Iterator iterator;
    typedef ConstIterator const_iterator;

    IntrusiveList()
    {
        m_impl.head.next = m_impl.head.prev = &m_impl.head;
        m_impl.SetSize(0);
    }

    // STL compatible interfaces
    ::std::size_t size() const { return m_impl.GetSize(); }
    bool empty() const { return m_impl.head.next == &m_impl.head; }  // O(1)
    T& front() { return *NodeToElem(m_impl.head.next); }
    const T& front() const { return *NodeToElem(m_impl.head.next); }
    T& back() { return *NodeToElem(m_impl.head.prev); }
    const T& back() const { return *NodeToElem(m_impl.head.prev); }
    iterator begin() { return iterator(m_impl.head.next); }
    const_iterator begin() const { return const_iterator(m_impl.head.next); }
    iterator end() { return iterator(&m_impl.head); }
    const_iterator end() const { return const_iterator(&m_impl.head); }
    void clear() { m_impl.Clear(); }

    iterator insert(iterator pos, T* elem)
    {
        Node* node = &(elem->*MemberOffset);
        m_impl.Add(node, pos.GetNode()->prev, pos.GetNode());
        return iterator(node);
    }

    iterator erase(iterator pos)
    {
        Node* next = pos.GetNode()->next;
        m_impl.Delete(pos.GetNode()->prev, next);
        return iterator(next);
    }

    // erase range [first, last)
    iterator erase(iterator first, iterator last)
    {
        m_impl.DeleteRange(first.GetNode()->prev, last.GetNode());
        return last;
    }

    void push_front(T* elem)
    {
        m_impl.Add(&(elem->*MemberOffset), &m_impl.head, m_impl.head.next);
    }

    // User should ensure size() > 0
    void pop_front()
    {
        m_impl.Delete(&m_impl.head, m_impl.head.next->next);
    }

    void push_back(T* elem)
    {
        m_impl.Add(&(elem->*MemberOffset), m_impl.head.prev, &m_impl.head);
    }

    // User should ensure size() > 0
    void pop_back()
    {
        m_impl.Delete(m_impl.head.prev->prev, &m_impl.head);
    }

    // STL incompatible interfaces
    T* PopFront()
    {
        Node* node = m_impl.head.next;
        pop_front();
        return NodeToElem(node);
    }

    T* PopBack()
    {
        Node* node = m_impl.head->prev;
        pop_back();
        return NodeToElem(node);
    }

    ::std::size_t GetUsedBytes() const { return sizeof(*this) + sizeof(T) * size(); }

private:
    detail::IntrusiveListStorage<Node, STORE_SIZE> m_impl;
};

}  // namespace algo
}  // namespace snippet

#endif  // ALGO_INTRUSIVELIST_H_
