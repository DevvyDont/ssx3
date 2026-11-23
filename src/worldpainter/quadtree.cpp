#include "vtable.h"
#include "common.h"
#include "util/localefile.h"
#include "util/bxps2memcard.h"
#include "worldpainter/quadtree.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_getFreeNode);
#ifdef SKIP_ASM
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void*);
extern "C" void func_0041605C(void* dst, void* src, int n);
extern char str_QuadTreeNodeList[];



// PORT: node index computed from pointers cast to unsigned int
static inline unsigned short getNodeId_17D0(cQuadTree* self, tQuadTreeNode* node)
{
    return ((unsigned int)node - (unsigned int)(self->m_nodes)) >> 3;
}

/**
 * Takes a node from the quad tree's free list, growing the node array by 1024 when the list is empty.
 *
 * Growing allocates a new "QuadTreeNodeList" array, copies the old nodes over, chains the new nodes into the
 * free list (index 0 ends it) and frees the old array. The taken node is counted in m_numUsedNodes.
 *
 * @return the index of the node taken
 */
extern "C" unsigned short cQuadTree_getFreeNode(cQuadTree* self)
{
    unsigned short ret;
    if (self->m_freeNodes == self->m_nodes) {
        tQuadTreeNode* newnodelist = new (str_QuadTreeNodeList, 0, 0) tQuadTreeNode[self->m_numNodes + 1024];
        if (self->m_nodes != 0) {
            func_0041605C(newnodelist, self->m_nodes, self->m_numNodes * sizeof(tQuadTreeNode));
        }
        int i;
        for (i = self->m_numNodes; i < self->m_numNodes + 1023; i++) {
            newnodelist[i].setNext(i + 1);
        }
        newnodelist[self->m_numNodes + 1023].setNext(0);
        (*(tQuadTreeNode**)&self->m_freeNodes) = &newnodelist[self->m_numNodes];
        self->m_numNodes += 1024;
        if (self->m_nodes != 0) {
            cMemMan_free(self->m_nodes);
        }
        (*(tQuadTreeNode**)&self->m_nodes) = newnodelist;
    }
    ret = getNodeId_17D0(self, self->m_freeNodes);
    (*(tQuadTreeNode**)&self->m_freeNodes) = &(self->m_nodes)[(self->m_freeNodes)->getNext()];
    self->m_numUsedNodes++;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_setFreeNodes);
#ifdef SKIP_ASM
/**
 * Returns four sibling nodes to the free chain: chains @p nodeid0, @p nodeid1, @p nodeid2, @p nodeid3 in front of the old head (the first
 * short of *m_freeNodes), makes @p nodeid0 the head and lowers m_numUsedNodes by 4.
 *
 * @param nodeid0  the first node index (On Tour nodeid0)
 * @param nodeid1  the second (On Tour nodeid1)
 * @param nodeid2  the third (On Tour nodeid2)
 * @param nodeid3  the fourth (On Tour nodeid3)
 */
extern "C" void cQuadTree_setFreeNodes(cQuadTree* self, unsigned short nodeid0, unsigned short nodeid1, unsigned short nodeid2, unsigned short nodeid3) {
    char *arr = (char*)self->m_nodes;
    *(unsigned short *)(arr + (nodeid0 << 3)) = nodeid1;
    *(unsigned short *)(arr + (nodeid1 << 3)) = nodeid2;
    *(unsigned short *)(arr + (nodeid2 << 3)) = nodeid3;
    *(unsigned short *)(arr + (nodeid3 << 3)) = *(unsigned short*)self->m_freeNodes;
    *(unsigned short*)self->m_freeNodes = nodeid0;
    self->m_numUsedNodes = self->m_numUsedNodes - 4;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_setData);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_002C1A00(void* self, void* node, unsigned short x, unsigned short z, int size, int a5, int a6);

/**
 * Stores a key and data pointer for a square area of the quad tree.
 *
 * Maps @p fx / @p fz to grid coordinates ((v - m_offsetX / m_offsetY) * m_scale) and @p fsize to grid units (*
 * m_scale); when either coordinate falls outside 0..0x7FFF it does nothing. Otherwise it passes the root node (m_nodes
 * + m_root), the coordinates doubled, 0x8000 / size and the key and data to func_002C1A00, which walks down and splits
 * nodes. Nothing in retail calls it (trees come pre-built from the world-painter data).
 *
 * @param leafKey  the leaf key to store (the region index)
 * @param leafData  the leaf data pointer to store
 * @param fx  the area's minimum x in world units
 * @param fz  the area's minimum y (the tree's second axis) in world units
 * @param fsize  the area's width in world units
 */
extern "C" void cQuadTree_setData(cQuadTree* self, int leafKey, int leafData, float fx, float fz, float fsize)
{
    int x = (int)((fx - self->m_offsetX) * *(float*)self);
    int z = (int)((fz - self->m_offsetY) * *(float*)self);
    fsize = fsize * *(float*)self;
    if ((unsigned int)x > 0x7FFF || (unsigned int)z > 0x7FFF) {
        return;
    }
    int size = 0x8000 / (int)fsize;
    func_002C1A00(self, &self->m_nodes[self->m_root], (unsigned short)(x << 1), (unsigned short)(z << 1), size, leafKey, leafData);
}
#endif
INCLUDE_ASM("worldpainter/quadtree", func_002C1A00);

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_getLeaf);
#ifdef SKIP_ASM
/**
 * Returns the leaf covering a world position: m_defaultNode (+0x18) outside the grid, else the leaf found from
 * m_root.
 *
 * The position becomes 15-bit grid coordinates ((p - offset) * m_scale); each branch level (bit 0 of the node's first
 * short set) takes the child picked by the next x and z bits. The final `return 0` for a branch node cannot be
 * reached.
 *
 * @param fx  the world x (On Tour x)
 * @param fz  the world z (On Tour y)
 * @return the leaf node (On Tour const tQuadTreeLeafNode*)
 */
extern "C" void* cQuadTree_getLeaf(cQuadTree* self, float fx, float fz)
{
    int x = (int)((fx - self->m_offsetX) * *(float*)self);
    int z = (int)((fz - self->m_offsetY) * *(float*)self);
    if ((unsigned int)x > 0x7FFF || (unsigned int)z > 0x7FFF) {
        return (char*)&self->m_defaultNode;
    }
    unsigned short ux = x << 1;
    unsigned short uz = z << 1;
    tQuadTreeNode* nodes = self->m_nodes;
    unsigned short* node = &nodes[self->m_root].m_sub.m_children[0];
    while (node[0] & 1) {
        int idx = ((ux >> 15) << 1) | (uz >> 15);
        ux <<= 1;
        uz <<= 1;
        node = &nodes[*(unsigned short*)((char*)node + (idx << 1)) >> 1].m_sub.m_children[0];
    }
    if (node[0] & 1) {
        return 0;
    }
    return node;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_exportTree);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* memset(void* dst, int c, int n);
extern "C" void tQuadTreeNode_setSubNodes(unsigned short* p, unsigned short a, unsigned short b, unsigned short c, unsigned short d);
struct cQuadTree_exportNode_sOut;
struct cQuadTree_exportNode_sNode;
extern "C" void cQuadTree_exportNode(cQuadTree* self, cQuadTree_exportNode_sOut* out, int* count, int* map, cQuadTree_exportNode_sNode* node);
extern char str_QuadTreeRemapTable[];
extern char str_QuadTreeOutputBuffer[];

struct sQTHeader_1DA8 {
    float f0;           // 0x00
    float f4;           // 0x04
    float f8;           // 0x08
    int fC;             // 0x0C
    int f10;            // 0x10
    unsigned short f14; // 0x14
    unsigned short f16;
    int f18;            // 0x18
    int f1C;            // 0x1C
    char* f20;          // 0x20
    int f24;            // 0x24
};

/**
 * Writes a quad tree into a new flat buffer ("QuadTreeOutputBuffer"), remapping node links and converting
 * byte order.
 *
 * Probably an editor or tool export. The output is pre-filled with 0xDEADBEEF; the "QuadTreeRemapTable" it
 * allocates is never freed.
 *
 * @param out  receives the new buffer (On Tour char*& buffer)
 * @param size  receives the buffer size in bytes
 * @param ctx  passed to the converters (On Tour bool isBigEndian)
 * @param fixFloat  float converter (On Tour convertFloat)
 * @param fixInt  int converter (On Tour convertInt)
 * @param fixShort  unsigned short converter
 */
extern "C" void cQuadTree_exportTree(cQuadTree* self, char** out, int* size, void* ctx,
                                     float (*fixFloat)(void*, float), int (*fixInt)(int, void*),
                                     unsigned short (*fixShort)(unsigned short, void*))
{
    int i;
    *size = self->m_numUsedNodes * 8 + 40;
    int* map = new (str_QuadTreeRemapTable, 0, 0) int[self->m_numNodes];
    for (i = 0; i < self->m_numNodes; i++)
        map[i] = -1;
    *out = new (str_QuadTreeOutputBuffer, 0, 0) char[*size];
    memset(*out, 0xDEADBEEF, *size);
    sQTHeader_1DA8* header = (sQTHeader_1DA8*)*out;
    *header = *((sQTHeader_1DA8*)self);
    int cnt = 0;
    unsigned short* nodeData = (unsigned short*)(header + 1);
    cQuadTree_exportNode(self, (cQuadTree_exportNode_sOut*)nodeData, &cnt, map, (cQuadTree_exportNode_sNode*)&self->m_nodes[self->m_root]);
    for (i = 0; i < cnt; i++, nodeData += 4) {
        if (nodeData[0] & 1) {
            tQuadTreeNode_setSubNodes(nodeData, map[nodeData[0] >> 1], map[nodeData[1] >> 1], map[nodeData[2] >> 1], map[nodeData[3] >> 1]);
            nodeData[0] = fixShort(nodeData[0], ctx);
            nodeData[1] = fixShort(nodeData[1], ctx);
            nodeData[2] = fixShort(nodeData[2], ctx);
            nodeData[3] = fixShort(nodeData[3], ctx);
        } else {
            ((int*)nodeData)[0] = fixInt(((int*)nodeData)[0], ctx);
            ((int*)nodeData)[1] = fixInt(((int*)nodeData)[1], ctx);
        }
    }
    header->f10 = 0;
    header->f24 = 0;
    header->f1C = fixInt((int)self->m_defaultNode.m_key, ctx);
    header->f18 = fixInt(*(int*)&self->m_defaultNode, ctx);
    header->f0 = fixFloat(ctx, header->f0);
    header->f4 = fixFloat(ctx, header->f4);
    header->f8 = fixFloat(ctx, header->f8);
    header->fC = fixInt(header->fC, ctx);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_exportNode);
#ifdef SKIP_ASM
struct cQuadTree_exportNode_sNode {
    unsigned short child[4];
};
struct cQuadTree_exportNode_sOut {
    int f0;
    int f4;
};

/**
 * Copies a node and, for a branch, its four subtrees depth first into an export list, recording each node's new
 * index.
 *
 * out[*count] gets the node's two words, map[the node's index in m_nodes] gets *count, and *count grows by one per
 * node.
 *
 * @param out  the export node list (On Tour filenodelist)
 * @param count  in/out: nodes exported so far (On Tour int& numexportednodes)
 * @param map  the old-to-new index table (On Tour remaptable)
 * @param node  the node to export
 */
// PORT: node index computed from pointers cast to unsigned int
extern "C" void cQuadTree_exportNode(cQuadTree* self, cQuadTree_exportNode_sOut* out, int* count, int* map, cQuadTree_exportNode_sNode* node)
{
    int i;
    unsigned short* childPtr;
    out[*count].f4 = *(int*)((char*)node + 4);
    out[*count].f0 = *(int*)((char*)node + 0);
    map[(unsigned short)(((unsigned int)node - (unsigned int)self->m_nodes) >> 3)] = (*count)++;
    if (node->child[0] & 1) {
        childPtr = node->child;
        for (i = 0; i < 4; i++) {
            cQuadTree_exportNode(self, out, count, map, (cQuadTree_exportNode_sNode*)self->m_nodes + (*childPtr++ >> 1));
        }
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cQuadTree_calcUsedNodes);
#ifdef SKIP_ASM
/**
 * Counts the nodes in a subtree: 1 for @p node plus, for a branch, its four children's counts.
 *
 * A null @p node also counts 1.
 *
 * @param node  the subtree's root
 * @return the node count
 */
extern "C" int cQuadTree_calcUsedNodes(cQuadTree* self, unsigned short *node) {
    int count = 1;
    if (node != 0) {
        if (*node & 1) {
            unsigned short *childPtr = node;
            int i = 3;
            do {
                unsigned short childRef = *childPtr;
                childPtr += 1;
                i -= 1;
                count += cQuadTree_calcUsedNodes(self, &self->m_nodes[childRef >> 1].m_sub.m_children[0]);
            } while (i >= 0);
        }
    }
    return count;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2200);
#ifdef SKIP_ASM
/**
 * Does nothing. Nothing in retail references it.
 */
extern "C" void func_002C2200(void) {

}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2208);
#ifdef SKIP_ASM
/**
 * Does nothing. Nothing in retail references it.
 */
extern "C" void func_002C2208(void) {

}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cMemCard_AsciiToShiftJIS);
#ifdef SKIP_ASM
extern "C" short func_002C2338(unsigned char c);

/**
 * Converts an ASCII string into 16-bit Shift-JIS full-width characters, one func_002C2338 call per byte, and ends @p
 * dst with a 0.
 *
 * A character func_002C2338 has no full-width form for becomes 0 and so ends the output early. func_002C48C0 (state 38)
 * uses it for the save title of the icon.sys it writes.
 *
 * @param dst  the 16-bit output buffer
 * @param src  the ASCII string
 */
