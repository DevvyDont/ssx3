#ifndef OBJECT_BEING_H
#define OBJECT_BEING_H

// cBeing: anything that moves through the world as a rider does: position, velocity and transform, its AI index,
// human or computer, boosts and controller vibration. cRider's second base (ai/rider.h: cRider : cRiderState,
// cBeing), so its vptr is cRider's, at 0x6C0. EA's header was object/being.h: builds/x/v2/file_hints.tsv places seven
// inline virtual bodies emitted in ai/control/handplantcontrol (0x001406B0-0x00140700) at its lines 13-21, by vtable
// slot against the July 2003 preview's line info.
//
// A real polymorphic class with no data: ee-gcc 2.95.3 puts the vptr at 0x0 (no own data before it). Nothing defines a
// member here, so no vtable or type_info is emitted.
// Evidence:
//   [R] retail cRider_vtbl (the vptr cRider_cRider stores at 0x6C0) and the per-component copies (cRider is a virtual
//       base of cPlayer, cComputer and ~30 components): 19 slots plus slot 20 (below). Slots 6 and 11 hold cBeing's
//       own getTransform / resetToPath and 16-19 empty functions with delta 0 (they take the cBeing subobject: cBeing's
//       defaults, cPlayer overrides them); cRider's overrides carry delta -0x6C0;
//   [O] SSX On Tour's DWARF: class cBeing (size 0x4, the vptr only) with ~cBeing, getVelocity ... flagEventTrickMultiplier
//       in this order (slots 1-15); On Tour's cRider declares vibrationUpdate, SSX 3's table has the four vibration
//       slots in cBeing's part;
//   [R] parameter and return types from the retail implementations (cRider_applyBoost(cRider*, t4Vector*),
//       cRider_initForMission(.., t4Matrix*), cPlayer_vibrationBump(.., float), ...): pointers where On Tour has
//       references (`const t4Vector& getVelocity() const`), which compile to the same code.
// Slot 20 is a pure virtual, scorePath (On Tour: cRider::scorePath() = 0; cPlayer_scorePath / cComputer_scorePath fill
// it). It is not declared: the flat cPlayer / cComputer / cMissionRider headers embed a cRider by value (their virtual
// base), and an abstract class cannot be a member.

#include "layout.h"

struct t4Vector;
struct t4Matrix;
struct tInstanceRecord;

class cBeing {
public:
    // 0x000: the vptr (in a cRider: 0x6C0). Slots:
    virtual ~cBeing();                                       // 1  cRider__cRider (deleting dtor)
    virtual t4Vector* getVelocity();                         // 2  cRider_getVelocity
    virtual bool isIntersecting(tInstanceRecord* inst);      // 3  cRider_isIntersecting
    virtual void applyBoost(t4Vector* v);                    // 4  cRider_applyBoost
    virtual t4Vector* getPosition();                         // 5  cRider_getPosition
    virtual t4Matrix* getTransform();                        // 6  cBeing_getTransform
    virtual int getAIIndex() __asm__("cBeing_getAIIndex__FPv");                                // 7  cRider_getAIIndex
    virtual bool isHuman() __asm__("cBeing_isHuman__FPv");                                  // 8  cRider_isHuman
    virtual bool isComputer() __asm__("cBeing_isComputer__FPv");                               // 9  cRider_isComputer
    virtual void transport(t4Matrix* mat);                   // 10 cRider_transport
    virtual void resetToPath() __asm__("cBeing_resetToPath__FPv");                              // 11 cBeing_resetToPath
    virtual void initForMission(t4Matrix* mat);              // 12 cRider_initForMission
    virtual void flagEventSpeedBoost(float sec);             // 13 cRider_flagEventSpeedBoost
    virtual void flagEventTrickBoost(float sec);             // 14 cRider_flagEventTrickBoost
    virtual void flagEventTrickMultiplier(float mult);       // 15 cRider_flagEventTrickMultiplier
    virtual void vibrationReset();                           // 16 empty; cPlayer_vibrationReset
    virtual void vibrationBump(float val);                   // 17 empty; cPlayer_vibrationBump
    virtual void vibrationRumble(float val);                 // 18 empty; cPlayer_vibrationRumble
    virtual void vibrationUpdate();                          // 19 empty; cPlayer_vibrationUpdate
};

CHECK_SIZE(cBeing, 0x4);

#endif /* OBJECT_BEING_H */
