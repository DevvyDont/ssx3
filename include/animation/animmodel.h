#ifndef ANIMATION_ANIMMODEL_H
#define ANIMATION_ANIMMODEL_H

// cAnimModel: a skinned model made of parts, each with up to 4 LOD slots, plus a bone array and bone
// matrices. cRider::mAnimModel points to one.
//
// This is a FLAT layout built from how the matched methods in animation/animmodel use each offset
// (2026-10-17). It replaces 43 per-function overlay structs. Unlike ai/rider.h there is no preview evidence:
// the July preview has no accessors for this class. Only "cAnimModel", "ModelPart" and "LOD" are EA's own
// words (cAnimModel_addModelPartLOD, cAnimModel_compile). Every other name is ours, from usage, and the
// comment says which function shows it. Unknown size; only the fields below are known.

#include "layout.h"
#include "bx/math3d.h"

struct tTextureRef;             // render/ps2graphicsman.h

// One LOD's entry for one ref index: the source part block and its data (addModelPartLOD).
struct sAnimModelLodRef {
    char* part;     // 0x0 source part block (non-null = drawable, cAnimModel_render)
    char* data;     // 0x4 part + 0x60
    int unk_08;     // 0x8 passed to the renderer's set-matrices call (cAnimModel_render)
    int unk_0C;     // 0xC same
};

// A part's LOD slot. The 4 slots double as the part's "channels": enabling a part sets used on all 4
// (cAnimModel_enablePart), drawing checks the slot of the requested LOD (cAnimModel_render).
struct sAnimModelLod {
    int used;                   // 0x0
    sAnimModelLodRef* refs;     // 0x4 numLodRefs entries (cAnimModel_addModelPartLOD)
};

// 0x58 bytes, one per model part (cAnimModel::parts).
struct sAnimModelPart {
    // The board's part id (name ours). [O] On Tour's cAnimModel::render(int lod, bool additiveBoard, bool
    // additivePlayer); [U] retail's render passes its first flag for the part with this id, the second for the others.
    enum { kPartBoard = 2 };

    int id;                     // 0x00 = the source part's signed byte at +0x52; 2 (or 1..2) picks the draw argument
    int firstBone;              // 0x04 first bone of this part (cAnimModel_applyOffset start + j; cAnimModel_getBoneSkelIndex returns firstBone + i)
    int unk_08;
    int unk_0C;                 // 0x0C a channel bit; >= 0 when the part has one (cAnimModel_getGraftMask)
    unsigned long mask;         // 0x10 64-bit bone/channel mask OR'd into cAnimModel::mask (cAnimModel_enablePart)
    sAnimModelLod lods[4];      // 0x18
    char* boneNames;            // 0x38 numBones names of 0x50 bytes (cAnimModel_getBoneSkelIndex, cAnimModel_getGraftMask)
    char* HotSpots;               // 0x3C data + src[0x1C] (addModelPartLOD)
    char* unk_40;               // 0x40 data + src[0x2C]
    int numBones;               // 0x44 = (short)src[0x4A]; loop bound over this part's bones and names
    int unk_48;                 // 0x48 = (short)src[0x4E]
    int unk_4C;                 // 0x4C = (short)src[0x50]
    int refIndex;               // 0x50 which lods[].refs entry is current (addModelPartLOD, cAnimModel_setModelPartVariant)
    int unk_54;
};

// 0x20 bytes per bone (cAnimModel::bones).
struct sAnimModelBone {
    t4Vector pos;               // 0x00 translated with the model (cAnimModel_applyOffset)
    t4Vector rot;               // 0x10 a quaternion x, y, z, w (cRider_checkRailBoardCollisions)
};

struct sAnimModelLodInfo {
    int unk_00;                 // passed with unk_04 to the renderer when the model isn't per-part LOD'd (cAnimModel_render)
    int unk_04;
};

struct t4Vector;

struct cBxReplayFrame;

