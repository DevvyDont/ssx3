#ifndef FE_FERIDER_H
#define FE_FERIDER_H

// cFERider: one front-end rider model (the character shown in the FE screens: character select, equip gear, lodge):
// its cAnimModel and cRiderAnim, the textures and model parts of its equipped items (loaded through the FE file
// cache at RealFE + 0x1A70), the rider and board poses, and the load / show state machine. cFERiderManager
// (fe/feridermanager) holds two of them (cFERiderManager_getHuman / _getHuman1); the FE states reach them through it.
// EA's source is fe/ferider.cpp (the July 2003 preview's assert strings; builds/x/v2 file_hints for retail
// cFERider_initBoneMap and cFERider_update). Retail code: fe/festatecharequipdetail (0x0019E3D0 ... 0x0019E588) and
// fe/feridermanager (0x0019E680 ... 0x001A0358).
//
// Size 0xCE0:
//   [R] cFERiderManager_cFERiderManager constructs two in place, 0xCE0 apart (cFERider_cFERider(this + i * 0xCE0));
//       the last field written is the word at 0xCD8 (cFERider_init) and the tPoses make the class 16-byte aligned,
//       so 0xCDC-0xCE0 is tail padding.
//   On Tour's DWARF says 0xCC0: retail's bools are words, it has two FX pointers On Tour does not (0xC70, 0xC74), and
//   its mAnimFilter is one word (sw), not On Tour's unsigned long long. With those, On Tour's members fall on every
//   retail use below, in On Tour's order.
// No base class and no virtual functions (no vtable store in the constructor).
//
// A FLAT layout, like main/game.h: every field at its retail offset. bools are ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructor cFERider_cFERider, destructor cFERider__cFERider, cFERider_init and cFERider_purge;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFERider): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFERider methods.)

#include "layout.h"
#include "bx/math3d.h"
#include "render/ps2graphicsman.h"

struct cAnimModel;
struct cRiderAnim;

struct SHAPEFILE;
struct cBECharProfileDB;
struct t4Vector;
struct tBEProfileBolt;

struct cFERider {
    int mCharID;                    // 0x000 [O]; [R] -1; init stores the character, purge -1; [U] below 10 = a real
                                    //       character (setCharID / update)
    int mProfileIndex;              // 0x004 [O]; [R] -1; init stores the profile, purge -1
    cAnimModel* mpAnimModel;        // 0x008 [O]; [R] 0; init: cAnimModel_cAnimModel(cMemMan_alloc(352, "AnimModel"));
                                    //       purge destroys it
    cRiderAnim* mpRiderAnim;        // 0x00C [O]; [R] 0; compileModel: cMemMan_alloc(0x70, "RiderAnim") (rideranim)
    int mLoadAll;                   // 0x010 [O] (bool); [R] 0; init's loadAll argument (queueLoadExtraTextures when
                                    //       set)
    int mRiderIsPosed;              // 0x014 [O] (bool); [R] 0; [U] updateModelPose sets it, render waits for it
    int mNumTextureRefs;            // 0x018 [O]; [R] 0
    tTextureRef mTextureRefs[256];  // 0x01C [O] (render/ps2graphicsman.h, 0xC each); [R] TextureID -1, Name[0] 0;
                                    //       the destructor releases each TextureID >= 0 (GrMan slot 50)
    int mCustomized;                // 0xC1C [O] (bool); [R] 0; [U] speak plays the customize line (8381) and clears it
    int mSelected;                  // 0xC20 [O] (bool); [R] 0; [U] speak's character-select line (8380)
    int mNewCharID;                 // 0xC24 [O]; [R] -1; [U] setCharID stores the requested character, init -1
    int mNewLoadAll;                // 0xC28 [O] (bool); [R] 0; [U] setCharID's flag
    int mDrawLayer;                 // 0xC2C [O] eGMDrawLayer; [R] -1 (0xFFFFFFFF); [U] render puts it in the draw
                                    //       state's layer bits when not negative; the equip screen's onDestroyScreen
                                    //       sets -1
    tPose mPose;                    // 0xC30 [O]; [R] tPose_Identity (lq/sq); [U] setPose / translate / rotate
    tPose mBoardPose;               // 0xC50 [O]; [R] tPose_Identity; [U] setBoardPose / translateBoard / rotateBoard
    void* mpAttachedEmitterFX;      // 0xC70 [R] 0; init: func_002F1890(cMemMan_alloc(168, "AttachedEmitterFX"), this);
                                    //       purge destroys it (no On Tour member; named after the allocation tag)
    void* mpBoltOnFX;               // 0xC74 [R] 0; init: cBoltOnFXInterface_cBoltOnFXInterface1(cMemMan_alloc(200, "cBoltOnFXInterface"), this,
                                    //       mpAttachedEmitterFX); [U] updateAnimModelParts sets its first word
    int mBoltPoints;                // 0xC78 [O] order only (On Tour: after mBoardPose, before mBoneMap); [R] 0 in the
                                    //       ctor and purge, no other retail access
    int mBoneMap[14];               // 0xC7C [O]; [U] initBoneMap fills it; getBonePosition & co. index it;
                                    //       updateModelPose resets bones [12] and [13] (0xCAC, 0xCB0, the board)
    int mCompiledModel;             // 0xCB4 [O] (bool); [R] 0; [U] compileModel sets it, cFERider_loadModels clears it
    int mDoneLoadingTextures;       // 0xCB8 [O] (bool); [R] 0; [U] cFERider_isDoneLoadingTextures / cFERider_releasePartTextures set it,
                                    //       queueLoadExtraTextures / cFERider_loadTextures clear it
    int mDoneLoadingModels;         // 0xCBC [O] (bool); [R] 0; [U] update sets it once cFERider_isDoneLoadingModels succeeds
    int mDoneUpdatingModelParts;    // 0xCC0 [O] (bool); [R] 0; [U] update runs updateAnimModelParts once and sets it
    int mSetVisible;                // 0xCC4 [O] (bool); [R] 0; [U] setVisible's deferred show, folded into mVisible
    int mVisible;                   // 0xCC8 [O] (bool); [R] 0; [U] setVisible / update; purge clears it
    int mAnimateBoard;              // 0xCCC [O] (bool); [R] 0; [U] updateModelPose uses mPose for the board too
    int mUberIrradiance;            // 0xCD0 [O] (bool); [U] render: character 0 uses the "elise" locator when set
    int mSafeRenderCount;           // 0xCD4 [O]; [R] -1; [U] setCharID arms a deferred re-init for frame + 4, update
                                    //       runs it, render skips while it is >= 0, purge sets -1
    unsigned int mAnimFilter;       // 0xCD8 [O] (On Tour: unsigned long long; retail sw); [U] init: one bit per
                                    //       character (1, 4, 0x20, 2, ...)
    char unk_CDC[0x4];              // 0xCDC tail padding (16-byte class alignment)

