#include "common.h"

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_cleanupOnExit);
#ifdef SKIP_ASM
struct cActiveTriggerList;
struct cCameraTriggerList;
struct cCameraTriggerStack;
void cCameraTriggerStack_init(cCameraTriggerStack* self);
void cActiveTriggerList_purge(cActiveTriggerList* self);
void cCameraTriggerList_init(cCameraTriggerList* self);
extern "C" void cCameraTriggerMan_purge(void* self);
extern "C" void cCamera_resetChaseControllerSwitches();

extern "C" void cCameraTriggerMan_cleanupOnExit(void* self)
{
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x14));
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x18));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x3C));
    cCameraTriggerMan_purge(self);
    cCameraTriggerList_init((cCameraTriggerList*)self);
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA08);
#ifdef SKIP_ASM
extern "C" void func_0016CA08(void) {

}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA10__FPv);
#ifdef SKIP_ASM
void func_0016CA10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CA18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
extern void* D_004A28A8;
extern "C" void func_0016BF40(void* self);

struct sCtVec4_CA18 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_CA18 {
    sCtVec4_CA18 min;
    sCtVec4_CA18 max;
};
struct sCtCell_CA18 {
    int level;
    int x, y, z;
};
struct sCtNode_CA18 {
    sCtNode_CA18* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_CA18 {
    sCtCell_CA18 cell;              // 0x00
    sCtNode_CA18* node;             // 0x10
};

extern "C" void func_00328F28_CA18(void* out, const void* box) __asm__("func_00328F28");
extern "C" void func_003284B8_CA18(void* node, int idx, void* item, const void* target, const void* cur) __asm__("func_003284B8");

class cCtShape_CA18 {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_CA18 bounds();
};

struct sCtTrig_CA18 {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_CA18* shape;           // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_CA18* reg;              // 0x20
};

struct sCtMan_CA18 {
    sCtTrig_CA18** trigs;           // 0x0
    int count;                      // 0x4
};

static inline void remove_CA18(char* w, void* item, const sCtBox_CA18* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_CA18 c;
    func_00328F28_CA18(&c, box);
    sCtRoot_CA18* r = (sCtRoot_CA18*)w;
    sCtRoot_CA18* rx = c.x < 0 ? r + 4 : r;
    sCtRoot_CA18* ry = c.y < 0 ? rx + 2 : rx;
    sCtRoot_CA18* rz = c.z < 0 ? ry + 1 : ry;
    sCtRoot_CA18* q = rz;
    func_003284B8_CA18(q->node, 2, item, &c, q);
    if (q->node->empty()) {
        operator_delete((int*)q->node);
        q->node = 0;
    }
}

extern "C" void func_0016CA18_impl(void* vself) __asm__("func_0016CA18");
extern "C" void func_0016CA18_impl(void* vself)
{
    sCtMan_CA18* self = (sCtMan_CA18*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_CA18* t = self->trigs[i];
        char* w = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20);
        sCtBox_CA18 box = t->shape->bounds();
        remove_CA18(w, t->item, &box);
    }
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x3C));
    func_0016BF40(vself);
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void operator_delete(int*);
extern void* D_004A28A8;
extern "C" void func_0016C098(void** self);

