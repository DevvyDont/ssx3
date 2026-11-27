#ifndef UI_UITHING_H
#define UI_UITHING_H

// cUIThing: the base of every object on a UI screen (cUIGroup, cUIText, cUIIcon, cUIMenu, cUIPair, cUIListBox,
// cUIProgress, cUIScroll, cUISlider, cUITextScroll, cUIVector). cUIScreen_createObjectByStruct builds each kind by
// calling the cUIThing constructor cUIThing_cUIThing(this, pScreen, pParent) and then storing the derived vtable; derived
// members all start at 0x74 (cUIGroup = 0x74 + a 0x1C cList = 0x90 bytes, tag "cUIGroup"), so cUIThing is 0x74 bytes.
//
// A real class: cUIThing : cListNode (util/list.h: mpPrev 0x0, mpNext 0x4, the vptr at 0x8), its own members at
// their retail offsets from 0xC. The derived widgets' headers still copy these fields (FLAT). cUIThing's vtable
// _vt$8cUIThing (cUIThing_vtbl) keeps cListNode's traverseFlat/traverseRecurse (cListNode_traverseFlat/cListNode_traverseRecurse) in
// slots 2-3. Objects are linked into their group's cList through mpPrev/mpNext.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIThing), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (getAnimation lw 0xC ... getScreen lw 0x68). Retail keeps their
//       out-of-line copies in the same order as the preview (cUIThing_isPlayingAnimation isPlayingAnimation ... func_003A4CB0 setSize,
//       in ui/uivector), which places each one: unshifted up to mScale, then mpScreen moved in front of mSize;
//   [R] retail's constructor cUIThing_cUIThing (stores every field; pScreen -> 0x5C, pParent -> 0x40);
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail = On Tour - 4 from mColour on (On Tour aligns tARGB to 16), and has no field after mSound.
//
// The attribute word mAttr is EA's tAttrThing bitfield (On Tour, LSB first): mOrientation:1 mActive:1 mCached:1
// mEnabled:1 mFocused:1 mStatic:1 mVisible:1 mWrap:1 mLayer:5 mPivot:6 (struct tAttrThing below). Retail code reads and
// writes it as a whole word (lw/sw 0x14), so it is an int here.

#include "layout.h"
#include "bx/math3d.h"
#include "util/list.h"

struct cUIAnimation;
struct cUIScreen;
struct tUIBaseObject;
struct cUIInput;
struct cUIRenderContext;
struct cUIEngine;

// EA's colour (type name from the preview's setColour__8cUIThingRC5tARGB). 16 bytes, only 4-byte aligned on the PS2
// (it sits at cUIThing+0x1C; copied with two ldl/ldr pairs). Members from On Tour's DWARF (struct tARGB: A R G B).
struct tARGB {
    float A, R, G, B;
};

// EA's tAttrThing, the type of cUIThing::mAttr (On Tour struct tAttrThing, size 0x4, LSB first). cUIThing keeps mAttr
// an int (every accessor uses lw/sw 0x14); this type names the bits for code that reads one field, e.g.
// ((tAttrThing*)&thing->mAttr)->mPivot. [R] tAttrThing_tAttrThing (the ctor: sets bits 0, 1, 3, 6, clears 2, 4, 5, 7,
// mLayer = 0, mPivot = 9); [P] the inline accessors isActive .. isWrap, get/setDrawLayer, get/setPivot.
struct tAttrThing {
    unsigned int mOrientation : 1;   // bit 0 [O]; [P] isHorizontal
    unsigned int mActive : 1;        // bit 1 [O]; [P] isActive / setActive
    unsigned int mCached : 1;        // bit 2 [O]; [P] isCached / setCached (vtable slot 5)
    unsigned int mEnabled : 1;       // bit 3 [O]; [P] isEnabled / setEnabled (slot 6)
    unsigned int mFocused : 1;       // bit 4 [O]; [P] isFocused / setFocus (slot 7); [U] cUIMenu_setFocus sets bit 4
    unsigned int mStatic : 1;        // bit 5 [O]; [P] isStatic / setStatic (slot 8)
    unsigned int mVisible : 1;       // bit 6 [O]; [P] isVisible / setVisible (slot 9)
    unsigned int mWrap : 1;          // bit 7 [O]; [P] isWrap
    unsigned int mLayer : 5;         // bits 8-12 [O]; [P] getDrawLayer / setDrawLayer
    unsigned int mPivot : 6;         // bits 13-18 [O]; [P] getPivot / setPivot; [U] cScreenPopup_changePivot and
                                     //       cUIVector_setChannelDataKCP channel 12 write bits 13-18
};

