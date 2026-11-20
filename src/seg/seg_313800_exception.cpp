/* GNU CC C++ runtime support, reconstructed: cp/exception.cc (terminate, unexpected, the throw
   helpers). Original: Copyright (C) 1994, 95-97, 1998 Free Software Foundation.
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception: "if you link this library with other files, some of which are compiled with GCC, to
   produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).
   Bodies credited to peak3project come from their fork (github.com/peak3project/ssx3). */

#include "common.h"
#include "seg/seg_313800_exception.h"

extern "C" {

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412800);
#ifdef SKIP_ASM
// __terminate_func / __unexpected_func (retail data D_00453440 / D_00453A38); never return
typedef void (*sVfpNoRet)(void) __attribute__((__noreturn__));
extern sVfpNoRet D_00453440_f __asm__("D_00453440");
// terminate()
void terminate_800(void) __asm__("func_00412800") __attribute__((__noreturn__));
/**
 * Calls the current terminate handler D_00453440 (never returns).
 *
 * func_00412818, the C++ exception helpers and func_004128E0 call it.
 */
void terminate_800(void)
{
    D_00453440_f();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412818);
#ifdef SKIP_ASM
// __default_unexpected(): unexpected() falls back to terminate()
void unexpected_dflt_818(void) __asm__("func_00412818") __attribute__((__noreturn__));
/**
 * Default unexpected-exception handler: calls func_00412800 (never returns).
 *
 * The unexpected-handler pointer D_00453A38 starts out pointing here.
 */
void unexpected_dflt_818(void)
{
    terminate_800();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412828);
#ifdef SKIP_ASM
// set_terminate(): install a new terminate handler, return the old one
extern void* D_00453440_p __asm__("D_00453440");
void* set_terminate_828(void* f) __asm__("func_00412828");
/**
 * Installs a new terminate handler in D_00453440 and returns the old one.
 *
 * @param newHandler  the new handler
 * @return the previous handler
 */
void* set_terminate_828(void* newHandler)
{
    void* old = D_00453440_p;
    D_00453440_p = newHandler;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412838);
#ifdef SKIP_ASM
extern int D_00453A38[];
/**
 * Installs a new unexpected-exception handler in D_00453A38 and returns the old one.
 *
 * @param newHandler  the new handler
 * @return the previous handler
 */
int func_00412838(int newHandler) {
    int oldHandler = D_00453A38[0];
    D_00453A38[0] = newHandler;
    return oldHandler;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412848);
#ifdef SKIP_ASM
// __terminate_func / __unexpected_func (retail data D_00453440 / D_00453A38); never return
extern sVfpNoRet D_00453A38_f __asm__("D_00453A38");
// unexpected()
void unexpected_848(void) __asm__("func_00412848") __attribute__((__noreturn__));
/**
 * Calls the current unexpected-exception handler D_00453A38 (never returns).
 *
 * func_00412B20 calls it.
 */
void unexpected_848(void)
{
    D_00453A38_f();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412860);
#ifdef SKIP_ASM
// __cp_exception_info(): address of the current exception's value in its cp_eh_info
extern void* func_00410490_v(void) __asm__("func_00410490");
void* cp_exception_info_860(void) __asm__("func_00412860");
/**
 * Returns the address of the current C++ exception's value field.
 *
 * Reads the cp_eh_info record through func_00410490 and returns the address 8 bytes in (after the generic info).
 *
 * @return the address of the value pointer
 */
void* cp_exception_info_860(void)
{
    return (char*)*(void**)func_00410490_v() + 8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412888);
#ifdef SKIP_ASM
extern int *func_00410490(void);
/**
 * Returns the current C++ exception's info record (the word func_00410490 points at).
 *
 * @return the record
 */
int func_00412888(void) {
    return *func_00410490();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_004128A8);
#ifdef SKIP_ASM
extern int *func_00410490(void);
/**
 * Marks the current C++ exception as caught and counts one more active handler.
 *
 * Sets the record's caught word (+0x14) to 1 and increments its 64-bit handler count (+0x20).
 *
 * @return the record
 */
int *func_004128A8(void) {
    int *ehInfo = (int *)*func_00410490();
    ehInfo[5] = 1;
    *(long *)(ehInfo + 8) += 1;
    return ehInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_004128E0);
#ifdef SKIP_ASM
extern int func_004188C0(void);
extern int func_00412800(void) __attribute__((noreturn));
/**
 * Allocates memory for a thrown object, terminating the program when none is left.
 *
 * The size arrives in the argument register and goes straight to func_004188C0 (the C definition declares no
 * parameter). A null result calls func_00412800.
 *
 * @return the block
 */
