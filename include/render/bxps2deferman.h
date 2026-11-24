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
// cPS2DeferManager_cPS2DeferManager. No base class, no vtable (the ctor stores none; no vtable in vtables.tsv).
// The ctor (render/font.cpp, view sFontCache) fills the sDrawState texture ids of the 0xA28 submit records, the 0x400
// draw-state list entries and the current state with -1 (sTextureState ctor) and runs the (empty) constructors of the
// 0x800 buckets and the 12 view states.
//
// A FLAT layout: every field at its retail offset. Evidence tags: [R] retail constructor / allocation; [P] the July
// 2003 preview's assert strings in bxps2deferman.cpp/.h, which name the counters (the preview's offsets are smaller:
// kMaxDrawStates etc. differ, so each was mapped by the counter's role); [U] retail use (render/font.cpp
// cPS2DeferManager_Find / Find1; object/railmodifier.cpp func_003629B8 .. func_00364CD0; render/bezierman.cpp;
// render/particle.cpp cPS2GraphicsMan_RenderModel and the sPtRing* views); [X] refs/ssx3-rust's render notes
// (docs/investigation/phase3-render.md: "Record", "Bucket", "MatrixRecord"; names are guesses).
// The element struct names other than cDrawStateList ([P]) are ours (tDefer*).

#include "layout.h"
#include "bx/math3d.h"
#include "render/ps2graphicsman.h"

// One submitted draw (0x80): [U] func_00362DE8 (view sRmEntry_2DE8) walks the mNumSubmitDMA of them;
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