struct sCtVec4_P {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_P {
    sCtVec4_P min;
    sCtVec4_P max;
};
struct sCtCell_P {
    int level;
    int x, y, z;
};
struct sCtNode_P {
    sCtNode_P* child[2][2][2];      // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_P {
    sCtCell_P cell;                 // 0x00
    sCtNode_P* node;                // 0x10
};

extern "C" void func_00328F28_P(void* out, const void* box) __asm__("func_00328F28");
extern "C" void func_003284B8_P(void* node, int idx, void* item, const void* target, const void* cur) __asm__("func_003284B8");

class cCtShape_P {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_P bounds();
};

struct sCtTrig_P {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_P* shape;              // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_P* reg;                 // 0x20
};

struct sCtMan_P {
    sCtTrig_P** trigs;              // 0x0
    int count;                      // 0x4
};

static inline void remove_P(char* w, void* item, const sCtBox_P* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_P c;
    func_00328F28_P(&c, box);
    sCtRoot_P* r = (sCtRoot_P*)w;
    sCtRoot_P* rx = c.x < 0 ? r + 4 : r;
    sCtRoot_P* ry = c.y < 0 ? rx + 2 : rx;
    sCtRoot_P* rz = c.z < 0 ? ry + 1 : ry;
    sCtRoot_P* q = rz;
    func_003284B8_P(q->node, 2, item, &c, q);
    if (q->node->empty()) {
        operator_delete((int*)q->node);
        q->node = 0;
    }
}

// Takes every registered trigger out of the world's space hash and clears the trigger stacks and active lists.
extern "C" void cCameraTriggerMan_purge(void* vself)
{
    sCtMan_P* self = (sCtMan_P*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_P* t = self->trigs[i];
        if (t->reg != 0) {
            char* w = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20);
            sCtBox_P box = t->shape->bounds();
            remove_P(w, t->item, &box);
        }
    }
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)vself + 0x3C));
    func_0016C098((void**)vself);
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)vself + 0x14));
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)vself + 0x18));
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_loadTriggers);
#ifdef SKIP_ASM
struct sCamTrigMan {
    char** triggers; // 0x0
    int count;       // 0x4
};

extern "C" int cCameraTriggerList_loadFromFile(void* self, const char* file);

extern "C" int cCameraTriggerMan_loadTriggers(sCamTrigMan* self, const char* file)
{
    int err = cCameraTriggerList_loadFromFile(self, file);
    if (err != 0)
        return err;
    for (int i = 0; i < self->count; i++)
        *(int*)(self->triggers[i] + 0x20) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_streamIn);
#ifdef SKIP_ASM
struct sCamTrigManB {
    char** triggers; // 0x0
    int count;       // 0x4
};

extern "C" int cCameraTriggerList_loadFromBuffer(void* self, void* buf);
void cCameraTriggerMan_setInGameTriggers(void* self);

extern "C" void cCameraTriggerMan_streamIn(void* self, void* stream)
{
    cCameraTriggerMan_cleanupOnExit(self);
    cCameraTriggerList_loadFromBuffer(self, stream);
    sCamTrigManB* m = (sCamTrigManB*)self;
    for (int i = 0; i < m->count; i++)
        *(int*)(m->triggers[i] + 0x20) = 0;
    cCameraTriggerMan_setInGameTriggers(self);
}
#endif

extern void* D_004C5830[];
extern "C" void* func_0016CEF0(void*, void*, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEA8);
#ifdef SKIP_ASM
extern "C" void func_0016CA18();

extern "C" void func_0016CEA8(void) {
    func_0016CA18();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEC8__FPvi);