int func_004128E0(void) {
    if (func_004188C0() == 0)
        return func_00412800();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412908);
#ifdef SKIP_ASM
extern int func_00418910(void);
/**
 * Frees memory of a thrown object (passes the pointer on to func_00418910).
 *
 * The pointer arrives in the argument register; the C definition declares no parameter.
 *
 * @return func_00418910's result
 */
int func_00412908(void) {
    return func_00418910();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412928);
#ifdef SKIP_ASM
extern int func_00415260(int, int, int);
/**
 * Tests the current C++ exception against one catch clause and returns the object pointer to bind on a match.
 *
 * It is the match function func_00412998 stores in every C++ exception record (cp_eh_info). With an exception table
 * whose language halfword (+4) is not 4 (the DWARF code for C++) it returns 0. A catch-all clause (fn == -1) returns
 * the record's value (info[2]) unchanged. Otherwise it calls fn for the catch type's type_info and passes it, the
 * thrown type (info[3]) and the original object (info[10]) to func_00415260, the RTTI matcher; a nonzero result (the
 * object pointer adjusted to the caught base) is stored back into info[2].
 *
 * @param info            the C++ exception record (value at +8, type_info at +0xC, original object at +0x28)
 * @param fn              the catch clause's type_info function, or -1 for catch (...)
 * @param exceptionTable  the frame's exception descriptor, or null for the old-style mechanism
 * @return the object pointer to bind on a match, 0 when the clause does not match
 */
int func_00412928(int *info, int (*fn)(void), short *exceptionTable) {
    int matchedValue;
    if (exceptionTable != 0 && exceptionTable[2] != 4)
        return 0;
    if (fn == (int (*)(void))-1)
        return info[2];
    matchedValue = func_00415260(fn(), info[3], info[10]);
    if (matchedValue)
        info[2] = matchedValue;
    return matchedValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412998);
#ifdef SKIP_ASM
extern int *func_004128E0_a(int) __asm__("func_004128E0");
extern int *func_00410490_l(void) __asm__("func_00410490");
/**
 * Pushes a new C++ exception record for a throw onto the exception chain.
 *
 * Allocates the 0x30-byte record with func_004128E0 (allocates, terminates on failure) and stores the thrown object as
 * both value (+8) and original value (+0x28), the type_info (+0xC) and the cleanup (+0x10); clears the caught flag
 * (+0x14) and the 64-bit handler count (+0x20); sets the match function to func_00412928 with language 4 (C++) and
 * version 1; then links the record in front of the chain whose head func_00410490 returns.
 *
 * @param value    the thrown object
 * @param type     the thrown object's type_info
 * @param cleanup  the object's destructor, called with (value, 2) when the record is popped; null for none
 */
void func_00412998(int value, int type, int cleanup) {
    int *ehInfo = func_004128E0_a(0x30);
    int *infoChain;
    ehInfo[3] = type;
    ehInfo[4] = cleanup;
    ehInfo[2] = value;
    ehInfo[10] = value;
    *(long long *)(ehInfo + 8) = 0;
    ehInfo[5] = 0;
    ehInfo[0] = (int)func_00412928;
    ((short *)ehInfo)[2] = 4;
    ((short *)ehInfo)[3] = 1;
    infoChain = func_00410490_l();
    ehInfo[6] = *infoChain;
    *infoChain = (int)ehInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412A28);
#ifdef SKIP_ASM
struct sCpEh { int (*match)(void); short lang, ver; int value; int type; void (*cleanup)(int, int); int caught; sCpEh *next; long handlers; int orig; };
extern sCpEh **eh_get_a28(void) __asm__("func_00410490");
extern int ptr_type_a28(int) __asm__("func_00415728");
extern void eh_free_a28(void *) __asm__("func_00412908");
extern int term_a28(void) __asm__("func_00412800") __attribute__((noreturn));
/**
 * Releases one catch handler's hold on a C++ exception record and frees the record once nothing uses it.
 *
 * Decrements handlers and returns while it is nonzero, or while the record is the newest one on the chain
 * (func_00410490) and not caught (a rethrow in progress). Otherwise it unlinks the record, calling func_00412800
 * (terminate) if it is not on the chain, calls cleanup(value, 2) when set, frees the thrown object (original value)
 * with func_00412908 unless func_00415728 says the type is a pointer type, and frees the record.
 *
 * Possible bug: cleanup gets value, which func_00412928 may have adjusted to a base subobject, rather than the original
 * object pointer (later GCC releases pass the original value).
 *
 * @param ehInfo  the exception record whose catch block is ending
 */
