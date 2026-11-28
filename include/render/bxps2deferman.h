#ifndef RENDER_BXPS2DEFERMAN_H
#define RENDER_BXPS2DEFERMAN_H

// cPS2DeferManager: the PS2 renderer's deferred draw list. Model / particle / font code submits DMA packets tagged
// with a draw state (sDrawState), a texture key and a view (matrix record); at the end of the frame the manager
// interns the states, groups the packets into buckets per (state, texture, view), radix-sorts the buckets by a
// state-derived key and chains their packets into one DMA list. One instance: cPS2GraphicsMan::mpDeferMan
// (render/ps2graphicsman.h, 0x18F0; [P] GetDeferMan__C15cPS2GraphicsMan). EA's source: d:/ssx/code/source/render/
// bxps2deferman.h / .cpp ([P] preview assert strings and the out-of-line inlines __16cPS2DeferManager,
// Find__16cPS2DeferManagerRC10sDrawStateUc, Find__16cPS2DeferManagerPC14cDrawStateListG7cTexKeyUci).
//
// Size 0x6B100: [R] cPS2GraphicsMan_Init allocates it with cMemMan_alloc(0x6B100, "DEFER MAN") and calls
// cPS2DeferManager::cPS2DeferManager. No base class, no vtable (the ctor stores none; no vtable in vtables.tsv).
// The ctor (render/font.cpp, view sFontCache) fills the sDrawState texture ids of the 0xA28 submit records, the 0x400
// draw-state list entries and the current state with -1 (sTextureState ctor) and runs the (empty) constructors of the
// 0x800 buckets and the 12 view states.
//
// A FLAT layout: every field at its retail offset. Evidence tags: [R] retail constructor / allocation; [P] the July
// 2003 preview's assert strings in bxps2deferman.cpp/.h, which name the counters (the preview's offsets are smaller:
// kMaxDrawStates etc. differ, so each was mapped by the counter's role); [U] retail use (render/font.cpp
// cPS2DeferManager_Find / Find1; object/railmodifier.cpp cPS2DeferManager_buildPresetStates .. cPS2DeferManager_writeVu1ProgramSetup; render/bezierman.cpp;
// render/particle.cpp cPS2GraphicsMan_RenderModel and the sPtRing* views); [X] refs/ssx3-rust's render notes
// (docs/investigation/phase3-render.md: "Record", "Bucket", "MatrixRecord"; names are guesses).
// The element struct names other than cDrawStateList ([P]) are ours (tDefer*).

#include "layout.h"
#include "bx/math3d.h"
#include "render/ps2graphicsman.h"

// One submitted draw (0x80): [U] cPS2DeferManager_bucketRecords (view sRmEntry_2DE8) walks the mNumSubmitDMA of them;
// cPS2GraphicsMan_RenderModel / the particle and font code fill them. [X] "Record".
struct tDeferSubmit {
    sDrawState mDrawState;          // 0x00 [R] the ctor fills the texture ids with -1; [U] interned by Find (w[4], tex[2])
    int mPacket;                    // 0x14 [U] DMA packet start (sRmEntry_2DE8::packet)
    int mTail;                      // 0x18 [U] DMA packet end (sRmEntry_2DE8::tail)
    short mViewIndex;               // 0x1C [U] index into mViewStates (sRmEntry_2DE8::mtxIndex; passed to Find1)
    short mContinuation;            // 0x1E [U] nonzero: continue the previous bucket (sRmEntry_2DE8::continuation)
    char unk_20[0x60];              // 0x20 not accessed by the defer manager's methods (the submitters' own data)
};

CHECK_OFFSET(tDeferSubmit, mDrawState, 0x00);
CHECK_OFFSET(tDeferSubmit, mPacket, 0x14);
CHECK_OFFSET(tDeferSubmit, mTail, 0x18);
CHECK_OFFSET(tDeferSubmit, mViewIndex, 0x1C);
CHECK_OFFSET(tDeferSubmit, mContinuation, 0x1E);
CHECK_OFFSET(tDeferSubmit, unk_20, 0x20);
CHECK_SIZE(tDeferSubmit, 0x80);

