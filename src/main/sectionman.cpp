#include "common.h"
#include "main/sectionman.h"

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_setSky);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char cIrradianceDataBase_TheDataBase[];
extern char str_APBR1[]; // "APBR1"
extern char str_BPBR1[]; // "BPBR1"
extern char str_CPBR1[]; // "CPBR1"
extern char str_DPBR1[]; // "DPBR1"
extern char str_EPBR1[]; // "EPBR1"
extern int cIrradianceDataBase_mDefaultId;
extern "C" int cIrradianceDataBase_GetIDByName(void* self, const char* name);

/**
 * Selects the particle-library lighting entry for a location.
 *
 * Maps @p sky to one of the entries "APBR1" to "EPBR1" (A: 0, 5, 8, 14, 17 and any other value; B: 1, 11, 18;
 * C: 2, 9, 12, 19; D: 3, 6, 15, 20; E: 4, 7, 10, 13, 16, 21) and stores its index in cIrradianceDataBase_TheDataBase (0 if absent)
 * in cIrradianceDataBase_mDefaultId, which func_00389C38() applies. The source's note calls it the default rider irradiance bank.
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
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_BPBR1);
        break;
    case 2:
    case 9:
    case 12:
    case 19:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_CPBR1);
        break;
    case 3:
    case 6:
    case 15:
    case 20:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_DPBR1);
        break;
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 21:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_EPBR1);
        break;
    case 0:
    case 5:
    case 8:
    case 14:
    case 17:
    default:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_APBR1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_initEventPool);
#ifdef SKIP_ASM
struct sSectionNode {
    int SectionID;
    sSectionNode* pNext; // 0x4
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
        ((sSectionNode*)&self->m_EventPool[0])[i].SectionID = 49;
        ((sSectionNode*)&self->m_EventPool[0])[i].pNext = &((sSectionNode*)&self->m_EventPool[0])[i + 1];
    }
    self->m_EventPool[50].SectionID = 49;
    self->m_EventPool[50].pNext = 0;
    self->m_pActiveEventList = 0;
    self->m_iNumActiveEvent = 0;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_allocEvent__FPv);
#ifdef SKIP_ASM
struct sSectionLink {
    int pad0;
    sSectionLink* next; // 0x4
};

/**
 * Takes an event from the section manager's free list: pops the head of m_pFreeEventList (+0x198), whose next link is
 * at +0x4.
 *
 * There is no check for an empty list. cSectionMan_addActiveEvent calls it.
 *
 * @return the event
 */
void* cSectionMan_allocEvent(void* vself)
{
    cSectionMan* self = (cSectionMan*)vself;
    sSectionLink* node = (sSectionLink*)self->m_pFreeEventList;
    self->m_pFreeEventList = (tSectionManEvent*)node->next;
    return node;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", cSectionMan_freeEvent__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns an event to the section manager's free list: links @p node in front of m_pFreeEventList (+0x198).
 *
 * cSectionMan_removeActiveEvent calls it.
 *
 * @param node  the event to free
 * @return the previous head of the free list
 */
int cSectionMan_freeEvent(void* vself, void* node)
{
    cSectionMan* self = (cSectionMan*)vself;
    int prevHead = (int)self->m_pFreeEventList;
    *(int*)((char*)node + 0x4) = prevHead;
    self->m_pFreeEventList = (tSectionManEvent*)((int)node);
    return prevHead;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/sectionman", cSectionMan_addActiveEvent);
#ifdef SKIP_ASM

void* cSectionMan_allocEvent(void* self);
extern "C" void cSectionMan_removeActiveEvent(cSectionMan* self, tSectionManEvent* prev, tSectionManEvent* node);

/**
 * Makes a section event active, moving it to the end of the active list if it was already there.
 *
 * Removes any active entries with the same ID, takes a node from the free list and appends it.
 *
 * @param id  section ID to activate (On Tour eBESectionID)
 */
extern "C" void cSectionMan_addActiveEvent(cSectionMan* self, int id)
{
    tSectionManEvent* n = self->m_pActiveEventList;
    tSectionManEvent* prev = 0;
    while (n != 0) {
        if (n->SectionID == id) {
            tSectionManEvent* cur = n;
            n = n->pNext;
            cSectionMan_removeActiveEvent(self, (tSectionManEvent*)prev, (tSectionManEvent*)cur);
        } else {
            prev = n;
            n = n->pNext;
        }
    }
    n = (tSectionManEvent*)cSectionMan_allocEvent(self);
    n->SectionID = id;
    n->pNext = 0;
    if (prev != 0) {
        prev->pNext = n;
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
extern "C" int cSectionMan_getNextActiveEvent(cSectionMan* self, tSectionManEvent* pPrevEvent)
{
    if (((void*)pPrevEvent) != 0) {
        return (int)pPrevEvent->pNext;
    }
    return (int)self->m_pActiveEventList;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/sectionman", cSectionMan_removeActiveEvent);
#ifdef SKIP_ASM
// PORT: the unit defines cSectionMan_freeEvent as returning int; this caller treats it as void.
void cSectionMan_freeEvent_v(void* self, void* node) __asm__("cSectionMan_freeEvent__FPvT0");

/**
 * Unlinks an event from the active list and returns its node to the free list.
 *
 * @param prev  the event before @p node in the active list, or 0 when @p node is the head
 * @param node  the event to remove
 */
extern "C" void cSectionMan_removeActiveEvent(cSectionMan* self, tSectionManEvent* prev, tSectionManEvent* node)
{
    if (((void*)prev) == 0) {
        self->m_pActiveEventList = (tSectionManEvent*)((int)node->pNext);
    } else {
        prev->pNext = (tSectionManEvent*)((int)node->pNext);
    }
    cSectionMan_freeEvent_v(self, node);
    self->m_iNumActiveEvent = self->m_iNumActiveEvent - 1;
}
#endif