void func_00412A28(sCpEh *ehInfo) {
    sCpEh **chainLink = eh_get_a28();
    if (--ehInfo->handlers != 0)
        return;
    if (ehInfo == *chainLink && !ehInfo->caught)
        return;
    for (; *chainLink && *chainLink != ehInfo; chainLink = &(*chainLink)->next)
        ;
    if (!*chainLink)
        term_a28();
    *chainLink = ehInfo->next;
    if (ehInfo->cleanup)
        ehInfo->cleanup(ehInfo->value, 2);
    if (!ptr_type_a28(ehInfo->type))
        eh_free_a28((void *)ehInfo->orig);
    eh_free_a28(ehInfo);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412AF0);
#ifdef SKIP_ASM
extern int *func_00410490(void);
extern int func_00412800(void) __attribute__((noreturn));
/**
 * Marks the current C++ exception as uncaught again, as a rethrow requires.
 *
 * Takes the newest record from the chain head func_00410490 returns; with none it calls func_00412800 (terminate),
 * otherwise it clears the record's caught flag (+0x14). func_00412B20 calls it before each rethrow (func_00410F48).
 */
void func_00412AF0(void) {
    int *ehInfo = (int *)*func_00410490();
    if (ehInfo == 0) {
        func_00412800();
    }
    ehInfo[5] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412B20);
#ifdef SKIP_ASM
/* cp/exception.cc (GCC runtime, GPL v2+ with the runtime linking exception): the declarations at the top of the
   file that __check_eh_spec, __throw_bad_cast and __throw_bad_typeid use, with the pieces of <typeinfo>,
   <exception> and eh-common.h they need. g++ 2.9 is run without -fhonor-std, so std:: names live in the global
   namespace (terminate__Fv, not terminate__3stdFv); the classes exception/bad_cast/bad_typeid/bad_exception come
   from include/seg/seg_313800_exception.h. */

/* g++ calls the EH runtime under fixed names, and the source declares or defines them under those names; bind
   each name to the retail symbol (assembler assignments: the relocations then name the retail symbols). __throw
   is libgcc2.c's (emitted by the back end), __throw_type_match_rtti tinfo2.cc's, the vtables, out-of-line
   destructors and typeid functions those of exception.cc's #pragma implementation "exception". */
__asm__("__get_eh_info = func_00410490\n"
        "__throw = func_00410F48\n"
        "terminate__Fv = func_00412800\n"
        "unexpected__Fv = func_00412848\n"
        "__start_cp_handler = func_004128A8\n"
        "__eh_alloc = func_004128E0\n"
        "__cp_push_exception = func_00412998\n"
        "__cp_pop_exception = func_00412A28\n"
        "__uncatch_exception = func_00412AF0\n"
        "__throw_type_match_rtti = func_00415260\n"
        "_vt$9exception = D_004998B8\n"
        "_vt$13bad_exception = D_004998A0\n"
        "_$_13bad_exception = func_00412E38\n"
        "__tf13bad_exception = func_00412E80\n"
        "_vt$8bad_cast = D_00499A30\n"
        "_$_8bad_cast = func_00415120\n"
        "_vt$10bad_typeid = D_00499A18\n"
        "_$_10bad_typeid = func_00415088");

extern "C++" {

/* <typeinfo> (needed complete for typeid) */
class type_info {
private:
  // assigning type_info is not supported.  made private.
  type_info& operator= (const type_info&);
  type_info (const type_info&);

protected:
  explicit type_info (const char *n): _name (n) { }

  const char *_name;

public:
  // destructor
  virtual ~type_info ();

  bool before (const type_info& arg) const;
  const char* name () const
    { return _name; }
  bool operator== (const type_info& arg) const;
  bool operator!= (const type_info& arg) const;
};

/* <exception> */
void terminate () __attribute__ ((__noreturn__));
void unexpected () __attribute__ ((__noreturn__));

}

typedef __SIZE_TYPE__ size_t;

/* eh-common.h */
typedef void * (*__eh_matcher)          (void *, void *, void *);

typedef struct __eh_info
{
  __eh_matcher match_function;
  short language;
  short version;
} __eh_info;