struct cAnimModel {
    /* 0x000 */ int MaxModelParts;          // [O] cAnimModel::MaxModelParts (init(int maxModels, int maxModelVariants));
                                            //     [R] cAnimModel_init stores its count here and allocates count * 0x58 parts
                                            //     (view cAnimModel_init_sPool count) [lstruct L2]
    /* 0x004 */ int numLodRefs;             // refs per LOD slot (addModelPartLOD allocates this many)
    /* 0x008 */ int numParts;               // loop bound over parts everywhere
    /* 0x00C */ sAnimModelPart* parts;
    /* 0x010 */ int numBones;               // bones/matrices count; also the first channel bit (cAnimModel_getGraftMask)
    /* 0x014 */ int maxPartIndex;           // i > this -> no such part (cAnimModel_enablePart, cAnimModel_getGraftMask)
    /* 0x018 */ int unk_018;                // nonzero: draw per-part LOD refs, else one call with lodInfo (cAnimModel_render)
    /* 0x01C */ int* partIndex;             // part number -> index into parts, < 0 = none
    /* 0x020 */ int* boneChannelIndex;      // per-bone first-component index into the channel data (func_0030ECD8: first = arr[k] - arr[firstBone])
    /* 0x024 */ t4Vector* localPos;         // numBones local bone positions, scaled componentwise by ScaleVect (cAnimModel_transform)
    /* 0x028 */ t4Vector* localRot;         // numBones local bone rotation quaternions (cAnimModel_transform)
    /* 0x02C */ sAnimModelBone* bones;      // numBones of them; streamed as numBones << 5 bytes (cAnimModel_writeToReplayFrame)
    /* 0x030 */ t4Matrix* SkelMatrices;          // numBones bone matrices, row 3 = position (rider bone lookups); translated with the model
    /* 0x034 */ t4Matrix* SkelMatricesScaled;          // numBones, translated with the model (cAnimModel_applyOffset)
    /* 0x038 */ t4Matrix* InvDefaultMatrices;          // numBones; palette[i] = SkelMatrices[i] or SkelMatricesScaled[i] times this (cAnimModel_rendershadow, cAnimModel_render)
    /* 0x03C */ float* MorphChannels; // [O] float*; [R] cAnimModel_compile allocates it ("MorphChannels")
    /* 0x040 */ sAnimModelLodInfo lodInfo[4];
    /* 0x060 */ int NumTextureFiles;        // [O] cAnimModel::NumTextureFiles; [R] cAnimModel_init zeroes it (view
                                            //     cAnimModel_init_sPool field_0x60); [U] cAnimModel_addTextureFile appends at
                                            //     TextureFiles[NumTextureFiles++] (view sAnimModelList count) [lstruct L2]
    /* 0x064 */ const char* TextureFiles[50]; // [O] cAnimModel::TextureFiles (const char*[50]; 0xC8 bytes end exactly at
                                            //     NumTextureRefs 0x12C, as in On Tour); [U] addTextureFile stores its data
                                            //     argument (view sAnimModelList items) [lstruct L2]
    /* 0x12C */ int NumTextureRefs;         // [O] cAnimModel::NumTextureRefs (+0x78C there, 0x14 before ScaleVect, as here);
                                            //     [R] cAnimModel_cAnimModel clears it between numParts and localPos, the same store
                                            //     order as On Tour's ctor (NumModelParts, NumTextureRefs, TextureRefs, BoneOffset)
    /* 0x130 */ tTextureRef* TextureRefs;   // [O] cAnimModel::TextureRefs (+0x790); [R] cAnimModel_cAnimModel clears it (see NumTextureRefs)
    /* 0x134 */ char unk_134[0xC];
    /* 0x140 */ t4Vector ScaleVect;           // = (a, b, b, 1) (cAnimModel_setScale); .y is the draw call's float (cAnimModel_render); .x scales rider lean (ai/rider)
    /* 0x150 */ unsigned long mask;         // 64-bit mask of enabled parts (cAnimModel_enablePart, cAnimModel_disableAllParts)
    /* 0x158 */ unsigned long AnimMinPartsMask;      // another such mask (cAnimModel_addAnimMinPart)

