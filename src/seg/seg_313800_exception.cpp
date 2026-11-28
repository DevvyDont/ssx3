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
INCLUDE_ASM("seg/seg_313800_exception", terminate);
#ifdef SKIP_ASM
// __terminate_func / __unexpected_func (retail data __terminate_func / __unexpected_func); never return
typedef void (*sVfpNoRet)(void) __attribute__((__noreturn__));
extern sVfpNoRet __terminate_func_f __asm__("__terminate_func");
// terminate()
void terminate_800(void) __asm__("terminate") __attribute__((__noreturn__));
/**
 * Calls the current terminate handler __terminate_func (never returns).
 *
 * __default_unexpected__Fv, the C++ exception helpers and __eh_alloc call it.
 */
void terminate_800(void)
{
    __terminate_func_f();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __default_unexpected__Fv);
#ifdef SKIP_ASM
// __default_unexpected(): unexpected() falls back to terminate()
void unexpected_dflt_818(void) __asm__("__default_unexpected__Fv") __attribute__((__noreturn__));
/**
 * Default unexpected-exception handler: calls terminate (never returns).
 *
 * The unexpected-handler pointer __unexpected_func starts out pointing here.
 */
void unexpected_dflt_818(void)
{
    terminate_800();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", set_terminate__FPFv_v);
#ifdef SKIP_ASM
// set_terminate(): install a new terminate handler, return the old one
extern void* __terminate_func_p __asm__("__terminate_func");
void* set_terminate_828(void* f) __asm__("set_terminate__FPFv_v");
/**
 * Installs a new terminate handler in __terminate_func and returns the old one.
 *
 * @param newHandler  the new handler
 * @return the previous handler
 */
void* set_terminate_828(void* newHandler)
{
    void* old = __terminate_func_p;
    __terminate_func_p = newHandler;
    return old;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", set_unexpected__FPFv_v);
#ifdef SKIP_ASM
extern int __unexpected_func[];
/**
 * Installs a new unexpected-exception handler in __unexpected_func and returns the old one.
 *
 * @param newHandler  the new handler
 * @return the previous handler
 */
int set_unexpected__FPFv_v(int newHandler) {
    int oldHandler = __unexpected_func[0];
    __unexpected_func[0] = newHandler;
    return oldHandler;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", unexpected__Fv);
#ifdef SKIP_ASM
// __terminate_func / __unexpected_func (retail data __terminate_func / __unexpected_func); never return
extern sVfpNoRet __unexpected_func_f __asm__("__unexpected_func");
// unexpected()
void unexpected_848(void) __asm__("unexpected__Fv") __attribute__((__noreturn__));
/**
 * Calls the current unexpected-exception handler __unexpected_func (never returns).
 *
 * __check_eh_spec calls it.
 */
void unexpected_848(void)
{
    __unexpected_func_f();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __cp_exception_info);
#ifdef SKIP_ASM
// A C++ exception record (cp/exception.cc struct cp_eh_info, 0x30): the generic __eh_info header (match function,
// language, version), then the thrown object and its handling state. This compiler stores the bool caught as a word.
struct sCpEh {
    int (*match)(int*, int (*)(void), short*);
    short lang, ver;
    void* value;
    void* type;
    void (*cleanup)(void*, int);
    int caught;
    sCpEh *next;
    long handlers;
    void* orig;
};
// __cp_exception_info(): address of the current exception's value in its cp_eh_info
extern sCpEh** __get_eh_info_v(void) __asm__("__get_eh_info");
void* cp_exception_info_860(void) __asm__("__cp_exception_info");
/**
 * Returns the address of the current C++ exception's value field.
 *
 * Reads the cp_eh_info record through __get_eh_info and returns the address 8 bytes in (after the generic info).
 *
 * @return the address of the value pointer
 */
void* cp_exception_info_860(void)
{
    return &(*__get_eh_info_v())->value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __cp_eh_info);
#ifdef SKIP_ASM
extern sCpEh **__get_eh_info_a(void) __asm__("__get_eh_info");
/**
 * Returns the current C++ exception's info record (the word __get_eh_info points at).
 *
 * @return the record
 */
sCpEh *__cp_eh_info(void) {
    return *__get_eh_info_a();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __start_cp_handler);
#ifdef SKIP_ASM
extern sCpEh **__get_eh_info_a(void) __asm__("__get_eh_info");
sCpEh *start_cp_handler_8a8(void) __asm__("__start_cp_handler");
/**
 * Marks the current C++ exception as caught and counts one more active handler.
 *
 * Sets the record's caught word (+0x14) to 1 and increments its 64-bit handler count (+0x20).
 *
 * @return the record
 */
sCpEh *start_cp_handler_8a8(void) {
    sCpEh *ehInfo = *__get_eh_info_a();
    ehInfo->caught = 1;
    ehInfo->handlers += 1;
    return ehInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __eh_alloc);
#ifdef SKIP_ASM
extern int malloc_a(void) __asm__("malloc");
extern int term_8e0(void) __asm__("terminate") __attribute__((noreturn));
int eh_alloc_8e0(void) __asm__("__eh_alloc");
/**
 * Allocates memory for a thrown object, terminating the program when none is left.
 *
 * The size arrives in the argument register and goes straight to malloc (the C definition declares no
 * parameter). A null result calls terminate.
 *
 * @return the block
 */
int eh_alloc_8e0(void) {
    if (malloc_a() == 0)
        return term_8e0();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __eh_free);
#ifdef SKIP_ASM
extern int free_a(void) __asm__("free");
/**
 * Frees memory of a thrown object (passes the pointer on to free).
 *
 * The pointer arrives in the argument register; the C definition declares no parameter.
 *
 * @return free's result
 */
int __eh_free(void) {
    return free_a();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __cplus_type_matcher);
#ifdef SKIP_ASM
extern int type_match_rtti_928(int, int, int) __asm__("__throw_type_match_rtti");
/**
 * Tests the current C++ exception against one catch clause and returns the object pointer to bind on a match.
 *
 * It is the match function __cp_push_exception stores in every C++ exception record (cp_eh_info). With an exception table
 * whose language halfword (+4) is not 4 (the DWARF code for C++) it returns 0. A catch-all clause (fn == -1) returns
 * the record's value (info[2]) unchanged. Otherwise it calls fn for the catch type's type_info and passes it, the
 * thrown type (info[3]) and the original object (info[10]) to __throw_type_match_rtti, the RTTI matcher; a nonzero result (the
 * object pointer adjusted to the caught base) is stored back into info[2].
 *
 * @param info            the C++ exception record (value at +8, type_info at +0xC, original object at +0x28)
 * @param fn              the catch clause's type_info function, or -1 for catch (...)
 * @param exceptionTable  the frame's exception descriptor, or null for the old-style mechanism
 * @return the object pointer to bind on a match, 0 when the clause does not match
 */
int __cplus_type_matcher(int *info, int (*fn)(void), short *exceptionTable) {
    int matchedValue;
    if (exceptionTable != 0 && exceptionTable[2] != 4)
        return 0;
    if (fn == (int (*)(void))-1)
        return info[2];
    matchedValue = type_match_rtti_928(fn(), info[3], info[10]);
    if (matchedValue)
        info[2] = matchedValue;
    return matchedValue;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __cp_push_exception);
#ifdef SKIP_ASM
extern sCpEh *eh_alloc_998(int) __asm__("__eh_alloc");
extern sCpEh **__get_eh_info_l(void) __asm__("__get_eh_info");
void cp_push_exception_998(void* value, void* type, void (*cleanup)(void*, int)) __asm__("__cp_push_exception");
/**
 * Pushes a new C++ exception record for a throw onto the exception chain.
 *
 * Allocates the 0x30-byte record with __eh_alloc (allocates, terminates on failure) and stores the thrown object as
 * both value (+8) and original value (+0x28), the type_info (+0xC) and the cleanup (+0x10); clears the caught flag
 * (+0x14) and the 64-bit handler count (+0x20); sets the match function to __cplus_type_matcher with language 4 (C++) and
 * version 1; then links the record in front of the chain whose head __get_eh_info returns.
 *
 * @param value    the thrown object
 * @param type     the thrown object's type_info
 * @param cleanup  the object's destructor, called with (value, 2) when the record is popped; null for none
 */
void cp_push_exception_998(void* value, void* type, void (*cleanup)(void*, int)) {
    sCpEh *ehInfo = eh_alloc_998(sizeof(sCpEh));
    sCpEh **infoChain;
    ehInfo->type = type;
    ehInfo->cleanup = cleanup;
    ehInfo->value = value;
    ehInfo->orig = value;
    ehInfo->handlers = 0;
    ehInfo->caught = 0;
    ehInfo->match = __cplus_type_matcher;
    ehInfo->lang = 4;
    ehInfo->ver = 1;
    infoChain = __get_eh_info_l();
    ehInfo->next = *infoChain;
    *infoChain = ehInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __cp_pop_exception);
#ifdef SKIP_ASM
extern sCpEh **eh_get_a28(void) __asm__("__get_eh_info");
extern int ptr_type_a28(void*) __asm__("__is_pointer");
extern void eh_free_a28(void *) __asm__("__eh_free");
extern int term_a28(void) __asm__("terminate") __attribute__((noreturn));
void cp_pop_exception_a28(sCpEh *ehInfo) __asm__("__cp_pop_exception");
/**
 * Releases one catch handler's hold on a C++ exception record and frees the record once nothing uses it.
 *
 * Decrements handlers and returns while it is nonzero, or while the record is the newest one on the chain
 * (__get_eh_info) and not caught (a rethrow in progress). Otherwise it unlinks the record, calling terminate
 * (terminate) if it is not on the chain, calls cleanup(value, 2) when set, frees the thrown object (original value)
 * with __eh_free unless __is_pointer says the type is a pointer type, and frees the record.
 *
 * Possible bug: cleanup gets value, which __cplus_type_matcher may have adjusted to a base subobject, rather than the original
 * object pointer (later GCC releases pass the original value).
 *
 * @param ehInfo  the exception record whose catch block is ending
 */
void cp_pop_exception_a28(sCpEh *ehInfo) {
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
        eh_free_a28(ehInfo->orig);
    eh_free_a28(ehInfo);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __uncatch_exception);
#ifdef SKIP_ASM
extern sCpEh **__get_eh_info_a(void) __asm__("__get_eh_info");
extern int term_af0(void) __asm__("terminate") __attribute__((noreturn));
/**
 * Marks the current C++ exception as uncaught again, as a rethrow requires.
 *
 * Takes the newest record from the chain head __get_eh_info returns; with none it calls terminate (terminate),
 * otherwise it clears the record's caught flag (+0x14). __check_eh_spec calls it before each rethrow (__throw).
 */
void __uncatch_exception(void) {
    sCpEh *ehInfo = *__get_eh_info_a();
    if (ehInfo == 0) {
        term_af0();
    }
    ehInfo->caught = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __check_eh_spec);
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
__asm__("terminate__Fv = terminate\n"
        "_vt$9exception = exception_vtbl\n"
        "_vt$13bad_exception = bad_exception_vtbl\n"
        "_$_13bad_exception = bad_exception__bad_exception\n"
        "_vt$8bad_cast = bad_cast_vtbl\n"
        "_$_8bad_cast = bad_cast__bad_cast\n"
        "_vt$10bad_typeid = bad_typeid_vtbl\n"
        "_$_10bad_typeid = bad_typeid__bad_typeid");

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

/* Defined earlier in exception.cc (__start_cp_handler, __eh_alloc, __cp_push_exception, __cp_pop_exception, __uncatch_exception);
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

extern "C" void __check_eh_spec (int n, const void **spec) __asm__("__check_eh_spec");
/**
 * Enforces a function's exception specification against the exception currently leaving it; never returns.
 *
 * Rethrows when the current exception's type matches one of the n allowed type_infos (__throw_type_match_rtti). Otherwise it
 * calls unexpected__Fv (unexpected); if that throws a different exception, the new one is checked against the list the
 * same way and rethrown on a match. Failing that it throws a new bad_exception (vtable bad_exception_vtbl, type __tf13bad_exception,
 * destructor bad_exception__bad_exception) when bad_exception's type_info matches an entry, and calls terminate (terminate)
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
INCLUDE_ASM("seg/seg_313800_exception", __throw_bad_cast);
#ifdef SKIP_ASM
/* exception.cc __throw_bad_cast (GCC runtime, GPL v2+ with the runtime linking exception) */
extern "C" void __throw_bad_cast (void) __asm__("__throw_bad_cast");
/**
 * Throws a new bad_cast; never returns.
 *
 * Allocates the object with __eh_alloc, gives it bad_cast's vtable (bad_cast_vtbl), pushes it with __cp_push_exception (type
 * from the vtable's type_info function, destructor bad_cast__bad_cast) and throws with __throw. g++ calls this for a
 * failed dynamic_cast to a reference type.
 */
extern "C" void
__throw_bad_cast (void)
{
  throw std::bad_cast ();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __throw_bad_typeid);
#ifdef SKIP_ASM
/* exception.cc __throw_bad_typeid (GCC runtime, GPL v2+ with the runtime linking exception) */
extern "C" void __throw_bad_typeid (void) __asm__("__throw_bad_typeid");
/**
 * Throws a new bad_typeid; never returns.
 *
 * Allocates the object with __eh_alloc, gives it bad_typeid's vtable (bad_typeid_vtbl), pushes it with __cp_push_exception
 * (type from the vtable's type_info function, destructor bad_typeid__bad_typeid) and throws with __throw. g++ calls this
 * when typeid is applied to a dereferenced null pointer.
 */
extern "C" void
__throw_bad_typeid (void)
{
  throw std::bad_typeid ();
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", uncaught_exception);
#ifdef SKIP_ASM
extern sCpEh** __get_eh_info_dd8(void) __asm__("__get_eh_info");
// uncaught_exception(): an exception is being thrown and no handler has caught it yet
bool uncaught_exception_dd8(void) __asm__("uncaught_exception");
/**
 * Returns whether an exception is being thrown that no handler has caught yet.
 *
 * True when the chain head __get_eh_info returns is a record whose caught flag (+0x14) is clear.
 *
 * @return true while an uncaught exception is in flight
 */
bool uncaught_exception_dd8(void)
{
    sCpEh* ehInfo = *__get_eh_info_dd8();
    return ehInfo && !ehInfo->caught;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", exception_what);
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
int exception_what(int *excObject) {
    int *(**vt)(int *) = *(int *(***)(int *))excObject;
    return *vt[1](excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", bad_exception__bad_exception);
#ifdef SKIP_ASM
extern int exception_vtbl[];
extern int operator_delete__FPi(int *);
/**
 * Destroys a bad_exception: puts back the base exception vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores exception_vtbl (exception's vtable) and calls operator_delete__FPi on flag & 1.
 * It fills the destructor slot of bad_exception's vtable bad_exception_vtbl.
 *
 * @param excObject  the bad_exception object
 * @param flags  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int bad_exception__bad_exception(int **excObject, int flags) {
    *excObject = exception_vtbl;
    if (flags & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", bad_exception_bad_exception);
#ifdef SKIP_ASM
extern char bad_exception_vtbl[];
/**
 * Constructs a bad_exception: stores its vtable (bad_exception_vtbl) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
void *bad_exception_bad_exception(void **excObject) {
    *excObject = bad_exception_vtbl;
    return excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __tf13bad_exception);
#ifdef SKIP_ASM
extern int D_0053EA10[];
extern char D_004998D0[];
extern int __ti9exception[];
extern int *__tf9exception_a(void) __asm__("__tf9exception");
extern void __rtti_si(void *, const char *, const void *);
int *__tf13bad_exception_a(void) __asm__("__tf13bad_exception");
/**
 * Returns the type_info object of class bad_exception, building it on first use.
 *
 * When D_0053EA10 is still zero, builds the base's type_info first (__tf9exception) and then constructs a
 * single-inheritance type_info in D_0053EA10 with __rtti_si, name "13bad_exception" (D_004998D0) and base
 * __ti9exception (exception's).
 *
 * @return the type_info object D_0053EA10
 */
int *__tf13bad_exception_a(void) {
    if (D_0053EA10[0] == 0) {
        __tf9exception_a();
        __rtti_si(D_0053EA10, D_004998D0, __ti9exception);
    }
    return D_0053EA10;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", exception__exception);
#ifdef SKIP_ASM
extern int exception_vtbl[];
extern int operator_delete__FPi(int *);
/**
 * Destroys an exception object: stores exception's vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores exception_vtbl and calls operator_delete__FPi on flag & 1. It fills the
 * destructor slot of exception's vtable exception_vtbl.
 *
 * @param excObject     the exception object
 * @param flag  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int exception__exception(int **excObject, int flag) {
    *excObject = exception_vtbl;
    if (flag & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", exception_exception);
#ifdef SKIP_ASM
extern int exception_vtbl[];
/**
 * Constructs an exception object: stores exception's vtable (exception_vtbl) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
int *exception_exception(int **excObject) {
    *excObject = exception_vtbl;
    return (int*)excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_313800_exception", __tf9exception);
#ifdef SKIP_ASM
extern char D_004998E0[];
extern int __ti9exception[];
extern void __rtti_user(void *, const char *);
int *__tf9exception_a(void) __asm__("__tf9exception");
/**
 * Returns the type_info object of class exception, building it on first use.
 *
 * When __ti9exception is still zero, constructs a user type_info (no bases) there with __rtti_user and the name
 * "9exception" (D_004998E0). The bad_exception, bad_cast and bad_typeid type_info functions call it for their base.
 *
 * @return the type_info object __ti9exception
 */
int *__tf9exception_a(void) {
    if (__ti9exception[0] == 0)
        __rtti_user(__ti9exception, D_004998E0);
    return __ti9exception;
}
#endif

}