/* C++-specific state about the current exception.
   This must match init_exception_processing().

   Note that handlers and caught are not redundant; when rethrown, an
   exception can have multiple active handlers and still be considered
   uncaught.  */

struct cp_eh_info
{
  __eh_info eh_info;
  void *value;
  void *type;
  void (*cleanup)(void *, int);
  bool caught;
  cp_eh_info *next;
  long handlers;
  void *original_value;
};

/* Language-specific EH info pointer, defined in libgcc2. */

extern "C" cp_eh_info **__get_eh_info (); 	// actually void **

#define CP_EH_INFO ((cp_eh_info *) *__get_eh_info ())

/* Defined earlier in exception.cc (func_004128A8, func_004128E0, func_00412998, func_00412A28, func_00412AF0);
   declared here under their source names and signatures, so the code g++ generates for catch, throw and rethrow
   uses them as in the original file. */
extern "C" cp_eh_info *__start_cp_handler (void);
extern "C" void *__eh_alloc (size_t size);
extern "C" void __cp_push_exception (void *value, void *type, void (*cleanup)(void *, int));
extern "C" void __cp_pop_exception (cp_eh_info *p);
extern "C" void __uncatch_exception (void);

/* exception.cc __check_eh_spec (GCC runtime, GPL v2+ with the runtime linking exception) */
/* As per [except.unexpected]:
   If an exception is thrown, we check it against the spec.  If it doesn't
   match, we call unexpected ().  If unexpected () throws, we check that
   exception against the spec.  If it doesn't match, if the spec allows
   bad_exception we throw that; otherwise we call terminate ().

   The compiler treats an exception spec as a try block with a generic
   handler that just calls this function with a list of the allowed
   exception types, so we have an active exception that can be rethrown.

   This function does not return.  */

extern "C" void __check_eh_spec (int n, const void **spec) __asm__("func_00412B20");
/**
 * Enforces a function's exception specification against the exception currently leaving it; never returns.
 *
 * Rethrows when the current exception's type matches one of the n allowed type_infos (func_00415260). Otherwise it
 * calls func_00412848 (unexpected); if that throws a different exception, the new one is checked against the list the
 * same way and rethrown on a match. Failing that it throws a new bad_exception (vtable D_004998A0, type func_00412E80,
 * destructor func_00412E38) when bad_exception's type_info matches an entry, and calls func_00412800 (terminate)
 * otherwise.
 *
 * @param n     number of entries in spec
 * @param spec  the type_info pointers of the allowed exception types
 */
