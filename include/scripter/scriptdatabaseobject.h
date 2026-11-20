#ifndef SCRIPTER_SCRIPTDATABASEOBJECT_H
#define SCRIPTER_SCRIPTDATABASEOBJECT_H

// cScriptDatabaseObject: the script engine's object and entity database (scripter/ssxscriptengine): 20 script objects
// and 30 entity records, each reserved / used / freed per player. Methods: AddObject, AddEntity, GetObject, the Mark*
// reservation calls, IsObjInUseByPlayer, IsEntityInUseByPlayer.
//
// Size 0x2AC (On Tour's; retail's methods reach 0x2A8, inside mEntityDatabase). A FLAT layout, like main/game.h: member
// names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the
// methods falls on a member start (tools/gen/self_offsets.py). Members of class type are byte arrays (type in the
// comment).

#include "layout.h"

struct cScriptObject;

struct cScriptDatabaseObject {
    cScriptObject* mpObjectDatabase[20]; // 0x000 [O]
    char mEntityDatabase[0x25C];         // 0x050 [O] tScriptEntityRecord[30]
};

CHECK_OFFSET(cScriptDatabaseObject, mpObjectDatabase, 0x000);
CHECK_OFFSET(cScriptDatabaseObject, mEntityDatabase, 0x050);
CHECK_SIZE(cScriptDatabaseObject, 0x2AC);

#endif
