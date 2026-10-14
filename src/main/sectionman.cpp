#include "main/sectionman.h"

// Not yet decompiled: its switch over courses needs a jump table in .rodata,
// which is still one unsplit block
INCLUDE_ASM("main/sectionman", cSectionMan_setSky);

// Puts every node on the free list and empties the active list
void cSectionMan::func_0022E228()
{
    int i;

    m_free = m_nodes;
    for (i = 0; i < 50; i++) {
        m_nodes[i].value = 49;
        m_nodes[i].next = &m_nodes[i + 1];
    }
    m_unk190 = 49;
    m_unk194 = 0;
    m_active = 0;
    m_activeCount = 0;
}

// Takes a node off the free list
tSectionNode* cSectionMan::func_0022E278()
{
    tSectionNode* node = m_free;
    m_free = node->next;
    return node;
}

// Returns a node to the free list
void cSectionMan::func_0022E288(tSectionNode* node)
{
    node->next = m_free;
    m_free = node;
}

// Appends value to the active list, removing any entries already holding it
void cSectionMan::func_0022E298(int value)
{
    tSectionNode* prev = 0;
    tSectionNode* node = m_active;

    while (node) {
        if (node->value == value) {
            tSectionNode* next = node->next;
            func_0022E360(prev, node);
            node = next;
        } else {
            prev = node;
            node = node->next;
        }
    }

    node = func_0022E278();
    node->value = value;
    node->next = 0;
    if (prev)
        prev->next = node;
    else
        m_active = node;
    m_activeCount++;
}

// Next active node after prev, or the first one when prev is null
tSectionNode* cSectionMan::func_0022E348(tSectionNode* prev)
{
    if (prev)
        return prev->next;
    return m_active;
}

// Unlinks node (which follows prev) from the active list and frees it
void cSectionMan::func_0022E360(tSectionNode* prev, tSectionNode* node)
{
    if (prev == 0)
        m_active = node->next;
    else
        prev->next = node->next;
    func_0022E288(node);
    m_activeCount--;
}