// One bucket (0x20): a (state, texture, view) combination and its DMA chain. [U] func_00362DE8 / func_00363490
// (views sRmNode_2DE8 / sRmNode_3490), cPS2DeferManager_Find1, func_00362978 (appends a packet: self = a bucket).
// [X] "Bucket".
struct tDeferBucket {
    cDrawStateList* mpState;        // 0x00 [P] Find(const cDrawStateList*, ...); [U] Find1 compares it
    int mTexKey;                    // 0x04 [P] cTexKey (passed by value; two shorts); [U] Find1 compares it
    int mViewIndex;                 // 0x08 [P] Find's int argument; [U] Find1 compares it (sRmNode::layer/mtxIndex)
    int mPacket;                    // 0x0C [U] func_00362978: first packet (0 = empty)
    int mTail;                      // 0x10 [U] func_00362978: last packet's end (the next packet is chained there)
    int mLinked;                    // 0x14 [U] func_00362DE8: set for a continuation bucket (not hashed)
    tDeferBucket* mpLink;           // 0x18 [U] func_00362DE8 / func_00363490: the continuation chain
    tDeferBucket* mpNext;           // 0x1C [U] the hash chain (mBucketHash) Find1 walks
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

// One sort element (8): [U] func_00364050 radix-sorts them by key (4 passes of 8 bits); [P] "length <=
// kMaxSortElements" (1024) in the preview's sort.
struct tDeferSortItem {
    unsigned int mKey;              // 0x0 [U] func_00364240(state, texKey) (sRmSortItem::key)
    tDeferBucket* mpBucket;         // 0x4 [U] (sRmSortItem::node)
};

CHECK_OFFSET(tDeferSortItem, mKey, 0x0);
CHECK_OFFSET(tDeferSortItem, mpBucket, 0x4);
CHECK_SIZE(tDeferSortItem, 0x8);

// One view state (0x180): the matrices and viewport constants a bucket's packets are drawn with. [U] func_003645B8
// (view sRailXf_45B8) interns them, comparing 0xE0..0x17F; [X] "MatrixRecord" (member names below).
struct tDeferViewState {
    t4Matrix unk_000;               // 0x000 [U] copied (m0); [X] world
    t4Matrix unk_040;               // 0x040 [U] copied (m1); [X] view
    t4Matrix unk_080;               // 0x080 [U] copied (m2); [X] worldView
    t4Vector unk_0C0;               // 0x0C0 [U] copied (v0); [X] guardScale
    t4Vector unk_0D0;               // 0x0D0 [U] copied (v1); [X] guardOffset
    t4Matrix unk_0E0;               // 0x0E0 [U] compared + copied (key0); [X] viewProj
    t4Matrix unk_120;               // 0x120 [U] compared + copied (key1); [X] worldClip
    t4Vector unk_160;               // 0x160 [U] compared + copied (key2); [X] screenScale
    t4Vector unk_170;               // 0x170 [U] compared + copied (key3); [X] screenOffset
};

CHECK_OFFSET(tDeferViewState, unk_000, 0x000);
CHECK_OFFSET(tDeferViewState, unk_040, 0x040);
CHECK_OFFSET(tDeferViewState, unk_080, 0x080);
CHECK_OFFSET(tDeferViewState, unk_0C0, 0x0C0);
CHECK_OFFSET(tDeferViewState, unk_0D0, 0x0D0);
CHECK_OFFSET(tDeferViewState, unk_0E0, 0x0E0);
CHECK_OFFSET(tDeferViewState, unk_120, 0x120);
CHECK_OFFSET(tDeferViewState, unk_160, 0x160);
CHECK_OFFSET(tDeferViewState, unk_170, 0x170);
CHECK_SIZE(tDeferViewState, 0x180);

// One view port / scissor rectangle (8): [U] func_00364B88 (view sRailKey4) interns them; func_00363C20 (view
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

struct cPS2DeferManager {
    int mNumSubmitDMA;              // 0x00000 [P] "mNumSubmitDMA < kMaxSubmitDMA" reads the manager's first word;
                                    //         [U] func_00362CC8 0; func_00362DE8 loops over it (sRmBatch_2DE8::count)
    char unk_00004[0x7C];           // 0x00004 not accessed
    tDeferSubmit mSubmits[0xA28];   // 0x00080 [R] ctor (2600 records, stride 0x80); [U] func_00362DE8 (entries)
    int mUsedDrawStates;            // 0x51480 [P] "mUsedDrawStates < kMaxDrawStates" in Find, "mUsedDrawStates ==
                                    //         kMaxStaticStates" after the presets; [U] Find appends at it; func_00362CC8
                                    //         resets it to mNumStaticStates
    cDrawStateList mDrawStates[0x400]; // 0x51484 [R] ctor (1024); [P] kMaxDrawStates 1024; [U] Find (sGlyphCache::entries)
    int mNumStaticStates;           // 0x57484 [U] func_003629B8 0, then the count after the 3 presets
                                    //         (sRmBatch_29B8::base); func_00362CC8 copies it to mUsedDrawStates
    cDrawStateList* unk_57488;      // 0x57488 [U] func_003629B8: preset state a
    cDrawStateList* unk_5748C;      // 0x5748C [U] func_003629B8: preset state b
    cDrawStateList* unk_57490;      // 0x57490 [U] func_003629B8: preset state c
    int mUsedStates;                // 0x57494 [P] "mUsedStates < kMaxStates" (preview kMaxStates 2048) after a failed
                                    //         bucket Find; [U] func_00362CC8 0; func_00362DE8 appends at it (nNodes)
    tDeferBucket mBuckets[0x800];   // 0x57498 [R] ctor (2048 empty ctors); [U] func_00362DE8 (nodes)
    char unk_67498[0x8];            // 0x67498 not accessed (sRmBatch_2DE8 places an unused stateHash here)
    cDrawStateList* mStateHash[0x100]; // 0x674A0 [U] Find walks bucket [hash]; func_00362CC8 invalidates/clears it
    tDeferBucket* mBucketHash[0x100];  // 0x678A0 [U] Find1 walks bucket [hash]; func_00362DE8 pushes; func_00362CC8 clears
    tDeferBucket* mpLastBucket;     // 0x67CA0 [U] func_00362DE8: the previous bucket (continuations chain to it)
    int mNumSortItems;              // 0x67CA4 [P] "length <= kMaxSortElements"; [U] func_00362CC8 0; func_00362DE8
                                    //         appends; func_00364050 sorts that many; func_00363490 walks them
    tDeferSortItem mSortItems[0x400]; // 0x67CA8 [P] kMaxSortElements 1024; [U] func_00364050 (src), func_00363490
    sDrawState mCurrentState;       // 0x69CA8 [R] ctor (texture ids -1); [U] func_00362CC8 = sDrawState::s_defaultDrawState;
                                    //         func_00364C90 copies one in; func_00363C20 reads it
    unsigned int unk_69CBC;         // 0x69CBC [U] func_00363C20 (sRmRend3C20::frame)
    int mActiveViewIndex;           // 0x69CC0 (name ours) [U] the active view state: func_00362CC8 -1; func_003645B8
                                    //         sets the index it found or added (sRailXfTable_45B8::found); the
                                    //         submitters store it as tDeferSubmit::mViewIndex
    int mActivePortIndex;           // 0x69CC4 [P] "mActivePortIndex >= 0" (the word after the view index); [U]
                                    //         func_00362CC8 -1; func_00364B88 sets it (sRailKeyTable::found)
    int mIndexView;                 // 0x69CC8 [P] "mIndexView < kMaxNumViewState"; [U] func_00362CC8 0;
                                    //         func_003645B8 appends at it (sRailXfTable_45B8::count)
    int unk_69CCC;                  // 0x69CCC not accessed (alignment of mViewStates)
    tDeferViewState mViewStates[12];// 0x69CD0 [R] ctor (12 empty ctors); [U] func_003645B8 (entries)
    int mIndexPort;                 // 0x6AED0 [P] "mIndexPort < kMaxNumViewPort"; [U] func_00362CC8 0; func_00364B88
                                    //         appends at it (sRailKeyTable::count)
    tDeferViewPort mViewPorts[64];  // 0x6AED4 [P] kMaxNumViewPort 64; [U] func_00364B88 (keys), func_00363C20
    char unk_6B0D4[0x2C];           // 0x6B0D4 not accessed by any matched or asm method (size from the allocation)
};

CHECK_OFFSET(cPS2DeferManager, mNumSubmitDMA, 0x00000);
CHECK_OFFSET(cPS2DeferManager, unk_00004, 0x00004);
CHECK_OFFSET(cPS2DeferManager, mSubmits, 0x00080);
CHECK_OFFSET(cPS2DeferManager, mUsedDrawStates, 0x51480);
CHECK_OFFSET(cPS2DeferManager, mDrawStates, 0x51484);
CHECK_OFFSET(cPS2DeferManager, mNumStaticStates, 0x57484);
CHECK_OFFSET(cPS2DeferManager, unk_57488, 0x57488);
CHECK_OFFSET(cPS2DeferManager, unk_5748C, 0x5748C);
CHECK_OFFSET(cPS2DeferManager, unk_57490, 0x57490);
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