#ifdef SKIP_ASM
// PORT: the unit declares func_0016CEF0 as returning void*; this caller treats it as void.
void func_0016CEF0_CEC8(void*, void*, int) __asm__("func_0016CEF0");
void func_0016CEC8(void* self, int a1)
{
    func_0016CEF0_CEC8((void*)D_004C5830, self, a1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CEF0);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_streamIn(void* self, void* stream);

// PORT: the unit declares func_0016CEF0 as returning void*, but the body returns nothing.
void func_0016CEF0_impl(void* self, void* unused, void* stream) __asm__("func_0016CEF0");
void func_0016CEF0_impl(void* self, void* unused, void* stream)
{
    if (stream != 0) {
        cCameraTriggerMan_streamIn(self, stream);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF18);
#ifdef SKIP_ASM
// PORT: func_0016CF40__FPv is mangled as (void*), but this caller passes two args.
void func_0016CF40_2(void* mgr, void* self) __asm__("func_0016CF40__FPv");

extern "C" void func_0016CF18(void* self)
{
    func_0016CF40_2((void*)D_004C5830, self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF40__FPv);
#ifdef SKIP_ASM
void func_0016CF40(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016CF48);
#ifdef SKIP_ASM
void operator_delete(int*);
extern void* D_004A28A8;

struct sCtVec4_CF48 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sCtBox_CF48 {
    sCtVec4_CF48 min;
    sCtVec4_CF48 max;
};
struct sCtCell_CF48 {
    int level;
    int x, y, z;
};
struct sCtNode_CF48 {
    sCtNode_CF48* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sCtRoot_CF48 {
    sCtCell_CF48 cell;              // 0x00
    sCtNode_CF48* node;             // 0x10
};

extern "C" void func_00328F28(void* out, const void* box);
extern "C" void func_003284B8(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void func_00328C20(void* root, int idx, void* item, const void* target);

class cCtShape_CF48 {
public:
    char pad[0x24];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual sCtBox_CF48 bounds();
};

struct sCtTrig_CF48 {
    int f0;
    unsigned int mask;              // 0x4
    cCtShape_CF48* shape;           // 0x8
    char padC[0x8];
    char item[0xC];                 // 0x14
    sCtTrig_CF48* reg;              // 0x20
};

struct sCtMan_CF48 {
    sCtTrig_CF48** trigs;           // 0x0
    int count;                      // 0x4
};

static inline sCtRoot_CF48* findRoot_CF48(char* w, int x, int y, int z)
{
    sCtRoot_CF48* r = (sCtRoot_CF48*)w;
    sCtRoot_CF48* rx = x >= 0 ? r : r + 4;
    sCtRoot_CF48* ry = y >= 0 ? rx : rx + 2;
    sCtRoot_CF48* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_CF48(char* w, void* item, const sCtBox_CF48* box)
{
    (*(int*)(w + 0xA0))++;
    sCtCell_CF48 c;
    func_00328F28(&c, box);
    sCtRoot_CF48* r = findRoot_CF48(w, c.x, c.y, c.z);
    func_003284B8(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

static inline void insert_CF48(char* w, void* item, const sCtBox_CF48* box)
{
    sCtCell_CF48 c;
    func_00328F28(&c, box);
    func_00328C20(findRoot_CF48(w, c.x, c.y, c.z), 2, item, &c);
}

// PORT: the unit declares func_0016CF48 as returning void*; it returns nothing.
extern "C" void func_0016CF48_impl(void* vself, int mask) __asm__("func_0016CF48");

extern "C" void func_0016CF48_impl(void* vself, int mask)
{
    sCtMan_CF48* self = (sCtMan_CF48*)vself;
    for (int i = 0; i < self->count; i++) {
        sCtTrig_CF48* t = self->trigs[i];
        if (t->mask & mask) {
            if (t->reg == 0) {
                t->reg = t;
                char* w = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20);
                sCtBox_CF48 box = t->shape->bounds();
                insert_CF48(w, t->item, &box);
            }
        } else if (t->reg != 0) {
            char* w = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20);
            sCtBox_CF48 box = t->shape->bounds();
            remove_CF48(w, t->item, &box);
            t->reg = 0;
        }
    }
}
#endif

extern "C" void* func_0016CF48(void* self, int type);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerMan_setInGameTriggers__FPv);
#ifdef SKIP_ASM
void cCameraTriggerMan_setInGameTriggers(void* self)
{
    func_0016CF48(self, 2);
}
#endif

//99.29%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D1D8__FPv);
#ifdef SKIP_ASM
void* func_0016D1D8(void* self)
{
    return func_0016CF48(self, 1);
}
#endif

struct cCameraTriggerStack {
    int field_0x0;
    int field_0x4;
};

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D1F8);
#ifdef SKIP_ASM
extern "C" int func_0016D1F8(int arg0) {
    return arg0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", cCameraTriggerStack_init__FP19cCameraTriggerStack);
#ifdef SKIP_ASM
void cCameraTriggerStack_init(cCameraTriggerStack* self)
{
    self->field_0x0 = 0;
    self->field_0x4 = 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D210);
#ifdef SKIP_ASM
void cCameraTriggerStack_init(cCameraTriggerStack* self);

extern "C" cCameraTriggerStack* func_0016D210(cCameraTriggerStack* self)
{
    cCameraTriggerStack_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D238__FPvi);
#ifdef SKIP_ASM
void func_0016D238(void* self, int a1)
{
    *(int*)((char*)self + (*(int*)self << 2) + 0x8) = a1;
    *(int*)((char*)self + 0x4) = a1;
    *(int*)self = *(int*)self + 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D260);
#ifdef SKIP_ASM
struct sCamTrigStack {
    unsigned int count; // 0x0
    int top; // 0x4
    int items[1]; // 0x8
};

extern "C" int func_0016D260(sCamTrigStack* self, int key, int* changed, int* out)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        if (self->items[i] == key) {
            unsigned int j;
            for (j = i + 1; j < self->count; j++) {
                self->items[j - 1] = self->items[j];
            }
            self->count--;
            break;
        }
    }
    if (self->count != 0) {
        int t = self->items[self->count - 1];
        if (t == self->top) {
            *changed = 0;
        } else {
            *changed = 1;
            *out = t;
        }
        return 0;
    }
    *changed = 0;
    return 1;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016D320);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016E1D8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* D_004A28A8;
extern "C" void func_00162060(void* self, int mode, int arg, float blend);

struct sCamEvt_E1D8 {
    int type;
    int pad4;
    unsigned int id;
    float dur;
};

struct sCams_E1D8 {
    int count;
    char* cam[1];
};

extern "C" void func_0016E1D8(sCamEvt_E1D8* evt, char* rider)
{
    sCams_E1D8* cams = *(sCams_E1D8**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
    void* cam = *(void**)(cams->cam[*(int*)(rider + 0x870)] + 0xA8);
    switch (evt->type) {
    case 2: {
        float rate;
        if (evt->dur != 0.0f)
            rate = 1.0f / evt->dur * 0.016666668f;
        else
            rate = 1.0f;
        func_00162060(cam, 0x5C, (int)evt, rate);
        break;
    }
    case 1: {
        float rate;
        if (evt->dur != 0.0f)
            rate = 1.0f / evt->dur * 0.016666668f;
        else
            rate = 1.0f;
        func_00162060(cam, 0x5B, (int)evt, rate);
        break;
    }
    case 0: {
        float rate = 0.0f;
        if (evt->dur == rate)
            rate = 1.0f;
        else
            rate = 1.0f / evt->dur * 0.016666668f;
        int mode;
        switch (evt->id) {
        case 3: mode = 4; break;
        case 7: mode = 3; break;
        case 1: mode = 2; break;
        case 2: mode = 1; break;
        case 4: mode = 7; break;
        case 5: mode = 5; break;
        case 6: mode = 6; break;
        case 8: mode = 8; break;
        case 10: mode = 9; break;
        case 9: mode = 10; break;
        case 11: mode = 11; break;
        case 12: mode = 12; break;
        case 13: mode = 13; break;
        case 14: mode = 14; break;
        case 15: mode = 15; break;
        case 16: mode = 16; break;
        case 17: mode = 17; break;
        case 18: mode = 18; break;
        case 19: mode = 19; break;
        case 20: mode = 20; break;
        case 21: mode = 21; break;
        case 22: mode = 22; break;
        case 23: mode = 23; break;
        case 24: mode = 24; break;
        case 25: mode = 25; break;
        case 26: mode = 26; break;
        case 27: mode = 27; break;
        case 28: mode = 28; break;
        case 29: mode = 29; break;
        case 30: mode = 30; break;
        case 31: mode = 31; break;
        case 32: mode = 32; break;
        case 33: mode = 0x29; break;
        case 34: case 35: case 36: case 37: case 38: case 39: case 40: case 41: mode = evt->id - 1; break;
        case 44: mode = 0x2A; break;
        case 45: mode = 0x2B; break;
        case 46: mode = 0x2C; break;
        case 47: mode = 0x2D; break;
        case 48: mode = 0x2E; break;
        case 49: mode = 0x2F; break;
        case 50: mode = 0x30; break;
        case 51: mode = 0x31; break;
        case 52: mode = 0x32; break;
        case 53: mode = 0x33; break;
        case 54: mode = 0x34; break;
        case 55: mode = 0x35; break;
        case 56: mode = 0x36; break;
        case 57: mode = 0x37; break;
        case 58: mode = 0x38; break;
        case 59: mode = 0x39; break;
        case 60: mode = 0x3A; break;
        case 61: mode = 0x3B; break;
        case 62: mode = 0x3C; break;
        case 63: mode = 0x3D; break;
        case 64: mode = 0x3E; break;
        case 65: mode = 0x3F; break;
        case 66: mode = 0x40; break;
        case 67: mode = 0x41; break;
        case 68: mode = 0x42; break;
        case 69: mode = 0x43; break;
        case 70: mode = 0x44; break;
        case 71: mode = 0x45; break;
        case 72: mode = 0x46; break;
        case 73: mode = 0x47; break;
        case 74: mode = 0x48; break;
        case 75: mode = 0x49; break;
        case 42: mode = 0x4C; break;
        case 0:
        case 43:
        default: mode = 0; break;
        }
        func_00162060(cam, mode, 0, rate);
        break;
    }
    }
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016E560);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016F068);

INCLUDE_ASM("camera/trigger/cameratriggerman", func_0016FBB8);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_00170240);
#ifdef SKIP_ASM
struct sCtmV {
    float x, y, z, w;
    sCtmV() {}
    sCtmV(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM {
    float m[4][4];
    sCtmM() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM(const sCtmM& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM& operator=(const sCtmM& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM D_004FF1A0;
extern sCtmV D_004FF140;
extern sCtmV D_004FF150;
extern sCtmV D_004FF160;
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx(sCtmM* dst, const sCtmM* a, const sCtmM* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx(sCtmM* m, const sCtmV& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM ctmScaleMtx(const sCtmM* m, const sCtmV& v)
{
    sCtmM d;
    sCtmM* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV ctmApplyMtx(const sCtmM* m, const sCtmV& v)
{
    sCtmV out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis(sCtmM* m, const sCtmV& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    func_0031BE50(&s, &c, angle);
    sCtmM r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx(m, m, &r);
}

// World position of the point at src+0x2C: translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV func_00170240(char* src)
{
    sCtmM m(D_004FF1A0);
    float one = 1.0f;
    ctmTransMtx(&m, sCtmV(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis(&m, D_004FF140, *(float*)(src + 0x10));
    ctmRotAxis(&m, D_004FF150, *(float*)(src + 0x14));
    ctmRotAxis(&m, D_004FF160, *(float*)(src + 0xC));
    m = ctmScaleMtx(&m, sCtmV(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx(&m, sCtmV(*(float*)(src + 0x2C), *(float*)(src + 0x30), *(float*)(src + 0x34), one));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_001707B0);
#ifdef SKIP_ASM
extern sCtmM D_004FF1A0;
extern sCtmV D_004FF140;
extern sCtmV D_004FF150;
extern sCtmV D_004FF160;
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
// PORT: PS2-only VU0 inline asm (out = m * v).
// World position of the point at src+0x38: translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV func_001707B0(char* src)
{
    sCtmM m(D_004FF1A0);
    float one = 1.0f;
    ctmTransMtx(&m, sCtmV(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis(&m, D_004FF140, *(float*)(src + 0x10));
    ctmRotAxis(&m, D_004FF150, *(float*)(src + 0x14));
    ctmRotAxis(&m, D_004FF160, *(float*)(src + 0xC));
    m = ctmScaleMtx(&m, sCtmV(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx(&m, sCtmV(*(float*)(src + 0x38), *(float*)(src + 0x3C), *(float*)(src + 0x40), one));
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerman", func_00170D20);
#ifdef SKIP_ASM
struct sCtmV3_0D20 {
    float x, y, z;
};

struct sCtmV_0D20 {
    float x, y, z, w;
    sCtmV_0D20() {}
    sCtmV_0D20(const sCtmV3_0D20& v, float aw) : x(v.x), y(v.y), z(v.z), w(aw) {}
    sCtmV_0D20(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCtmM_0D20 {
    float m[4][4];
    sCtmM_0D20() {}
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_0D20(const sCtmM_0D20& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (64-byte matrix copy).
    sCtmM_0D20& operator=(const sCtmM_0D20& s)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

extern sCtmM_0D20 D_004FF1A0_0D20 __asm__("D_004FF1A0");
extern sCtmV_0D20 D_004FF140_0D20 __asm__("D_004FF140");
extern sCtmV_0D20 D_004FF150_0D20 __asm__("D_004FF150");
extern sCtmV_0D20 D_004FF160_0D20 __asm__("D_004FF160");
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void ctmMulMtx_0D20(sCtmM_0D20* dst, const sCtmM_0D20* a, const sCtmM_0D20* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void ctmTransMtx_0D20(sCtmM_0D20* m, const sCtmV_0D20& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale the rows of m by v: dst.row[i] = m.row[i] * v[i]).
static inline sCtmM_0D20 ctmScaleMtx_0D20(const sCtmM_0D20* m, const sCtmV_0D20& v)
{
    sCtmM_0D20 d;
    sCtmM_0D20* dst = &d;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf1, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulx.xyzw   $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw   $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw   $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw   $vf11, $vf7, $vf1w\n"
        "sqc2         $vf8, 0x0(%0)\n"
        "sqc2         $vf9, 0x10(%0)\n"
        "sqc2         $vf10, 0x20(%0)\n"
        "sqc2         $vf11, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(m), "m"(v)
        : "memory");
    return d;
}

// PORT: PS2-only VU0 inline asm (out = m * v).
static inline sCtmV_0D20 ctmApplyMtx_0D20(const sCtmM_0D20* m, const sCtmV_0D20& v)
{
    sCtmV_0D20 out;
    __asm__(
        ".set noreorder\n"
        "lqc2         $vf8, %2\n"
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set reorder\n"
        : "=m"(out)
        : "r"(m), "m"(v)
        : "memory");
    return out;
}

static inline void ctmRotAxis_0D20(sCtmM_0D20* m, const sCtmV_0D20& axis, float angle)
{
    float s;
    float c;
    float one = 1.0f;
    func_0031BE50(&s, &c, angle);
    sCtmM_0D20 r;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = one;
    ctmMulMtx_0D20(m, m, &r);
}

struct sCtmSrc_0D20 {
    char pad_0x0[0x28];
    sCtmV3_0D20 corners[4];      // 0x28 (count from the callers: corners 0..3)
};


// World position of corner `corner` (the 3-float points at src+0x28): translate(src+0x0) * rotations(0x10, 0x14, 0xC) * scale(src+0x18).
extern "C" sCtmV_0D20 func_00170D20(char* src, int corner)
{
    sCtmM_0D20 m(D_004FF1A0_0D20);
    float one = 1.0f;
    ctmTransMtx_0D20(&m, sCtmV_0D20(*(float*)(src + 0x0), *(float*)(src + 0x4), *(float*)(src + 0x8), one));
    ctmRotAxis_0D20(&m, D_004FF140_0D20, *(float*)(src + 0x10));
    ctmRotAxis_0D20(&m, D_004FF150_0D20, *(float*)(src + 0x14));
    ctmRotAxis_0D20(&m, D_004FF160_0D20, *(float*)(src + 0xC));
    m = ctmScaleMtx_0D20(&m, sCtmV_0D20(*(float*)(src + 0x18), *(float*)(src + 0x1C), *(float*)(src + 0x20), one));
    return ctmApplyMtx_0D20(&m, sCtmV_0D20(((sCtmSrc_0D20*)src)->corners[corner], one));
}
#endif