// One interned draw state (0x18; [P] EA's cDrawStateList: Find(const sDrawState&, unsigned char hash) returns one,
// Find(const cDrawStateList*, cTexKey, unsigned char, int) takes one).
struct cDrawStateList {
    sDrawState mDrawState;          // 0x00 [R] ctor fills the texture ids with -1; [U] Find compares the 4 words + texture
    cDrawStateList* mpNext;         // 0x14 [U] the hash chain (mStateHash) cPS2DeferManager_Find walks (sGlyphEntry::next)
};

CHECK_OFFSET(cDrawStateList, mDrawState, 0x00);
CHECK_OFFSET(cDrawStateList, mpNext, 0x14);
CHECK_SIZE(cDrawStateList, 0x18);

// One bucket (0x20): a (state, texture, view) combination and its DMA chain. [U] cPS2DeferManager_bucketRecords / cPS2DeferManager_buildDmaChains
// (views sRmNode_2DE8 / sRmNode_3490), cPS2DeferManager_Find1, tDeferBucket_appendPacket (appends a packet: self = a bucket).
// [X] "Bucket".
struct tDeferBucket {
    cDrawStateList* mpState;        // 0x00 [P] Find(const cDrawStateList*, ...); [U] Find1 compares it
    int mTexKey;                    // 0x04 [P] cTexKey (passed by value; two shorts); [U] Find1 compares it
    int mViewIndex;                 // 0x08 [P] Find's int argument; [U] Find1 compares it (sRmNode::layer/mtxIndex)
    int mPacket;                    // 0x0C [U] tDeferBucket_appendPacket: first packet (0 = empty)
    int mTail;                      // 0x10 [U] tDeferBucket_appendPacket: last packet's end (the next packet is chained there)
    int mLinked;                    // 0x14 [U] cPS2DeferManager_bucketRecords: set for a continuation bucket (not hashed)
    tDeferBucket* mpLink;           // 0x18 [U] cPS2DeferManager_bucketRecords / cPS2DeferManager_buildDmaChains: the continuation chain
    tDeferBucket* mpNext;           // 0x1C [U] the hash chain (mBucketHash) Find1 walks

    // Methods bound to their retail symbols by asm labels.
    void appendPacket(int packet, int tail) __asm__("tDeferBucket_appendPacket");
};

CHECK_OFFSET(tDeferBucket, mpState, 0x00);
CHECK_OFFSET(tDeferBucket, mTexKey, 0x04);
CHECK_OFFSET(tDeferBucket, mViewIndex, 0x08);
CHECK_OFFSET(tDeferBucket, mPacket, 0x0C);
CHECK_OFFSET(tDeferBucket, mTail, 0x10);
CHECK_OFFSET(tDeferBucket, mLinked, 0x14);
CHECK_OFFSET(tDeferBucket, mpLink, 0x18);
CHECK_OFFSET(tDeferBucket, mpNext, 0x1C);
CHECK_SIZE(tDeferBucket, 0x20);

// One sort element (8): [U] cPS2DeferManager_sortItems radix-sorts them by key (4 passes of 8 bits); [P] "length <=
// kMaxSortElements" (1024) in the preview's sort.
struct tDeferSortItem {
    unsigned int mKey;              // 0x0 [U] sDrawState_makeSortKey(state, texKey) (sRmSortItem::key)
    tDeferBucket* mpBucket;         // 0x4 [U] (sRmSortItem::node)
};

CHECK_OFFSET(tDeferSortItem, mKey, 0x0);
CHECK_OFFSET(tDeferSortItem, mpBucket, 0x4);
CHECK_SIZE(tDeferSortItem, 0x8);

