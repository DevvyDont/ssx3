#include "ealib/shape.h"
#include "vtable.h"
#include "bx/appman.h"
#include "visualfx/crowdanim2d.h"
#include "render/ps2graphicsman.h"
#include "visualfx/crowdrender2d.h"
#include <stdint.h>

//100%
//https://decomp.me/scratch/pOTwa

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_cCrowdRender2D__Fi);
#ifdef SKIP_ASM
/**
 * Constructs the 2D crowd renderer by calling cCrowdRender2D_init().
 *
 * @param param_1  the object being constructed; it is still in the first argument register when
 *                 cCrowdRender2D_init() is called
 * @return @p param_1
 */
int cCrowdRender2D_cCrowdRender2D(int param_1) {
	cCrowdRender2D_init();
	return param_1;
}
#endif

//100%
//https://decomp.me/scratch/zsTkS
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D__cCrowdRender2D__FPii);
#ifdef SKIP_ASM
/**
 * Destroys the 2D crowd renderer: purges it, then frees the object when asked to.
 *
 * @param param_1  the object being destroyed
 * @param param_2  g++ destructor flags; bit 0 set frees the object's memory
 */
void cCrowdRender2D__cCrowdRender2D(int* param_1, int param_2)
{
	cCrowdRender2D_purge(param_1);

	if ((param_2 & 1) != 0) {
		operator_delete(param_1);
	}

	return;
}
#endif

//100%
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_init__Fv);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCrowdAnimDef_B6D0 {
    int f0;
    int count;              // 0x04
    char names[16][5];      // 0x08
    char* shapes[16];       // 0x58
    int handles[16];        // 0x98
};
struct sVEnt_B6D0 {
    short delta;
    short index;
    void* fn;
};
typedef int (*tRegister_B6D0)(void*, char*, char*, int, int, int);
typedef void (*tRelease_B6D0)(void*, int);

extern sCrowdAnimDef_B6D0 cCrowdRender2D_mAnimTable_B6D0[] __asm__("cCrowdRender2D_mAnimTable");
extern char str_data_textures_crwd_ssh[];
extern "C" int FILE_load(const char* name, int flags);
extern "C" char* SHAPE_locate(int file, const char* name);
extern "C" int MEM_free(int file);

// PORT: the unit declares this as void(); defined under an asm label with its real type
extern "C" void cCrowdRender2D_init_impl(int* self) __asm__("cCrowdRender2D_init__Fv");
/**
 * Loads the 2D crowd textures ("data/textures/crwd.ssh") and registers every frame of the crowd animations.
 *
 * Each frame's shape is looked up and registered with the graphics manager (slot 46). The first shape's size
 * and format byte are stored at +0x4..+0xC. The unit declares this function without its parameter.
 *
 * @param self  the crowd renderer
 */
extern "C" void cCrowdRender2D_init_impl(int* self)
{
    int i;
    self[0] = FILE_load(str_data_textures_crwd_ssh, 0x5000000);
    for (i = 0; i < 1; i++) {
        int j;
        for (j = 0; j < cCrowdRender2D_mAnimTable_B6D0[i].count; j++) {
            char* shape = SHAPE_locate(self[0], cCrowdRender2D_mAnimTable_B6D0[i].names[j]);
            cCrowdRender2D_mAnimTable_B6D0[i].shapes[j] = shape;
            sVEnt_B6D0* vt = (sVEnt_B6D0*)VPTR_AT(cGraphicsMan_TheGraphicsMan, 0x10D8);
            cCrowdRender2D_mAnimTable_B6D0[i].handles[j] = ((tRegister_B6D0)vt[46].fn)(vthis(cGraphicsMan_TheGraphicsMan, vt[46]), shape, cCrowdRender2D_mAnimTable_B6D0[i].names[j], 0, 0, -1);
        }
    }
    SHAPE* first = (SHAPE*)cCrowdRender2D_mAnimTable_B6D0[0].shapes[0];
    self[1] = first->width;
    self[2] = first->height;
    self[3] = *(unsigned char*)first;
}
#endif

