#ifndef SCRIPTER_SCRIPTDATABASEOBJECT_H
#define SCRIPTER_SCRIPTDATABASEOBJECT_H

// cScriptDatabaseObject: the script engine's object and entity database (scripter/ssxscriptengine): 20 script objects
// and 30 entity records, each reserved / used / freed per player. Methods: AddObject, AddEntity, GetObject, the Mark*
// reservation calls, IsObjInUseByPlayer, IsEntityInUseByPlayer.
//
// Size 0x2AC (On Tour's). A FLAT layout, like main/game.h: member names, order and offsets are EA's, from SSX On Tour's
// PSP DWARF ([O]), and every retail access through `this` in the methods falls on a member start
// (tools/gen/self_offsets.py). Members of class type are byte arrays (type in the comment). g++ 2.95 puts the class's
// vptr after its own data: 0x2A8 ([R] the dtor stores cScriptDatabaseObject_vtbl there; RemoveObjectAll and the
// Mark*AsUsed / Mark*AsFree calls make virtual calls through it).

#include "layout.h"

struct cScriptObject;

// One entity record of the script database (On Tour tScriptEntityRecord, 0x14). [R]
// cScriptDatabaseObject_cScriptDatabaseObject sets 30 of them to {0, -1, -1, 0, -1}; [U] AddEntity / RemoveEntity /
// MarkEntityAsUsed / GetPlayerIDByEntity / GetAvailableEntities / IsObjInUseByPlayer (views sVar0050, sScrSlotD400,
// sScrEnt27D390).
struct cScriptEntity;

// Whether a player uses a script entity (tScriptEntityRecord::relation). [O] eScriptEntityRelation.
enum eScriptEntityRelation {
    keScriptEntityRelation_None = 0,
    keScriptEntityRelation_InUse = 1,
    keScriptEntityRelation_Num = 2
};

struct tScriptEntityRecord {
    cScriptEntity* pEntity;         // 0x00 [O]; [U] RemoveEntity clears it, GetAvailableEntities returns it
    int objID;                      // 0x04 [O]
    int playerID;                   // 0x08 [O]; [U] MarkEntityAsUsed stores it, GetPlayerIDByEntity returns it
    eScriptEntityRelation relation; // 0x0C [O]; [U] MarkEntityAsUsed stores 1 (in use)
    int reservedPlayerID;           // 0x10 [O]
};

CHECK_OFFSET(tScriptEntityRecord, pEntity, 0x00);
CHECK_OFFSET(tScriptEntityRecord, objID, 0x04);
CHECK_OFFSET(tScriptEntityRecord, playerID, 0x08);
CHECK_OFFSET(tScriptEntityRecord, relation, 0x0C);
CHECK_OFFSET(tScriptEntityRecord, reservedPlayerID, 0x10);
CHECK_SIZE(tScriptEntityRecord, 0x14);

struct cScriptDatabaseObject {
    cScriptObject* mpObjectDatabase[20]; // 0x000 [O]
    tScriptEntityRecord mEntityDatabase[30]; // 0x050 [O] tScriptEntityRecord[30] {pEntity, objID, playerID, relation,
                                         //       reservedPlayerID}
    void** mVtbl;                        // 0x2A8 [R] cScriptDatabaseObject_vtbl

    // Methods bound to their retail symbols by asm labels.
    int AddObject(void* scriptObject) __asm__("cScriptDatabaseObject_AddObject");
    bool RemoveObject(int idx) __asm__("cScriptDatabaseObject_RemoveObject");
    void RemoveObjectAll() __asm__("cScriptDatabaseObject_RemoveObjectAll");
    void MarkObjectAsReserved(int idx, int val) __asm__("cScriptDatabaseObject_MarkObjectAsReserved");
    void MarkObjectAsUnreserved(int idx) __asm__("cScriptDatabaseObject_MarkObjectAsUnreserved");
    bool IsEntityInUseByPlayer(int i, int val) __asm__("cScriptDatabaseObject_IsEntityInUseByPlayer");
    int GetPlayerIDByEntity(cScriptEntity* entity) __asm__("cScriptDatabaseObject_GetPlayerIDByEntity");
    int GetObject(int i) __asm__("cScriptDatabaseObject_GetObject__FPvi");
    int AddEntity(int idx, void* item) __asm__("cScriptDatabaseObject_AddEntity");
    int RemoveEntity(void* msg) __asm__("cScriptDatabaseObject_RemoveEntity");
    void MarkEntityAsUsed(cScriptEntity* key, int value) __asm__("cScriptDatabaseObject_MarkEntityAsUsed");
    void MarkEntityAsFree(cScriptEntity* key) __asm__("cScriptDatabaseObject_MarkEntityAsFree");
    void MarkEntityAsReserved(void* entity, int playerId) __asm__("cScriptDatabaseObject_MarkEntityAsReserved");
    void MarkEntityAsUnreserved(void* entity) __asm__("cScriptDatabaseObject_MarkEntityAsUnreserved");
    int GetAvailableEntities(int key, int* out, int max) __asm__("cScriptDatabaseObject_GetAvailableEntities");
};

CHECK_OFFSET(cScriptDatabaseObject, mpObjectDatabase, 0x000);
CHECK_OFFSET(cScriptDatabaseObject, mEntityDatabase, 0x050);
CHECK_OFFSET(cScriptDatabaseObject, mVtbl, 0x2A8);
CHECK_SIZE(cScriptDatabaseObject, 0x2AC);

#endif
