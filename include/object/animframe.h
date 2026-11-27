#ifndef OBJECT_ANIMFRAME_H
#define OBJECT_ANIMFRAME_H

// The animation-frame evaluators: a cAnimFrame samples a tAnimRecord's up-to-16 cubic channels at a time into Vals[16]
// (translation 0..2, rotation 3..5, more channels after), remembering each channel's current segment (Seg[16], the
// cAnimFrameState base) so the next update starts there. Two subclasses add nothing but behaviour or a matrix:
//   cCamAnimFrame  - a camera path frame: position Vals[0..2], angles Vals[3..5], focal length Vals[11];
//   cMeshAnimFrame - an animated mesh's frame: update also builds FrameMatrix from the translation/rotation.
// cAnimNode keeps an array of cMeshAnimFrame (AnimFrames, one per animated mesh; cAnimNode_setAnimMeshCache) and the
// crowd node another (func_003443A8, "CrowdAnimMesh"). EA's source file is unknown (no preview assert names it; the
// preview keeps the inlines getAngles__C10cAnimFrameR8t4Vector, getFrameMatrix__C14cMeshAnimFrame,
// init__14cMeshAnimFrame, setAnimRecord__13cCamAnimFrameP11tAnimRecordf, getPosition__C13cCamAnimFrameR8t4Vector,
// getAngles__C13cCamAnimFrameR8t3Vector, getFocalLength__C13cCamAnimFrameRf and the vtables _vt$10cAnimFrame,
// _vt$14cMeshAnimFrame); this header is named after the class, in the units' directory (object/instanceman holds the
// out-of-line methods 0x00351400..0x00351B00, object/railmodifier the inlines 0x003611A8..0x00361400).
// Vtables (2 slots each: 1 dtor, 2 update(float)): cAnimFrame_vtbl 0x00490AF0, cCamAnimFrame_vtbl 0x00490AB0,
// cMeshAnimFrame_vtbl 0x00490AD0. Every derived ctor/dtor stores cAnimFrame_vtbl first/last (base part).
//
// Sizes:
//   cAnimFrame 0x88: [O] On Tour 0x88; [R] the vptr at 0x84 is the last word any ctor writes; cMeshAnimFrame's own data
//     starts at 0x90 (the 16-aligned t4Matrix after a 0x88-byte base). No allocation of a plain cAnimFrame in retail.
//   cCamAnimFrame 0x88: [O] On Tour 0x88, no members of its own; [R] its ctors/dtor touch nothing past 0x84.
//   cMeshAnimFrame 0xD0: [R] cAnimNode_setAnimMeshCache (object/animnode, "IntersectInfo") and func_003443A8
//     (object/debouncenode, "CrowdAnimMesh") allocate arrays with operator new[] whose element (sMeshInst_E448 /
//     sDbMesh_43A8) is 0xD0 with the default ctor inlined, and walk them with stride 0xD0; [O] On Tour 0xD0.
// Bases: cAnimFrame = cAnimFrameState (On Tour class, 0x40: int Seg[16]) + its own data; it declares the virtuals, so
// its vptr follows its data (0x84). cCamAnimFrame and cMeshAnimFrame derive from cAnimFrame (written out inline).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors cAnimFrame_cAnimFrame(record, time) / cAnimFrame_cAnimFrame1() / cAnimFrame_init, the derived ctors
//       and dtors, cMeshAnimFrame_init, and the array allocations above;
//   [P] the preview's out-of-line inlines listed above (tools/layout_xlate.py: preview == retail at 0x0, 0x40, 0x84,
//       0xC0..0xCC);
//   [U] how retail code uses the field (src/object/instanceman.cpp, src/object/railmodifier.cpp, the retail asm);
//   [O] SSX On Tour's DWARF (cAnimFrameState { int Seg[16]; }, cAnimFrame { AnimRecord, Vals[16] }, cCamAnimFrame,
//       cMeshAnimFrame { FrameMatrix }; tAnimRecord_PSP: translation, rotation, m_ChannelMask 0x18, m_NumChannels
//       0x1C, m_pChannels 0x20): same offsets as retail.

#include "layout.h"
#include "bx/math3d.h"

struct tAnimRecord;

struct cBxReplayFrame;
struct t4Vector;

struct cAnimFrame {
    // --- cAnimFrameState (On Tour 0x40)
    int Seg[16];                    // 0x000 [O] cAnimFrameState::Seg; [R] cAnimFrame_init MEM_fill(this, 0, 0x40); [U]
                                    //       update / getDeriv keep each active channel's current segment index here
                                    //       (cAnimChannel_nextSegment(curve, &Seg[i], t)); write/readFromReplayFrame
                                    //       move exactly these 0x40 bytes
    // --- cAnimFrame
    const tAnimRecord* AnimRecord;  // 0x040 [O]; [R] init zeroes it; [P] cCamAnimFrame::setAnimRecord (sw 0x40); [U]
                                    //       setAnimRecord stores it; update / getDeriv read its channel mask (+0x18) and
                                    //       channel list (+0x20); getStartTime reads the first channel's first segment
    float Vals[16];                 // 0x044 [O]; [R] init MEM_fill(this + 0x44, 0, 0x40); [U] setAnimRecord copies the
                                    //       record's translation / rotation into Vals[0..5]; update evaluates each active
                                    //       channel into Vals[i]; getAngles returns -Vals[3..5]; cCamAnimFrame reads
                                    //       Vals[0..2] / [3..5] / [11] (0x70); cMeshAnimFrame_update reads Vals[0..5]
    void** mVtbl;                   // 0x084 [R] cAnimFrame_vtbl (0x00490AF0) in both ctors and the dtor; [P] 0x84 in the
                                    //       preview's ~cAnimFrame