struct t3Vector;

// The bits of tAttrThing::mPivot, the anchor a thing's position refers to (names ours). [U] cUIThing_applyPivot:
// with kPivotTop set it leaves y alone, with kPivotBottom it subtracts the height, otherwise (kPivotMiddle) half of it;
// with kPivotLeft it leaves x alone, with kPivotRight it subtracts the width, otherwise (kPivotCenter) half of it.
// [R] tAttrThing_tAttrThing sets kPivotTopLeft (9); [U] cScreenPopup_changePivot moves x by half a width between the
// left and centre anchors.
enum tUIPivot {
    kPivotTop = 1,
    kPivotMiddle = 2,
    kPivotBottom = 4,
    kPivotLeft = 8,
    kPivotCenter = 16,
    kPivotRight = 32,
    kPivotTopLeft = kPivotTop | kPivotLeft,
    kPivotMiddleLeft = kPivotMiddle | kPivotLeft,
    kPivotTopCenter = kPivotTop | kPivotCenter,
    kPivotMiddleCenter = kPivotMiddle | kPivotCenter
};

struct cUIThing : public cListNode {
    // cListNode (util/list.h): mpPrev 0x000 and mpNext 0x004 ([R] the constructor self-links them; [U] walked by the
    // cList code), the vptr 0x008 ([R] _vt$8cUIThing (cUIThing_vtbl), stored by cUIThing_cUIThing and cUIThing_cUIThing1)
    cUIAnimation* mpAnim;            // 0x00C [P] getAnimation/isPlayingAnimation (lw 0xC; retail cUIThing_getAnimation/cUIThing_isPlayingAnimation);
                                     //       [U] cUIThing_playAnimation stores it, cUIThing_deleteAnimation deletes it
    unsigned char mAnimPlaybackStatus; // 0x010 [O]; [R] = 0; [U] cUIThing_playAnimation stores the playback mode, cUIThing_update passes it on
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] tAttrThing: isActive..isWrap, get/setDrawLayer, get/setPivot (lw/sw 0x14;
                                     //       retail cUIThing_isHorizontal..cUIThing_getPivot, cUIThing_setActive..setVisible); [R] ctor sets it
    int mMiscData;                   // 0x018 [P] getMiscData/setMiscData (lw/sw 0x18; retail cUIThing_getMiscData/cUIThing_setMiscData); [R] = 0
    tARGB mColour;                   // 0x01C [P] setColour/getColour (0x1C; retail cUIThing_setColour, cUIThing_getColour/cUIThing_getColour1);
                                     //       [R] = {1,1,1,1}; [U] keyframe channels 13-16 (x 1/255) in cUIThing_setChannelData
    t3Vector mAngle;                 // 0x02C [O]; [P] getRadAngle/setRadAngle = mAngle.z (lwc1/swc1 0x34; retail cUIThing_getRadAngle/cUIThing_setRadAngle);
                                     //       [R] = t3vect_Zero; [U] channels 3-5 (degrees -> radians)
    int mHashName;                   // 0x038 [P] getHashName/setHashName (lw/sw 0x38; retail cUIThing_getHashName/cUIThing_setHashName);
                                     //       [U] compared by cUIThing_getObjectByHashName; set from the object record (cUIScreen_createAllObjects)
    // Keyframe channels (names ours). [U] cUIThing_setChannelData / setChannelDataKCP: position, angle in degrees,
    // size, scale in percent, the pivot bits of mAttr and the colour in 0-255; cUIVector_setChannelData takes the
    // channels from kUIChannel_FirstVertex on as vertex data and passes the rest here.
    enum eUIChannel {
        kUIChannel_PosX = 0, kUIChannel_PosY = 1, kUIChannel_AngleX = 3, kUIChannel_AngleY = 4,
        kUIChannel_AngleZ = 5, kUIChannel_SizeX = 6, kUIChannel_SizeY = 7, kUIChannel_ScaleX = 9,
        kUIChannel_ScaleY = 10, kUIChannel_Pivot = 12, kUIChannel_Alpha = 13, kUIChannel_Red = 14,
        kUIChannel_Green = 15, kUIChannel_Blue = 16, kUIChannel_FirstVertex = 21
    };
    unsigned short* mpEvents;        // 0x03C [O]; [R] = 0; [U] keyframer event per index, 0xFFFF = none (cUIThing_getKeyframerEvent);
                                     //       cUIThing_setUIData points it after the object record
    cUIThing* mpParent;              // 0x040 [P] getParent/setParent (lw/sw 0x40; retail cUIThing_getParent/cUIThing_setParent); [R] = pParent;
                                     //       [U] cUIThing_getAbsPosition sums mPosition up the parent chain
    t3Vector mPosition;              // 0x044 [P] getPosition/setPosition (retail cUIThing_getPosition/cUIThing_setPosition); [U] channels 0-1
    t3Vector mScale;                 // 0x050 [P] getScale/setScale (retail cUIThing_getScale/cUIThing_setScale); [R] = {1,1,1}; [U] channels 9-10 (x 0.01)
    cUIScreen* mpScreen;             // 0x05C [P] getScreen (preview lw 0x68; retail cUIThing_getScreen lw 0x5C); [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] getSize/setSize (preview 0x5C; retail cUIThing_getSize/setSize, vtable slots 20/21);
                                     //       [U] channels 6-7; cUIText wraps at mSize.x
    int mSound[2];                   // 0x06C [O] (On Tour: int mSound[2]); [R] = {4, 6}; [U] cUIThing_getSound/cUIThing_setSound get/set by index

    // EA's inline accessor ([P] the preview's setMiscData, sw 0x18; retail's out-of-line copy cUIThing_setMiscData).
    void setMiscData(int data) { mMiscData = data; }
    // EA's inline draw-layer accessors, bits 8-12 of mAttr ([P] the preview's getDrawLayer / setDrawLayer; retail's
    // out-of-line copies cUIThing_getDrawLayer / cUIThing_setDrawLayer; inlined in cScreenPopup_adjustLayer).
    int getDrawLayer() { return ((unsigned int)mAttr >> 8) & 0x1F; }
    void setDrawLayer(int layer) { mAttr = (mAttr & ~0x1F00) | ((layer & 0x1F) << 8); }

    // Virtuals. Slots [R] cUIThing_vtbl (slots 2-3 are cListNode's traverseFlat / traverseRecurse, not overridden),
    // names and order [O] (On Tour class cUIThing); parameter types from the retail implementations (the bool
    // parameters are words: cUIThing_setActive(cUIThing*, int)), pointers where On Tour passes references. Slot 22 is
    // getType: pure here ([R] cUIThing_vtbl[22] = __pure_virtual; On Tour's DWARF drops pure entries), and every
    // subclass vtable fills it with its own <class>_getType ([O] each subclass declares virtual char getType()).
    virtual ~cUIThing();                                                  // 1  cUIThing__cUIThing
    virtual void setActive(int a)
        __asm__("cUIThing_setActive");                                        // 4  cUIThing_setActive
    virtual void setCached(int a)
        __asm__("cUIThing_setCached");                                        // 5  cUIThing_setCached
    virtual void setEnabled(int a)
        __asm__("cUIThing_setEnabled");                                       // 6  cUIThing_setEnabled
    virtual void setFocus(int a)
        __asm__("cUIThing_setFocus");                                         // 7  cUIThing_setFocus
    virtual void setStatic(int a)
        __asm__("cUIThing_setStatic");                                        // 8  cUIThing_setStatic
    virtual void setVisible(int a)
        __asm__("cUIThing_setVisible");                                       // 9  cUIThing_setVisible
    virtual void setUIData(tUIBaseObject* pObj);                          // 10 cUIThing_setUIData
    virtual void setColour(const tARGB* colour)
        __asm__("cUIThing_setColour");                          // 11 cUIThing_setColour
    virtual void setChannelData(unsigned short channel, float data)
        __asm__("cUIThing_setChannelData");      // 12 cUIThing_setChannelData
    virtual void setChannelDataKCP(unsigned int NumChannels, short* pPtr)
        __asm__("cUIThing_setChannelDataKCP"); // 13 cUIThing_setChannelDataKCP
    virtual cUIThing* getObjectByHashName(unsigned int hashname);         // 14 cUIThing_getObjectByHashName
    virtual bool onInput(const cUIInput* pInput)
        __asm__("cUIThing_onInput__FPv");                         // 15 cUIThing_onInput
    virtual void updateLocale()
        __asm__("cUIThing_updateLocale__FPv");                                          // 16 cUIThing_updateLocale
    virtual void render2D(float x, float y)
        __asm__("cUIThing_render2D__FPv");                              // 17 cUIThing_render2D
    virtual void render3D(cUIRenderContext* pRenCon)
        __asm__("cUIThing_render3D__FPv");                     // 18 cUIThing_render3D
    virtual void update(unsigned short FrameNum)
        __asm__("cUIThing_update");                         // 19 cUIThing_update
    virtual void getSize(t3Vector* vec)
        __asm__("cUIThing_getSize");                                  // 20 cUIThing_getSize
    virtual void setSize(const t3Vector* vec)
        __asm__("cUIThing_setSize");                            // 21 cUIThing_setSize
    virtual char getType() = 0;                                           // 22 __pure_virtual

    // Methods bound to their retail symbols by asm labels.
    void playAnimation(void* anim, unsigned char mode, unsigned short frameNum) __asm__("cUIThing_playAnimation");
    void deleteAnimation() __asm__("cUIThing_deleteAnimation");
    void setFrameByPercent(float fraction) __asm__("cUIThing_setFrameByPercent");
    int isLastAnimFrame() __asm__("cUIThing_isLastAnimFrame");
    cUIEngine* getEngine() __asm__("cUIThing_getEngine__FPv");
    unsigned short getKeyframerEvent(int x) __asm__("cUIThing_getKeyframerEvent__FP8cUIThingi");
    bool playKeyframerEvent(signed char idx) __asm__("cUIThing_playKeyframerEvent");
    void getAbsPosition(t3Vector* out) __asm__("cUIThing_getAbsPosition");
    int getSound(int soundIndex) __asm__("cUIThing_getSound");
    void setSound(int soundIndex, int sound) __asm__("cUIThing_setSound");
    void* cUIThing1() __asm__("cUIThing_cUIThing1");
    bool isPlayingAnimation() __asm__("cUIThing_isPlayingAnimation");
    cUIAnimation* getAnimation() __asm__("cUIThing_getAnimation");
    int isHorizontal() __asm__("cUIThing_isHorizontal");
    int isVertical() __asm__("cUIThing_isVertical");
    int isActive() __asm__("cUIThing_isActive");
    int isCached() __asm__("cUIThing_isCached");
    int isEnabled() __asm__("cUIThing_isEnabled");
    int isFocused() __asm__("cUIThing_isFocused");
    int isStatic() __asm__("cUIThing_isStatic");
    int isVisible() __asm__("cUIThing_isVisible");
    int isWrap() __asm__("cUIThing_isWrap");
    void setWrap(int enable) __asm__("cUIThing_setWrap");
    int getMiscData() __asm__("cUIThing_getMiscData");
    void setOrientation(int orientation) __asm__("cUIThing_setOrientation");
    void getColour1(tARGB* out) __asm__("cUIThing_getColour1");
    void setRadAngle(float fparg0) __asm__("cUIThing_setRadAngle");
    float getRadAngle() __asm__("cUIThing_getRadAngle");
    void setPivot(int pivot) __asm__("cUIThing_setPivot");
    int getPivot() __asm__("cUIThing_getPivot");
    void setHashName(int hashName) __asm__("cUIThing_setHashName");
    int getHashName() __asm__("cUIThing_getHashName");
    void setParent(cUIThing* parent) __asm__("cUIThing_setParent");
    cUIThing* getParent() __asm__("cUIThing_getParent");
    void getPosition(t3Vector* out) __asm__("cUIThing_getPosition");
    void setPosition(t3Vector* out) __asm__("cUIThing_setPosition");
    void getScale(t3Vector* out) __asm__("cUIThing_getScale");
    void setScale(t3Vector* out) __asm__("cUIThing_setScale");
    cUIScreen* getScreen() __asm__("cUIThing_getScreen");
};

CHECK_SIZE(tAttrThing, 0x4);

CHECK_OFFSET(cUIThing, mpAnim, 0x00C);
CHECK_OFFSET(cUIThing, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIThing, mAttr, 0x014);
CHECK_OFFSET(cUIThing, mMiscData, 0x018);
CHECK_OFFSET(cUIThing, mColour, 0x01C);
CHECK_OFFSET(cUIThing, mAngle, 0x02C);
CHECK_OFFSET(cUIThing, mHashName, 0x038);
CHECK_OFFSET(cUIThing, mpEvents, 0x03C);
CHECK_OFFSET(cUIThing, mpParent, 0x040);
CHECK_OFFSET(cUIThing, mPosition, 0x044);
CHECK_OFFSET(cUIThing, mScale, 0x050);
CHECK_OFFSET(cUIThing, mpScreen, 0x05C);
CHECK_OFFSET(cUIThing, mSize, 0x060);
CHECK_OFFSET(cUIThing, mSound, 0x06C);
CHECK_SIZE(cUIThing, 0x74);

#endif /* UI_UITHING_H */
