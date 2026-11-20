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

struct cScriptDatabaseObject {
    cScriptObject* mpObjectDatabase[20]; // 0x000 [O]
    char mEntityDatabase[30][0x14];      // 0x050 [O] tScriptEntityRecord[30] {pEntity, objID, playerID, relation,
                                         //       reservedPlayerID}
    void** mVtbl;                        // 0x2A8 [R] cScriptDatabaseObject_vtbl
};

CHECK_OFFSET(cScriptDatabaseObject, mpObjectDatabase, 0x000);
CHECK_OFFSET(cScriptDatabaseObject, mEntityDatabase, 0x050);
CHECK_OFFSET(cScriptDatabaseObject, mVtbl, 0x2A8);
CHECK_SIZE(cScriptDatabaseObject, 0x2AC);

#endif