// One view state (0x180): the matrices and viewport constants a bucket's packets are drawn with. [U] cPS2DeferManager_findOrAddViewState
// (view sRailXf_45B8) interns them, comparing 0xE0..0x17F; [X] "MatrixRecord" (the member names, m-prefixed).
struct tDeferViewState {
    t4Matrix mWorld;                // 0x000 [U] copied (m0); [X] world
    t4Matrix mView;                 // 0x040 [U] copied (m1); [X] view
    t4Matrix mWorldView;            // 0x080 [U] copied (m2); [X] worldView
    t4Vector mGuardScale;           // 0x0C0 [U] copied (v0); [X] guardScale
    t4Vector mGuardOffset;          // 0x0D0 [U] copied (v1); [X] guardOffset
    t4Matrix mViewProj;             // 0x0E0 [U] compared + copied (key0); [X] viewProj
    t4Matrix mWorldClip;            // 0x120 [U] compared + copied (key1); [X] worldClip
    t4Vector mScreenScale;          // 0x160 [U] compared + copied (key2); [X] screenScale
    t4Vector mScreenOffset;         // 0x170 [U] compared + copied (key3); [X] screenOffset
};

CHECK_OFFSET(tDeferViewState, mWorld, 0x000);
CHECK_OFFSET(tDeferViewState, mView, 0x040);
CHECK_OFFSET(tDeferViewState, mWorldView, 0x080);
CHECK_OFFSET(tDeferViewState, mGuardScale, 0x0C0);
CHECK_OFFSET(tDeferViewState, mGuardOffset, 0x0D0);
CHECK_OFFSET(tDeferViewState, mViewProj, 0x0E0);
CHECK_OFFSET(tDeferViewState, mWorldClip, 0x120);
CHECK_OFFSET(tDeferViewState, mScreenScale, 0x160);
CHECK_OFFSET(tDeferViewState, mScreenOffset, 0x170);
CHECK_SIZE(tDeferViewState, 0x180);

// One view port / scissor rectangle (8): [U] cPS2DeferManager_findOrAddScissor (view sRailKey4) interns them; cPS2DeferManager_writeStateChange (view
// sRmKey3C20) reads the active one.
struct tDeferViewPort {
    unsigned short unk_0;           // 0x0 [U] compared (a)
    unsigned short unk_2;           // 0x2 [U] compared (b)
    unsigned short unk_4;           // 0x4 [U] compared (c)
    unsigned short unk_6;           // 0x6 [U] compared (d)
};

CHECK_OFFSET(tDeferViewPort, unk_0, 0x0);
CHECK_OFFSET(tDeferViewPort, unk_2, 0x2);
CHECK_OFFSET(tDeferViewPort, unk_4, 0x4);
CHECK_OFFSET(tDeferViewPort, unk_6, 0x6);
CHECK_SIZE(tDeferViewPort, 0x8);

struct sDrawState;

