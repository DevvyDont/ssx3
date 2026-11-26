#include "common.h"
#include "main/sectionman.h"
#include "be/beintnewrace.h"

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
 * Maps @p sky to one of the entries "APBR1" to "EPBR1" by the location's peak, the first letter of its kLOCATION_ name
 * (A: kLOCATION_ARA1, ASS1, ABA1, ABC1, HUB_A and any other value; B: BRA2, BHP1, HUB_B; C: CRA3, CBA2, CHP2, HUB_C; D:
 * DRA4, DSS2, DBC2, HUB_D; E: ERA5, ESS3, EBA3, EHP3, EBC3, HUB_E) and stores its index in
 * cIrradianceDataBase_TheDataBase (0 if absent) in cIrradianceDataBase_mDefaultId, which func_00389C38() applies. The
 * source's note calls it the default rider irradiance bank.
 *
 * @param sky  BE location ID (callers pass the section manager's or the new race's location)
 */
// Picks the default rider irradiance bank for the given course.
void cSectionMan::setSky(int sky)
{
    switch (sky) {
    case kLOCATION_BRA2:
    case kLOCATION_BHP1:
    case kLOCATION_HUB_B:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_BPBR1);
        break;
    case kLOCATION_CRA3:
    case kLOCATION_CBA2:
    case kLOCATION_CHP2:
    case kLOCATION_HUB_C:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_CPBR1);
        break;
    case kLOCATION_DRA4:
    case kLOCATION_DSS2:
    case kLOCATION_DBC2:
    case kLOCATION_HUB_D:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_DPBR1);
        break;
    case kLOCATION_ERA5:
    case kLOCATION_ESS3:
    case kLOCATION_EBA3:
    case kLOCATION_EHP3:
    case kLOCATION_EBC3:
    case kLOCATION_HUB_E:
        cIrradianceDataBase_mDefaultId = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, str_EPBR1);
        break;
    case kLOCATION_ARA1:
    case kLOCATION_ASS1:
    case kLOCATION_ABA1:
    case kLOCATION_ABC1:
    case kLOCATION_HUB_A:
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
void cSectionMan::initEventPool()
{
    int i;
    m_pFreeEventList = (tSectionManEvent*)((sSectionNode*)&m_EventPool[0]);
    for (i = 0; i < 50; i++) {
        ((sSectionNode*)&m_EventPool[0])[i].SectionID = 49;
        ((sSectionNode*)&m_EventPool[0])[i].pNext = &((sSectionNode*)&m_EventPool[0])[i + 1];
    }
    m_EventPool[50].SectionID = 49;
    m_EventPool[50].pNext = 0;
    m_pActiveEventList = 0;
    m_iNumActiveEvent = 0;
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
void* cSectionMan::allocEvent()
{
    sSectionLink* node = (sSectionLink*)m_pFreeEventList;
    m_pFreeEventList = (tSectionManEvent*)node->next;
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
int cSectionMan::freeEvent(void* node)
{
    int prevHead = (int)m_pFreeEventList;
    *(int*)((char*)node + 0x4) = prevHead;
    m_pFreeEventList = (tSectionManEvent*)((int)node);
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
void cSectionMan::addActiveEvent(int id)
{
    tSectionManEvent* n = m_pActiveEventList;
    tSectionManEvent* prev = 0;
    while (n != 0) {
        if (n->SectionID == id) {
            tSectionManEvent* cur = n;
            n = n->pNext;
            removeActiveEvent((tSectionManEvent*)prev, (tSectionManEvent*)cur);
        } else {
            prev = n;
            n = n->pNext;
        }
    }
    n = (tSectionManEvent*)allocEvent();
    n->SectionID = id;
    n->pNext = 0;
    if (prev != 0) {
        prev->pNext = n;
    } else {
        m_pActiveEventList = (tSectionManEvent*)n;
    }
    m_iNumActiveEvent += 1;
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
int cSectionMan::getNextActiveEvent(tSectionManEvent* pPrevEvent)
{
    if (((void*)pPrevEvent) != 0) {
        return (int)pPrevEvent->pNext;
    }
    return (int)m_pActiveEventList;
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
void cSectionMan::removeActiveEvent(tSectionManEvent* prev, tSectionManEvent* node)
{
    if (((void*)prev) == 0) {
        m_pActiveEventList = (tSectionManEvent*)((int)node->pNext);
    } else {
        prev->pNext = (tSectionManEvent*)((int)node->pNext);
    }
    cSectionMan_freeEvent_v(this, node);
    m_iNumActiveEvent = m_iNumActiveEvent - 1;
}
#endif

