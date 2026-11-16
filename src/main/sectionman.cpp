#include "common.h"
#include "main/sectionman.h"

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_setSky);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_005047F8[];
extern char str_APBR1[]; // "APBR1"
extern char str_BPBR1[]; // "BPBR1"
extern char str_CPBR1[]; // "CPBR1"
extern char str_DPBR1[]; // "DPBR1"
extern char str_EPBR1[]; // "EPBR1"
extern int D_004A43C4;
extern "C" int func_0038AC50(void* self, const char* name);

/**
 * Selects the particle-library lighting entry for a location.
 *
 * Maps @p sky to one of the entries "APBR1" to "EPBR1" (A: 0, 5, 8, 14, 17 and any other value; B: 1, 11, 18;
 * C: 2, 9, 12, 19; D: 3, 6, 15, 20; E: 4, 7, 10, 13, 16, 21) and stores its index in D_005047F8 (0 if absent)
 * in D_004A43C4, which func_00389C38() applies. The source's note calls it the default rider irradiance bank.
 *
 * @param sky  BE location ID (callers pass the section manager's or the new race's location)
 */
// Picks the default rider irradiance bank for the given course.
extern "C" void cSectionMan_setSky(cSectionMan* self, int sky)
{
    switch (sky) {
    case 1:
    case 11:
    case 18:
        D_004A43C4 = func_0038AC50(D_005047F8, str_BPBR1);
        break;
    case 2:
    case 9:
    case 12:
    case 19:
        D_004A43C4 = func_0038AC50(D_005047F8, str_CPBR1);
        break;
    case 3:
    case 6:
    case 15:
    case 20:
        D_004A43C4 = func_0038AC50(D_005047F8, str_DPBR1);
        break;
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 21:
        D_004A43C4 = func_0038AC50(D_005047F8, str_EPBR1);
        break;
    case 0:
    case 5:
    case 8:
    case 14:
    case 17:
    default:
        D_004A43C4 = func_0038AC50(D_005047F8, str_APBR1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_initEventPool);
#ifdef SKIP_ASM
struct sSectionNode {
    int field_0x0;
    sSectionNode* next; // 0x4
};

struct sSectionMan {
    sSectionNode nodes[50]; // 0x0
    int field_0x190;
    int field_0x194;
    sSectionNode* freeList; // 0x198
    int field_0x19c;
    int field_0x1a0;
};

/**
 * Resets the section manager's event pool: links all entries into the free list and empties the active list.
 *
 * Every pooled entry gets section ID 49 (probably "none"); the last of the 51 entries ends the free list.
 */
extern "C" void cSectionMan_initEventPool(cSectionMan* self)
{
    int i;
    self->m_pFreeEventList = (tSectionManEvent*)((sSectionNode*)&self->m_EventPool[0]);
    for (i = 0; i < 50; i++) {
        ((sSectionNode*)&self->m_EventPool[0])[i].field_0x0 = 49;
        ((sSectionNode*)&self->m_EventPool[0])[i].next = &((sSectionNode*)&self->m_EventPool[0])[i + 1];
    }
    self->m_EventPool[50].SectionID = 49;
    self->m_EventPool[50].pNext = 0;
    self->m_pActiveEventList = 0;
    self->m_iNumActiveEvent = 0;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", func_0022E278__FPv);
#ifdef SKIP_ASM
struct sSectionLink {
    int pad0;
    sSectionLink* next; // 0x4
};

void* func_0022E278(void* self)
{
    sSectionLink* node = *(sSectionLink**)((char*)self + 0x198);
    *(sSectionLink**)((char*)self + 0x198) = node->next;
    return node;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", func_0022E288__FPvT0);
#ifdef SKIP_ASM
int func_0022E288(void* self, void* node)
{
    int prevHead = *(int*)((char*)self + 0x198);
    *(int*)((char*)node + 0x4) = prevHead;
    *(int*)((char*)self + 0x198) = (int)node;
    return prevHead;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/sectionman", cSectionMan_addActiveEvent);
#ifdef SKIP_ASM
struct sSectionNode_E298 {
    int id;
    sSectionNode_E298* next; // 0x4
};

void* func_0022E278(void* self);
extern "C" void cSectionMan_removeActiveEvent(cSectionMan* self, void* prev, void* node);

/**
 * Makes a section event active, moving it to the end of the active list if it was already there.
 *
 * Removes any active entries with the same ID, takes a node from the free list and appends it.
 *
 * @param id  section ID to activate (On Tour eBESectionID)
 */
extern "C" void cSectionMan_addActiveEvent(cSectionMan* self, int id)
{
    sSectionNode_E298* n = (sSectionNode_E298*)self->m_pActiveEventList;
    sSectionNode_E298* prev = 0;
    while (n != 0) {
        if (n->id == id) {
            sSectionNode_E298* cur = n;
            n = n->next;
            cSectionMan_removeActiveEvent(self, prev, cur);
        } else {
            prev = n;
            n = n->next;
        }
    }
    n = (sSectionNode_E298*)func_0022E278(self);
    n->id = id;
    n->next = 0;
    if (prev != 0) {
        prev->next = n;
    } else {
        self->m_pActiveEventList = (tSectionManEvent*)n;
    }
    self->m_iNumActiveEvent += 1;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_getNextActiveEvent);
#ifdef SKIP_ASM
/**
 * Steps through the active section events.
 *
 * @param pPrevEvent  the previous event, or 0 to start at the head of the list
 * @return the event after @p pPrevEvent, the first event when it is 0, or 0 at the end of the list
 */
extern "C" int cSectionMan_getNextActiveEvent(cSectionMan* self, void* pPrevEvent)
{
    if (pPrevEvent != 0) {
        return *(int*)((char*)pPrevEvent + 0x4);
    }
    return (int)self->m_pActiveEventList;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/sectionman", cSectionMan_removeActiveEvent);
#ifdef SKIP_ASM
// PORT: the unit defines func_0022E288 as returning int; this caller treats it as void.
void func_0022E288_v(void* self, void* node) __asm__("func_0022E288__FPvT0");

/**
 * Unlinks an event from the active list and returns its node to the free list.
 *
 * @param prev  the event before @p node in the active list, or 0 when @p node is the head
 * @param node  the event to remove
 */
extern "C" void cSectionMan_removeActiveEvent(cSectionMan* self, void* prev, void* node)
{
    if (prev == 0) {
        self->m_pActiveEventList = (tSectionManEvent*)(*(int*)((char*)node + 0x4));
    } else {
        *(int*)((char*)prev + 0x4) = *(int*)((char*)node + 0x4);
    }
    func_0022E288_v(self, node);
    self->m_iNumActiveEvent = self->m_iNumActiveEvent - 1;
}
#endif