struct cPS2DeferManager {
    int mNumSubmitDMA;              // 0x00000 [P] "mNumSubmitDMA < kMaxSubmitDMA" reads the manager's first word;
                                    //         [U] cPS2DeferManager_resetFrame 0; cPS2DeferManager_bucketRecords loops over it (sRmBatch_2DE8::count)
    char unk_00004[0x7C];           // 0x00004 not accessed
    tDeferSubmit mSubmits[0xA28];   // 0x00080 [R] ctor (2600 records, stride 0x80); [U] cPS2DeferManager_bucketRecords (entries)
    int mUsedDrawStates;            // 0x51480 [P] "mUsedDrawStates < kMaxDrawStates" in Find, "mUsedDrawStates ==
                                    //         kMaxStaticStates" after the presets; [U] Find appends at it; cPS2DeferManager_resetFrame
                                    //         resets it to mNumStaticStates
    cDrawStateList mDrawStates[0x400]; // 0x51484 [R] ctor (1024); [P] kMaxDrawStates 1024; [U] Find (sGlyphCache::entries)
    int mNumStaticStates;           // 0x57484 [U] cPS2DeferManager_buildPresetStates 0, then the count after the 3 presets
                                    //         (sRmBatch_29B8::base); cPS2DeferManager_resetFrame copies it to mUsedDrawStates
    cDrawStateList* mpPresetStates[3]; // 0x57488 (name ours) [U] cPS2DeferManager_buildPresetStates saves the three presets (blend modes
                                    //         1/17, 1/2, then 5/3 with alpha sort 1); cPS2GraphicsMan_RenderModel
                                    //         picks [0], [2] or [1] by its flags (& 0x660000: 0 or 0x600000, 0x20000,
                                    //         0x200000)
    int mUsedStates;                // 0x57494 [P] "mUsedStates < kMaxStates" (preview kMaxStates 2048) after a failed
                                    //         bucket Find; [U] cPS2DeferManager_resetFrame 0; cPS2DeferManager_bucketRecords appends at it (nNodes)
    tDeferBucket mBuckets[0x800];   // 0x57498 [R] ctor (2048 empty ctors); [U] cPS2DeferManager_bucketRecords (nodes)
    char unk_67498[0x8];            // 0x67498 not accessed (sRmBatch_2DE8 places an unused stateHash here)
    cDrawStateList* mStateHash[0x100]; // 0x674A0 [U] Find walks bucket [hash]; cPS2DeferManager_resetFrame invalidates/clears it
    tDeferBucket* mBucketHash[0x100];  // 0x678A0 [U] Find1 walks bucket [hash]; cPS2DeferManager_bucketRecords pushes; cPS2DeferManager_resetFrame clears
    tDeferBucket* mpLastBucket;     // 0x67CA0 [U] cPS2DeferManager_bucketRecords: the previous bucket (continuations chain to it)
    int mNumSortItems;              // 0x67CA4 [P] "length <= kMaxSortElements"; [U] cPS2DeferManager_resetFrame 0; cPS2DeferManager_bucketRecords
                                    //         appends; cPS2DeferManager_sortItems sorts that many; cPS2DeferManager_buildDmaChains walks them
    tDeferSortItem mSortItems[0x400]; // 0x67CA8 [P] kMaxSortElements 1024; [U] cPS2DeferManager_sortItems (src), cPS2DeferManager_buildDmaChains
    sDrawState mCurrentState;       // 0x69CA8 [R] ctor (texture ids -1); [U] cPS2DeferManager_resetFrame = sDrawState::s_defaultDrawState;
                                    //         cPS2DeferManager_setCurrentState copies one in; cPS2DeferManager_writeStateChange reads it
    unsigned int unk_69CBC;         // 0x69CBC [U] cPS2DeferManager_writeStateChange (sRmRend3C20::frame)
    int mActiveViewIndex;           // 0x69CC0 (name ours) [U] the active view state: cPS2DeferManager_resetFrame -1; cPS2DeferManager_findOrAddViewState
                                    //         sets the index it found or added (sRailXfTable_45B8::found); the
                                    //         submitters store it as tDeferSubmit::mViewIndex
    int mActivePortIndex;           // 0x69CC4 [P] "mActivePortIndex >= 0" (the word after the view index); [U]
                                    //         cPS2DeferManager_resetFrame -1; cPS2DeferManager_findOrAddScissor sets it (sRailKeyTable::found)
    // mActivePortIndex when no scissor port is active (name ours): [U] cPS2DeferManager_resetFrame resets it to -1, and
    // cPS2GraphicsMan_PopViewport re-registers the saved scissor (cPS2DeferManager_findOrAddScissor) when the saved index is -1.
    enum { kNoActivePort = -1 };
    int mIndexView;                 // 0x69CC8 [P] "mIndexView < kMaxNumViewState"; [U] cPS2DeferManager_resetFrame 0;
                                    //         cPS2DeferManager_findOrAddViewState appends at it (sRailXfTable_45B8::count)
    int unk_69CCC;                  // 0x69CCC not accessed (alignment of mViewStates)
    tDeferViewState mViewStates[12];// 0x69CD0 [R] ctor (12 empty ctors); [U] cPS2DeferManager_findOrAddViewState (entries)
    int mIndexPort;                 // 0x6AED0 [P] "mIndexPort < kMaxNumViewPort"; [U] cPS2DeferManager_resetFrame 0; cPS2DeferManager_findOrAddScissor
                                    //         appends at it (sRailKeyTable::count)
    tDeferViewPort mViewPorts[64];  // 0x6AED4 [P] kMaxNumViewPort 64; [U] cPS2DeferManager_findOrAddScissor (keys), cPS2DeferManager_writeStateChange
    char unk_6B0D4[0x2C];           // 0x6B0D4 not accessed by any matched or asm method (size from the allocation)