    // Methods bound to their retail symbols by asm labels.
    void setAnimRecord(float* src) __asm__("cAnimFrame_setAnimRecord");
    void init() __asm__("cAnimFrame_init");
    void writeToReplayFrame(cBxReplayFrame* sink) __asm__("cAnimFrame_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* sink) __asm__("cAnimFrame_readFromReplayFrame");
    float getStartTime() __asm__("cAnimFrame_getStartTime");
    void getDeriv(t4Vector* vel, t4Vector* rotVel, float time) __asm__("cAnimFrame_getDeriv");
    void* cAnimFrame1() __asm__("cAnimFrame_cAnimFrame1");
    void getAngles(t4Vector* out) __asm__("cAnimFrame_getAngles");
    void update(float time) __asm__("cAnimFrame_update");
};

CHECK_OFFSET(cAnimFrame, Seg, 0x000);
CHECK_OFFSET(cAnimFrame, AnimRecord, 0x040);
CHECK_OFFSET(cAnimFrame, Vals, 0x044);
CHECK_OFFSET(cAnimFrame, mVtbl, 0x084);
CHECK_SIZE(cAnimFrame, 0x88);

struct t3Vector;

struct cCamAnimFrame {
    // --- cAnimFrame (0x88; above)
    int Seg[16];                    // 0x000 [O] cAnimFrameState::Seg; [R] cAnimFrame_init (via the base ctor)
    const tAnimRecord* AnimRecord;  // 0x040 [O]; [P] setAnimRecord__13cCamAnimFrameP11tAnimRecordf (sw 0x40);
                                    //       [U] cCamAnimFrame_setAnimRecord stores it
    float Vals[16];                 // 0x044 [O]; [P] getPosition / getAngles / getFocalLength; [U] getPosition returns
                                    //       {Vals[0..2], 1}, getAngles Vals[3..5] (a t3Vector), getFocalLength Vals[11]
    void** mVtbl;                   // 0x084 [R] cCamAnimFrame_vtbl (0x00490AB0) in both ctors; the dtor restores
                                    //       cAnimFrame_vtbl; [P] 0x84 in the preview's ~cCamAnimFrame

    // Methods bound to their retail symbols by asm labels.
    void* cCamAnimFrame1() __asm__("cCamAnimFrame_cCamAnimFrame1");
    void setAnimRecord(const tAnimRecord* source) __asm__("cCamAnimFrame_setAnimRecord");
    void getPosition(t4Vector* out) __asm__("cCamAnimFrame_getPosition");
    void getAngles(t3Vector* out) __asm__("cCamAnimFrame_getAngles");
    void getFocalLength(float *out) __asm__("cCamAnimFrame_getFocalLength");
    void* update() __asm__("cCamAnimFrame_update__FPv");
};

CHECK_OFFSET(cCamAnimFrame, Seg, 0x000);
CHECK_OFFSET(cCamAnimFrame, AnimRecord, 0x040);
CHECK_OFFSET(cCamAnimFrame, Vals, 0x044);
CHECK_OFFSET(cCamAnimFrame, mVtbl, 0x084);
CHECK_SIZE(cCamAnimFrame, 0x88);

struct cMeshAnimFrame {
    // --- cAnimFrame (0x88; above)
    int Seg[16];                    // 0x000 [O] cAnimFrameState::Seg; [R] cAnimFrame_init (via the base ctor)
    const tAnimRecord* AnimRecord;  // 0x040 [O]; [R] the (record, time) ctor stores it again after the base ctor;
                                    //       cAnimNode_setAnimMeshCache / func_003443A8 set it with cAnimFrame_setAnimRecord
    float Vals[16];                 // 0x044 [O]; [U] update reads Vals[0..5] (translation, rotation)
    void** mVtbl;                   // 0x084 [R] cMeshAnimFrame_vtbl (0x00490AD0) in both ctors and the inlined array
                                    //       ctor; the dtor restores cAnimFrame_vtbl
    char unk_088[0x8];              // 0x088 padding (FrameMatrix is 16-byte aligned)
    // --- cMeshAnimFrame
    t4Matrix FrameMatrix;           // 0x090 [O]; [R] cMeshAnimFrame_init and the (record, time) ctor copy t4mat_Identity
                                    //       into it; [P] getFrameMatrix__C14cMeshAnimFrame; [U] update writes all 16
                                    //       words from Vals[0..5]; transform multiplies it; getFrameMatrix returns
                                    //       this + 0x90

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cMeshAnimFrame_update");
    void* cMeshAnimFrame1() __asm__("cMeshAnimFrame_cMeshAnimFrame1");
    void init() __asm__("cMeshAnimFrame_init");
};

CHECK_OFFSET(cMeshAnimFrame, Seg, 0x000);
CHECK_OFFSET(cMeshAnimFrame, AnimRecord, 0x040);
CHECK_OFFSET(cMeshAnimFrame, Vals, 0x044);
CHECK_OFFSET(cMeshAnimFrame, mVtbl, 0x084);
CHECK_OFFSET(cMeshAnimFrame, unk_088, 0x088);
CHECK_OFFSET(cMeshAnimFrame, FrameMatrix, 0x090);
CHECK_SIZE(cMeshAnimFrame, 0xD0);

#endif /* OBJECT_ANIMFRAME_H */