//100%
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_purge__FPi);
#ifdef SKIP_ASM
struct sCrowdVEntry2D {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Releases the 2D crowd textures: unregisters every animation frame (graphics slot 50) and frees the loaded
 * file.
 *
 * @param param_1  the crowd renderer; its first word is the loaded texture file
 * @return what MEM_free() returns
 */
int cCrowdRender2D_purge(int* param_1)
{
    int i;
    for (i = 0; i < 1; i++) {
        int j;
        for (j = 0; j < cCrowdRender2D_mAnimTable_B6D0[i].count; j++) {
            cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
            sCrowdVEntry2D* vt = (sCrowdVEntry2D*)VPTR_AT(obj, 0x10D8);
            vt[50].fn(vthis(obj, vt[50]), cCrowdRender2D_mAnimTable_B6D0[i].handles[j]);
        }
    }
    return MEM_free(*param_1);
}
#endif

//100%
//https://decomp.me/scratch/Rbiv4
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_constructCrowdAnim2D__FPv);
#ifdef SKIP_ASM

extern "C" void* cMemMan_alloc(int a, const char* b, uint32_t c, int d);

/**
 * Allocates and constructs a 2D crowd animation.
 *
 * Allocates 0x20 bytes named "cCrowdAnim2D" (type 0x20000000) and runs cCrowdAnim2D_cCrowdAnim2D() on them,
 * which starts the animation at the first frame of the default crowd animation in play mode 4.
 *
 * @param self  stored in the animation (offset 0x1C); the app passes its crowd renderer
 * @return the new animation
 */
void* cCrowdRender2D_constructCrowdAnim2D(void* self) {
	void* memory = cMemMan_alloc(0x20, str_cCrowdAnim2D, 0x20000000, 0);
	return cCrowdAnim2D_cCrowdAnim2D(memory, self);
}
#endif

//100%
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_cCrowdAnim2D__FPvT0);
#ifdef SKIP_ASM
struct sCrowdAnim2D {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    void* f1C;
};

// cCrowdRender2D_mAnimTable is one 0xD8-byte crowd anim def (sCrowdAnimDef_BAC0 below); its frames[] start at +0x98.
struct sCrowdAnimDefFrames_ctor { char pad_0x0[0x98]; int frames[16]; };
extern sCrowdAnimDefFrames_ctor cCrowdRender2D_mAnimTable_ctor __asm__("cCrowdRender2D_mAnimTable");

/**
 * Constructs a 2D crowd animation: type and animation 0 at frame 0, advancing every 3 ticks from now.
 *
 * @param memory   the memory for the animation (0x20 bytes)
 * @param param_1  stored at +0x1C (purpose unknown)
 * @return @p memory
 */
void* cCrowdAnim2D_cCrowdAnim2D(void* memory, void* param_1)
{
    sCrowdAnim2D* anim = (sCrowdAnim2D*)memory;
    anim->f1C = param_1;
    anim->f8 = 4;
    anim->f4 = 0;
    anim->fC = 0;
    anim->f10 = cCrowdRender2D_mAnimTable_ctor.frames[0];
    anim->f0 = 0;
    anim->f18 = cAppMan_TheAppMan->Tick;
    anim->f14 = 3;
    return anim;
}
#endif