    // Methods bound to their retail symbols by asm labels.
    void initBoneMap() __asm__("cFERider_initBoneMap");
    void queueLoadExtraTextures() __asm__("cFERider_queueLoadExtraTextures");
    void purge() __asm__("cFERider_purge");
    void cancelAllFiles() __asm__("cFERider_cancelAllFiles");
    void compileModel() __asm__("cFERider_compileModel");
    void render() __asm__("cFERider_render");
    void update() __asm__("cFERider_update");
    void updateModelPose() __asm__("cFERider_updateModelPose");
    void translate(t4Vector* delta) __asm__("cFERider_translate");
    void rotate(t4Vector* axis, float angle) __asm__("cFERider_rotate");
    void translateBoard(t4Vector* delta) __asm__("cFERider_translateBoard");
    void rotateBoard(t4Vector* axis, float angle) __asm__("cFERider_rotateBoard");
    int getBonePosition(int index) __asm__("cFERider_getBonePosition");
    int getTextureID(const char* name) __asm__("cFERider_getTextureID");
    void addTextureFile(SHAPEFILE* pkg, int replace) __asm__("cFERider_addTextureFile");
    void loadTextures(cBECharProfileDB* list, int ignored, int keepLoaded) __asm__("cFERider_loadTextures");
    int isDoneLoadingTextures() __asm__("cFERider_isDoneLoadingTextures");
    void loadModels(cBECharProfileDB* list, tBEProfileBolt* prev, int force, int keepLoaded)
        __asm__("cFERider_loadModels");
    void speak() __asm__("cFERider_speak");
    void setVisible(int enable) __asm__("cFERider_setVisible");
    void setCharID(unsigned int item, int flag) __asm__("cFERider_setCharID");
    void updateAnimModelParts() __asm__("cFERider_updateAnimModelParts");
    int getBoneQuaternion(int index) __asm__("cFERider_getBoneQuaternion");
    void getAnimID() __asm__("cFERider_getAnimID");
    void getAnimPercent() __asm__("cFERider_getAnimPercent");
};

CHECK_OFFSET(cFERider, mCharID, 0x000);
CHECK_OFFSET(cFERider, mProfileIndex, 0x004);
CHECK_OFFSET(cFERider, mpAnimModel, 0x008);
CHECK_OFFSET(cFERider, mpRiderAnim, 0x00C);
CHECK_OFFSET(cFERider, mLoadAll, 0x010);
CHECK_OFFSET(cFERider, mRiderIsPosed, 0x014);
CHECK_OFFSET(cFERider, mNumTextureRefs, 0x018);
CHECK_OFFSET(cFERider, mTextureRefs, 0x01C);
CHECK_OFFSET(cFERider, mCustomized, 0xC1C);
CHECK_OFFSET(cFERider, mSelected, 0xC20);
CHECK_OFFSET(cFERider, mNewCharID, 0xC24);
CHECK_OFFSET(cFERider, mNewLoadAll, 0xC28);
CHECK_OFFSET(cFERider, mDrawLayer, 0xC2C);
CHECK_OFFSET(cFERider, mPose, 0xC30);
CHECK_OFFSET(cFERider, mBoardPose, 0xC50);
CHECK_OFFSET(cFERider, mpAttachedEmitterFX, 0xC70);
CHECK_OFFSET(cFERider, mpBoltOnFX, 0xC74);
CHECK_OFFSET(cFERider, mBoltPoints, 0xC78);
CHECK_OFFSET(cFERider, mBoneMap, 0xC7C);
CHECK_OFFSET(cFERider, mCompiledModel, 0xCB4);
CHECK_OFFSET(cFERider, mDoneLoadingTextures, 0xCB8);
CHECK_OFFSET(cFERider, mDoneLoadingModels, 0xCBC);
CHECK_OFFSET(cFERider, mDoneUpdatingModelParts, 0xCC0);
CHECK_OFFSET(cFERider, mSetVisible, 0xCC4);
CHECK_OFFSET(cFERider, mVisible, 0xCC8);
CHECK_OFFSET(cFERider, mAnimateBoard, 0xCCC);
CHECK_OFFSET(cFERider, mUberIrradiance, 0xCD0);
CHECK_OFFSET(cFERider, mSafeRenderCount, 0xCD4);
CHECK_OFFSET(cFERider, mAnimFilter, 0xCD8);
CHECK_OFFSET(cFERider, unk_CDC, 0xCDC);
CHECK_SIZE(cFERider, 0xCE0);

#endif /* FE_FERIDER_H */
