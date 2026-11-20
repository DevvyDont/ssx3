#include "common.h"
#include "scripter/ssxscriptengine.h"

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", __sti__all_in_one_scripter_cpp);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_3E60 {
    float x, y, z, w;
    sV4_3E60(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sSlot_3E60 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_3E60(int n) { a = n; b = n; }
};

int GetHashValue32(char* str);

extern char D_004D3798[], D_004D37A8[], D_004D37B8[], D_004D37C8[], D_004D37D8[], D_004D37E8[], D_004D37F8[], D_004D3808[], D_004D3818[], D_004D3828[], D_004D3838[], D_004D3848[], D_004D3858[], D_004D3868[], D_004D3878[], D_004D3888[], D_004D3898[], D_004D38A8[], D_004D38B8[], D_004D38C8[], D_004D38D8[], D_004D38E8[], D_004D38F8[], D_004D3908[], D_004D3918[], D_004D3928[], D_004D3938[], D_004D3948[], D_004D3958[], D_004D3968[], D_004D3978[], D_004D3988[], D_004D3998[], D_004D39A8[], D_004D39B8[], D_004D39C8[], D_004D39D8[], D_004D39E8[], D_004D39F8[], D_004D3A08[], D_004D3A18[], D_004D3A28[], D_004D3A38[], D_004D3A48[], D_004D3A58[], D_004D3A68[], D_004D3A78[], D_004D3A88[], D_004D3A98[], D_004D3AA8[], D_004D3AB8[], D_004D3AC8[], D_004D3AD8[], D_004D3AE8[], D_004D3AF8[], D_004D3B08[];
extern char str_NoScript[];
extern int D_004A51E8;
extern int D_004A51EC;
extern int D_004A525C;
extern sSlot_3E60 D_004A51E0, D_004A51F0, D_004A51F8, D_004A5200, D_004A5208, D_004A5210, D_004A5218, D_004A5220, D_004A5228, D_004A5230, D_004A5238, D_004A5240, D_004A5248, D_004A5250, D_004A5258;

/**
 * Runs the static constructors of the scripter unit: colour constants, slot objects and the "NoScript" hash.
 *
 * Builds a slot with -1, 58 four-float constants (D_004D3798 on), 14 slots numbered 0-13 and stores
 * GetHashValue32("NoScript") in D_004A525C. Only acts for the construction pass at the default priority.
 *
 * @param init  nonzero for the construction pass
 * @param prio  initialisation priority; only 0xFFFF does anything
 */
extern "C" void __sti__all_in_one_scripter_cpp(int init, int prio)
{
    if (prio == 0xFFFF && init != 0) {
        new (&D_004A51E0) sSlot_3E60(-1);
        new (D_004D3798) sV4_3E60(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D37A8) sV4_3E60(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D37B8) sV4_3E60(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D37C8) sV4_3E60(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D37D8) sV4_3E60(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D37E8) sV4_3E60(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D37F8) sV4_3E60(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D3808) sV4_3E60(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D3818) sV4_3E60(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D3828) sV4_3E60(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D3838) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3848) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3858) sV4_3E60(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D3868) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3878) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3888) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3898) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D38A8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D38B8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38C8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38D8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38E8) sV4_3E60(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D38F8) sV4_3E60(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D3908) sV4_3E60(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D3918) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3928) sV4_3E60(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D3938) sV4_3E60(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D3948) sV4_3E60(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D3958) sV4_3E60(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D3968) sV4_3E60(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D3978) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3988) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3998) sV4_3E60(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D39A8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D39B8) sV4_3E60(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D39C8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D39D8) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D39E8) sV4_3E60(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D39F8) sV4_3E60(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D3A08) sV4_3E60(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D3A18) sV4_3E60(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D3A28) sV4_3E60(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D3A38) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3A48) sV4_3E60(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D3A58) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A68) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A78) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A88) sV4_3E60(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D3A98) sV4_3E60(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D3AA8) sV4_3E60(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D3AB8) sV4_3E60(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D3AC8) sV4_3E60(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D3AD8) sV4_3E60(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D3AE8) sV4_3E60(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D3AF8) sV4_3E60(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D3B08) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A51EC = 0;
        D_004A51E8 = 0;
        new (&D_004A51F0) sSlot_3E60(0);
        new (&D_004A51F8) sSlot_3E60(1);
        new (&D_004A5200) sSlot_3E60(2);
        new (&D_004A5208) sSlot_3E60(3);
        new (&D_004A5210) sSlot_3E60(4);
        new (&D_004A5218) sSlot_3E60(5);
        new (&D_004A5220) sSlot_3E60(6);
        new (&D_004A5228) sSlot_3E60(7);
        new (&D_004A5230) sSlot_3E60(8);
        new (&D_004A5238) sSlot_3E60(9);
        new (&D_004A5240) sSlot_3E60(10);
        new (&D_004A5248) sSlot_3E60(11);
        new (&D_004A5250) sSlot_3E60(12);
        new (&D_004A5258) sSlot_3E60(13);
        D_004A525C = GetHashValue32(str_NoScript);
    }
}
#endif