extern "C" void cMemCard_AsciiToShiftJIS(void* self, short* dst, const char* src)
{
    while (*src != 0) {
        *dst++ = func_002C2338(*src++);
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cMemCard_ShiftJISToAscii);
#ifdef SKIP_ASM
extern "C" char func_002C2430(short c);

/**
 * Converts a 16-bit Shift-JIS string into ASCII, one func_002C2430 call per character, and ends @p dst with a 0.
 *
 * A character with no ASCII form becomes 0 in the output (the loop goes on; the string just reads shorter).
 * cPS2MemCard_Update uses it for the save titles it reads from the cards.
 *
 * @param dst  the ASCII output buffer
 * @param src  the 16-bit Shift-JIS string
 */
extern "C" void cMemCard_ShiftJISToAscii(void* self, char* dst, const short* src)
{
    for (; *src != 0; src++, dst++) {
        *dst = func_002C2430(*src);
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cMemCard_CreateInstance);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2MemCard_cPS2MemCard(cPS2MemCard*);
extern const char str_cPS2MemCard[];

/**
 * Allocates and constructs the memory-card driver: cMemMan_alloc(0x1500, "cPS2MemCard", 0x100, 0) passed to
 * cPS2MemCard_cPS2MemCard.
 *
 * @return the new cPS2MemCard
 */
extern "C" void* cMemCard_CreateInstance(void)
{
    return cPS2MemCard_cPS2MemCard((cPS2MemCard*)cMemMan_alloc(5376, str_cPS2MemCard, 0x100, 0));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cMemCard_DeleteInstance);
#ifdef SKIP_ASM
class cMemCard_DeleteInstance_cObj {
public:
    virtual ~cMemCard_DeleteInstance_cObj();
};

/**
 * Destroys a memory card object.
 *
 * Calls the object's virtual deleting destructor; a null pointer is ignored.
 *
 * @param self  the memory card object to delete (On Tour pMemCard)
 */
extern "C" void cMemCard_DeleteInstance(cMemCard_DeleteInstance_cObj* self)
{
    delete self;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2338);
#ifdef SKIP_ASM
struct func_002C2338_sRange {
    unsigned short base;
    unsigned short first;
};
extern func_002C2338_sRange D_00445940[];
extern unsigned short D_00445950[];

/**
 * Returns the Shift-JIS full-width form of a printable ASCII character, byte-swapped so the lead byte comes first in
 * memory.
 *
 * Space and the punctuation ranges (0x20-0x2F, 0x3A-0x40, 0x5B-0x60, 0x7B-0x7E) come from the table D_00445950; digits,
 * upper-case and lower-case letters are their range's first code (D_00445940) plus the offset into the range. Any other
 * byte (control characters, 0x7F and above) gives 0. cMemCard_AsciiToShiftJIS calls it per character.
 *
 * @param asciiChar  the ASCII character
 * @return the 16-bit Shift-JIS code with its bytes swapped, or 0
 */
extern "C" short func_002C2338(unsigned char asciiChar)
{
    int off = 0;
    int idx = 0;
    unsigned short sjisCode;

    if (asciiChar >= 0x20 && asciiChar <= 47) {
        off = 1;
    } else if (asciiChar >= 0x30 && asciiChar <= 57) {
    } else if (asciiChar >= 58 && asciiChar <= 0x40) {
        off = 11;
    } else if (asciiChar >= 65 && asciiChar <= 90) {
        idx = 1;
    } else if (asciiChar >= 91 && asciiChar <= 0x60) {
        off = 37;
    } else if (asciiChar >= 97 && asciiChar <= 122) {
        idx = 2;
    } else if (asciiChar >= 123 && asciiChar <= 0x7E) {
        off = 0x3F;
    } else {
        return 0;
    }
    if (off != 0) {
        sjisCode = D_00445950[(asciiChar - 0x20) + 1 - off];
    } else {
        sjisCode = D_00445940[idx].base + asciiChar - D_00445940[idx].first;
    }
    return (sjisCode << 8) | (sjisCode >> 8);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2430);
#ifdef SKIP_ASM
extern unsigned char D_00445998[];
// Shift-JIS full-width range table: {first code, ASCII of first code} per range (digits, upper, lower case).
extern unsigned char D_004A3890[6];
extern unsigned char D_004A3891[5];

// Converts a Shift-JIS double-byte character (lead byte in the low byte) to ASCII; 0 if it has none.
// PORT: declared earlier in the unit as char(short); defined under its own name bound to the symbol.
extern "C" unsigned char func_002C2430_impl(unsigned int code) __asm__("func_002C2430");
/**
 * Returns the ASCII form of a Shift-JIS full-width character whose lead byte is in the low byte (the order
 * func_002C2338 produces), or 0 when it has none.
 *
 * Lead byte 0x81 (punctuation) looks the trail byte up in D_00445998 (from trail byte 0x40); lead byte 0x82 covers
 * digits (trail 0x4F-0x58), upper-case (0x60-0x79) and lower-case letters (0x81-0x9A), mapped through the range table
 * D_004A3890 / D_004A3891 (a trail byte outside the three ranges is mapped with the digits' entry). Any other lead byte
 * gives 0. cMemCard_ShiftJISToAscii calls it per character.
 *
 * @param code  the 16-bit character (lead byte low, trail byte high)
 * @return the ASCII character, or 0
 */
extern "C" unsigned char func_002C2430_impl(unsigned int code)
{
    unsigned int hi = (code >> 8) & 0xFF;
    int i = 0;
    unsigned char* ascii = D_004A3891;
    code &= 0xFF;
    if (code == 129)
        return D_00445998[hi - 0x40];
    if (code == 130) {
        if (hi - 79 < 10)
            i = 0;
        else if (hi - 0x60 < 26)
            i = 1;
        else if ((unsigned char)(hi + 0x7F) < 26)
            i = 2;
        else
            i = 0;
        return ascii[i * 2] + (hi - D_004A3890[i * 2]);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", strlen1);
#ifdef SKIP_ASM
/**
 * Returns the length of a 16-bit character string (On Tour: strlen for unsigned short strings).
 *
 * @param str  the NUL-terminated 16-bit string
 * @return the number of characters before the terminator
 */
extern "C" int strlen1(unsigned short* str)
{
    int n = 0;
    while (*str++ != 0) {
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", strcpy1);
#ifdef SKIP_ASM
/**
 * Copies a NUL-terminated 16-bit-character string.
 *
 * @param dst  destination buffer
 * @param src  string to copy
 * @return @p dst
 */
extern "C" unsigned short* strcpy1(unsigned short* dst, unsigned short* src)
{
    unsigned short* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2540);
#ifdef SKIP_ASM
/**
 * Copies an 8-bit string into a 16-bit one, widening each byte, and ends @p dst with a 0 (no length limit).
 *
 * The movie player's memory-card overlay (cMCOverlayManager) builds its text with it.
 *
 * @param dst  the 16-bit output buffer
 * @param src  the 8-bit string
 * @return @p dst
 */
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src)
{
    unsigned short* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", strcpy2);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares this as extern "C" void* strcpy2(void*) further down
// (its caller cPS2MemCard_SetTitleString passes one arg); bind the 2-arg body to the symbol.
char* strcpy2_impl(char* dst, unsigned short* src) __asm__("strcpy2");
/**
 * Copies a 16-bit-character string into a byte string, keeping only the low byte of each character.
 *
 * @param dst  destination byte buffer
 * @param src  NUL-terminated 16-bit string to copy
 * @return @p dst
 */
char* strcpy2_impl(char* dst, unsigned short* src)
{
    char* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C25B8);
#ifdef SKIP_ASM
/**
 * Copies at most @p n units of a 16-bit string and ends @p dst with a 0, so @p dst needs room for n + 1 units.
 *
 * @param dst  the 16-bit output buffer
 * @param src  the 16-bit string
 * @param n  the most units to copy
 * @return @p dst
 */
extern "C" unsigned short* func_002C25B8(unsigned short* dst, unsigned short* src, int n)
{
    unsigned short* ret = dst;
    while (*src != 0 && n > 0) {
        *dst++ = *src++;
        n--;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C25F8);
#ifdef SKIP_ASM
/**
 * Copies at most @p n bytes of an 8-bit string into a 16-bit one, widening each byte, and ends @p dst with a 0, so
 * @p dst needs room for n + 1 units. Nothing in retail calls it.
 *
 * @param dst  the 16-bit output buffer
 * @param src  the 8-bit string
 * @param n  the most characters to copy
 * @return @p dst
 */
extern "C" unsigned short* func_002C25F8(unsigned short* dst, const char* src, int n)
{
    unsigned short* start = dst;
    while (*src && n > 0) {
        *dst++ = *src++;
        n--;
    }
    *dst = 0;
    return start;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2640);
#ifdef SKIP_ASM
/**
 * Copies at most @p n units of a 16-bit string into an 8-bit one, keeping only each unit's low byte, and ends @p dst
 * with a 0, so @p dst needs room for n + 1 bytes. Nothing in retail calls it.
 *
 * @param dst  the 8-bit output buffer
 * @param src  the 16-bit string
 * @param n  the most characters to copy
 * @return @p dst
 */
extern "C" unsigned char* func_002C2640(unsigned char* dst, const unsigned short* src, int n)
{
    unsigned char* start = dst;
    while (*src && n > 0) {
        *dst++ = *src++;
        n--;
    }
    *dst = 0;
    return start;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2688);
#ifdef SKIP_ASM
extern "C" unsigned short* strcpy1(unsigned short* dst, unsigned short* src);

/**
 * Appends a 16-bit string to another: finds the end of @p dst and copies @p src there with strcpy1.
 *
 * @param dst  the 16-bit string to extend (needs room for both)
 * @param src  the 16-bit string to append
 * @return strcpy1's result (the end of the original @p dst)
 */
extern "C" unsigned short* func_002C2688(unsigned short* dst, unsigned short* src)
{
    while (*dst != 0) {
        dst++;
    }
    return strcpy1(dst, src);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", sprintf1);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* sprintf1_va_list;
#define sprintf1_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" int USTR_vsprintf(unsigned short* dst, const unsigned short* fmt, char* ap);

/**
 * Formats a 16-bit-character string, like sprintf(), through USTR_vsprintf().
 *
 * @param dst  destination buffer of 16-bit characters
 * @param fmt  16-bit format string, followed by its arguments
 * @return the value USTR_vsprintf() returns
 */
extern "C" int sprintf1(unsigned short* dst, const unsigned short* fmt, ...)
{
    sprintf1_va_list ap;
    sprintf1_va_start(ap);
    return USTR_vsprintf(dst, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", usprintf);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

/**
 * Formats a wide (16-bit) string from an 8-bit format string, like sprintf.
 *
 * Copies @p fmt character by character into a 1024-entry wide buffer, then formats it into @p dst with the variable
 * arguments through USTR_vsprintf.
 *
 * @param dst  the wide string to write (On Tour s)
 * @param fmt  printf-style format, 8-bit characters
 * @return USTR_vsprintf's result
 */
// PORT: hand-rolled EE EABI va_start (sprintf1_va_start, defined above); use <stdarg.h> off-PS2.
extern "C" int usprintf(unsigned short* dst, const char* fmt, ...)
{
    sprintf1_va_list ap;
    unsigned short buf[0x400];
    unsigned short* bufPos;
    int n;
    int i;
    bufPos = buf;
    n = strlen(fmt);
    for (i = 0; i < n; i++) {
        *bufPos++ = *fmt++;
    }
    *bufPos = 0;
    sprintf1_va_start(ap);
    return USTR_vsprintf(dst, buf, ap);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", sprintf2);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

/**
 * Formats into a 16-bit string from an 8-bit format string (On Tour: sprintf for unsigned short strings).
 *
 * Widens @p fmt into a 1024-character buffer, then formats the variable arguments with USTR_vsprintf().
 *
 * @param dst  the 16-bit destination string
 * @param fmt  the 8-bit format string
 * @return USTR_vsprintf()'s result
 */
// PORT: hand-rolled EE EABI va_start (sprintf1_va_start, defined above); use <stdarg.h> off-PS2.
extern "C" int sprintf2(unsigned short* dst, const char* fmt, ...)
{
    sprintf1_va_list ap;
    unsigned short buf[0x400];
    unsigned short* bufPos;
    int n;
    int i;
    bufPos = buf;
    n = strlen(fmt);
    for (i = 0; i < n; i++) {
        *bufPos++ = *fmt++;
    }
    *bufPos = 0;
    sprintf1_va_start(ap);
    return USTR_vsprintf(dst, buf, ap);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2868);
#ifdef SKIP_ASM
/**
 * Converts the ASCII upper-case letters of a string to lower case in place.
 *
 * @param str  the string
 */
extern "C" void func_002C2868(char* str)
{
    while (*str != 0) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 'a' - 'A';
        }
        str++;
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C28A0);
#ifdef SKIP_ASM
extern "C" void TIMER_gettick();

/**
 * Calls TIMER_gettick, which reads the counter D_00450DCC; the GameLink platform table set up by func_002C2BD8 holds it
 * at +0x58.
 */
extern "C" void func_002C28A0(void) {
    TIMER_gettick();
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C28C0);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_GAMELINK_data[];

/**
 * Allocates a block for GameLink: operator_new__FUi(size, "GAMELINK data", 0x100 when @p zeroFlag is set else 0, 0).
 *
 * func_002C2BD8 stores it in the GameLink platform table (+0x3C); func_002C2940 and func_002C29D0 use it.
 *
 * @param name  unused (the tag is always "GAMELINK data")
 * @param size  the block size in bytes
 * @param zeroFlag  nonzero to pass the 0x100 allocation flag
 * @return the block's address
 */
extern "C" int func_002C28C0(int name, int size, int zeroFlag)
{
    return (int)new (str_GAMELINK_data, zeroFlag ? 0x100 : 0, 0) char[size];
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C28F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Unit declares MEM_free / func_002C28F8 later with void return; keep the int-returning
// signatures under asm labels. The argument passes straight through in a0.
extern "C" int MEM_free_ret(void* p) __asm__("MEM_free");
extern "C" int func_002C28F8_impl(void* p) __asm__("func_002C28F8");

/**
 * Frees a GameLink block with MEM_free and returns whether MEM_free returned nonzero.
 *
 * @param block  the block to free
 * @return 1 when MEM_free returned nonzero, else 0
 */
extern "C" int func_002C28F8_impl(void* block)
{
    return MEM_free_ret(block) != 0;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2918);
#ifdef SKIP_ASM
extern "C" void MEM_copy();

/**
 * Calls MEM_copy with the caller's arguments passed through and returns the destination (GameLink's memory copy).
 * Nothing in retail references it; func_002C2BD8 stores memset in the copy slot instead.
 *
 * @param dst  the destination address
 * @return @p dst
 */
extern "C" int func_002C2918(int dst) {
    MEM_copy();
    return dst;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2940);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MUTEX_lock_p(void*) __asm__("MUTEX_lock");
extern "C" void MUTEX_unlock_p(void*) __asm__("MUTEX_unlock");
extern "C" int cMemMan_internalResizeBlock(int, int);
extern "C" int func_002C28C0(int, int, int);
extern "C" void func_002C28F8(int);
extern char cMemMan_m_mutex[];

/**
 * Resizes a GameLink block, realloc style.
 *
 * A null @p block allocates a new one (func_002C28C0); a zero @p size frees it (func_002C28F8) and returns 0; otherwise
 * cMemMan_internalResizeBlock resizes it with cMemMan_m_mutex locked.
 *
 * @param block  the block to resize, or 0
 * @param name  passed to func_002C28C0 (unused there)
 * @param size  the new size in bytes
 * @param zeroFlag  passed to func_002C28C0 for a new block
 * @return the block's (new) address, or 0
 */
extern "C" int func_002C2940(int block, int name, int size, int zeroFlag) {
    if (block == 0) {
        return func_002C28C0(name, size, zeroFlag);
    }
    if (size == 0) {
        func_002C28F8(block);
        return 0;
    } else {
        int result;
        MUTEX_lock_p(cMemMan_m_mutex);
        result = cMemMan_internalResizeBlock(block, size);
        MUTEX_unlock_p(cMemMan_m_mutex);
        return result;
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C29D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_002C28C0_29D0(int, int, int) __asm__("func_002C28C0");
extern "C" int func_002C28F8_29D0(int) __asm__("func_002C28F8");
extern "C" int THREAD_createparam(int* self, int a1, int a2, int a3, int a4, int a5);
extern char str_threadstack[];

// Predicate helper; keeps p[0] in one register for both tests.
inline int func_002C29D0_valid(int arg0)
{
    return arg0 != 0;
}

// PORT: the unit later declares func_002C29D0() for its address use; bound by asm label
int* func_002C29D0_impl(int a0, int a1, int size) __asm__("func_002C29D0");

/**
 * Creates and starts a thread for GameLink and returns its 0xC-byte handle, or 0 on failure.
 *
 * The handle comes from func_002C28C0; its first word is a @p size byte stack ("threadstack"), and THREAD_createparam
 * creates and starts the thread (CreateThread / StartThread, priority 99) with its id in the rest of the handle. When
 * the stack or the thread cannot be made, everything allocated is freed again.
 *
 * @param threadEntry  the thread's entry function
 * @param threadArg  the argument passed to the entry function
 * @param size  the stack size in bytes
 * @return the handle, or 0
 */
int* func_002C29D0_impl(int threadEntry, int threadArg, int size)
{
    int* thread = (int*)func_002C28C0_29D0(0, 0xC, 0);
    thread[0] = 0;
    thread[0] = func_002C28C0_29D0((int)str_threadstack, size, 0);
    if (thread[0] != 0) {
        if (THREAD_createparam(thread + 1, threadEntry, threadArg, thread[0], size, 99) == 0) {
            func_002C28F8_29D0(thread[0]);
            thread[0] = 0;
        }
    }
    if (func_002C29D0_valid(thread[0])) {
        return thread;
    }
    if (thread != 0) {
        if (thread[0] != 0) {
            func_002C28F8_29D0(thread[0]);
        }
        func_002C28F8_29D0((int)thread);
    }
    return 0;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2AA8);
#ifdef SKIP_ASM
extern "C" void func_002C28F8(int);

/**
 * Frees a thread handle made by func_002C29D0: frees its stack (first word) and clears it, then frees the handle itself
 * with func_002C28F8. The handle is read before it is checked for 0.
 */
extern "C" void func_002C2AA8(int *self) {
    if (*self != 0) {
        func_002C28F8(*self);
        *self = 0;
    }
    if (self != 0) {
        if (*self != 0) {
            func_002C28F8(*self);
        }
        func_002C28F8((int)self);
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2B08);
#ifdef SKIP_ASM
extern "C" void THREAD_yield();

/**
 * Calls THREAD_yield, which yields the current thread (GameLink's yield).
 */
extern "C" void func_002C2B08(void) {
    THREAD_yield();
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2B28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002C28C0(int, int, int);
extern "C" void MUTEX_create(int);
extern char str_mutex[];

// The unit declares func_002C2B28 later as void(); keep the real signature under an asm label.
extern "C" int func_002C2B28_impl(void) __asm__("func_002C2B28");

/**
 * Allocates a 0x10-byte block with func_002C28C0 and initialises it as a mutex with MUTEX_create (GameLink's mutex
 * create).
 *
 * @return the mutex block
 */
extern "C" int func_002C2B28_impl(void)
{
    int mutex = func_002C28C0((int)str_mutex, 0x10, 0);
    MUTEX_create(mutex);
    return mutex;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2B68);
#ifdef SKIP_ASM
extern "C" void func_002C28F8(int);
extern "C" void MUTEX_destroy();

/**
 * Destroys a mutex made by func_002C2B28: calls MUTEX_destroy (DeleteSema), then frees the block with func_002C28F8.
 *
 * @param mutexBlock  the mutex block
 */
extern "C" void func_002C2B68(int mutexBlock) {
    MUTEX_destroy();
    func_002C28F8(mutexBlock);
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2B98);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock();

/**
 * Calls MUTEX_lock with the caller's arguments passed through (GameLink's mutex lock).
 */
extern "C" void func_002C2B98(void) {
    MUTEX_lock();
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2BB8);
#ifdef SKIP_ASM
extern "C" void MUTEX_unlock();

/**
 * Calls MUTEX_unlock with the caller's arguments passed through (GameLink's mutex unlock).
 */
extern "C" void func_002C2BB8(void) {
    MUTEX_unlock();
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2BD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void strcmp();
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_002C29D0();
extern "C" void func_002C2AA8();
extern "C" void func_002C2B08();
extern "C" void func_002C2B28();
extern void func_002C28C0_2BD8() __asm__("func_002C28C0");
extern void func_002C2940_2BD8() __asm__("func_002C2940");
extern "C" void func_0041610C();
extern "C" void memcmp();
extern "C" void func_0041605C();
extern "C" void func_002C28A0();
extern "C" void atof();
extern "C" void atoi();
extern "C" void bsearch();
extern "C" void qsort();
/**
 * Fills a GameLink platform function table with the game's implementations.
 *
 * +0x4 strlen, +0x8 strcpy, +0xC strcmp, +0x10 atoi, +0x14 atof, +0x18 qsort, +0x1C bsearch, +0x20
 * func_002C29D0 (thread create), +0x24 func_002C2AA8 (thread free), +0x28 func_002C2B08 (yield), +0x2C func_002C2B28
 * (mutex create), +0x30 func_002C2B68 (mutex destroy), +0x34 / +0x38 func_002C2B98 / func_002C2BB8 (lock, unlock),
 * +0x3C func_002C28C0 (alloc), +0x40 func_002C2940 (realloc), +0x44 func_002C28F8 (free), +0x48 memset, +0x4C
 * func_0041605C, +0x50 func_0041610C, +0x54 memcmp and +0x58 func_002C28A0 (counter). Nothing in retail calls
 * it.
 */
extern "C" void func_002C2BD8(char* self) {
    *(void**)(self + 0x3C) = (void*)&func_002C28C0_2BD8;
    *(void**)(self + 0x40) = (void*)&func_002C2940_2BD8;
    *(void**)(self + 0x44) = (void*)&func_002C28F8;
    *(void**)(self + 0x48) = (void*)&memset;
    *(void**)(self + 0x50) = (void*)&func_0041610C;
    *(void**)(self + 0x54) = (void*)&memcmp;
    *(void**)(self + 0x4C) = (void*)&func_0041605C;
    *(void**)(self + 0x58) = (void*)&func_002C28A0;
    *(void**)(self + 0x14) = (void*)&atof;
    *(void**)(self + 0x10) = (void*)&atoi;
    *(void**)(self + 0x1C) = (void*)&bsearch;
    *(void**)(self + 0x18) = (void*)&qsort;
    *(void**)(self + 0xC) = (void*)&strcmp;
    *(void**)(self + 0x8) = (void*)&strcpy;
    *(void**)(self + 0x4) = (void*)&strlen;
    *(void**)(self + 0x20) = (void*)&func_002C29D0;
    *(void**)(self + 0x24) = (void*)&func_002C2AA8;
    *(void**)(self + 0x28) = (void*)&func_002C2B08;
    *(void**)(self + 0x2C) = (void*)&func_002C2B28;
    *(void**)(self + 0x30) = (void*)&func_002C2B68;
    *(void**)(self + 0x34) = (void*)&func_002C2B98;
    *(void**)(self + 0x38) = (void*)&func_002C2BB8;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2D30);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_ISTR_data[];

/**
 * Allocates a block for ISTR: operator_new__FUi(size, "ISTR data", 0x100 when @p zeroFlag is set else 0, 0).
 *
 * func_002C2E78 stores it in the ISTR function table (+0x1C); func_002C2DB0 uses it.
 *
 * @param name  unused (the tag is always "ISTR data")
 * @param size  the block size in bytes
 * @param zeroFlag  nonzero to pass the 0x100 allocation flag
 * @return the block's address
 */
extern "C" int func_002C2D30(int name, int size, int zeroFlag)
{
    return (int)new (str_ISTR_data, zeroFlag ? 0x100 : 0, 0) char[size];
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2D68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Unit declares MEM_free / func_002C2D68 later with void return; keep the int-returning
// signatures under asm labels. The argument passes straight through in a0.
extern "C" int MEM_free_ret(void* p) __asm__("MEM_free");
extern "C" int func_002C2D68_impl(void* p) __asm__("func_002C2D68");

/**
 * Frees an ISTR block with MEM_free and returns whether MEM_free returned nonzero.
 *
 * @param block  the block to free
 * @return 1 when MEM_free returned nonzero, else 0
 */
extern "C" int func_002C2D68_impl(void* block)
{
    return MEM_free_ret(block) != 0;
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2D88);
#ifdef SKIP_ASM
extern "C" void MEM_copy();

/**
 * Calls MEM_copy with the caller's arguments passed through and returns the destination (ISTR's memory copy, stored at
 * +0x2C of the table by func_002C2E78).
 *
 * @param dst  the destination address
 * @return @p dst
 */
extern "C" int func_002C2D88(int dst) {
    MEM_copy();
    return dst;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2DB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MUTEX_lock_p(void*) __asm__("MUTEX_lock");
extern "C" void MUTEX_unlock_p(void*) __asm__("MUTEX_unlock");
extern "C" int cMemMan_internalResizeBlock(int, int);
extern "C" int func_002C2D30(int, int, int);
extern "C" void func_002C2D68(int);
extern char cMemMan_m_mutex[];

/**
 * Resizes an ISTR block, realloc style.
 *
 * A null @p block allocates a new one (func_002C2D30); a zero @p size frees it (func_002C2D68) and returns 0; otherwise
 * cMemMan_internalResizeBlock resizes it with cMemMan_m_mutex locked.
 *
 * @param block  the block to resize, or 0
 * @param name  passed to func_002C2D30 (unused there)
 * @param size  the new size in bytes
 * @param zeroFlag  passed to func_002C2D30 for a new block
 * @return the block's (new) address, or 0
 */
extern "C" int func_002C2DB0(int block, int name, int size, int zeroFlag) {
    if (block == 0) {
        return func_002C2D30(name, size, zeroFlag);
    }
    if (size == 0) {
        func_002C2D68(block);
        return 0;
    } else {
        int result;
        MUTEX_lock_p(cMemMan_m_mutex);
        result = cMemMan_internalResizeBlock(block, size);
        MUTEX_unlock_p(cMemMan_m_mutex);
        return result;
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2E40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares func_002C2E40 later as void(); keep the real signature under an asm label.
extern "C" int func_002C2E40_impl(unsigned short* s) __asm__("func_002C2E40");

/**
 * Returns the length of a 16-bit string in units, not counting the terminating 0.
 *
 * @param str  the 16-bit string
 * @return the number of units before the 0
 */
extern "C" int func_002C2E40_impl(unsigned short* str)
{
    int n = 0;
    while (*str++) {
        n++;
    }
    return n;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C2E78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_002C2D30_2E78() __asm__("func_002C2D30");
extern "C" void func_002C2D68();
extern void func_002C2DB0_2E78() __asm__("func_002C2DB0");
extern "C" void func_002C2E40();
extern "C" void memcmp();
extern "C" void func_0041610C();
extern "C" void strcat();
extern "C" void atoi();
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Fills an ISTR function table with the game's implementations.
 *
 * +0x4 strcat, +0x8 strlen, +0xC func_002C2E40 (16-bit length), +0x10 sprintf, +0x14 strcpy, +0x18 atoi, +0x1C
 * func_002C2D30 (alloc), +0x20 func_002C2DB0 (realloc), +0x24 func_002C2D68 (free), +0x28 memset, +0x2C
 * func_002C2D88 (copy), +0x30 func_0041610C and +0x34 memcmp. Nothing in retail calls it.
 */
extern "C" void func_002C2E78(char* self) {
    *(void**)(self + 0x1C) = (void*)&func_002C2D30_2E78;
    *(void**)(self + 0x20) = (void*)&func_002C2DB0_2E78;
    *(void**)(self + 0x24) = (void*)&func_002C2D68;
    *(void**)(self + 0x2C) = (void*)&func_002C2D88;
    *(void**)(self + 0x28) = (void*)&memset;
    *(void**)(self + 0x30) = (void*)&func_0041610C;
    *(void**)(self + 0x34) = (void*)&memcmp;
    *(void**)(self + 0x04) = (void*)&strcat;
    *(void**)(self + 0x08) = (void*)&strlen;
    *(void**)(self + 0x0C) = (void*)&func_002C2E40;
    *(void**)(self + 0x10) = (void*)&sprintf;
    *(void**)(self + 0x14) = (void*)&strcpy;
    *(void**)(self + 0x18) = (void*)&atoi;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C2F18);

//100%
INCLUDE_ASM("worldpainter/quadtree", cSurfPropCache_purge__FPv);
#ifdef SKIP_ASM
/**
 * Releases the surface property cache; does nothing in this build.
 */
void cSurfPropCache_purge(void* self)
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_cPS2MemCard);
#ifdef SKIP_ASM
extern "C" void cPS2MemCard_Init(void*);
extern void* cPS2MemCard_vtbl[];

/**
 * Constructs the PS2 memory card object: stores cPS2MemCard_vtbl and resets every field (cPS2MemCard_Init, Reset).
 *
 * The unit is worldpainter/quadtree, but the class is util/bxps2memcard.h's.
 *
 * @return @p self
 */
extern "C" void* cPS2MemCard_cPS2MemCard(cPS2MemCard* self)
{
    *(void***)self = cPS2MemCard_vtbl;
    cPS2MemCard_Init(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard__cPS2MemCard);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void MEM_free(void* p);
extern "C" void cPS2MemCard_CleanUp(cPS2MemCard* self);
extern void* cPS2MemCard_vtbl[];
extern void* cMemCard_vtbl[];

/**
 * Destroys the PS2 memory card object: finishes any pending card operation, frees the icon data and restores the
 * cMemCard base vtable.
 *
 * Calls cPS2MemCard_CleanUp (waits for a pending sceMcSync and clears the operation state), MEM_frees the icon file data
 * (+0x30), stores cMemCard_vtbl and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2MemCard__cPS2MemCard(cPS2MemCard* self, int flags)
{
    *(void***)self = cPS2MemCard_vtbl;
    cPS2MemCard_CleanUp((cPS2MemCard*)self);
    void* iconData = (void*)self->unk_030;
    if (iconData != 0) {
        MEM_free(iconData);
    }
    *(void***)self = cMemCard_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_Init);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void MEM_fill_4050(void* dst, int c, int n) __asm__("MEM_fill");
extern int D_004A3938;
extern int D_004A393C;
extern int D_004A3940;
extern int D_004A3944;

struct cPS2MemCard_Init_sSlot {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
};

struct cPS2MemCard_Init_sSelf {
    int vtbl;                        // 0x00
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
    int f50;
    int f54;
    char pad_0x58[0x21];
    char buf79[0x40];                // 0x79
    char bufB9[0x40];                // 0xB9
    char bufF9[0x40];                // 0xF9
    char buf139[0x40];               // 0x139
    char pad_0x179[3];
    cPS2MemCard_Init_sSlot slots[2];    // 0x17C
    char buf1A4[0x2D0];              // 0x1A4
    char buf474[0xC];
    char buf480[0x40];               // 0x480
    char pad_0x4C0[0];
    char buf4C0[0x1000];             // 0x4C0
    char buf14C0[0x20];              // 0x14C0
    int f14E0;
    int f14E4;
    int f14E8;
};

// PORT: the unit declares cPS2MemCard_Init(void*) earlier; bound by asm label
void cPS2MemCard_Init_impl(cPS2MemCard_Init_sSelf* self) __asm__("cPS2MemCard_Init");

/**
 * Puts the memory-card object into its idle state.
 *
 * Called by the constructor and through virtual slot 2. Sets unk_008 to 0x1C, mState, mDevice and unk_010 to 0,
 * mMaxFiles to 6, mFileDescriptor, unk_01C and mIconFileHandle to -1, clears the remaining counters and pending flags,
 * the static libmc result words D_004A3938 / D_004A393C / D_004A3944 (sceMcGetInfo) and D_004A3940 (last result), the
 * name/path buffers, both device records (unk_17C), the file slots (unk_1A4), the directory entries (unk_480, unk_4C0)
 * and mSourceIconPath.
 */
void cPS2MemCard_Init_impl(cPS2MemCard_Init_sSelf* self)
{
    self->f8 = 0x1C;
    self->f4 = 0;
    self->fC = 0;
    self->f10 = 0;
    self->f40 = 0;
    self->f4C = 0;
    D_004A3938 = 0;
    D_004A393C = 0;
    D_004A3940 = 0;
    self->f18 = -1;
    self->f1C = -1;
    self->f38 = -1;
    self->f14 = 6;
    self->f50 = 0;
    self->f54 = 0;
    self->f20 = 0;
    self->f24 = 0;
    self->f2C = 0;
    self->f30 = 0;
    self->f34 = 0;
    self->f44 = 0;
    self->f48 = 0;
    D_004A3944 = 0;
    self->f28 = 0;
    self->f14E0 = 0;
    self->f14E8 = 0;
    self->f14E4 = 0;
    self->f3C = 0;
    MEM_fill_4050(self->bufB9, 0, 0x40);
    MEM_fill_4050(self->bufF9, 0, 0x40);
    MEM_fill_4050(self->buf79, 0, 0x40);
    MEM_fill_4050(self->buf139, 0, 0x40);
    for (int i = 0; i < 2; i++) {
        self->slots[i].f0 = 0;
        self->slots[i].f4 = 0;
        self->slots[i].f8 = 0;
        self->slots[i].fC = 0;
        self->slots[i].f10 = 0;
    }
    MEM_fill_4050(self->buf480, 0, 0x40);
    MEM_fill_4050(self->buf1A4, 0, 720);
    MEM_fill_4050(self->buf4C0, 0, 0x1000);
    MEM_fill_4050(self->buf14C0, 0, 0x20);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetDir1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char* strcpy2_impl(char* dst, unsigned short* src) __asm__("strcpy2");


/**
 * Starts a directory listing from a 16-bit pattern: converts @p name to ASCII into a 1000-byte stack buffer (strcpy2)
 * and passes it to the card's virtual slot 6 (cPS2MemCard_GetDir).
 *
 * @param name  the sceMcGetDir pattern (16-bit characters)
 */
extern "C" void cPS2MemCard_GetDir1(cPS2MemCard* self, unsigned short* name)
{
    char buf[1000];
    strcpy2_impl(buf, name);
    sVEntry<void(void*, char*)>* vt = *(sVEntry<void(void*, char*)>**)self;
    vt[6].fn(vthis(self, vt[6]), buf);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetDir);
#ifdef SKIP_ASM
extern "C" void* func_002C48C0(cPS2MemCard*, int);

/**
 * Starts a directory listing: copies @p name into the search pattern (unk_139) and calls func_002C48C0 with state 5;
 * cPS2MemCard_Update then runs func_002C4210 (sceMcGetDir of up to 6 entries into unk_4C0).
 *
 * @param name  the sceMcGetDir pattern (ASCII; no length check)
 */
extern "C" void cPS2MemCard_GetDir(cPS2MemCard* self, const char* name)
{
    strcpy((char*)self + 0x139, name);
    func_002C48C0(self, 5);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4210);
#ifdef SKIP_ASM
extern "C" int sceMcGetDir(int h, int a1, char* name, int a3, int a4, void* buf);
extern int D_004A3940;
extern "C" void* MEM_fill(void* dst, int c, int n);

/**
 * Issues the directory listing of cPS2MemCard_GetDir: clears the directory table (unk_4C0, 0x1000 bytes) and calls
 * sceMcGetDir (sceMcGetDir) on the current port (mDevice) with the pattern unk_139, for at most 6 entries.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 6, where
 * cPS2MemCard_Update waits for the listing. cPS2MemCard_Update calls it in state 5.
 */
extern "C" void func_002C4210(cPS2MemCard* self)
{
    char* buf = (char*)&self->unk_4C0[0];
    MEM_fill(buf, 0, 0x1000);
    int mcResult = sceMcGetDir(self->mDevice, 0, (char*)self + 0x139, 0, 6, buf);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 6);
    }
}
#endif

extern "C" void* func_002C48C0(cPS2MemCard*, int);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4290__FPvi);
#ifdef SKIP_ASM
/**
 * Starts reading the directory entry of a file slot: stores @p fileSlot in the current slot (unk_01C) and calls
 * func_002C48C0 with state 34; cPS2MemCard_Update then runs func_002C42B0. Card vtable slot 76.
 *
 * @param fileSlot  the file slot index (0 .. mMaxFiles - 1; not checked)
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C4290(void* vself, int fileSlot)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_01C = fileSlot;
    return func_002C48C0((cPS2MemCard*)self, 34);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C42B0);
#ifdef SKIP_ASM
extern "C" int sceMcGetDir(int h, int a1, char* name, int a3, int a4, void* buf);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A3940;
extern char str_pct_s_pct_s_h[];

/**
 * Issues the single-entry directory read for the current file slot: clears unk_03C, copies the slot's name
 * (unk_1A4[unk_01C].unk_0C) into the file name (unk_0B9), writes "/<name>/<name>" into the path (unk_0F9) and calls
 * sceMcGetDir (sceMcGetDir) for that path with one entry into unk_480.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 35.
 * cPS2MemCard_Update calls it in state 34; cPS2MemCard_GetDataSize__FPv later reads the entry's m_fileSizeByte.
 */
extern "C" void func_002C42B0(cPS2MemCard* self)
{
    char* name = (char*)self + 0xB9;
    self->unk_03C = 0;
    char* path = (char*)self + 0xF9;
    strcpy(name, (char*)self + self->unk_01C * 0x78 + 0x1B0);
    sprintf(path, str_pct_s_pct_s_h, name, name);
    int mcResult = sceMcGetDir(self->mDevice, 0, path, 0, 1, &self->unk_480);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 35);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_Format__FPv);
#ifdef SKIP_ASM
/**
 * Starts formatting the card in the current port.
 *
 * Calls func_002C48C0 with state 17; Update (cPS2MemCard_Update) then issues sceMcFormat through func_002C4388.
 * cPS2MemCard_DidFormatFail (slot 9) reports a failure afterwards.
 *
 * @return func_002C48C0's result
 */
void* cPS2MemCard_Format(void* self)
{
    return func_002C48C0((cPS2MemCard*)self, 17);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4388);
#ifdef SKIP_ASM
extern "C" int sceMcFormat(int h, int a1);
extern int D_004A3940;

/**
 * Issues the format of cPS2MemCard_Format__FPv: calls sceMcFormat (sceMcFormat) on the current port (mDevice).
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 18.
 * cPS2MemCard_Update calls it in state 17.
 */
extern "C" void func_002C4388(cPS2MemCard* self)
{
    int mcResult = sceMcFormat(self->mDevice, 0);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 18);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_DeleteFileSlot__FPvi);
#ifdef SKIP_ASM
/**
 * Starts deleting the save in a file slot.
 *
 * Stores @p fileSlot in unk_01C and calls func_002C48C0 with state 19; Update (cPS2MemCard_Update) then runs func_002C4410,
 * which builds the "<name>/<name>" path from the slot and requests the sceMcDelete states.
 *
 * @param fileSlot  the file slot (0..mMaxFiles-1)
 * @return func_002C48C0's result
 */
void* cPS2MemCard_DeleteFileSlot(void* vself, int fileSlot)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_01C = fileSlot;
    return func_002C48C0((cPS2MemCard*)self, 19);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetDataSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns the size of the save file found by the last file lookup.
 *
 * The whole body reads unk_480.m_fileSizeByte (+0x490), filled by the single-entry sceMcGetDir of func_002C42B0.
 *
 * @return the file size in bytes
 */
int cPS2MemCard_GetDataSize(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    return (int)self->unk_480.m_fileSizeByte;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetDataSize__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the size of the save data the next operations expect.
 *
 * Stores @p val in unk_024: Update compares it with the directory entry's m_fileSizeByte, slot 23 (cPS2MemCard_SpaceRequired1) turns
 * it into the space needed, and the raw read/write requests use it as the transfer size.
 *
 * @param val  the data size in bytes
 */
void cPS2MemCard_SetDataSize(void* vself, int val)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_024 = val;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4410);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_002C48C0(cPS2MemCard*, int);
extern char str_pct_s_pct_s_h[];

/**
 * Starts deleting the save in the current file slot: clears unk_03C, copies the slot's name (unk_1A4[unk_01C].unk_0C)
 * into the file name (unk_0B9), writes "/<name>/<name>" into the path (unk_0F9) and calls func_002C48C0 with state 54.
 *
 * States 54-59 then delete the data file, the icon files, icon.sys and finally the directory (sceMcDelete each).
 * cPS2MemCard_Update calls it in state 19, which cPS2MemCard_DeleteFileSlot__FPvi requests.
 */
extern "C" void func_002C4410(cPS2MemCard* self)
{
    char* name = (char*)self + 0xB9;
    self->unk_03C = 0;
    strcpy(name, (char*)self + self->unk_01C * 0x78 + 0x1B0);
    sprintf((char*)self + 0xF9, str_pct_s_pct_s_h, name, name);
    func_002C48C0(self, 54);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_CheckDevice);
#ifdef SKIP_ASM
class cPS2MemCard_CheckDevice_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual int IsBusy(); // 0x58
};

/**
 * Selects a memory-card port and starts checking what is in it.
 *
 * Stores @p device in mDevice; when the card is not busy (virtual slot 11, cPS2MemCard_IsBusy) also stores it in unk_010 and
 * calls func_002C48C0 with state 3, which starts a sceMcGetInfo on the port (func_002C50E0). Update (cPS2MemCard_Update)
 * records the result in the device record.
 *
 * @param device  the port to use (0 or 1)
 */
extern "C" void cPS2MemCard_CheckDevice(cPS2MemCard* self, int device)
{
    self->mDevice = device;
    if (((cPS2MemCard_CheckDevice_cObj*)self)->IsBusy() == 0) {
        self->unk_010 = device;
        func_002C48C0(self, 3);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_ClearCardChanged);
#ifdef SKIP_ASM


/**
 * Clears the current port's card-changed flag (unk_10 of unk_17C[mDevice]) and turns a last libmc result (unk_08) of -1
 * into 0. Card vtable slot 47.
 */
extern "C" void cPS2MemCard_ClearCardChanged(cPS2MemCard* self)
{
    self->unk_17C[self->mDevice].unk_10 = 0;
    if (self->unk_17C[self->mDevice].unk_08 == -1) {
        self->unk_17C[self->mDevice].unk_08 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_CreateFileSlot);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_slash_pct_s[];

/**
 * Starts creating a save directory: copies @p name into the file name (unk_0B9), writes "/<name>" into the path
 * (unk_0F9) and calls func_002C48C0 with state 20.
 *
 * cPS2MemCard_Update then, once no error is pending, runs state 36 (sceMcMkdir of the path) and the icon.sys and icon
 * file writes that follow (states 37-51).
 *
 * @param name  the directory name (ASCII; no length check)
 */
extern "C" void cPS2MemCard_CreateFileSlot(cPS2MemCard* self, const char* name)
{
    char* fileName = (char*)self + 0xB9;
    strcpy(fileName, name);
    sprintf((char*)self + 0xF9, str_slash_pct_s, fileName);
    func_002C48C0(self, 20);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_CreateFileSlot1);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_slash_pct_s[];
// PORT: strcpy2 takes (dst, src); the unit declares it with one arg
char* strcpy2_impl(char* dst, unsigned short* src) __asm__("strcpy2");

/**
 * Starts creating a save directory from a 16-bit name: converts @p name to ASCII into the file name (unk_0B9, strcpy2),
 * writes "/<name>" into the path (unk_0F9) and calls func_002C48C0 with state 20, like cPS2MemCard_CreateFileSlot.
 *
 * @param name  the directory name (16-bit characters)
 */
extern "C" void cPS2MemCard_CreateFileSlot1(cPS2MemCard* self, unsigned short* name)
{
    char* fileName = (char*)self + 0xB9;
    strcpy2_impl(fileName, name);
    sprintf((char*)self + 0xF9, str_slash_pct_s, fileName);
    func_002C48C0(self, 20);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_StartFileSlotWrite1);
#ifdef SKIP_ASM
// PORT: strcpy2 takes (dst, src); the unit declares it with one arg
char* strcpy2_impl(char* dst, unsigned short* src) __asm__("strcpy2");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_002C48C0(cPS2MemCard*, int);
extern char str_pct_s_pct_s_h[];

/**
 * Starts writing a save file named by a 16-bit string: converts @p wname to ASCII into the file name (unk_0B9,
 * strcpy2), then does what cPS2MemCard_StartFileSlotWrite does (path "/<name>/<name>", buffer, size, state 21).
 *
 * @param wname  the save directory and file name (16-bit characters)
 * @param writeBuffer  the data to write
 * @param writeSize  the number of bytes to write
 */
extern "C" void cPS2MemCard_StartFileSlotWrite1(cPS2MemCard* self, unsigned short* wname, int writeBuffer, int writeSize)
{
    char* name = (char*)self + 0xB9;
    strcpy2_impl(name, wname);
    sprintf((char*)self + 0xF9, str_pct_s_pct_s_h, name, name);
    self->unk_02C = writeBuffer;
    self->unk_020 = writeSize;
    func_002C48C0(self, 21);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_StartFileSlotWrite);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_002C48C0(cPS2MemCard*, int);
extern char str_pct_s_pct_s_h[];

/**
 * Starts writing a save file: copies @p src into the file name (unk_0B9), writes "/<name>/<name>" into the path
 * (unk_0F9), stores the buffer in unk_02C and the size in unk_020 and calls func_002C48C0 with state 21.
 *
 * cPS2MemCard_Update then, once no error is pending, opens the path for writing (state 22, sceMcOpen mode 0x203) and
 * writes the buffer (state 23, sceMcWrite).
 *
 * @param src  the save directory and file name (ASCII; no length check)
 * @param writeBuffer  the data to write
 * @param writeSize  the number of bytes to write
 */
extern "C" void cPS2MemCard_StartFileSlotWrite(cPS2MemCard* self, const char* src, int writeBuffer, int writeSize)
{
    char* name = (char*)self + 0xB9;
    strcpy(name, src);
    sprintf((char*)self + 0xF9, str_pct_s_pct_s_h, name, name);
    self->unk_02C = writeBuffer;
    self->unk_020 = writeSize;
    func_002C48C0(self, 21);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_StartFileSlotWrite2);
#ifdef SKIP_ASM
class cPS2MemCard_StartFileSlotWrite2_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void StartFileSlotWrite(char* name, int a2, int a3); // 0x120
};

extern "C" void func_002C54D0(cPS2MemCard* self, int a1, char* dst);

/**
 * Starts writing a save file into an existing file slot: copies the slot's name into a 64-byte stack buffer
 * (func_002C54D0) and passes it, @p writeBuffer and @p writeSize to the card's virtual slot 36 (cPS2MemCard_StartFileSlotWrite).
 *
 * @param fileSlot  the file slot index (0 .. mMaxFiles - 1; not checked)
 * @param writeBuffer  the data to write
 * @param writeSize  the number of bytes to write
 */
extern "C" void cPS2MemCard_StartFileSlotWrite2(cPS2MemCard* self, int fileSlot, int writeBuffer, int writeSize)
{
    char name[64];
    func_002C54D0(self, fileSlot, name);
    ((cPS2MemCard_StartFileSlotWrite2_cObj*)self)->StartFileSlotWrite(name, writeBuffer, writeSize);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_WriteData__FPvii);
#ifdef SKIP_ASM
/**
 * Starts writing more data to the open save file: stores the buffer in unk_02C and the size in unk_020 and calls
 * func_002C48C0 with state 23, which issues sceMcWrite on mFileDescriptor. Card vtable slot 41.
 *
 * @param buffer  the data to write
 * @param transferSize  the number of bytes to write
 * @return func_002C48C0's return register (it returns nothing)
 */
void* cPS2MemCard_WriteData(void* vself, int buffer, int transferSize)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_02C = buffer;
    self->unk_020 = transferSize;
    return func_002C48C0((cPS2MemCard*)self, 23);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsWriteWaiting);
#ifdef SKIP_ASM
/**
 * Tests whether a file write is waiting for the caller.
 *
 * @return 1 when mState is 52 (0x34), else 0
 */
extern "C" int cPS2MemCard_IsWriteWaiting(cPS2MemCard* self)
{
    return self->mState == 52;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetWriteFailed__FPv);
#ifdef SKIP_ASM
/**
 * Marks the current write as failed: sets unk_048 to 1, and cPS2MemCard_Update then abandons the write by calling
 * func_002C48C0 with state 24, which closes the save file. On Tour's cMemCard::SetWriteFailed; card vtable slot 44.
 */
void cPS2MemCard_SetWriteFailed(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_048 = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetWriteClose__FPv);
#ifdef SKIP_ASM
/**
 * Closes the save file after writing: calls func_002C48C0 with state 24, which issues sceMcClose when mFileDescriptor
 * is open and otherwise goes on to state 25. On Tour's cMemCard::SetWriteClose; card vtable slot 43.
 *
 * @return func_002C48C0's return register (it returns nothing)
 */
void* cPS2MemCard_SetWriteClose(void* self)
{
    return func_002C48C0((cPS2MemCard*)self, 0x18);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4788);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_002C48C0(cPS2MemCard*, int);
extern char str_pct_s_pct_s_h[];

/**
 * Starts reading a save file: copies @p src into the file name (unk_0B9), writes "/<name>/<name>" into the path
 * (unk_0F9), stores the buffer in unk_02C and the size in unk_020 and calls func_002C48C0 with state 27.
 *
 * cPS2MemCard_Update then, once no error is pending, opens the path for reading (state 28, sceMcOpen mode 1) and reads
 * the buffer (state 29, sceMcRead); a read that returns fewer bytes than asked for ends in the error state with -10001.
 * cPS2MemCard_StartFileSlotRead calls it with a file slot's name.
 *
 * @param src  the save directory and file name (ASCII; no length check)
 * @param readBuffer  the buffer to read into
 * @param readSize  the number of bytes to read
 */
extern "C" void func_002C4788(cPS2MemCard* self, char* src, int readBuffer, int readSize)
{
    char* name = (char*)self + 0xB9;
    strcpy(name, src);
    sprintf((char*)self + 0xF9, str_pct_s_pct_s_h, name, name);
    self->unk_02C = readBuffer;
    self->unk_020 = readSize;
    func_002C48C0(self, 27);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_StartFileSlotRead);
#ifdef SKIP_ASM
extern "C" void func_002C54D0(cPS2MemCard* self, int a1, char* dst);
extern "C" void func_002C4788(cPS2MemCard* self, char* name, int a2, int a3);

/**
 * Starts loading the save in a file slot into a buffer.
 *
 * Copies the slot's directory name into a local buffer (func_002C54D0) and hands it to func_002C4788, which builds the
 * "<name>/<name>" path, records the buffer and size and requests the open/read states.
 *
 * @param fileSlot  the file slot to read
 * @param buffer  where the data goes
 * @param transferSize  number of bytes to read
 */
extern "C" void cPS2MemCard_StartFileSlotRead(cPS2MemCard* self, int fileSlot, int buffer, int transferSize)
{
    char name[64];
    func_002C54D0(self, fileSlot, name);
    func_002C4788(self, name, buffer, transferSize);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_ReadData__FPvii);
#ifdef SKIP_ASM
/**
 * Starts reading more data from the open save file: stores the buffer in unk_02C and the size in unk_020 and calls
 * func_002C48C0 with state 29, which issues sceMcRead on mFileDescriptor. Card vtable slot 30.
 *
 * @param buffer  the buffer to read into
 * @param transferSize  the number of bytes to read
 * @return func_002C48C0's return register (it returns nothing)
 */
void* cPS2MemCard_ReadData(void* vself, int buffer, int transferSize)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_02C = buffer;
    self->unk_020 = transferSize;
    return func_002C48C0((cPS2MemCard*)self, 29);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsReadWaiting);
#ifdef SKIP_ASM
/**
 * Tests whether a file read is waiting for the caller.
 *
 * @return 1 when mState is 53 (0x35), else 0
 */
extern "C" int cPS2MemCard_IsReadWaiting(cPS2MemCard* self)
{
    return self->mState == 53;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetReadFailed__FPv);
#ifdef SKIP_ASM
/**
 * Marks the current read as failed: sets unk_044 to 1, and cPS2MemCard_Update then abandons the read by calling
 * func_002C48C0 with state 30, which closes the save file. On Tour's cMemCard::SetReadFailed; card vtable slot 33.
 */
void cPS2MemCard_SetReadFailed(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_044 = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetReadClose__FPv);
#ifdef SKIP_ASM
/**
 * Closes the save file after reading: calls func_002C48C0 with state 30, which issues sceMcClose when mFileDescriptor
 * is open and otherwise goes on to state 31. On Tour's cMemCard::SetReadClose; card vtable slot 32.
 *
 * @return func_002C48C0's return register (it returns nothing)
 */
void* cPS2MemCard_SetReadClose(void* self)
{
    return func_002C48C0((cPS2MemCard*)self, 0x1e);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C48C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_free(void* p);
extern "C" void cMemCard_AsciiToShiftJIS(void* self, short* dst, const char* src);
extern "C" int func_002C50E0(cPS2MemCard* self);
extern "C" void func_002C5570(cPS2MemCard* self, int a1);
extern "C" int ASYNCFILE_load(char* path, int a1);
extern "C" void* MEM_fill(void* dst, int c, int n);
extern "C" void MEM_copy_K(void* dst, const void* src, int n) __asm__("MEM_copy");
extern "C" int sceMcOpen(int h, int a1, char* name, int a3);
extern "C" int sceMcMkdir(int h, int a1, char* name);
extern "C" int sceMcClose(int h);
extern "C" int sceMcRead(int h, int a1, int a2);
extern "C" int sceMcWrite(int h, int a1, int a2);
extern "C" int sceMcGetDir(int h, int a1, char* name, int a3, int a4, void* buf);
extern "C" int sceMcDelete(int h, int a1, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned int n);
extern int D_004A3940;
extern char str_pct_s_pct_s_h[];
extern char str_pct_s_pct_s_i[];
extern char str_PS2D[] __attribute__((aligned(4)));
extern char str_icon_sys[];
extern char* D_004459F0[];
extern char D_00445A00[];
extern char D_00445A40[];
extern char D_00445A70[];
extern char D_00445AA0[];

struct sIconSys_48C0 {
    char head[4];               // 0x000
    unsigned short reserv1;     // 0x004
    unsigned short offsLF;      // 0x006
    unsigned int reserv2;       // 0x008
    unsigned int transRate;     // 0x00C
    int bgColor[4][4];          // 0x010
    float lightDir[3][4];       // 0x050
    float lightColor[3][4];     // 0x080
    float ambient[4];           // 0x0B0
    short title[34];            // 0x0C0
    char fnameView[64];         // 0x104
    char fnameCopy[64];         // 0x144
    char fnameDel[64];          // 0x184
    char reserve3[512];         // 0x1C4
};
extern sIconSys_48C0 D_005376F8;

class cMemCard_48C0 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66();
    virtual int HasFileName(int idx); // 0x218
};

static inline int slotOf_48C0(int state, int a, int b)
{
    return state == a ? 1 : (state == b ? 2 : 0);
}

#define S_48C0(off) ((char*)self + (off))
#define I_48C0(off) (*(int*)((char*)self + (off)))

/**
 * Moves the memory-card driver to a new state and issues the libmc request that state starts.
 *
 * Stores @p state in mState. Each sceMc* result goes to D_004A3940, and a negative result (for opens and reads a
 * nonzero one) moves to the error state 1 by a recursive call. State 1 records the result in the current port's record
 * (unk_17C[mDevice].unk_08), frees the icon data (unk_030), closes an open file (then state 2) and clears the transfer
 * fields; 3 starts sceMcGetInfo (func_002C50E0); 7 finds the next named file slot and opens its icon.sys (8), whose
 * read (10), close (9) and directory reads of the slot's files (11-14) fill the file list (a failed read marks the slot
 * with func_002C5570); 22/23 open and write the save file, 28/29 open and read it, 24 and 30 close it; 36-51 create the
 * save directory and write icon.sys (title from unk_058 through cMemCard_AsciiToShiftJIS, colours and lights from
 * D_00445A00..D_00445AA0) and the icon files (loaded asynchronously with ASYNCFILE_load from mSourceIconPath); 54-59
 * delete a save's files and directory. Waiting states return without a call; any other state starts sceMcGetInfo when
 * none is pending (unk_040 clear). cPS2MemCard_Update polls sceMcSync and picks the next state.
 *
 * @param vself  the cPS2MemCard
 * @param state  the new state
 * @return nothing; the return register holds whatever the last call left there
 */
extern "C" void* func_002C48C0(cPS2MemCard* vself, int state)
{
    cMemCard_48C0* self = (cMemCard_48C0*)vself;
    I_48C0(0x4) = state;
    switch (state) {
    case 1: {
        *(int*)((char*)self + I_48C0(0xC) * 0x14 + 0x184) = D_004A3940;
        if (I_48C0(0x30) != 0) {
            MEM_free(*(void**)S_48C0(0x30));
            I_48C0(0x30) = 0;
        }
        if (I_48C0(0x18) >= 0) {
            int mcResult = sceMcClose(I_48C0(0x18));
            D_004A3940 = mcResult;
            if (mcResult >= 0) {
                func_002C48C0((cPS2MemCard*)self, 2);
            }
            I_48C0(0x18) = -1;
        }
        I_48C0(44) = 0;
        I_48C0(0x1C) = -1;
        I_48C0(0x20) = 0;
        break;
    }
    case 3:
        func_002C50E0((cPS2MemCard*)self);
        break;
    case 36: {
        I_48C0(72) = 0;
        int mcResult = sceMcMkdir(I_48C0(0xC), 0, S_48C0(249));
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 37: {
        char* path = S_48C0(121);
        sprintf(path, str_pct_s_pct_s_i, S_48C0(249), str_icon_sys);
        int mcResult = sceMcOpen(I_48C0(0xC), 0, path, 515);
        D_004A3940 = mcResult;
        I_48C0(0x18) = -1;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 38: {
        char* title = S_48C0(88);
        MEM_fill(&D_005376F8, 0, 964);
        memcpy(&D_005376F8, str_PS2D, 5);
        cMemCard_AsciiToShiftJIS(self, D_005376F8.title, title);
        if (I_48C0(0x8) > 0)
            D_005376F8.offsLF = I_48C0(0x8);
        else
            D_005376F8.offsLF = strlen(title) * 2;
        D_005376F8.transRate = 0x60;
        MEM_copy_K(D_005376F8.bgColor, D_00445A00, 0x40);
        MEM_copy_K(D_005376F8.lightDir, D_00445A40, 0x30);
        MEM_copy_K(D_005376F8.lightColor, D_00445A70, 0x30);
        MEM_copy_K(D_005376F8.ambient, D_00445AA0, 0x10);
        strcpy(D_005376F8.fnameView, D_004459F0[0]);
        strcpy(D_005376F8.fnameCopy, D_004459F0[1]);
        strcpy(D_005376F8.fnameDel, D_004459F0[2]);
        int mcResult = sceMcWrite(I_48C0(0x18), (int)&D_005376F8, 964);
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 39: {
        int mcResult = sceMcClose(I_48C0(0x18));
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C48C0((cPS2MemCard*)self, 1);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 40: case 44: case 0x30: {
        char path[64];
        if (I_48C0(0x30) != 0) {
            MEM_free(*(void**)S_48C0(0x30));
            I_48C0(0x30) = 0;
        }
        int slot = slotOf_48C0(I_48C0(0x4), 44, 0x30);
        sprintf(path, str_pct_s_pct_s_i, S_48C0(5312), D_004459F0[slot]);
        I_48C0(0x38) = ASYNCFILE_load(path, 0x100);
        break;
    }
    case 41: case 45: case 49: {
        int slot = slotOf_48C0(I_48C0(0x4), 45, 49);
        char* path = S_48C0(121);
        sprintf(path, str_pct_s_pct_s_h, S_48C0(185), D_004459F0[slot]);
        int mcResult = sceMcOpen(I_48C0(0xC), 0, path, 515);
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C48C0((cPS2MemCard*)self, 1);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 42: case 46: case 50: {
        int mcResult = sceMcWrite(I_48C0(0x18), I_48C0(0x30), I_48C0(52));
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 43: case 47: case 51: {
        if (I_48C0(0x30) != 0) {
            MEM_free(*(void**)S_48C0(0x30));
            I_48C0(0x30) = 0;
        }
        int mcResult = sceMcClose(I_48C0(0x18));
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C48C0((cPS2MemCard*)self, 1);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 22: {
        int mcResult = sceMcOpen(I_48C0(0xC), 0, S_48C0(249), 515);
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C48C0((cPS2MemCard*)self, 1);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 23: {
        int mcResult = sceMcWrite(I_48C0(0x18), I_48C0(44), I_48C0(0x20));
        D_004A3940 = mcResult;
        I_48C0(44) = 0;
        I_48C0(0x20) = 0;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 0x18:
        if (I_48C0(0x18) >= 0) {
            int mcResult = sceMcClose(I_48C0(0x18));
            D_004A3940 = mcResult;
            if (mcResult < 0) {
                I_48C0(0x18) = -1;
                func_002C48C0((cPS2MemCard*)self, 1);
            } else {
                I_48C0(0x18) = -1;
            }
        } else {
            func_002C48C0((cPS2MemCard*)self, 25);
        }
        break;
    case 0x1C: {
        I_48C0(68) = 0;
        int mcResult = sceMcOpen(I_48C0(0xC), 0, S_48C0(249), 1);
        D_004A3940 = mcResult;
        I_48C0(0x18) = -1;
        if (mcResult != 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 29: {
        int mcResult = sceMcRead(I_48C0(0x18), I_48C0(44), I_48C0(0x20));
        D_004A3940 = mcResult;
        if (mcResult != 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 0x1E:
        if (I_48C0(0x18) >= 0) {
            int mcResult = sceMcClose(I_48C0(0x18));
            D_004A3940 = mcResult;
            if (mcResult < 0) {
                I_48C0(0x18) = -1;
                func_002C48C0((cPS2MemCard*)self, 1);
            } else {
                I_48C0(0x18) = -1;
            }
        } else {
            func_002C48C0((cPS2MemCard*)self, 0x1F);
        }
        break;
    case 54: {
        *(int*)((char*)self + I_48C0(0xC) * 0x14 + 0x184) = 0;
        int mcResult = sceMcDelete(I_48C0(0xC), 0, S_48C0(249));
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 55: case 0x38: case 57: {
        int slot = slotOf_48C0(I_48C0(0x4), 0x38, 57);
        char* path = S_48C0(121);
        sprintf(path, str_pct_s_pct_s_h, S_48C0(185), D_004459F0[slot]);
        int mcResult = sceMcDelete(I_48C0(0xC), 0, path);
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 58: {
        char* path = S_48C0(121);
        sprintf(path, str_pct_s_pct_s_h, S_48C0(185), str_icon_sys);
        int mcResult = sceMcDelete(I_48C0(0xC), 0, path);
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 59: {
        int mcResult = sceMcDelete(I_48C0(0xC), 0, S_48C0(185));
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 1);
        break;
    }
    case 7:
        for (I_48C0(40) = 0; I_48C0(40) < I_48C0(20); I_48C0(40)++) {
            if (self->HasFileName(I_48C0(40))) {
                sprintf(S_48C0(121), str_pct_s_pct_s_h, (char*)self + I_48C0(0x28) * 0x78 + 0x1B0, str_icon_sys);
                func_002C48C0((cPS2MemCard*)self, 8);
                break;
            }
        }
        break;
    case 8: {
        int mcResult = sceMcOpen(I_48C0(0xC), 0, S_48C0(121), 1);
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C5570((cPS2MemCard*)self, I_48C0(40));
            func_002C48C0((cPS2MemCard*)self, 0x10);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 10: {
        int mcResult = sceMcRead(I_48C0(0x18), (int)&D_005376F8, 964);
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            func_002C5570((cPS2MemCard*)self, I_48C0(40));
            func_002C48C0((cPS2MemCard*)self, 0xF);
        }
        break;
    }
    case 9: {
        int mcResult = sceMcClose(I_48C0(0x18));
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            func_002C48C0((cPS2MemCard*)self, 0x10);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 11: {
        char* path = S_48C0(249);
        I_48C0(0x3C) = 0;
        char* buf = S_48C0(1152);
        char* name = (char*)self + I_48C0(0x28) * 0x78 + 0x1B0;
        sprintf(path, str_pct_s_pct_s_h, name, name);
        MEM_fill(buf, 0, 0x40);
        int mcResult = sceMcGetDir(I_48C0(0xC), 0, path, 0, 1, buf);
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 0x10);
        break;
    }
    case 0xC: case 13: case 0xE: {
        int slot = slotOf_48C0(I_48C0(0x4), 13, 0xE);
        char* path = S_48C0(121);
        char* buf = S_48C0(1152);
        sprintf(path, str_pct_s_pct_s_h, (char*)self + I_48C0(0x28) * 0x78 + 0x1B0, D_004459F0[slot]);
        MEM_fill(buf, 0, 0x40);
        int mcResult = sceMcGetDir(I_48C0(0xC), 0, path, 0, 1, buf);
        D_004A3940 = mcResult;
        if (mcResult < 0)
            func_002C48C0((cPS2MemCard*)self, 0x10);
        break;
    }
    case 0xF: {
        int mcResult = sceMcClose(I_48C0(0x18));
        D_004A3940 = mcResult;
        if (mcResult < 0) {
            I_48C0(0x18) = -1;
            func_002C48C0((cPS2MemCard*)self, 0x10);
        } else {
            I_48C0(0x18) = -1;
        }
        break;
    }
    case 2: case 4: case 6: case 0x10: case 18: case 21: case 25: case 27: case 0x1F:
    case 35: case 52: case 53: case 61: case 0x40: case 66: case 68: case 70:
    case 72: case 74: case 76: case 78:
        break;
    default:
        if (I_48C0(0x40) == 0)
            func_002C50E0((cPS2MemCard*)self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C50E0);
#ifdef SKIP_ASM
extern "C" int sceMcGetInfo(int h, int a1, int* a2, int* a3, int* a4);
extern int D_004A3938;
extern int D_004A393C;
extern int D_004A3944;

/**
 * Starts a sceMcGetInfo on the current port: clears unk_020 and the three result words (type D_004A3938, free clusters
 * D_004A393C, format D_004A3944), calls sceMcGetInfo on mDevice, stores whether the call was accepted in unk_040 (the
 * pending flag cPS2MemCard_Update polls) and sets unk_04C to 5.
 *
 * @return 1 when the request was issued, else 0
 */
extern "C" int func_002C50E0(cPS2MemCard* self)
{
    D_004A3938 = 0;
    self->unk_020 = 0;
    int devicePort = self->mDevice;
    D_004A3944 = 0;
    D_004A393C = 0;
    int syncPending = sceMcGetInfo(devicePort, 0, &D_004A3938, &D_004A393C, &D_004A3944) == 0;
    self->unk_040 = syncPending;
    self->unk_04C = 5;
    return syncPending;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsBusy);
#ifdef SKIP_ASM
/**
 * Tests whether the card object is in the middle of an operation.
 *
 * @return 1 when mState is not idle (0) or a sceMcGetInfo is still pending (unk_040), else 0
 */
extern "C" int cPS2MemCard_IsBusy(cPS2MemCard* self)
{
    if (self->mState != 0 || self->unk_040 != 0) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_DidFormatFail);
#ifdef SKIP_ASM
/**
 * Tests whether the last card operation in the current port failed.
 *
 * Looks at the device record's result word (unk_17C[mDevice].unk_08, the mapped sceMcSync result): -1, -3, -8, -9, -10,
 * -10001 and -10002 count as a failure. Virtual slot 9, declared right after Format; the test itself is not specific to
 * formatting.
 *
 * @return 1 when the last result is one of those codes, else 0
 */
extern "C" int cPS2MemCard_DidFormatFail(cPS2MemCard* self)
{
    switch (((&self->unk_17C[0]))[self->mDevice].unk_08) {
    case -1:
    case -3:
    case -8:
    case -9:
    case -10:
    case -10001:
    case -10002:
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetLastError);
#ifdef SKIP_ASM
// PORT: the unit declares this as `extern "C" void* cPS2MemCard_GetLastError(void*, int)`
// for its callers; the body returns an int code, bound with an asm label.
extern "C" int cPS2MemCard_GetLastError_impl(cPS2MemCard* self, int i) __asm__("cPS2MemCard_GetLastError");

/**
 * Returns the last libmc result of a port (unk_08 of device record unk_17C[@p i]) when it is one of the codes the
 * driver knows (0, -1 .. -7, -10001, -10002), and -10 for anything else (-8, -9, -10, other negatives, positives).
 *
 * cPS2MemCard_Update resets the port's info (func_002C5250) on -10 or -1 and the directory data (func_002C5278) on -10
 * or -2.
 *
 * @param i  the memory-card port (0 or 1)
 * @return the result code, or -10
 */
extern "C" int cPS2MemCard_GetLastError_impl(cPS2MemCard* self, int i)
{
    switch (self->unk_17C[i].unk_08) {
    case -10002:
    case -10001:
    case -7:
    case -6:
    case -5:
    case -4:
    case -3:
    case -2:
    case -1:
    case 0:
        return self->unk_17C[i].unk_08;
    case -10:
    case -9:
    case -8:
    default:
        return -10;
    }
}
#endif

extern "C" void* cPS2MemCard_GetLastError(void*, int);

//99.38%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_DidErrorOccur__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether the current port (mDevice) has an error: cPS2MemCard_GetLastError for it is nonzero. Card vtable slot 73;
 * cPS2MemCard_Update checks it before starting a create, write or read.
 *
 * @return 1 when cPS2MemCard_GetLastError returns nonzero, else 0
 */
int cPS2MemCard_DidErrorOccur(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    return (cPS2MemCard_GetLastError(self, self->mDevice) != 0);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5250);
#ifdef SKIP_ASM
/**
 * Clears the current port's card type (unk_00) and free-cluster count (unk_04) in unk_17C[mDevice].
 */
extern "C" void func_002C5250(cPS2MemCard* self)
{
    ((&self->unk_17C[0]))[self->mDevice].unk_00 = 0;
    (*(void**)&((&self->unk_17C[0]))[self->mDevice].unk_04) = 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5278);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int c, int n);
extern "C" void func_002C52D8(cPS2MemCard* self);

/**
 * Empties the directory data: clears unk_050 and mNumDirTable, the single directory entry unk_480 (0x40 bytes), the six
 * file slots unk_1A4 (720 bytes) and the directory table unk_4C0 (0x1000 bytes), then calls func_002C52D8 (which clears
 * unk_480 again).
 */
extern "C" void func_002C5278(cPS2MemCard* self)
{
    self->unk_050 = 0;
    self->mNumDirTable = 0;
    MEM_fill(&self->unk_480, 0, 0x40);
    MEM_fill(&self->unk_1A4[0], 0, 720);
    MEM_fill(&self->unk_4C0[0], 0, 0x1000);
    func_002C52D8(self);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C52D8);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int c, int n);

/**
 * Clears the single directory entry unk_480 (0x40 bytes).
 */
extern "C" void func_002C52D8(cPS2MemCard* self)
{
    MEM_fill(&self->unk_480, 0, 0x40);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsInvalid);
#ifdef SKIP_ASM
/**
 * Returns whether the last libmc result of a port (unk_08 of device record unk_17C[@p device]) is -10002, the code
 * cPS2MemCard_Update stores for a card that is not a PS2 memory card.
 *
 * @param device  the memory-card port (0 or 1)
 * @return 1 for a non-PS2 card, else 0
 */
extern "C" int cPS2MemCard_IsInvalid(cPS2MemCard* self, int device)
{
    char* devicePtr = (char*)self + device * 0x14;
    return *(int*)(devicePtr + 0x184) == -10002;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetFreeSpace1);
#ifdef SKIP_ASM
/**
 * Returns the free space on the card in a port: the free-cluster count (unk_04) sceMcGetInfo stored in device record
 * unk_17C[@p device].
 *
 * @param device  the memory-card port (0 or 1)
 * @return the free clusters (1 KB each on a PS2 card)
 */
extern "C" void* cPS2MemCard_GetFreeSpace1(cPS2MemCard* self, int device)
{
    return (void*)self->unk_17C[device].unk_04;
}
#endif

//99.29%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetFreeSpace__FPv);
#ifdef SKIP_ASM
/**
 * Returns the free space on the card in the current port.
 *
 * Calls cPS2MemCard_GetFreeSpace1 with mDevice, which returns the free-cluster count sceMcGetInfo reported
 * (unk_17C[mDevice].unk_04; 1 KB clusters).
 *
 * @return the free space in KB
 */
void* cPS2MemCard_GetFreeSpace(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    return cPS2MemCard_GetFreeSpace1((cPS2MemCard*)self, self->mDevice);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsAvailable);
#ifdef SKIP_ASM
/**
 * Returns whether the last libmc result of a port (unk_08 of device record unk_17C[@p i]) is one that leaves a card
 * usable: 0, -1 to -5, -7 or -10001. Any other code (-6, -8 .. -10, -10002, positive values) gives 0.
 *
 * @param i  the memory-card port (0 or 1)
 * @return 1 for a usable code, else 0
 */
extern "C" int cPS2MemCard_IsAvailable(cPS2MemCard* self, int i)
{
    switch (((&self->unk_17C[0]))[i].unk_08) {
    case -10001:
    case -7:
    case -5:
    case -4:
    case -3:
    case -2:
    case -1:
    case 0:
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsFormatted);
#ifdef SKIP_ASM
/**
 * Returns whether the card in a port is formatted: the format word (unk_0C) sceMcGetInfo stored in device record
 * unk_17C[@p device] is nonzero.
 *
 * @param device  the memory-card port (0 or 1)
 * @return 1 when formatted, else 0
 */
extern "C" int cPS2MemCard_IsFormatted(cPS2MemCard* self, int device)
{
    char* devicePtr = (char*)self + device * 0x14;
    return *(int*)(devicePtr + 0x188) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_CardChanged);
#ifdef SKIP_ASM
/**
 * Returns whether the card in a port has changed: the card-changed flag (unk_10) of device record unk_17C[@p device],
 * which cPS2MemCard_Update sets.
 *
 * @param device  the memory-card port (0 or 1)
 * @return 1 when the flag is set, else 0
 */
extern "C" int cPS2MemCard_CardChanged(cPS2MemCard* self, int device)
{
    char* devicePtr = (char*)self + device * 0x14;
    return *(int*)(devicePtr + 0x18c) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetStatus);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src);
extern "C" int sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
// PORT: the unit defines cPS2MemCard_GetFreeSpace1 as returning void*; it returns an int count here.
extern "C" int cPS2MemCard_GetFreeSpace1_i(void* self, int idx) __asm__("cPS2MemCard_GetFreeSpace1");
extern char str_sp_d[];
extern char str_kT_MEMFreePS2[];

struct sQTEntry_53E0 {
    int count;
    int status;
    void* data;
    int fC;
    int f10;
};

struct sQT_53E0 {
    char pad[0x180];
    sQTEntry_53E0 entries[1];
};

struct cPS2MemCard_GetStatus_sVEntry {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

/**
 * Writes a status line for a port into a 16-bit buffer.
 *
 * When the port's format word (unk_0C of unk_17C[@p idx]) is 0 or its last result (unk_08) is not 0, the line is a
 * single space (str_sp_d, widened by func_002C2540). For a result of 0 it is the front-end locale's "kT_MEMFreePS2"
 * string (looked up through slot 4 of SSXApp->FELocale's vtable) formatted by sprintf1 with the free clusters
 * (cPS2MemCard_GetFreeSpace1, at least 0).
 *
 * @param idx  the memory-card port (0 or 1)
 * @param dst  the 16-bit output buffer
 */
extern "C" void cPS2MemCard_GetStatus(sQT_53E0* self, int idx, unsigned short* dst)
{
    if (self->entries[idx].data == 0) {
        func_002C2540(dst, str_sp_d);
        return;
    }
    switch (self->entries[idx].status) {
    case -2:
        func_002C2540(dst, str_sp_d);
        break;
    case 0: {
        // PORT: g++ `>?` (max) operator, removed in GCC 4.3.
        int freeClusters = cPS2MemCard_GetFreeSpace1_i(self, idx) >? 0;
        char* obj = (char*)SSXApp->FELocale;
        cPS2MemCard_GetStatus_sVEntry* vt = *(cPS2MemCard_GetStatus_sVEntry**)(obj + 4);
        char* thisp = obj + vt[4].delta;
        sprintf1(dst, vt[4].fn(thisp, GetHashValue32(str_kT_MEMFreePS2)), freeClusters);
        break;
    }
    default:
        func_002C2540(dst, str_sp_d);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C54D0);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Copies the name of a file slot (unk_1A4[@p fileSlot].unk_0C) into @p dst with strcpy.
 *
 * @param fileSlot  the file slot index (0 .. mMaxFiles - 1; not checked)
 * @param dst  the output buffer (64 bytes is enough)
 */
extern "C" void func_002C54D0(cPS2MemCard* self, int fileSlot, char* dst)
{
    char* filePtr = (char*)self + fileSlot * 0x78;
    strcpy(dst, filePtr + 0x1b0);
}
#endif

extern "C" void* strcpy2(void*);

//99.29%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetTitleString__FPv);
#ifdef SKIP_ASM
/**
 * Sets the save title (unk_058): converts the 16-bit string the caller passes in the second argument register to ASCII
 * into it with strcpy2 (no length check against its 0x21 bytes). Card vtable slot 45.
 *
 * @return strcpy2's result
 */
void* cPS2MemCard_SetTitleString(void* vself)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    return strcpy2((char*)self + 0x58);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5520__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (card vtable slot 46).
 */
void func_002C5520(void* self)
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_HasFileName);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

/**
 * Tests whether a file slot has a directory name (holds a save).
 *
 * @param fileSlot  the file slot
 * @return true when unk_1A4[@p fileSlot].unk_0C is not an empty string
 */
extern "C" bool cPS2MemCard_HasFileName(cPS2MemCard* self, int fileSlot)
{
    char* filePtr = (char*)self + fileSlot * 0x78;
    return strlen(filePtr + 0x1b0) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetBrokenFile);
#ifdef SKIP_ASM
/**
 * Marks a file slot as broken (corrupt).
 *
 * Sets unk_1A4[@p fileSlot].unk_00 to 1; cPS2MemCard_GetTitleString then shows the slot as corrupt and cPS2MemCard_GetLastSavedFile skips it.
 *
 * @param fileSlot  the file slot
 */
extern "C" void cPS2MemCard_SetBrokenFile(cPS2MemCard* self, int fileSlot)
{
    char* filePtr = (char*)self + fileSlot * 0x78;
    *(int*)(filePtr + 0x1a4) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5570);
#ifdef SKIP_ASM
/**
 * Marks a file slot as unreadable: sets unk_04 of unk_1A4[@p fileSlot] to 1. func_002C48C0 and cPS2MemCard_Update call
 * it when the slot's icon.sys cannot be opened or read.
 *
 * @param fileSlot  the file slot index
 */
extern "C" void func_002C5570(cPS2MemCard* self, int fileSlot)
{
    char* filePtr = (char*)self + fileSlot * 0x78;
    *(int*)(filePtr + 0x1a8) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsFileSlotBroken);
#ifdef SKIP_ASM
struct sQTSlot {
    int a;
    int b;
    char pad08[0x70];
};

struct sQTSlots {
    char pad000[0x1A4];
    sQTSlot slots[1];
};

/**
 * Returns whether a file slot is flagged: its unk_00 (set by cPS2MemCard_SetBrokenFile) or unk_04 (set by
 * func_002C5570) is nonzero. Card vtable slot 65.
 *
 * @param i  the file slot index (0 .. mMaxFiles - 1; not checked)
 * @return 1 when either flag is set, else 0
 */
extern "C" int cPS2MemCard_IsFileSlotBroken(sQTSlots* self, int i)
{
    if (self->slots[i].b != 0 || self->slots[i].a != 0) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsFileSlotDisabled);
#ifdef SKIP_ASM
/**
 * Tests whether a file slot is disabled.
 *
 * Tests unk_1A4[@p fileSlot].unk_08; cPS2MemCard_GetTitleString shows such a slot as kT_MEMNoFreeSpace.
 *
 * @param fileSlot  the file slot
 * @return 1 when the flag is set, else 0
 */
extern "C" int cPS2MemCard_IsFileSlotDisabled(cPS2MemCard* self, int fileSlot)
{
    char* filePtr = (char*)self + fileSlot * 0x78;
    return *(int*)(filePtr + 0x1ac) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_Update);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sMCPort_55D8 {
    int a;
    int b;
    int status;
    int d;
    int e;
};
struct sMCEnt_55D8 {
    int f0;
    int f4;
    int f8;
    char name[0x40];
    char x4C[0x22];
    char x6E[8];
    char pad76[2];
};
struct sMC_55D8 {
    char* vt;
    int state;
    int x08;
    int port;
    int port2;
    int count;
    int x18;
    int x1C;
    int size;
    int x24;
    int index;
    int x2C;
    int x30;
    int x34;
    int handle;
    int x3C;
    int busy;
    int x44;
    int x48;
    int x4C;
    int x50;
    int x54;
    char name[0x21];
    char path[0x40];
    char dir[0x40];
    char padF9[0x83];
    sMCPort_55D8 ports[2];
    sMCEnt_55D8 entries[6];
    char pad474[0x14];
    char x488[8];
    int x490;
    char pad494[0x4C];
    char dirbuf[0x40][0x40];
};
typedef int (*fn1_55D8)(void*);
typedef int (*fn2_55D8)(void*, int);
typedef void (*fn2v_55D8)(void*, int);
// PORT: the unit declares these later with other signatures; bound by asm label.
extern "C" int sceMcSync_i(int a0, int* a1, int* out) __asm__("sceMcSync");
extern "C" int cPS2MemCard_GetLastError_i(void* self, int i) __asm__("cPS2MemCard_GetLastError");
extern "C" void func_002C6280_v(void* self) __asm__("func_002C6280");
extern "C" int func_002C63E8(cPS2MemCard* self, unsigned int a1);
extern "C" int ASYNCFILE_getstatus(int h);
extern "C" void ASYNCFILE_release(int h, int* a, int* b);
extern "C" void func_002C4210(cPS2MemCard* self);
extern "C" void func_002C42B0(cPS2MemCard* self);
extern "C" void func_002C4388(cPS2MemCard* self);
extern "C" void func_002C4410(cPS2MemCard* self);
extern "C" void func_002C6848(cPS2MemCard* self);
extern "C" void func_002C69A0(cPS2MemCard* self);
extern "C" void func_002C6A28(cPS2MemCard* self);
extern "C" void func_002C6A88(cPS2MemCard* self);
extern "C" void func_002C6B00(cPS2MemCard* self);
extern "C" void func_002C6B78(cPS2MemCard* self);
extern "C" void func_002C6BF8(cPS2MemCard* self);
extern "C" void func_002C6C88(cPS2MemCard* self);
extern "C" void func_002C6D18(cPS2MemCard* self);
extern int D_00487450[];
// PORT: void-returning alias of MEM_fill (memset); the unused result changes register choice in case 6.
extern "C" void MEM_fill_v(void* dst, int c, int n) __asm__("MEM_fill");
extern short D_005377B8[];

#define VC1_55D8(s, o) (((fn1_55D8)*(void**)((s)->vt + (o) + 4))((char*)(s) + *(short*)((s)->vt + (o))))
#define VC2v_55D8(s, o, x) (((fn2v_55D8)*(void**)((s)->vt + (o) + 4))((char*)(s) + *(short*)((s)->vt + (o)), x))
#define VC2_55D8(s, o, x) (((fn2_55D8)*(void**)((s)->vt + (o) + 4))((char*)(s) + *(short*)((s)->vt + (o)), x))

/**
 * Advances the memory-card state machine; called every frame.
 *
 * Empties the file slots and directory counts when the current device's format word is 0. If a sceMcGetInfo is pending
 * (unk_040) it polls sceMcSync (sceMcSync) without waiting, maps the result (ranges of libmc codes to -10, a non-
 * PS2 card to -10002, an unformatted one to -2, -1/-2 set the device's changed flag) and stores type, free clusters,
 * result and format word in the device record (unk_17C). Then, for a state that waits on a libmc call, polls sceMcSync
 * the same way and moves to the next state with func_002C48C0, or to the error state 1 on a failure; the request states
 * (5, 17, 19, 34...) call their handlers (func_002C4210, func_002C4388, func_002C4410, func_002C42B0...). Also finishes
 * the directory scan (copies names, titles and dates into the file slots) and the asynchronous icon file loads. unk_03C
 * is 1 while no call is outstanding.
 */
extern "C" void cPS2MemCard_Update(sMC_55D8* self)
{
    if (self->ports[self->port].d == 0) {
        self->x50 = 0;
        self->x54 = 0;
        MEM_fill(self->entries, 0, 720);
    }
    int one = 1;
    self->x3C = one;
    if (self->busy != 0) {
        int syncCmd;
        D_004A3940 = 0;
        syncCmd = 0;
        if (sceMcSync_i(1, &syncCmd, &D_004A3940) == 0) {
            return;
        }
        int syncResult = D_004A3940;
        if (syncResult == -1) {
            goto set;
        }
        if (syncResult == -2) {
        set:
            *(int*)((char*)self + self->port2 * 0x14 + 0x18C) = one;
            syncResult = D_004A3940;
        }
        if ((unsigned)(syncResult + 19) < 9 || (unsigned)(syncResult + 49) < 10 || (unsigned)(syncResult + 59) < 10 || (unsigned)(syncResult + 79) < 10) {
            D_004A3940 = -10;
        }
        if (func_002C63E8((cPS2MemCard*)self, D_004A3940) == 0) {
            if (D_004A3938 != 2) {
                D_004A3940 = -10002;
            } else if (D_004A3944 == 0) {
                D_004A3940 = -2;
            }
        }
        self->busy = 0;
        self->ports[self->port2].status = D_004A3940;
        self->ports[self->port2].a = D_004A3938;
        self->ports[self->port2].b = D_004A393C;
        self->ports[self->port2].d = D_004A3944;
    }
    switch (self->state) {
    case 2: case 6: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
    case 18: case 22: case 23: case 24: case 28: case 29: case 30: case 35: case 36: case 37:
    case 38: case 39: case 41: case 42: case 43: case 45: case 46: case 47: case 49: case 50:
    case 51: case 54: case 55: case 56: case 57: case 58: case 59: case 61: case 64: case 66:
    case 68: case 70: case 72: case 74: case 76: case 78:
        {
            self->x3C = 0;
            D_004A3940 = 0;
            if (sceMcSync_i(1, 0, &D_004A3940) == 0) {
                return;
            }
            self->x3C = 1;
            if (D_004A3940 == -1) {
                self->ports[self->port].e = 1;
            }
            int syncResult = D_004A3940;
            if (syncResult < 0) {
                int st = self->state;
                if (st != 36 && (unsigned)(st - 54) >= 6) {
                    if ((unsigned)(st - 8) < 8 && syncResult != -1 && func_002C63E8((cPS2MemCard*)self, syncResult) == 0) {
                        VC2v_55D8(self, 320, self->index);
                        func_002C48C0((cPS2MemCard*)self, 0x10);
                        return;
                    }
                } else {
                    if (syncResult == -4) {
                        D_004A3940 = 0;
                    }
                }
                if (D_004A3940 < 0) {
                    if (self->state == 35 || self->state == 7) {
                        func_002C52D8((cPS2MemCard*)self);
                    }
                    if (self->state == 19) {
                        self->x1C = -1;
                    }
                    func_002C48C0((cPS2MemCard*)self, 1);
                    return;
                }
            }
            self->ports[self->port].status = 0;
        }
        break;
    }
    if (self->x44 != 0) {
        self->x44 = 0;
        func_002C48C0((cPS2MemCard*)self, 0x1E);
        return;
    }
    if (self->x48 != 0) {
        self->x48 = 0;
        func_002C48C0((cPS2MemCard*)self, 0x18);
        return;
    }
    switch (self->state) {
    case 0:
    case 1:
    case 78:
        break;
    case 2:
        D_004A3940 = 0;
        if (sceMcSync_i(1, 0, &D_004A3940) == 0) {
            return;
        }
        if (D_004A3940 == -1) {
            self->ports[self->port].e = 1;
        }
        break;
    case 3:
        func_002C48C0((cPS2MemCard*)self, 4);
        break;
    case 5:
        if (VC1_55D8(self, 584) == 0) {
            func_002C4210((cPS2MemCard*)self);
            return;
        }
        break;
    case 34:
        if (VC1_55D8(self, 584) == 0) {
            func_002C42B0((cPS2MemCard*)self);
            return;
        }
        break;
    case 6:
        {
            int i;
            self->x50 = self->count;
            MEM_fill_v(self->entries, 0, 720);
            for (i = 0; i < self->count; i++) {
                unsigned char slotDigit = i + '1';
                int j;
                for (j = 0; j < self->x50; j++) {
                    int n = strlen(self->dirbuf[j]);
                    if (n != 0 && (unsigned char)self->dirbuf[j][n - 1] == slotDigit) {
                        strcpy(self->entries[i].name, self->dirbuf[j]);
                        break;
                    }
                }
            }
            D_004A3940 = 0;
            func_002C48C0((cPS2MemCard*)self, 7);
            return;
        }
    case 8:
        self->x18 = D_004A3940;
        func_002C48C0((cPS2MemCard*)self, 10);
        return;
    case 10:
        cMemCard_ShiftJISToAscii(self, self->entries[self->index].x4C, D_005377B8);
        func_002C48C0((cPS2MemCard*)self, 9);
        return;
    case 9:
        func_002C48C0((cPS2MemCard*)self, 11);
        return;
    case 11:
        if (self->x490 != self->x24) {
            VC2v_55D8(self, 320, self->index);
            func_002C48C0((cPS2MemCard*)self, 0x10);
        }
        __builtin_memcpy(self->entries[self->index].x6E, self->x488, 8);
        func_002C48C0((cPS2MemCard*)self, 12);
        return;
    case 12:
    case 13:
    case 14:
        {
            int k;
            if (self->state != 13) {
                k = self->state == 14 ? 2 : 0;
            } else {
                k = 1;
            }
            if (self->x490 != D_00487450[k]) {
                func_002C5570((cPS2MemCard*)self, self->index);
                func_002C48C0((cPS2MemCard*)self, 0x10);
                return;
            }
        }
        goto next;
    case 15:
    case 16:
    next:
        while (++self->index < self->count) {
            if (VC2_55D8(self, 536, self->index) != 0) {
                goto found;
            }
        }
    case 7:
        func_002C6280_v(self);
        break;
    case 35:
        if (D_004A3940 != 1) {
            func_002C52D8((cPS2MemCard*)self);
        }
        D_004A3940 = 0;
        break;
    case 17:
        if (self->ports[self->port].d != 0) {
            break;
        }
        func_002C4388((cPS2MemCard*)self);
        return;
    case 19:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C4410((cPS2MemCard*)self);
        return;
    case 40:
    case 44:
    case 48:
        if (ASYNCFILE_getstatus(self->handle) != 1) {
            return;
        }
        self->x30 = 0;
        self->x34 = 0;
        ASYNCFILE_release(self->handle, &self->x30, &self->x34);
        func_002C48C0((cPS2MemCard*)self, self->state + 1);
        return;
    case 22:
    case 28:
    case 37:
    case 41:
    case 45:
    case 49:
        self->x18 = D_004A3940;
        func_002C48C0((cPS2MemCard*)self, self->state + 1);
        return;
    case 20:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C48C0((cPS2MemCard*)self, 36);
        return;
    case 27:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C48C0((cPS2MemCard*)self, 0x1C);
        return;
    case 55:
    case 56:
        func_002C48C0((cPS2MemCard*)self, 58);
        return;
    case 36:
    case 38:
    case 39:
    case 42:
    case 46:
    case 50:
    case 54:
    case 57:
    case 58:
        func_002C48C0((cPS2MemCard*)self, self->state + 1);
        return;
    case 21:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C48C0((cPS2MemCard*)self, 22);
        return;
    case 23:
        func_002C48C0((cPS2MemCard*)self, 52);
        return;
    case 29:
        if (D_004A3940 != self->size) {
            D_004A3940 = -10001;
            func_002C48C0((cPS2MemCard*)self, 1);
            return;
        }
        func_002C48C0((cPS2MemCard*)self, 53);
        return;
    case 25:
        self->x3C = 0;
        self->entries[self->x1C].f4 = 0;
        self->entries[self->x1C].f0 = 0;
        self->entries[self->x1C].f8 = 0;
        strcpy(self->entries[self->x1C].name, self->dir);
        strcpy(self->entries[self->x1C].x4C, self->name);
        break;
    case 60:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C6848((cPS2MemCard*)self);
        return;
    case 61:
        self->x54 = D_004A3940;
        break;
    case 63:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C69A0((cPS2MemCard*)self);
        return;
    case 73:
        if (VC1_55D8(self, 584) != 0 || self->x18 < 0) {
            break;
        }
        func_002C6A28((cPS2MemCard*)self);
        return;
    case 65:
        if (VC1_55D8(self, 584) != 0 || self->x18 < 0) {
            break;
        }
        func_002C6A88((cPS2MemCard*)self);
        return;
    case 71:
        if (VC1_55D8(self, 584) != 0 || self->x18 < 0) {
            break;
        }
        func_002C6B00((cPS2MemCard*)self);
        return;
    case 67:
        if (VC1_55D8(self, 584) != 0 || self->x18 < 0) {
            break;
        }
        func_002C6B78((cPS2MemCard*)self);
        return;
    case 69:
        if (VC1_55D8(self, 584) != 0 || self->x18 < 0) {
            break;
        }
        func_002C6BF8((cPS2MemCard*)self);
        return;
    case 75:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C6C88((cPS2MemCard*)self);
        return;
    case 77:
        if (VC1_55D8(self, 584) != 0) {
            break;
        }
        func_002C6D18((cPS2MemCard*)self);
        return;
    case 64:
        {
            int lastResult = D_004A3940;
            self->state = 0;
            self->x18 = lastResult;
        }
        break;
    case 74:
        self->state = 0;
        self->x18 = -1;
        break;
    found:
        sprintf(self->path, str_pct_s_pct_s_h, self->entries[self->index].name, str_icon_sys);
        func_002C48C0((cPS2MemCard*)self, 8);
        return;
    case 52:
    case 53:
        return;
    case 66:
    case 68:
    case 70:
    case 72:
        self->state = 0;
        break;
    }
    if (cPS2MemCard_GetLastError_i(self, self->port) == -10 || cPS2MemCard_GetLastError_i(self, self->port) == -1) {
        func_002C5250((cPS2MemCard*)((cPS2MemCard*)self));
    }
    if (cPS2MemCard_GetLastError_i(self, self->port) == -10 || cPS2MemCard_GetLastError_i(self, self->port) == -2) {
        func_002C5278((cPS2MemCard*)self);
    }
    if (self->state != 0) {
        func_002C48C0((cPS2MemCard*)self, 0);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetTitleString);
#ifdef SKIP_ASM
struct sQTVt6128 {
    short delta;
    short index;
    int (*fn)(void*);
};


struct sQTEnt6128 {
    int f0;                     // 0x00 (self + 0x1A4)
    int f4;                     // 0x04
    int f8;                     // 0x08
    char padC[0x52 - 0xC];
    char name[0x78 - 0x52];     // 0x52 (self + 0x1F6)
};

struct sQT6128 {
    sQTVt6128* vt;              // 0x000
    char pad4[0x1A0];
    sQTEnt6128 ents[1];         // 0x1A4
};

extern char str_kT_MEMEmpty_c[];
extern char str_kT_MEMUnavailable[];
extern char str_kT_MEMFileCorruptPS2[];
extern char str_kT_MEMNoFreeSpace[];

static inline unsigned short* qtLoc6128(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    cPS2MemCard_GetStatus_sVEntry* vt = *(cPS2MemCard_GetStatus_sVEntry**)(obj + 4);
    char* thisp = obj + vt[4].delta;
    return vt[4].fn(thisp, GetHashValue32(key));
}

/**
 * Builds the text the save menus show for a file slot.
 *
 * Picks a localized message when the slot cannot be shown: kT_MEMEmpty when slot 53 (cPS2MemCard_IsAvailable1) fails,
 * kT_MEMUnavailable when slot 55 (cPS2MemCard_IsFormatted1) fails, kT_MEMFileCorruptPS2 when the slot's unk_00 or unk_04 flag is
 * set, kT_MEMNoFreeSpace when unk_08 is set, kT_MEMEmpty again when the slot has no name (slot 67, cPS2MemCard_HasFileName).
 * Otherwise widens the save title (unk_1A4[i].unk_4C from byte 6 on) with func_002C2540. The text is cut to 32
 * characters and copied to @p dst.
 *
 * @param fileSlot  the file slot
 * @param dst  the wide-character buffer that receives the text
 */
extern "C" void cPS2MemCard_GetTitleString(sQT6128* self, int fileSlot, unsigned short* dst)
{
    unsigned short buf[64];
    if (self->vt[53].fn(vthis(self, self->vt[53])) == 0) {
        strcpy1(buf, qtLoc6128(str_kT_MEMEmpty_c));
    } else if (self->vt[55].fn(vthis(self, self->vt[55])) == 0) {
        strcpy1(buf, qtLoc6128(str_kT_MEMUnavailable));
    } else if (self->ents[fileSlot].f0 != 0 || self->ents[fileSlot].f4 != 0) {
        strcpy1(buf, qtLoc6128(str_kT_MEMFileCorruptPS2));
    } else if (self->ents[fileSlot].f8 != 0) {
        strcpy1(buf, qtLoc6128(str_kT_MEMNoFreeSpace));
    } else if (((sVEntry<int(void*, int)>*)self->vt)[67].fn(vthis(self, self->vt[67]), fileSlot) == 0) {
        strcpy1(buf, qtLoc6128(str_kT_MEMEmpty_c));
    } else {
        func_002C2540(buf, self->ents[fileSlot].name);
    }
    buf[32] = 0;
    strcpy1(dst, buf);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6280);
#ifdef SKIP_ASM
class func_002C6280_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual int GetFreeSpace();
    virtual void v21();
    virtual int SpaceRequired(int a);
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual int HasFileName(int i);
};

struct func_002C6280_sElem {
    int flag;
    char pad[0x74];
};

struct func_002C6280_sData {
    char pad[0x1AC];
    func_002C6280_sElem elems[1];
};

/**
 * Marks the empty file slots a new save no longer fits in.
 *
 * Takes the free space from virtual slot 20 (cPS2MemCard_GetFreeSpace__FPv) and the space one save needs from slot 22
 * (cPS2MemCard_SpaceRequired with unk_024). For each of the mMaxFiles slots it clears unk_08; for a slot without a name
 * (slot 67, cPS2MemCard_HasFileName, returns 0) it subtracts the needed space while enough is left and otherwise sets
 * unk_08 to 1, the flag cPS2MemCard_IsFileSlotDisabled reads. cPS2MemCard_Update calls it in state 7.
 */
extern "C" void func_002C6280(cPS2MemCard* self)
{
    int i = 0;
    int remain = ((func_002C6280_cObj*)self)->GetFreeSpace();
    int unit = ((func_002C6280_cObj*)self)->SpaceRequired(self->unk_024);
    for (; i < self->mMaxFiles; i++) {
        ((func_002C6280_sData*)self)->elems[i].flag = 0;
        if (((func_002C6280_cObj*)self)->HasFileName(i) == 0) {
            if (remain >= unit) {
                remain -= unit;
            } else {
                ((func_002C6280_sData*)self)->elems[i].flag = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_CleanUp);
#ifdef SKIP_ASM
extern "C" void sceMcSync(int a0, int a1, int* out);
extern "C" void func_002C5278(cPS2MemCard* self);
extern int D_004A3940;

/**
 * Finishes any outstanding card operation and clears the card state.
 *
 * When a sceMcGetInfo is pending (unk_040) it waits for it (sceMcSync, sceMcSync, blocking) and sets the current
 * device's changed flag on a -1 result. Then clears unk_040, mDevice (back to port 0), the first four words of device
 * record 0 (unk_17C[0]: type, free clusters, result, format word), unk_03C and the directory table (func_002C5278).
 * mState is left as it is. Called by the destructor.
 *
 * @param pS2MemCard  the memory card object (this)
 */
extern "C" void cPS2MemCard_CleanUp(cPS2MemCard* pS2MemCard)
{
    cPS2MemCard* self = pS2MemCard;
    if (self->unk_040 != 0) {
        D_004A3940 = 0;
        sceMcSync(0, 0, &D_004A3940);
        if (D_004A3940 == -1) {
            self->unk_17C[self->mDevice].unk_10 = 1;
        }
    }
    self->unk_040 = 0;
    self->mDevice = 0;
    self->unk_17C[0].unk_00 = 0;
    self->unk_17C[0].unk_04 = 0;
    self->unk_17C[0].unk_08 = 0;
    self->unk_17C[0].unk_0C = 0;
    self->unk_03C = 0;
    func_002C5278((cPS2MemCard*)self);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C63E8);
#ifdef SKIP_ASM
/**
 * Returns whether a libmc result lies in -9999 .. -10 (as an unsigned compare of status + 9999 against 9990).
 *
 * cPS2MemCard_Update uses it while polling: results outside this range are taken as they are.
 *
 * @param status  the result code
 * @return 1 for -9999 .. -10, else 0
 */
extern "C" int func_002C63E8(cPS2MemCard* self, unsigned int status)
{
    return status + 9999 < 9990;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetDeviceName);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src);
extern "C" int sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
// PORT: void view of sprintf1 (defined returning int); this caller treats it as void.
extern "C" void sprintf1_v(unsigned short* dst, const unsigned short* fmt, ...) __asm__("sprintf1");
extern char str_pct_d_i[];
extern char str_kT_MEMMemDevicePS2_b[];

struct cPS2MemCard_GetDeviceName_sVEntry {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

/**
 * Writes the display name of a memory-card port.
 *
 * Formats the port number (@p device + 1) with "%d" and inserts it into the localized kT_MEMMemDevicePS2 string
 * (SSXApp->FELocale's virtual slot 4). Writes nothing for a port outside 0..1.
 *
 * @param device  the port (0 or 1)
 * @param dst  the wide-character buffer that receives the name
 */
extern "C" void cPS2MemCard_GetDeviceName(cPS2MemCard* self, int device, unsigned short* dst)
{
    unsigned short fmt[8];
    unsigned short num[8];
    func_002C2540(fmt, str_pct_d_i);
    sprintf1_v(num, fmt, device + 1);
    if (device < 2) {
        if (device >= 0) {
            char* obj = (char*)SSXApp->FELocale;
            cPS2MemCard_GetDeviceName_sVEntry* vt = *(cPS2MemCard_GetDeviceName_sVEntry**)(obj + 4);
            char* thisp = obj + vt[4].delta;
            sprintf1(dst, vt[4].fn(thisp, GetHashValue32(str_kT_MEMMemDevicePS2_b)), num);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetFileDate);
#ifdef SKIP_ASM
extern char str_sp_d[];

/**
 * Writes a file slot's save date as text; the PS2 version has none and writes a single space.
 *
 * Widens str_sp_d (" ") into @p dst with func_002C2540; the slot is ignored.
 *
 * @param fileSlot  the file slot (unused)
 * @param dst  the wide-character buffer that receives the text
 */
extern "C" void cPS2MemCard_GetFileDate(cPS2MemCard* self, int fileSlot, unsigned short* dst)
{
    func_002C2540(dst, str_sp_d);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetFileTime);
#ifdef SKIP_ASM
extern char str_sp_d[];

/**
 * Writes a file slot's save time as text; the PS2 version has none and writes a single space.
 *
 * Widens str_sp_d (" ") into @p dst with func_002C2540; the slot is ignored.
 *
 * @param fileSlot  the file slot (unused)
 * @param dst  the wide-character buffer that receives the text
 */
extern "C" void cPS2MemCard_GetFileTime(cPS2MemCard* self, int fileSlot, unsigned short* dst)
{
    func_002C2540(dst, str_sp_d);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetLastSavedFile);
#ifdef SKIP_ASM
struct sSaveSlot_2C64F8 {
    int busy;               // 0x00
    int locked;             // 0x04
    int bad;                // 0x08
    char pad0C[0x63];
    unsigned char sec;      // 0x6F
    unsigned char min;      // 0x70
    unsigned char hour;     // 0x71
    unsigned char day;      // 0x72
    unsigned char month;    // 0x73
    unsigned short year;    // 0x74
    char pad76[0x2];
};
struct sSaveMgrBase_2C64F8 {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual int IsFormatted1();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual int HasFileName(int slot);
};
struct sSaveMgr_2C64F8 : sSaveMgrBase_2C64F8 {
    char pad4[0x1A0];
    sSaveSlot_2C64F8 slots[6];  // 0x1A4
};

/**
 * Finds the most recently saved file slot.
 *
 * Returns -1 unless slot 55 (cPS2MemCard_IsFormatted1) passes. Among the six slots that are not flagged (unk_00, unk_04, unk_08
 * all 0) and have a name (slot 67, cPS2MemCard_HasFileName) it keeps the one with the latest modify time (unk_6E), compared by
 * year, month, day and then time of day. Likely bug: the candidate's time of day adds the current best slot's seconds
 * instead of its own.
 *
 * @return the newest file slot, or -1 when there is none
 */
extern "C" int cPS2MemCard_GetLastSavedFile(sSaveMgr_2C64F8* self)
{
    int best = -1;
    if (self->IsFormatted1() == 0) {
        return -1;
    }
    for (int i = 0; i < 6; i++) {
        if (self->slots[i].busy == 0 && self->slots[i].locked == 0 && self->HasFileName(i) != 0 && self->slots[i].bad == 0) {
            if (best != -1) {
                unsigned int ct = self->slots[i].hour * 3600 + self->slots[i].min * 60 + self->slots[best].sec;
                unsigned short cy = self->slots[i].year;
                unsigned char cd = self->slots[i].day;
                unsigned int bt = self->slots[best].hour * 3600 + self->slots[best].min * 60 + self->slots[best].sec;
                unsigned short by = self->slots[best].year;
                unsigned char bm = self->slots[best].month;
                unsigned char bd = self->slots[best].day;
                unsigned char cm = self->slots[i].month;
                if (cy < by) {
                } else if (by < cy) {
                    best = i;
                } else if (cm < bm) {
                } else if (bm < cm) {
                    best = i;
                } else if (cd < bd) {
                } else if (bd < cd) {
                    best = i;
                } else if (ct < bt) {
                } else if (bt < ct) {
                    best = i;
                }
            } else {
                best = i;
            }
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SetFileSlotInfo);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Sets the source path the save icon files are loaded from.
 *
 * Copies @p name into mSourceIconPath when @p flag is 0 (no length check in this build; the preview asserts it fits);
 * the icon states build their file paths from it.
 *
 * @param name  the icon source path (the preview's pszInfo)
 * @param flag  nonzero leaves the path unchanged
 */
extern "C" void cPS2MemCard_SetFileSlotInfo(cPS2MemCard* self, const char* name, int flag)
{
    if (flag == 0) {
        strcpy((char*)&self->mSourceIconPath, name);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_FileExistsError);
#ifdef SKIP_ASM
/**
 * Tests whether the last card operation in the current port failed because the file exists.
 *
 * Compares the device record's result word (unk_17C[mDevice].unk_08) with -11.
 *
 * @return 1 when the result is -11, else 0
 */
extern "C" int cPS2MemCard_FileExistsError(cPS2MemCard* self)
{
    char* devicePtr = (char*)self + self->mDevice * 0x14;
    return *(int*)(devicePtr + 0x184) == -11;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6700__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1 (card vtable slot 57).
 *
 * @return 1
 */
int func_002C6700(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6708__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1 (card vtable slot 58).
 *
 * @return 1
 */
int func_002C6708(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6710__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1 (card vtable slot 61, which cPS2MemCard_IsRemovable calls).
 *
 * @return 1
 */
int func_002C6710(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsRemovable);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

class cQuadTreeVObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    QT_V10(v1)
    virtual void v20(); virtual void v21();
    virtual int SpaceRequired(int a); // 0xB0
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29();
    QT_V10(v3) QT_V10(v4)
    virtual int v50(int a); // 0x190
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual int v61(int a); // 0x1E8
};

/**
 * Asks the card's virtual slot 61 about the current port (mDevice) and passes its result back; in cPS2MemCard_vtbl that
 * slot is func_002C6710__FPv, which always returns 1.
 */
extern "C" void cPS2MemCard_IsRemovable(cPS2MemCard* self)
{
    ((cQuadTreeVObj*)self)->v61(self->mDevice);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetBlockSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns the card's block size.
 *
 * @return always 0x4000
 */
int cPS2MemCard_GetBlockSize(void* self)
{
    return 0x4000;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetTotalSpace__FPv);
#ifdef SKIP_ASM
/**
 * Returns the card's total space; not available on PS2.
 *
 * @return always -1
 */
int cPS2MemCard_GetTotalSpace(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SpaceRequired);
#ifdef SKIP_ASM
/**
 * Returns the card space a save of @p byteSize bytes needs: its size rounded up to 1 KB clusters plus 77 clusters for
 * the directory and icon files.
 *
 * @param byteSize  the save data size in bytes
 * @return (byteSize + 1023) / 1024 + 77
 */
extern "C" int cPS2MemCard_SpaceRequired(cPS2MemCard* self, int byteSize)
{
    return (byteSize + 0x3ff) / 0x400 + 77;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_SpaceRequired1);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

/**
 * Asks the card's virtual slot 22 (cPS2MemCard_SpaceRequired) for the space the current data size (unk_024) needs and
 * passes its result back.
 */
extern "C" void cPS2MemCard_SpaceRequired1(cPS2MemCard* self)
{
    ((cQuadTreeVObj*)self)->SpaceRequired(self->unk_024);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67A8__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0 (card vtable slot 50, which cPS2MemCard_IsDamaged calls).
 *
 * @return 0
 */
int func_002C67A8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_IsDamaged);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

/**
 * Asks the card's virtual slot 50 about the current port (mDevice) and passes its result back; in cPS2MemCard_vtbl that
 * slot is func_002C67A8__FPv, which always returns 0.
 */
extern "C" void cPS2MemCard_IsDamaged(cPS2MemCard* self)
{
    ((cQuadTreeVObj*)self)->v50(self->mDevice);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetFreeDirEntries__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of free directory entries on the card; not available on PS2.
 *
 * @return always -1
 */
int cPS2MemCard_GetFreeDirEntries(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67E8__FPvi);
#ifdef SKIP_ASM
/**
 * Selects a port and checks its card: stores @p device in mDevice and unk_010 and calls func_002C48C0 with state 3,
 * which starts a sceMcGetInfo (func_002C50E0).
 *
 * The network-config screen (cFEStateNetConfig) and the network-account library's card callbacks (dirtysock/tagsunk)
 * call it.
 *
 * @param device  the memory-card port (0 or 1)
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C67E8(void* vself, int device)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->mDevice = device;
    self->unk_010 = device;
    return func_002C48C0((cPS2MemCard*)self, 3);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6810);
#ifdef SKIP_ASM
/**
 * Starts a full directory listing: clears mNumDirTable, copies @p name into the search pattern (unk_139) and calls
 * func_002C48C0 with state 60; cPS2MemCard_Update then runs func_002C6848.
 *
 * @param name  the sceMcGetDir pattern (ASCII; no length check)
 */
extern "C" void func_002C6810(cPS2MemCard* self, const char* name)
{
    self->mNumDirTable = 0;
    strcpy((char*)self + 0x139, name);
    func_002C48C0(self, 0x3C);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6848);
#ifdef SKIP_ASM
extern "C" int sceMcGetDir(int h, int a1, char* name, int a3, int a4, void* buf);
extern int D_004A3940;
extern "C" void* MEM_fill(void* dst, int c, int n);

/**
 * Issues the listing of func_002C6810: clears the directory table (unk_4C0, 0x1000 bytes) and calls sceMcGetDir
 * (sceMcGetDir) on mDevice with the pattern unk_139, for at most 64 entries.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 61.
 * cPS2MemCard_Update calls it in state 60.
 */
extern "C" void func_002C6848(cPS2MemCard* self)
{
    char* buf = (char*)&self->unk_4C0[0];
    MEM_fill(buf, 0, 0x1000);
    int mcResult = sceMcGetDir(self->mDevice, 0, (char*)self + 0x139, 0, 0x40, buf);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 61);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C68C8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Copies the name (m_entryName) of directory table entry @p i (unk_4C0[i]) into @p dst with strcpy.
 *
 * @param i  the entry index (0 .. mNumDirTable - 1; not checked)
 * @param dst  the output buffer (32 bytes is enough)
 */
extern "C" void func_002C68C8(cPS2MemCard* self, int i, char* dst)
{

    strcpy(dst, (char*)self->unk_4C0[i].m_entryName);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cPS2MemCard_GetNumFiles);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

struct cPS2MemCard_GetNumFiles_sName {
    char name[0x78];
};

struct cPS2MemCard_GetNumFiles_sTree {
    char pad_0x00[0x14];
    int count;                          // 0x14
    char pad_0x18[0x1B0 - 0x18];
    cPS2MemCard_GetNumFiles_sName names[1];       // 0x1B0
};

/**
 * Counts the file slots that hold a save.
 *
 * Counts the slots below mMaxFiles whose directory name (unk_1A4[i].unk_0C) is not empty.
 *
 * @return the number of named file slots
 */
extern "C" int cPS2MemCard_GetNumFiles(cPS2MemCard_GetNumFiles_sTree* self)
{
    int n = 0;
    int i;
    for (i = 0; i < self->count; i++) {
        if (strlen(self->names[i].name) != 0) {
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6968);
#ifdef SKIP_ASM
/**
 * Starts opening a file: stores the sceMcOpen mode in unk_14E0, copies @p name into the path (unk_0F9) and calls
 * func_002C48C0 with state 63; cPS2MemCard_Update then runs func_002C69A0 and, when the open completes, stores the
 * descriptor in mFileDescriptor.
 *
 * @param name  the file path (ASCII; no length check)
 * @param openMode  the sceMcOpen mode flags
 */
extern "C" void func_002C6968(cPS2MemCard* self, const char* name, int openMode)
{
    self->unk_14E0 = openMode;
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 0x3F);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C69A0);
#ifdef SKIP_ASM
extern "C" int sceMcOpen(int h, int a1, char* name, int a3);
extern int D_004A3940;

/**
 * Issues the open of func_002C6968: sets mFileDescriptor to -1 and calls sceMcOpen (sceMcOpen) on mDevice with the
 * path unk_0F9 and the mode unk_14E0.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 64.
 * cPS2MemCard_Update calls it in state 63.
 */
extern "C" void func_002C69A0(cPS2MemCard* self)
{
    self->mFileDescriptor = -1;
    int mcResult = sceMcOpen(self->mDevice, 0, (char*)self + 0xF9, self->unk_14E0);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 0x40);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A08__FPv);
#ifdef SKIP_ASM
/**
 * Starts closing the open file: calls func_002C48C0 with state 73; cPS2MemCard_Update then runs func_002C6A28.
 *
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C6A08(void* self)
{
    return func_002C48C0((cPS2MemCard*)self, 73);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A28);
#ifdef SKIP_ASM
// PORT: the unit declares func_002C48C0 as returning void*, but its body returns
// nothing (v0 is leftover); this caller needs the void prototype.
extern "C" void func_002C48C0_v(void* self, int state) __asm__("func_002C48C0");
extern "C" int sceMcClose(int h);
extern int D_004A3940;

/**
 * Issues the close of func_002C6A08: stores sceMcClose(mFileDescriptor) (sceMcClose) in D_004A3940 and calls
 * func_002C48C0 with state 74, without checking the result. cPS2MemCard_Update calls it in state 73.
 */
extern "C" void func_002C6A28(cPS2MemCard* self)
{
    D_004A3940 = sceMcClose(self->mFileDescriptor);
    func_002C48C0_v(self, 74);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A60__FPvii);
#ifdef SKIP_ASM
/**
 * Starts a seek in the open file: stores the offset in unk_14E4 and the origin in unk_14E8 and calls func_002C48C0 with
 * state 65; cPS2MemCard_Update then runs func_002C6A88.
 *
 * @param seekOffset  the offset in bytes
 * @param seekOrigin  the sceMcSeek origin
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C6A60(void* vself, int seekOffset, int seekOrigin)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_14E4 = seekOffset;
    self->unk_14E8 = seekOrigin;
    return func_002C48C0((cPS2MemCard*)self, 65);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A88);
#ifdef SKIP_ASM
extern "C" int sceMcSeek(int h, int a1, int a2);
extern int D_004A3940;

/**
 * Issues the seek of func_002C6A60__FPvii: calls sceMcSeek (sceMcSeek) on mFileDescriptor with unk_14E4 and
 * unk_14E8.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 66.
 * cPS2MemCard_Update calls it in state 65.
 */
extern "C" void func_002C6A88(cPS2MemCard* self)
{
    int mcResult = sceMcSeek(self->mFileDescriptor, self->unk_14E4, self->unk_14E8);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 66);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6AE0__FPv);
#ifdef SKIP_ASM
/**
 * Starts flushing the open file: calls func_002C48C0 with state 71; cPS2MemCard_Update then runs func_002C6B00.
 *
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C6AE0(void* self)
{
    return func_002C48C0((cPS2MemCard*)self, 71);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6B00);
#ifdef SKIP_ASM
extern "C" int sceMcFlush(int h);
extern int D_004A3940;

/**
 * Issues the flush of func_002C6AE0__FPv: calls sceMcFlush (sceMcFlush) on mFileDescriptor.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 72.
 * cPS2MemCard_Update calls it in state 71.
 */
extern "C" void func_002C6B00(cPS2MemCard* self)
{
    int mcResult = sceMcFlush(self->mFileDescriptor);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 72);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6B50__FPvii);
#ifdef SKIP_ASM
/**
 * Starts a read from the open file: stores the buffer in unk_02C and the size in unk_024 and calls func_002C48C0 with
 * state 67; cPS2MemCard_Update then runs func_002C6B78.
 *
 * @param buffer  the buffer to read into
 * @param ioSize  the number of bytes to read
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C6B50(void* vself, int buffer, int ioSize)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_02C = buffer;
    self->unk_024 = ioSize;
    return func_002C48C0((cPS2MemCard*)self, 67);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6B78);
#ifdef SKIP_ASM
extern "C" int sceMcRead(int h, int a1, int a2);
extern int D_004A3940;

/**
 * Issues the read of func_002C6B50__FPvii: calls sceMcRead (sceMcRead) on mFileDescriptor into unk_02C for unk_024
 * bytes.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 68.
 * cPS2MemCard_Update calls it in state 67.
 */
extern "C" void func_002C6B78(cPS2MemCard* self)
{
    int mcResult = sceMcRead(self->mFileDescriptor, self->unk_02C, self->unk_024);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 68);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6BD0__FPvii);
#ifdef SKIP_ASM
/**
 * Starts a write to the open file: stores the buffer in unk_02C and the size in unk_024 and calls func_002C48C0 with
 * state 69; cPS2MemCard_Update then runs func_002C6BF8.
 *
 * @param buffer  the data to write
 * @param ioSize  the number of bytes to write
 * @return func_002C48C0's return register (it returns nothing)
 */
void* func_002C6BD0(void* vself, int buffer, int ioSize)
{
    cPS2MemCard* self = (cPS2MemCard*)vself;
    self->unk_02C = buffer;
    self->unk_024 = ioSize;
    return func_002C48C0((cPS2MemCard*)self, 69);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6BF8);
#ifdef SKIP_ASM
extern "C" int sceMcWrite(int h, int a1, int a2);
extern int D_004A3940;

/**
 * Issues the write of func_002C6BD0__FPvii: calls sceMcWrite (sceMcWrite) on mFileDescriptor from unk_02C for
 * unk_024 bytes.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 70.
 * cPS2MemCard_Update calls it in state 69.
 */
extern "C" void func_002C6BF8(cPS2MemCard* self)
{
    int mcResult = sceMcWrite(self->mFileDescriptor, self->unk_02C, self->unk_024);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 70);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6C50);
#ifdef SKIP_ASM
/**
 * Starts creating a directory: stores @p requestValue in unk_020, copies @p name into the path (unk_0F9) and calls func_002C48C0
 * with state 75; cPS2MemCard_Update then runs func_002C6C88 (sceMcMkdir). The mkdir does not use unk_020.
 *
 * @param name  the directory path (ASCII; no length check)
 * @param requestValue  the value stored in unk_020
 */
extern "C" void func_002C6C50(cPS2MemCard* self, const char* name, int requestValue)
{
    self->unk_020 = requestValue;
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 75);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6C88);
#ifdef SKIP_ASM
extern "C" int sceMcMkdir(int h, int a1, char* name);
extern int D_004A3940;

/**
 * Issues the mkdir of func_002C6C50: calls sceMcMkdir (sceMcMkdir) on mDevice with the path unk_0F9.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 76.
 * cPS2MemCard_Update calls it in state 75.
 */
extern "C" void func_002C6C88(cPS2MemCard* self)
{
    int mcResult = sceMcMkdir(self->mDevice, 0, (char*)self + 0xF9);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 76);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6CE0);
#ifdef SKIP_ASM
/**
 * Starts deleting a file or directory: copies @p name into the path (unk_0F9) and calls func_002C48C0 with state 77;
 * cPS2MemCard_Update then runs func_002C6D18 (sceMcDelete).
 *
 * @param name  the path (ASCII; no length check)
 */
extern "C" void func_002C6CE0(cPS2MemCard* self, const char* name)
{
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 77);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6D18);
#ifdef SKIP_ASM
extern "C" int sceMcDelete(int h, int a1, char* name);
extern int D_004A3940;

/**
 * Issues the delete of func_002C6CE0: calls sceMcDelete (sceMcDelete) on mDevice with the path unk_0F9.
 *
 * The result goes to D_004A3940; a negative one moves to the error state 1 (func_002C48C0), otherwise to state 78.
 * cPS2MemCard_Update calls it in state 77.
 */
extern "C" void func_002C6D18(cPS2MemCard* self)
{
    int mcResult = sceMcDelete(self->mDevice, 0, (char*)self + 0xF9);
    D_004A3940 = mcResult;
    if (mcResult < 0) {
        func_002C48C0(self, 1);
    } else {
        func_002C48C0(self, 78);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cLocaleFile__cLocaleFile);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void MEM_free(void* p);

/**
 * Destroys a locale file (the 0x14-byte "LocaleFile" entry cFELocale_addFile creates): frees its loaded data (+0x4)
 * with MEM_free when set and, when bit 0 of @p flags is set, frees the object with operator_delete.
 *
 * @param flags  bit 0 set: also free the object
 */
extern "C" void cLocaleFile__cLocaleFile(cLocaleFile* self, int flags)
{
    void* buffer = self->mpStringTableData;
    if (buffer != 0) {
        MEM_free(buffer);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* LOCALE_getstr(int);

//100%
INCLUDE_ASM("worldpainter/quadtree", cLocaleFile_getStringByIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns the string at an index of the loaded locale data: LOCALE_getstr on the file data (+0x4), with the index the
 * caller passes in the second argument register going through untouched.
 *
 * @return LOCALE_getstr's result (the string, or 0)
 */
void* cLocaleFile_getStringByIndex(void* vself)
{
    cLocaleFile* self = (cLocaleFile*)vself;
    return LOCALE_getstr((int)self->mpStringTableData);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cLocaleFile_getStringByHash);
#ifdef SKIP_ASM


/**
 * Looks a string up by its hash in the locale file's sorted {hash, string} table (+0x10, +0x8 entries).
 *
 * Returns 0 when the file has no "LOCT" table (+0xC is 0), when the key lies outside the first and last keys, or when
 * the binary search does not find it. The range check and the search start at entry [+0x8], one past the entries
 * cLocaleFile_loadFile converts (likely bug, unless the table ends with a sentinel).
 *
 * @param key  the string's hash (GetHashValue32 of its id)
 * @return the string, or 0
 */
extern "C" int cLocaleFile_getStringByHash(cLocaleFile* self, unsigned int key)
{
    int lo = 0;
    int hi = ((int)self->mNumEntries);
    if (((int)self->mpHashTableHeader) != 0) {
        if ((self->mpHashTableData)[hi].mHashID < key || key < (self->mpHashTableData)[0].mHashID) {
            return 0;
        }
        int mid = hi >> 1;
        while ((self->mpHashTableData)[mid].mHashID != key) {
            if (key < (self->mpHashTableData)[mid].mHashID) {
                hi = mid;
            } else {
                lo = mid;
            }
            int next = ((hi - lo) >> 1) + lo;
            if (next == mid) {
                return 0;
            }
            mid = next;
        }
        return ((int)(self->mpHashTableData)[mid].mID);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", cLocaleFile_loadFile);
#ifdef SKIP_ASM
extern "C" void* FILE_load(const char* name, int flags);
extern "C" unsigned int LOCALE_getstate(void* p, int a);
extern "C" int strncmp(const void* a, const void* b, int n);
// PORT: cLocaleFile_getStringByIndex is declared (void*) but called with a second argument; bound by asm label
void* cLocaleFile_getStringByIndex_2(void* self, int v) __asm__("cLocaleFile_getStringByIndex__FPv");
extern char str_LOCT[];

struct cLocaleFile_loadFile_sEnt {
    int f0;
    int f4;
};
struct cLocaleFile_loadFile_sChunk {
    int id;
    int size;
};

/**
 * Loads a locale (.loc) file into this entry.
 *
 * Stores @p kind (the language) at +0x0, loads the file with FILE_load into +0x4 and counts its strings with
 * LOCALE_getstate(data, 2) into +0x8. The chunk after the first one (data + its size) must be "LOCT"; otherwise +0xC is
 * set to 0 and the function returns. Then +0xC points at that chunk, +0x10 at the {hash, index} table after it, and
 * each entry's index is replaced by its string pointer (cLocaleFile_getStringByIndex__FPv).
 *
 * @param name  the file path
 * @param kind  the language byte stored at +0x0 (cFELocale_addFile passes the file's D_00441188 entry)
 */
extern "C" void cLocaleFile_loadFile(cLocaleFile* self, const char* name, char kind)
{
    unsigned int i;
    int size;
    self->mLanguage = kind;
    (*(cLocaleFile_loadFile_sChunk**)&self->mpStringTableData) = (cLocaleFile_loadFile_sChunk*)FILE_load(name, 0);
    self->mNumEntries = LOCALE_getstate(((cLocaleFile_loadFile_sChunk*)self->mpStringTableData), 2);
    size = ((cLocaleFile_loadFile_sChunk*)self->mpStringTableData)->size;
    (*(cLocaleFile_loadFile_sChunk**)&self->mpHashTableHeader) = (cLocaleFile_loadFile_sChunk*)((char*)((cLocaleFile_loadFile_sChunk*)self->mpStringTableData) + size);
    if (strncmp(((cLocaleFile_loadFile_sChunk*)self->mpHashTableHeader), str_LOCT, 4) != 0) {
        (*(cLocaleFile_loadFile_sChunk**)&self->mpHashTableHeader) = 0;
        return;
    }
    size = ((cLocaleFile_loadFile_sChunk*)self->mpHashTableHeader)->size;
    (*(cLocaleFile_loadFile_sEnt**)&self->mpHashTableData) = (cLocaleFile_loadFile_sEnt*)((char*)((cLocaleFile_loadFile_sChunk*)self->mpHashTableHeader) + size);
    for (i = 0; i < ((int)self->mNumEntries); i++) {
        ((cLocaleFile_loadFile_sEnt*)self->mpHashTableData)[i].f4 = (int)cLocaleFile_getStringByIndex_2(self, ((cLocaleFile_loadFile_sEnt*)self->mpHashTableData)[i].f4);
    }
}
#endif

