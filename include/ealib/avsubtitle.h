#ifndef EALIB_AVSUBTITLE_H
#define EALIB_AVSUBTITLE_H

// RCMP::AV_SUBTITLE (one subtitle of a movie: position, size, string id, start frame, justification, shape and colour)
// and RCMP::AV_SUBTITLE_ARRAY (a counted array of them, searched by frame). EA source:
// rcmp/dev/source/av/cmn/avsubtitle.cpp (SSX On Tour's DWARF). Retail code: unit ealib/seg_2AEDC0,
// RCMP_AV_SUBTITLE_ARRAY_AV_SUBTITLE_ARRAY .. RCMP_AV_SUBTITLE_SetB (0x3AF000..0x3B0408), plus the subtitle-file parser
// func_003AF7F0 and its helpers (keywords "FRAME", "STRINGID", "WIDTH", "HEIGHT", "JUSTIFY_X", "JUSTIFY_Y", "SHAPE").
// The movie player (ealib/avplayer.h, +0x70) keeps the array; nothing in SSX 3 sets one (func_003AE2F8 has no caller).
//
// Names: On Tour's classes AV_SUBTITLE (0x34) and AV_SUBTITLE_ARRAY (0x8) in namespace RCMP (enum names
// RCMP::AV_SUBTITLE::JUST_*), with the same members at the same offsets as every retail access, and the project's function
// names (RCMP_AV_SUBTITLE_SetPosX ...), which follow On Tour's method names. No vtable (no virtual destructor: the array
// deletes its elements with delete[] and a cookie).
//
// FLAT layouts. Evidence tags: [R] retail constructor RCMP_AV_SUBTITLE_AV_SUBTITLE and RCMP_AV_SUBTITLE_ARRAY_Init's
// element size (count * 52 + 0x10 cookie); [O] On Tour's DWARF; [U] retail use (the setters write one field each).

#include "layout.h"

struct RCMP_AV_SUBTITLE {
    int mPosX;                          // 0x00 [O]; [R] ctor 0x20; [U] SetPosX
    int mPosY;                          // 0x04 [O]; [R] ctor 350; [U] SetPosY
    unsigned int mWidth;                // 0x08 [O]; [R] ctor 600; [U] SetWidth
    unsigned int mHeight;               // 0x0C [O]; [R] ctor 50; [U] SetHeight
    unsigned int mStrID;                // 0x10 [O]; [R] ctor 0xFFFFFFFF; [U] SetStrID (ARRAY_Init: the index)
    unsigned int mFrameNumber;          // 0x14 [O]; [R] ctor 0xFFFFFFFF; [U] SetFrameNumber; FindSubtitle compares it
    int mJustifyX;                      // 0x18 [O] JUSTIFY_ENUM (RIGHT 0, TOP 1, CENTER 2, LEFT 3, BOTTOM 4); [R] ctor 2
    int mJustifyY;                      // 0x1C [O] JUSTIFY_ENUM; [R] ctor 2; [U] SetJustifyY
    unsigned int mShapeIndex;           // 0x20 [O]; [R] ctor 0; [U] SetShapeIndex
    unsigned char mA;                   // 0x24 [O]; [R] ctor 0xFF (sb); [U] SetA
    unsigned char mR;                   // 0x25 [O]; [R] ctor 0xFF; [U] SetR
    unsigned char mG;                   // 0x26 [O]; [R] ctor 0xFF; [U] SetG
    unsigned char mB;                   // 0x27 [O]; [R] ctor 0xFF; [U] SetB
    void* mTexturePtr;                  // 0x28 [O]; [R] ctor 0
    void* mStringPtr;                   // 0x2C [O]; [R] ctor 0
    void* mWrapStringPtr;               // 0x30 [O]; [R] ctor 0
#ifdef __cplusplus
    ~RCMP_AV_SUBTITLE() {}              // [R] RCMP_AV_SUBTITLE_ARRAY__AV_SUBTITLE_ARRAY's delete[] reads the count
                                        //     cookie and runs an empty per-element destructor loop
    void operator delete[](void* p);    // [R] that delete[] frees through RCMP_rcmp_sys.FreeMemFunc (defined in the
                                        //     unit)
#endif