    // Methods bound to their retail symbols by asm labels.
    void addModelPartLOD(int lod, char* part, int index) __asm__("cAnimModel_addModelPartLOD");
    int isPartEnabled(int i, int channel) __asm__("cAnimModel_isPartEnabled");
    void addAnimMinPart(int i) __asm__("cAnimModel_addAnimMinPart");
    void buildSkelMatrices() __asm__("cAnimModel_buildSkelMatrices");
    void applyOffset(t4Vector* offset) __asm__("cAnimModel_applyOffset");
    int getBoneSkelIndex(int idx, const char* name) __asm__("cAnimModel_getBoneSkelIndex");
    ulong getGraftMask(int idx, char* names) __asm__("cAnimModel_getGraftMask");
    void setModelPartVariant(int i, int variant) __asm__("cAnimModel_setModelPartVariant");
    void render(int lod, int x, int y) __asm__("cAnimModel_render");
    void rendershadow(int lod, int x, int y) __asm__("cAnimModel_rendershadow");
    void readFromReplayFrame(cBxReplayFrame* data) __asm__("cAnimModel_readFromReplayFrame");
    void setScale(float lengthScale, float widthScale) __asm__("cAnimModel_setScale");
    void setSkelMatrix(int i, sAnimModelBone* animModelBone) __asm__("cAnimModel_setSkelMatrix");
    void transform(sAnimModelBone* roots) __asm__("cAnimModel_transform");
    void writeToReplayFrame(cBxReplayFrame* data) __asm__("cAnimModel_writeToReplayFrame");
    void init(int count, int maxVariants) __asm__("cAnimModel_init");
    void addTextureFile(int item) __asm__("cAnimModel_addTextureFile__FPvi");
};

CHECK_OFFSET(sAnimModelPart, mask, 0x10);
CHECK_OFFSET(sAnimModelPart, lods, 0x18);
CHECK_OFFSET(sAnimModelPart, boneNames, 0x38);
CHECK_OFFSET(sAnimModelPart, numBones, 0x44);
CHECK_OFFSET(sAnimModelPart, refIndex, 0x50);
CHECK_OFFSET(cAnimModel, MaxModelParts, 0x0);
CHECK_OFFSET(cAnimModel, numLodRefs, 0x4);
CHECK_OFFSET(cAnimModel, numParts, 0x8);
CHECK_OFFSET(cAnimModel, parts, 0xC);
CHECK_OFFSET(cAnimModel, numBones, 0x10);
CHECK_OFFSET(cAnimModel, maxPartIndex, 0x14);
CHECK_OFFSET(cAnimModel, unk_018, 0x18);
CHECK_OFFSET(cAnimModel, partIndex, 0x1C);
CHECK_OFFSET(cAnimModel, boneChannelIndex, 0x20);
CHECK_OFFSET(cAnimModel, localPos, 0x24);
CHECK_OFFSET(cAnimModel, localRot, 0x28);
CHECK_OFFSET(cAnimModel, bones, 0x2C);
CHECK_OFFSET(cAnimModel, SkelMatrices, 0x30);
CHECK_OFFSET(cAnimModel, SkelMatricesScaled, 0x34);
CHECK_OFFSET(cAnimModel, InvDefaultMatrices, 0x38);
CHECK_OFFSET(cAnimModel, lodInfo, 0x40);
CHECK_OFFSET(cAnimModel, NumTextureFiles, 0x60);
CHECK_OFFSET(cAnimModel, TextureFiles, 0x64);
CHECK_OFFSET(cAnimModel, NumTextureRefs, 0x12C);
CHECK_OFFSET(cAnimModel, TextureRefs, 0x130);
CHECK_OFFSET(cAnimModel, ScaleVect, 0x140);
CHECK_OFFSET(cAnimModel, mask, 0x150);
CHECK_OFFSET(cAnimModel, AnimMinPartsMask, 0x158);
typedef char check_sAnimModelPart_size[(sizeof(sAnimModelPart) == 0x58) ? 1 : -1];
typedef char check_sAnimModelBone_size[(sizeof(sAnimModelBone) == 0x20) ? 1 : -1];
typedef char check_sAnimModelLodRef_size[(sizeof(sAnimModelLodRef) == 0x10) ? 1 : -1];

#endif
