#ifndef SECTIONMAN_H
#define SECTIONMAN_H

#include "common.h"

struct tSectionNode {
    int value;
    tSectionNode* next;
};

class cSectionMan {
public:
    void setSky(int course);
    void func_0022E228();
    tSectionNode* func_0022E278();
    void func_0022E288(tSectionNode* node);
    void func_0022E298(int value);
    tSectionNode* func_0022E348(tSectionNode* prev);
    void func_0022E360(tSectionNode* prev, tSectionNode* node);

    tSectionNode m_nodes[50];
    int m_unk190;
    int m_unk194;
    tSectionNode* m_free;
    tSectionNode* m_active;
    int m_activeCount;
};

#endif