    // Methods bound to their retail symbols by asm labels.
    void * AV_SUBTITLE() __asm__("RCMP_AV_SUBTITLE_AV_SUBTITLE");
    void SetPosY(int value) __asm__("RCMP_AV_SUBTITLE_SetPosY");
    void SetWidth(int value) __asm__("RCMP_AV_SUBTITLE_SetWidth");
    void SetHeight(int value) __asm__("RCMP_AV_SUBTITLE_SetHeight");
    void SetStrID(int index) __asm__("RCMP_AV_SUBTITLE_SetStrID");
    void SetFrameNumber(int value) __asm__("RCMP_AV_SUBTITLE_SetFrameNumber");
    void SetJustifyX(int value) __asm__("RCMP_AV_SUBTITLE_SetJustifyX");
    void SetJustifyY(int value) __asm__("RCMP_AV_SUBTITLE_SetJustifyY");
    void SetShapeIndex(int value) __asm__("RCMP_AV_SUBTITLE_SetShapeIndex");
    void SetA(signed char value) __asm__("RCMP_AV_SUBTITLE_SetA");
    void SetR(signed char value) __asm__("RCMP_AV_SUBTITLE_SetR");
    void SetG(signed char value) __asm__("RCMP_AV_SUBTITLE_SetG");
    void SetB(signed char value) __asm__("RCMP_AV_SUBTITLE_SetB");
};

struct RCMP_AV_SUBTITLE_ARRAY {
    RCMP_AV_SUBTITLE* mSubTitle;        // 0x0 [O]; [R] ctor 0; [U] Init allocates count * 0x34 (+ a 0x10 cookie) from the
                                        //     RCMP allocator and constructs each; the dtor delete[]s it; GetSubtitle /
                                        //     FindSubtitle index it
    unsigned int mNumberOfSubtitle;     // 0x4 [O]; [R] ctor 0;
                                        //     [U] Init stores the count, GetNumberOfSubtitle returns it

    // Methods bound to their retail symbols by asm labels.
    void * AV_SUBTITLE_ARRAY() __asm__("RCMP_AV_SUBTITLE_ARRAY_AV_SUBTITLE_ARRAY");
    RCMP_AV_SUBTITLE* GetSubtitle(unsigned int i) __asm__("RCMP_AV_SUBTITLE_ARRAY_GetSubtitle");
    int GetNumberOfSubtitle() __asm__("RCMP_AV_SUBTITLE_ARRAY_GetNumberOfSubtitle");
    RCMP_AV_SUBTITLE* FindSubtitle(unsigned int key) __asm__("RCMP_AV_SUBTITLE_ARRAY_FindSubtitle");
    void Init(unsigned int count, int posX, int posY, int width, int height) __asm__("RCMP_AV_SUBTITLE_ARRAY_Init");
};

CHECK_OFFSET(RCMP_AV_SUBTITLE, mPosX, 0x00);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mPosY, 0x04);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mWidth, 0x08);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mHeight, 0x0C);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mStrID, 0x10);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mFrameNumber, 0x14);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mJustifyX, 0x18);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mJustifyY, 0x1C);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mShapeIndex, 0x20);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mA, 0x24);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mR, 0x25);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mG, 0x26);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mB, 0x27);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mTexturePtr, 0x28);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mStringPtr, 0x2C);
CHECK_OFFSET(RCMP_AV_SUBTITLE, mWrapStringPtr, 0x30);
CHECK_SIZE(RCMP_AV_SUBTITLE, 0x34);

CHECK_OFFSET(RCMP_AV_SUBTITLE_ARRAY, mSubTitle, 0x0);
CHECK_OFFSET(RCMP_AV_SUBTITLE_ARRAY, mNumberOfSubtitle, 0x4);
CHECK_SIZE(RCMP_AV_SUBTITLE_ARRAY, 0x8);

#endif /* EALIB_AVSUBTITLE_H */