//100%
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_advanceFrame);
#ifdef SKIP_ASM
/**
 * Steps a crowd animation frame according to its play mode and returns the new frame.
 *
 * Mode 0 plays forward and holds the last frame, 1 plays backward and holds frame 0, 2 and 3 ping-pong forward
 * and backward (the mode flips at each end), and 4 plays forward and loops to frame 0. Modes 2-4 leave a
 * one-frame animation alone. @p self is not used.
 *
 * @param frame  current frame
 * @param n      number of frames
 * @param mode   play mode; switched between 2 and 3 in place when a ping-pong reverses
 * @return the new frame
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// Steps a crowd animation frame for the current play mode and returns the new frame.
extern "C" int cCrowdAnim2D_advanceFrame(cCrowdAnim2D* self, int frame, int n, int* mode)
{
    switch (*mode) {
    case 0:
        frame++;
        if (frame >= n) {
            frame = n - 1;
        }
        break;
    case 1:
        frame--;
        if (frame < 0) {
            frame = 0;
        }
        break;
    case 2:
        if (n == 1) {
            break;
        }
        frame++;
        if (frame >= n) {
            *mode = 3;
            frame = n - 2;
        }
        break;
    case 3:
        if (n == 1) {
            break;
        }
        frame--;
        if (frame < 0) {
            *mode = 2;
            frame = 1;
        }
        break;
    case 4:
        if (n == 1) {
            break;
        }
        frame++;
        if (frame >= n) {
            frame = 0;
        }
        break;
    }
    return frame;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_update);
#ifdef SKIP_ASM
struct sCrowdAnimDef_BAC0 {
    int f0;
    int count;              // 0x4
    char pad_0x8[0x90];
    int frames[16];         // 0x98
};

struct sCrowdTrans_BAC0 {
    int anim;
    int mode;
    int kind;
};

extern sCrowdAnimDef_BAC0 cCrowdRender2D_mAnimTable[];
extern sCrowdTrans_BAC0 cCrowdRender2D_mAnimStateMap[];
extern int cCrowdRender2D_mTargetAnimCrowdState[];
extern "C" int cCrowdAnim2D_advanceFrame(cCrowdAnim2D* self, int frame, int count, int* dir);

struct sCrowdAnim2D_BAC0 {
    int type;               // 0x0
    int anim;               // 0x4
    int dir;                // 0x8
    int frame;              // 0xC
    int tex;                // 0x10
    int delay;              // 0x14
    int last;               // 0x18
    void* param;            // 0x1C
};

/**
 * Advances a 2D crowd animation once its delay has passed, moving to the next animation through the
 * transition table.
 *
 * The table (cCrowdRender2D_mAnimStateMap, by type and current animation) gives the next animation. Kind 0 switches when back
 * at frame 0, kind 1 at the last frame, kind 2 at once; then the frame's texture is stored.
 */
extern "C" void cCrowdAnim2D_update(cCrowdAnim2D* self)
{
    int now = cAppMan_TheAppMan->Tick;
    if (now - self->m_iLastTick < self->m_iUpdateTick) {
        return;
    }
    self->m_iLastTick = now;
    sCrowdTrans_BAC0* base = &cCrowdRender2D_mAnimStateMap[cCrowdRender2D_mTargetAnimCrowdState[self->m_TargetState]];
    // PORT: int arithmetic on the table address (matches the original operand order).
    sCrowdTrans_BAC0* tr = (sCrowdTrans_BAC0*)(self->m_CurrentAnimation * (int)sizeof(sCrowdTrans_BAC0) + (int)base);
    if (tr->anim == self->m_CurrentAnimation) {
        self->m_iCurrentFrame = cCrowdAnim2D_advanceFrame(self, self->m_iCurrentFrame, cCrowdRender2D_mAnimTable[tr->anim].count, &self->m_CurrentPlayType);
    } else {
        switch (tr->kind) {
        case 0: {
            int* dir = &self->m_CurrentPlayType;
            if (*dir == 0) {
                *dir = 1;
            }
            if (self->m_iCurrentFrame == 0) {
                self->m_CurrentAnimation = tr->anim;
                self->m_CurrentPlayType = tr->mode;
                if (self->m_CurrentPlayType == 1) {
                    self->m_iCurrentFrame = cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].count - 1;
                } else {
                    self->m_iCurrentFrame = 0;
                }
            } else {
                self->m_iCurrentFrame = cCrowdAnim2D_advanceFrame(self, self->m_iCurrentFrame, cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].count, dir);
            }
            break;
        }
        case 1: {
            int* dir = &self->m_CurrentPlayType;
            if (*dir == 1) {
                *dir = 0;
            }
            int cnt = cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].count;
            if (self->m_iCurrentFrame == cnt - 1) {
                self->m_CurrentAnimation = tr->anim;
                self->m_CurrentPlayType = tr->mode;
                if (self->m_CurrentPlayType == 1) {
                    self->m_iCurrentFrame = cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].count - 1;
                } else {
                    self->m_iCurrentFrame = 0;
                }
            } else {
                self->m_iCurrentFrame = cCrowdAnim2D_advanceFrame(self, self->m_iCurrentFrame, cnt, dir);
            }
            break;
        }
        case 2:
            self->m_CurrentAnimation = tr->anim;
            self->m_CurrentPlayType = tr->mode;
            self->m_iCurrentFrame = cCrowdAnim2D_advanceFrame(self, self->m_iCurrentFrame, cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].count, &self->m_CurrentPlayType);
            break;
        }
    }
    self->m_iCurrentFrameTexID = cCrowdRender2D_mAnimTable[self->m_CurrentAnimation].frames[self->m_iCurrentFrame];
}
#endif