    // Methods bound to their retail symbols by asm labels.
    tDeferBucket* Find1(int stateEntry, int texKey, unsigned char hash, int matrixIndex)
        __asm__("cPS2DeferManager_Find1");

    // Methods bound to their retail symbols by asm labels.
    void buildPresetStates() __asm__("cPS2DeferManager_buildPresetStates");
    void resetFrame() __asm__("cPS2DeferManager_resetFrame");
    void bucketRecords() __asm__("cPS2DeferManager_bucketRecords");
    void sortItems() __asm__("cPS2DeferManager_sortItems");
    void writeDefaultState(ulong** pp) __asm__("cPS2DeferManager_writeDefaultState");
    void setCurrentState(const sDrawState* src) __asm__("cPS2DeferManager_setCurrentState");
    void writeVu1ProgramSetup(int type, int mode, unsigned int w3, ulong** pp)
        __asm__("cPS2DeferManager_writeVu1ProgramSetup");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cPS2DeferManager();
};

CHECK_OFFSET(cPS2DeferManager, mNumSubmitDMA, 0x00000);
CHECK_OFFSET(cPS2DeferManager, unk_00004, 0x00004);
CHECK_OFFSET(cPS2DeferManager, mSubmits, 0x00080);
CHECK_OFFSET(cPS2DeferManager, mUsedDrawStates, 0x51480);
CHECK_OFFSET(cPS2DeferManager, mDrawStates, 0x51484);
CHECK_OFFSET(cPS2DeferManager, mNumStaticStates, 0x57484);
CHECK_OFFSET(cPS2DeferManager, mpPresetStates, 0x57488);
CHECK_OFFSET(cPS2DeferManager, mUsedStates, 0x57494);
CHECK_OFFSET(cPS2DeferManager, mBuckets, 0x57498);
CHECK_OFFSET(cPS2DeferManager, unk_67498, 0x67498);
CHECK_OFFSET(cPS2DeferManager, mStateHash, 0x674A0);
CHECK_OFFSET(cPS2DeferManager, mBucketHash, 0x678A0);
CHECK_OFFSET(cPS2DeferManager, mpLastBucket, 0x67CA0);
CHECK_OFFSET(cPS2DeferManager, mNumSortItems, 0x67CA4);
CHECK_OFFSET(cPS2DeferManager, mSortItems, 0x67CA8);
CHECK_OFFSET(cPS2DeferManager, mCurrentState, 0x69CA8);
CHECK_OFFSET(cPS2DeferManager, unk_69CBC, 0x69CBC);
CHECK_OFFSET(cPS2DeferManager, mActiveViewIndex, 0x69CC0);
CHECK_OFFSET(cPS2DeferManager, mActivePortIndex, 0x69CC4);
CHECK_OFFSET(cPS2DeferManager, mIndexView, 0x69CC8);
CHECK_OFFSET(cPS2DeferManager, unk_69CCC, 0x69CCC);
CHECK_OFFSET(cPS2DeferManager, mViewStates, 0x69CD0);
CHECK_OFFSET(cPS2DeferManager, mIndexPort, 0x6AED0);
CHECK_OFFSET(cPS2DeferManager, mViewPorts, 0x6AED4);
CHECK_OFFSET(cPS2DeferManager, unk_6B0D4, 0x6B0D4);
CHECK_SIZE(cPS2DeferManager, 0x6B100);

#endif /* RENDER_BXPS2DEFERMAN_H */