/**
 * Does nothing: the base cDataManager hook for attached data (slot 2 of cDataManager_vtbl).
 *
 * cSSXScriptEngine replaces it with cSSXScriptEngine_OnAttachData in its cDataManager table (D_00482168).
 */
extern "C" void cDataManager_OnAttachData(void) {
}

/**
 * Does nothing: the base cDataManager hook for detached data (slot 3 of cDataManager_vtbl).
 *
 * cSSXScriptEngine replaces it with cSSXScriptEngine_OnDetachData in its cDataManager table (D_00482168).
 */
extern "C" void cDataManager_OnDetachData(void) {
}

/**
 * Does nothing: the base cDataManager hook for data attached from a big file (slot 4 of cDataManager_vtbl).
 *
 * cSSXScriptEngine keeps it (slot 4 of D_00482168).
 */
extern "C" void cDataManager_OnAttachDataBigFile(void) {
}

/**
 * Does nothing: the base cDataManager hook for data detached from a big file (slot 5 of cDataManager_vtbl).
 *
 * cSSXScriptEngine keeps it (slot 5 of D_00482168).
 */
extern "C" void cDataManager_OnDetachDataBigFile(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObject_GetObjectType);
#ifdef SKIP_ASM
/**
 * Returns the object type, the first word (+0x0) of a script object (On Tour's cScriptObject::mObjType).
 *
 * @param scriptObject  the script object
 * @return its object type
 */
extern "C" int cScriptObject_GetObjectType(int *scriptObject) {
    return *scriptObject;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObject_GetObjectID);
#ifdef SKIP_ASM
/**
 * Returns the object ID, the word at +0x4 of a script object (On Tour's cScriptObject::mObjID).
 *
 * @param scriptObject  the script object
 * @return its object ID
 */
