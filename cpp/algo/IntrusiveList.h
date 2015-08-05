struct ListNode
{
    ListNode* prev;
    ListNode* next;
};

template<typename T, typename ListNode, ListNode T::*MemberOffset = &T::list_node>
class IntrusiveList
{
public:
    IntrusiveList()
    {
        m_head.next = m_head.prev = &m_head;
    }

    void push_front(T* elem)
    {
        add(&(elem->*MemberOffset), &m_head, m_head.next);
    }

    void push_back(T* elem)
    {
        add(&(elem->*MemberOffset), m_head.prev, &m_head);
    }

private:
    void add(ListNode* node, ListNode* prev, ListNode* next)
    {
        node->prev = prev;
        node->next = next;
        prev->next = node;
        next->prev = node;
    }

    ListNode m_head;
};