extern "C" void
__check_eh_spec (int n, const void **spec)
{
  cp_eh_info *p = CP_EH_INFO;

  for (int i = 0; i < n; ++i)
    {
      if (__throw_type_match_rtti (spec[i], p->type, p->value))
	throw;
    }

  try
    {
      std::unexpected ();
    }
  catch (...)
    {
      // __exception_info is an artificial var pushed into each catch block.
      if (p != __exception_info)
	{
	  p = __exception_info;
	  for (int i = 0; i < n; ++i)
	    {
	      if (__throw_type_match_rtti (spec[i], p->type, p->value))
		throw;
	    }
	}

      const std::type_info &bad_exc = typeid (std::bad_exception);
      for (int i = 0; i < n; ++i)
	{
	  if (__throw_type_match_rtti (spec[i], &bad_exc, p->value))
	    throw std::bad_exception ();
	}

      terminate ();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412CD8);
#ifdef SKIP_ASM
/* exception.cc __throw_bad_cast (GCC runtime, GPL v2+ with the runtime linking exception) */
extern "C" void __throw_bad_cast (void) __asm__("func_00412CD8");
/**
 * Throws a new bad_cast; never returns.
 *
 * Allocates the object with func_004128E0, gives it bad_cast's vtable (D_00499A30), pushes it with func_00412998 (type
 * from the vtable's type_info function, destructor func_00415120) and throws with func_00410F48. g++ calls this for a
 * failed dynamic_cast to a reference type.
 */
extern "C" void
__throw_bad_cast (void)
{
  throw std::bad_cast ();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412D58);
#ifdef SKIP_ASM
/* exception.cc __throw_bad_typeid (GCC runtime, GPL v2+ with the runtime linking exception) */
extern "C" void __throw_bad_typeid (void) __asm__("func_00412D58");
/**
 * Throws a new bad_typeid; never returns.
 *
 * Allocates the object with func_004128E0, gives it bad_typeid's vtable (D_00499A18), pushes it with func_00412998
 * (type from the vtable's type_info function, destructor func_00415088) and throws with func_00410F48. g++ calls this
 * when typeid is applied to a dereferenced null pointer.
 */
extern "C" void
__throw_bad_typeid (void)
{
  throw std::bad_typeid ();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412DD8);
#ifdef SKIP_ASM
struct sCpEhInfoDD8 { int pad[5]; int caught; };
extern sCpEhInfoDD8** func_00410490_dd8(void) __asm__("func_00410490");
// uncaught_exception(): an exception is being thrown and no handler has caught it yet
bool uncaught_exception_dd8(void) __asm__("func_00412DD8");
/**
 * Returns whether an exception is being thrown that no handler has caught yet.
 *
 * True when the chain head func_00410490 returns is a record whose caught flag (+0x14) is clear.
 *
 * @return true while an uncaught exception is in flight
 */
bool uncaught_exception_dd8(void)
{
    sCpEhInfoDD8* ehInfo = *func_00410490_dd8();
    return ehInfo && !ehInfo->caught;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412E10);
#ifdef SKIP_ASM
/**
 * Returns the name of the exception object's dynamic type.
 *
 * Calls the type_info function in the first slot of the object's vtable (old-style 8-byte entries: the pointer is at
 * +4) and returns the first word of the type_info it returns, the mangled class name (e.g. "13bad_exception"). It fills
 * the method slot after the destructor in the exception, bad_exception, bad_cast and bad_typeid vtables.
 *
 * @param excObject  the exception object
 * @return its type_info's name string
 */
int func_00412E10(int *excObject) {
    int *(**vt)(int *) = *(int *(***)(int *))excObject;
    return *vt[1](excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412E38);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
/**
 * Destroys a bad_exception: puts back the base exception vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores D_004998B8 (exception's vtable) and calls operator_delete__FPi on flag & 1. It
 * fills the destructor slot of bad_exception's vtable D_004998A0.
 *
 * @param excObject  the bad_exception object
 * @param flags  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int func_00412E38(int **excObject, int flags) {
    *excObject = D_004998B8;
    if (flags & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412E68);
#ifdef SKIP_ASM
extern char D_004998A0[];
/**
 * Constructs a bad_exception: stores its vtable (D_004998A0) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
void *func_00412E68(void **excObject) {
    *excObject = D_004998A0;
    return excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412E80);
#ifdef SKIP_ASM
extern int D_0053EA10[];
extern int D_004998D0[];
extern int D_004A5BA8[];
extern int *func_00412F18(void);
extern void func_00414C00(int *, int, int);
/**
 * Returns the type_info object of class bad_exception, building it on first use.
 *
 * When D_0053EA10 is still zero, builds the base's type_info first (func_00412F18) and then constructs a
 * single-inheritance type_info in D_0053EA10 with func_00414C00, name "13bad_exception" (D_004998D0) and base
 * D_004A5BA8 (exception's).
 *
 * @return the type_info object D_0053EA10
 */
int *func_00412E80(void) {
    if (D_0053EA10[0] == 0) {
        func_00412F18();
        func_00414C00(D_0053EA10, (int)D_004998D0, (int)D_004A5BA8);
    }
    return D_0053EA10;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412ED0);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
/**
 * Destroys an exception object: stores exception's vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores D_004998B8 and calls operator_delete__FPi on flag & 1. It fills the destructor
 * slot of exception's vtable D_004998B8.
 *
 * @param excObject     the exception object
 * @param flag  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int func_00412ED0(int **excObject, int flag) {
    *excObject = D_004998B8;
    if (flag & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412F00);
#ifdef SKIP_ASM
extern int D_004998B8[];
/**
 * Constructs an exception object: stores exception's vtable (D_004998B8) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
int *func_00412F00(int **excObject) {
    *excObject = D_004998B8;
    return (int*)excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", func_00412F18);
#ifdef SKIP_ASM
extern int D_004998E0[];
extern int D_004A5BA8[];
extern void func_00414C20(int *, int);
/**
 * Returns the type_info object of class exception, building it on first use.
 *
 * When D_004A5BA8 is still zero, constructs a user type_info (no bases) there with func_00414C20 and the name
 * "9exception" (D_004998E0). The bad_exception, bad_cast and bad_typeid type_info functions call it for their base.
 *
 * @return the type_info object D_004A5BA8
 */
int *func_00412F18(void) {
    if (D_004A5BA8[0] == 0)
        func_00414C20(D_004A5BA8, (int)D_004998E0);
    return D_004A5BA8;
}
#endif

}