extern "C" int cScriptObject_GetObjectID(void *scriptObject) {
    return (*(int *)((char*)(scriptObject) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObject_IsInUse);
#ifdef SKIP_ASM
/**
 * Returns whether a script object is in use: 1 when its use count (+0x8, On Tour's cScriptObject::mInUse) is above 0.
 *
 * @param scriptObject  the script object
 * @return 1 if in use, else 0
 */
extern "C" int cScriptObject_IsInUse(void *scriptObject) {
    return (*(int *)((char*)(scriptObject) + (8))) > 0;
}
#endif

/**
 * Does nothing: cBXScriptEngine's per-frame InSeq player update hook (slot 2 of its cInSeqComm table, D_00482450).
 *
 * cSSXScriptEngine replaces it with cSSXScriptEngine_OnInSeqUpdate (D_00482100).
 */
extern "C" void cBXScriptEngine_OnInSeqUpdate(void) {
}

/**
 * Does nothing: cBXScriptEngine's hook for an InSeq player that is stopping (slot 3 of its cInSeqComm table,
 * D_00482450).
 *
 * cSSXScriptEngine replaces it with cSSXScriptEngine_OnInSeqPlayStopping (D_00482100).
 */
extern "C" void cBXScriptEngine_OnInSeqPlayStopping(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cBXScriptEngine_OnInSeqLoop);
#ifdef SKIP_ASM
/**
 * Returns 1: the script engine's InSeq loop hook (slot 5 of the cInSeqComm tables D_00482450 and D_00482100).
 *
 * @return always 1
 */
extern "C" int cBXScriptEngine_OnInSeqLoop(void) {
    return 1;
}
#endif

/**
 * Does nothing: the script engine's InSeq remote-disconnect hook (slot 7 of the cInSeqComm tables D_00482450 and
 * D_00482100).
 */
extern "C" void cBXScriptEngine_OnInSeqRemoteDisconnect(void) {
}

/**
 * Does nothing: the script engine's InSeq remote-message hook (slot 8 of the cInSeqComm tables D_00482450 and
 * D_00482100).
 */
extern "C" void cBXScriptEngine_OnInSeqRemoteMessageSend(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cBXScriptEngine_IsInSeqReadyForRemotePlayCommands);
#ifdef SKIP_ASM
/**
 * Returns 1: the script engine's answer to whether InSeq is ready for remote play commands (last slot of the cInSeqComm
 * tables D_00482450 and D_00482100).
 *
 * @return always 1
 */
extern "C" int cBXScriptEngine_IsInSeqReadyForRemotePlayCommands(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cBXScriptEngine_GetScriptDatabaseSize);
#ifdef SKIP_ASM
/**
 * Returns the engine's mScriptDatabaseSize (+0x2B4), the script slot count SetupBXEngine stores.
 *
 * @param bxEngine  the script engine (cSSXScriptEngine)
 * @return mScriptDatabaseSize
 */
extern "C" int cBXScriptEngine_GetScriptDatabaseSize(void *bxEngine) {
    return (*(int *)((char*)(bxEngine) + (0x2B4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cBXScriptEngine_OnDataUnavailable);
#ifdef SKIP_ASM
/**
 * Returns 0: cBXScriptEngine's default answer when a script's data is not loaded (slot 7 of cBXScriptEngine_vtbl).
 *
 * @return always 0
 */
extern "C" int cBXScriptEngine_OnDataUnavailable(void) {
    return 0;
}
#endif

/**
 * Does nothing: cBXScriptEngine's hook for cancelled pending script data (slot 8 of cBXScriptEngine_vtbl).
 */
extern "C" void cBXScriptEngine_OnPendingDataCancelled(void) {
}

/**
 * Does nothing: cBXScriptEngine's hook for released script data (slot 9 of cBXScriptEngine_vtbl).
 */
extern "C" void cBXScriptEngine_OnScriptDataReleased(void) {
}

/**
 * Does nothing: cBXScriptEngine's hook for a script that starts playing (slot 10 of cBXScriptEngine_vtbl).
 */
extern "C" void cBXScriptEngine_OnStartScript(void) {
}

/**
 * Does nothing: cBXScriptEngine's hook for a script that stops (slot 11 of cBXScriptEngine_vtbl; cSSXScriptEngine keeps
 * it).
 */
extern "C" void cBXScriptEngine_OnStopScript(void) {
}

/**
 * Does nothing: cBXScriptEngine's hook for a released script request (last slot of cBXScriptEngine_vtbl).
 */
extern "C" void cBXScriptEngine_OnReleaseRequest(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntity_GetPlayer);
#ifdef SKIP_ASM
/**
 * Returns the InSeq player entity (+0x8, On Tour's cScriptEntity::mpEntity) a script entity is bound to.
 *
 * @param entity  the script entity
 * @return its player entity
 */
extern "C" int cScriptEntity_GetPlayer(void *entity) {
    return (*(int *)((char*)(entity) + (8)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntity_GetDatabaseID);
#ifdef SKIP_ASM
/**
 * Returns a script entity's database ID (+0x4, On Tour's cScriptEntity::mDBID).
 *
 * @param entity  the script entity
 * @return its database ID
 */
extern "C" int cScriptEntity_GetDatabaseID(void *entity) {
    return (*(int *)((char*)(entity) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntity_SetDatabaseID);
#ifdef SKIP_ASM
/**
 * Sets a script entity's database ID (+0x4, On Tour's cScriptEntity::mDBID).
 *
 * @param entity  the script entity
 * @param databaseId  the new database ID
 */
extern "C" void cScriptEntity_SetDatabaseID(void *entity, int databaseId) {
    (*(int *)((char*)(entity) + (4))) = databaseId;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntity_ClearDatabaseID);
#ifdef SKIP_ASM
/**
 * Clears a script entity's database ID (+0x4, On Tour's cScriptEntity::mDBID) to -1.
 *
 * @param entity  the script entity
 */
extern "C" void cScriptEntity_ClearDatabaseID(void *entity) {
    (*(int *)((char*)(entity) + (4))) = -1;
}
#endif

/**
 * Does nothing: the base script entity's InSeq key-event hook (slot 3 of cScriptEntity_vtbl).
 *
 * cScriptObjGS (cScriptObjGS_OnInSeqKeyEvent) and cScriptObjEvent (cScriptObjEvent_OnInSeqKeyEvent) replace it.
 */
extern "C" void cScriptEntity_OnInSeqKeyEvent(void) {
}

/**
 * Does nothing: the base script entity's per-frame InSeq update hook (slot 4 of cScriptEntity_vtbl).
 *
 * The cScriptObjGS, cScriptObjEvent and cScriptObjRider tables keep it.
 */
extern "C" void cScriptEntity_OnInSeqUpdate(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntity_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns 1: the base script entity matches any InSeq player entity (slot 5 of cScriptEntity_vtbl).
 *
 * cScriptObjRider replaces it with cScriptObjRider_IsMatchingEntity.
 *
 * @return always 1
 */
extern "C" int cScriptEntity_IsMatchingEntity(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntityCamera_SetCameraManager);
#ifdef SKIP_ASM
/**
 * Sets a camera script entity's camera manager (+0x10, On Tour's cScriptEntityCamera::mpCamManager).
 *
 * @param entityCamera  the camera script entity
 * @param camManager  the camera manager (cScriptObjCameraManager)
 */
extern "C" void cScriptEntityCamera_SetCameraManager(void *entityCamera, int camManager) {
    (*(int *)((char*)(entityCamera) + (0x10))) = camManager;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptEntityCamera_GetSubType);
#ifdef SKIP_ASM
/**
 * Returns a camera script entity's sub-type (+0xC, On Tour's cScriptEntityCamera::mSubType).
 *
 * @param entityCamera  the camera script entity
 * @return its sub-type
 */
extern "C" int cScriptEntityCamera_GetSubType(void *entityCamera) {
    return (*(int *)((char*)(entityCamera) + (0xC)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCameraManager__cScriptObjCameraManager);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScriptObject__cScriptObject_48A0(void *) __asm__("cScriptObject__cScriptObject");
extern char D_00482360[];
extern char cScriptObjCameraManager_vtbl[];

/**
 * Destroys the script camera manager through the cScriptObject destructor.
 *
 * Stores the vtable of its cScriptEntity base part (D_00482360; its overridden entries adjust this by -0x10) at +0x10
 * and its own vtable cScriptObjCameraManager_vtbl at +0xC, then calls cScriptObject__cScriptObject, which receives the
 * g++ delete flags untouched in the second argument register (this declaration leaves them out). Nothing of the
 * manager's own is torn down.
 *
 * @param manager  the camera manager being destroyed (this)
 */
extern "C" void cScriptObjCameraManager__cScriptObjCameraManager(void *manager) {
    *(char **)((char*)manager + 0x10) = D_00482360;
    *(char **)((char*)manager + 0xC) = cScriptObjCameraManager_vtbl;
    cScriptObject__cScriptObject_48A0(manager);
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCameraManager_GetViewIndex);
#ifdef SKIP_ASM
/**
 * Returns a camera manager's view index (+0xF8; On Tour's cScriptObjCameraManager::mViewIndex).
 *
 * @param camManager  the camera manager
 * @return its view index
 */
extern "C" int cScriptObjCameraManager_GetViewIndex(void *camManager) {
    return (*(int *)((char*)(camManager) + (0xF8)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjGS__cScriptObjGS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScriptObject__cScriptObject_48D8(void *, int) __asm__("cScriptObject__cScriptObject");
void operator_delete(int *);
extern char cScriptObjGS_vtbl[];
extern char D_00482318[];

/**
 * Destroys a cScriptObjGS: puts back its vtables, destroys the cScriptObject base, and frees it when asked.
 *
 * Stores cScriptObjGS_vtbl at +0x0 and D_00482318 at +0x18 (the cScriptObject part's vtable), calls
 * cScriptObject__cScriptObject on the cScriptObject part at +0xC, then calls operator_delete on the object when bit 0
 * of
 * @p deleteFlags is set.
 *
 * @param gsObject  the object (cSSXScriptEngine::mObjectGameState)
 * @param deleteFlags  destructor flags; bit 0 frees the memory
 */
extern "C" void cScriptObjGS__cScriptObjGS(void *gsObject, int deleteFlags) {
    *(char **)((char*)gsObject + 0x18) = D_00482318;
    *(char **)((char*)gsObject + 0x0) = cScriptObjGS_vtbl;
    cScriptObject__cScriptObject_48D8((char*)gsObject + 0xC, 0);
    if (deleteFlags & 1) {
        operator_delete((int *)gsObject);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjGS_GetNumScriptEntities);
#ifdef SKIP_ASM
/**
 * Returns 1: a cScriptObjGS has a single script entity, itself (slot 6 of D_00482318).
 *
 * @return always 1
 */
extern "C" int cScriptObjGS_GetNumScriptEntities(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjGS_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns the cScriptObjGS itself as its only script entity (the cScriptEntity part at +0x0).
 *
 * Called through slot 7 of D_00482318, whose entry moves the cScriptObject pointer back by 0xC to the object start; the
 * entity index (second argument, not declared here) is ignored.
 *
 * @param gsObject  the cScriptObjGS
 * @return @p gsObject
 */
extern "C" int cScriptObjGS_GetScriptEntity(int gsObject) {
    return gsObject;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjEvent__cScriptObjEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScriptObject__cScriptObject_4950(void *, int) __asm__("cScriptObject__cScriptObject");
void operator_delete(int *);
extern char cScriptObjEvent_vtbl[];
extern char D_00482298[];

/**
 * Destroys a cScriptObjEvent: puts back its vtables, destroys the cScriptObject base, and frees it when asked.
 *
 * Stores cScriptObjEvent_vtbl at +0x0 and D_00482298 at +0x18 (the cScriptObject part's vtable), calls
 * cScriptObject__cScriptObject on the cScriptObject part at +0xC, then calls operator_delete on the object when bit 0
 * of
 * @p deleteFlags is set.
 *
 * @param eventObject  the object (cSSXScriptEngine::mObjectEvent)
 * @param deleteFlags  destructor flags; bit 0 frees the memory
 */
extern "C" void cScriptObjEvent__cScriptObjEvent(void *eventObject, int deleteFlags) {
    *(char **)((char*)eventObject + 0x18) = D_00482298;
    *(char **)((char*)eventObject + 0x0) = cScriptObjEvent_vtbl;
    cScriptObject__cScriptObject_4950((char*)eventObject + 0xC, 0);
    if (deleteFlags & 1) {
        operator_delete((int *)eventObject);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjEvent_GetNumScriptEntities);
#ifdef SKIP_ASM
/**
 * Returns 1: a cScriptObjEvent has a single script entity, itself (slot 6 of D_00482298).
 *
 * @return always 1
 */
extern "C" int cScriptObjEvent_GetNumScriptEntities(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjEvent_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns the cScriptObjEvent itself as its only script entity (the cScriptEntity part at +0x0).
 *
 * Called through slot 7 of D_00482298, whose entry moves the cScriptObject pointer back by 0xC to the object start; the
 * entity index (second argument, not declared here) is ignored.
 *
 * @param eventObject  the cScriptObjEvent
 * @return @p eventObject
 */
extern "C" int cScriptObjEvent_GetScriptEntity(int eventObject) {
    return eventObject;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cVidEngine_IsVideoEngineBusy);
#ifdef SKIP_ASM
/**
 * Returns whether the video engine has a movie player (mpPlayer, +0x0 of the cVidEngine part, set by
 * cVidEngine_ReadyVideo).
 *
 * @param vidEngine  the cVidEngine part of the script engine
 * @return 1 if mpPlayer is set, else 0
 */
extern "C" int cVidEngine_IsVideoEngineBusy(int *vidEngine) {
    return *vidEngine != 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cVidEngine_IsVideoEnginePlaying);
#ifdef SKIP_ASM
/**
 * Returns the video engine's mIsPlaying flag (+0x4 of the cVidEngine part; cVidEngine_PlayVideo sets it).
 *
 * @param vidEngine  the cVidEngine part of the script engine
 * @return mIsPlaying
 */
extern "C" int cVidEngine_IsVideoEnginePlaying(void *vidEngine) {
    return (*(int *)((char*)(vidEngine) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cSSXScriptEngine_IsNISActive);
#ifdef SKIP_ASM
/**
 * Returns whether an NIS list is playing: mPlayingNISList (+0x550) is not 2, the constructor's "none" value.
 *
 * @param scriptEngine  the script engine
 * @return 1 if an NIS list is active, else 0
 */
extern "C" int cSSXScriptEngine_IsNISActive(cSSXScriptEngine* scriptEngine) {
    return (scriptEngine->mPlayingNISList) != 2;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cSSXScriptEngine_GetActiveNIS);
#ifdef SKIP_ASM
/**
 * Returns the playing NIS list, mPlayingNISList (+0x550; 2 means none).
 *
 * @param scriptEngine  the script engine
 * @return mPlayingNISList
 */
extern "C" int cSSXScriptEngine_GetActiveNIS(cSSXScriptEngine* scriptEngine) {
    return (scriptEngine->mPlayingNISList);
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cSSXScriptEngine_IsBankAvailable);
#ifdef SKIP_ASM
struct sSlot_49F8 {
    int f0;     // 0x628
    int f4;
    int f8;     // 0x630
    int fC;
};

struct sEngine_49F8 {
    char pad[0x628];
    sSlot_49F8 slots[1];
};

/**
 * Returns whether script data bank @p idx is free: no async load in flight and no data attached.
 *
 * Reads mScriptDataBanks[@p idx] (+0x628, tSSXScriptBankInfo): 1 only when asyncHandle (+0x8) and dataBankHandle (+0x0)
 * are both 0.
 *
 * @param idx  the bank index (0-9)
 * @return 1 if the bank is free, else 0
 */
extern "C" int cSSXScriptEngine_IsBankAvailable(sEngine_49F8* self, int idx)
{
    int available = 0;
    if (self->slots[idx].f8 == 0) {
        available = self->slots[idx].f0 == 0;
    }
    return available;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera__cScriptObjCamera);
#ifdef SKIP_ASM
extern "C" void cScriptObject__cScriptObject();

/**
 * Destroys a script camera object by handing it straight to the cScriptObject destructor.
 *
 * Slot 1 of cScriptObjCamera_vtbl. The body is one call to cScriptObject__cScriptObject() with the incoming registers
 * untouched, so the object and the g++ destructor flags pass through and the base destructor unregisters the object and
 * frees it when bit 0 is set. The C declares no parameters for that reason; cScriptObjTransition__cScriptObjTransition
 * is the same code.
 */
extern "C" void cScriptObjCamera__cScriptObjCamera(void) {
    cScriptObject__cScriptObject();
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetCameraEntity);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's camera entity (+0x14, On Tour's cScriptObjCamera::mpCameraEntity).
 *
 * @param cameraObj  the camera object
 * @return its camera entity
 */
extern "C" int cScriptObjCamera_GetCameraEntity(void *cameraObj) {
    return (*(int *)((char*)(cameraObj) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetCameraWeight);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's weight (float +0x18, On Tour's cScriptObjCamera::mWeight).
 *
 * @param cameraObj  the camera object
 * @return its weight
 */
extern "C" float cScriptObjCamera_GetCameraWeight(void *cameraObj) {
    return (*(float *)((char*)(cameraObj) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetBaseTransform);
#ifdef SKIP_ASM
/**
 * Returns the address of a script camera object's base transform matrix (+0x30, On Tour's mBaseTransform).
 *
 * @param cameraObj  the camera object
 * @return @p cameraObj + 0x30
 */
extern "C" int cScriptObjCamera_GetBaseTransform(int cameraObj) {
    return cameraObj + 0x30;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetBaseRotX);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's base X rotation (float +0x70, On Tour's mBaseRotX).
 *
 * @param cameraObj  the camera object
 * @return its base X rotation
 */
extern "C" float cScriptObjCamera_GetBaseRotX(void *cameraObj) {
    return (*(float *)((char*)(cameraObj) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetBaseRotY);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's base Y rotation (float +0x74, On Tour's mBaseRotY).
 *
 * @param cameraObj  the camera object
 * @return its base Y rotation
 */
extern "C" float cScriptObjCamera_GetBaseRotY(void *cameraObj) {
    return (*(float *)((char*)(cameraObj) + (0x74)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetBaseRotZ);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's base Z rotation (float +0x78, On Tour's mBaseRotZ).
 *
 * @param cameraObj  the camera object
 * @return its base Z rotation
 */
extern "C" float cScriptObjCamera_GetBaseRotZ(void *cameraObj) {
    return (*(float *)((char*)(cameraObj) + (0x78)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetViewIndex);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's view index (+0x7C, On Tour's cScriptObjCamera::mViewIndex).
 *
 * @param cameraObj  the camera object
 * @return its view index
 */
extern "C" int cScriptObjCamera_GetViewIndex(void *cameraObj) {
    return (*(int *)((char*)(cameraObj) + (0x7C)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetCameraManager);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's camera manager (+0x10, On Tour's cScriptObjCamera::mpCameraManager).
 *
 * @param cameraObj  the camera object
 * @return its camera manager
 */
extern "C" int cScriptObjCamera_GetCameraManager(void *cameraObj) {
    return (*(int *)((char*)(cameraObj) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetNumScriptEntities);
#ifdef SKIP_ASM
/**
 * Reports that a script camera object owns no script entities.
 *
 * Slot 6 of cScriptObjCamera_vtbl (cScriptObject's GetNumScriptEntities); the object pointer is not read, so the C
 * declares no parameters. Same code as cScriptObjTransition_GetNumScriptEntities.
 *
 * @return 0
 */
extern "C" int cScriptObjCamera_GetNumScriptEntities(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns no script entity: a script camera object owns none.
 *
 * Slot 7 of cScriptObjCamera_vtbl (cScriptObject's GetScriptEntity(index)); neither the object nor the index is read,
 * so the C declares no parameters. Same code as cScriptObjTransition_GetScriptEntity.
 *
 * @return 0 (no entity)
 */
extern "C" int cScriptObjCamera_GetScriptEntity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_SetCameraAlgorithm);
#ifdef SKIP_ASM
/**
 * Sets a script camera object's camera algorithm (+0x1C, On Tour's cScriptObjCamera::mpCameraAlgorithm).
 *
 * @param cameraObj  the camera object
 * @param algorithm  the camera algorithm
 */
extern "C" void cScriptObjCamera_SetCameraAlgorithm(void *cameraObj, int algorithm) {
    (*(int *)((char*)(cameraObj) + (0x1C))) = algorithm;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjCamera_GetCameraAlgorithm);
#ifdef SKIP_ASM
/**
 * Returns a script camera object's camera algorithm (+0x1C, On Tour's cScriptObjCamera::mpCameraAlgorithm).
 *
 * @param cameraObj  the camera object
 * @return its camera algorithm
 */
extern "C" int cScriptObjCamera_GetCameraAlgorithm(void *cameraObj) {
    return (*(int *)((char*)(cameraObj) + (0x1C)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjTransition__cScriptObjTransition);
#ifdef SKIP_ASM
extern "C" void cScriptObject__cScriptObject();

/**
 * Destroys a script transition object through the script-object destructor cScriptObject__cScriptObject().
 *
 * Declared without parameters: the object and the destructor flags reach cScriptObject__cScriptObject() unchanged in their
 * registers. That destructor de-initialises the script object if it holds a valid handle and frees it when bit 0
 * of the flags is set.
 */
extern "C" void cScriptObjTransition__cScriptObjTransition(void) {
    cScriptObject__cScriptObject();
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjTransition_GetTransitionInfo);
#ifdef SKIP_ASM
/**
 * Returns the address of a script transition object's transition info (+0x18, On Tour's cScriptObjTransition::mInfo).
 *
 * @param transitionObj  the transition object
 * @return @p transitionObj + 0x18
 */
extern "C" int cScriptObjTransition_GetTransitionInfo(int transitionObj) {
    return transitionObj + 0x18;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjTransition_GetCameraManager);
#ifdef SKIP_ASM
/**
 * Returns a script transition object's camera manager (+0x10, On Tour's cScriptObjTransition::mpCameraManager).
 *
 * @param transitionObj  the transition object
 * @return its camera manager
 */
extern "C" int cScriptObjTransition_GetCameraManager(void *transitionObj) {
    return (*(int *)((char*)(transitionObj) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjTransition_GetNumScriptEntities);
#ifdef SKIP_ASM
/**
 * Reports that a script transition object has no script entities.
 *
 * @return always 0
 */
extern "C" int cScriptObjTransition_GetNumScriptEntities(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjTransition_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns no script entity for a script transition object.
 *
 * @return always 0
 */
extern "C" int cScriptObjTransition_GetScriptEntity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider__cScriptObjRider);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScriptObject__cScriptObject_4AE8(void *, int) __asm__("cScriptObject__cScriptObject");
void operator_delete(int *);
extern char cScriptObjRider_vtbl[];
extern char D_00482018[];

/**
 * Destroys a cScriptObjRider: puts back its vtables, destroys the cScriptObject base, and frees it when asked.
 *
 * Stores cScriptObjRider_vtbl at +0x0 and D_00482018 at +0x18 (the cScriptObject part's vtable), calls
 * cScriptObject__cScriptObject on the cScriptObject part at +0xC, then calls operator_delete on the object when bit 0
 * of
 * @p deleteFlags is set.
 *
 * @param riderObject  the object
 * @param deleteFlags  destructor flags; bit 0 frees the memory
 */
extern "C" void cScriptObjRider__cScriptObjRider(void *riderObject, int deleteFlags) {
    *(char **)((char*)riderObject + 0x18) = D_00482018;
    *(char **)((char*)riderObject + 0x0) = cScriptObjRider_vtbl;
    cScriptObject__cScriptObject_4AE8((char*)riderObject + 0xC, 0);
    if (deleteFlags & 1) {
        operator_delete((int *)riderObject);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetNumScriptEntities);
#ifdef SKIP_ASM
/**
 * Reports that a rider script object has exactly one script entity.
 *
 * @return always 1
 */
extern "C" int cScriptObjRider_GetNumScriptEntities(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns the cScriptObjRider itself as its only script entity (the cScriptEntity part at +0x0).
 *
 * Called through slot 7 of the riders' cScriptObject tables (D_00482018 and the cRider-derived ones), whose entries
 * move the cScriptObject pointer back by 0xC; the entity index (second argument, not declared here) is ignored.
 *
 * @param riderObject  the cScriptObjRider
 * @return @p riderObject
 */
extern "C" int cScriptObjRider_GetScriptEntity(int riderObject) {
    return riderObject;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetBaseTransform);
#ifdef SKIP_ASM
/**
 * Returns the address of a script rider object's base transform matrix (+0x30, On Tour's
 * cScriptObjRider::mBaseTransform).
 *
 * @param riderObject  the rider script object
 * @return @p riderObject + 0x30
 */
extern "C" int cScriptObjRider_GetBaseTransform(int riderObject) {
    return riderObject + 0x30;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetBaseRotX);
#ifdef SKIP_ASM
/**
 * Returns a script rider object's base X rotation (float +0x70, On Tour's cScriptObjRider::mBaseRotX).
 *
 * @param riderObject  the rider script object
 * @return its base X rotation
 */
extern "C" float cScriptObjRider_GetBaseRotX(void *riderObject) {
    return (*(float *)((char*)(riderObject) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetBaseRotY);
#ifdef SKIP_ASM
/**
 * Returns a script rider object's base Y rotation (float +0x74, On Tour's cScriptObjRider::mBaseRotY).
 *
 * @param riderObject  the rider script object
 * @return its base Y rotation
 */
extern "C" float cScriptObjRider_GetBaseRotY(void *riderObject) {
    return (*(float *)((char*)(riderObject) + (0x74)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", cScriptObjRider_GetBaseRotZ);
#ifdef SKIP_ASM
/**
 * Returns a script rider object's base Z rotation (float +0x78, On Tour's cScriptObjRider::mBaseRotZ).
 *
 * @param riderObject  the rider script object
 * @return its base Z rotation
 */
extern "C" float cScriptObjRider_GetBaseRotZ(void *riderObject) {
    return (*(float *)((char*)(riderObject) + (0x78)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B80);
#ifdef SKIP_ASM
extern "C" void __sti__all_in_one_scripter_cpp(int, int);

/**
 * Runs the unit's static initialisers: calls __sti__all_in_one_scripter_cpp(1, 0xFFFF).
 */
extern "C" void func_00284B80(void) {
    __sti__all_in_one_scripter_cpp(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284BA0);
#ifdef SKIP_ASM
/**
 * Returns the volume-duck mask for a duck stream: 371 (0x173) for stream 0, otherwise 0.
 *
 * func_00287F00 passes the result as the mask argument of cSSXAudio_DuckVolume.
 *
 * @param duckStream  the duck stream number
 * @return 0x173 for stream 0, else 0
 */
extern "C" int func_00284BA0(int duckStream) {
    return (duckStream != 0) ? 0 : 371;
}
#endif
