/* GNU CC C++ runtime support, reconstructed: cp/tinfo.cc and cp/tinfo2.cc (type_info, dynamic_cast).
   Original: Copyright (C) 1994, 1996, 1998 Free Software Foundation.
   It is free software under the GNU General Public License, version 2 or (at your option) any later version, with
   this special exception: "if you link this library with other files, some of which are compiled with GCC, to
   produce an executable, this library does not by itself cause the resulting executable to be covered by the
   GNU General Public License. This exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License."
   This file stays under those terms; the repository's CC0 dedication does not apply to it (README, License).
   Bodies credited to peak3project come from their fork (github.com/peak3project/ssx3). */

#include "common.h"
#include "seg/seg_315B68_tinfo.h"

extern "C" {

int func_00414B98(int *a, int *b);

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", type_info__type_info);
#ifdef SKIP_ASM
extern int D_00499A48[];
extern int operator_delete__FPi(int *);
/**
 * Destroys a type_info: stores type_info's vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape; type_info keeps its vptr at +4, after the name pointer. Stores D_00499A48 and calls
 * operator_delete__FPi on flag & 1. The destructors of the derived type_info classes jump here.
 *
 * @param typeInfo     the type_info object
 * @param flag  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int type_info__type_info(int *typeInfo, int flag) {
    typeInfo[1] = (int)D_00499A48;
    if (flag & 1) return operator_delete__FPi(typeInfo);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414B98);
#ifdef SKIP_ASM
extern int func_004165A8(int, int);
/**
 * Returns whether two type_info objects describe the same type.
 *
 * True when they are the same object or their names compare equal with func_004165A8 (strcmp), so types match across
 * separately emitted copies.
 *
 * @param lhs  first type_info
 * @param rhs  second type_info
 * @return 1 when equal, else 0
 */
int func_00414B98(int *lhs, int *rhs) {
    int equal = 0;
    if (rhs == lhs || func_004165A8(*lhs, *rhs) == 0)
        equal = 1;
    return equal;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_class);
#ifdef SKIP_ASM
extern int D_004999D0[];
/**
 * Constructs a multiple-inheritance class type_info in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0), the __class_type_info vtable D_004999D0 (+4), the base list (+8) and the base count (+0xC).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled class name
 * @param baseList  the base_info array
 * @param baseCount  number of bases
 */
void __rtti_class(int *storage, int typeName, int baseList, int baseCount) {
    if (storage != 0) {
        storage[3] = baseCount;
        storage[0] = typeName;
        storage[2] = baseList;
        storage[1] = (int)D_004999D0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_si);
#ifdef SKIP_ASM
extern int D_004999E8[];
/**
 * Constructs a single-inheritance class type_info in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0), the __si_type_info vtable D_004999E8 (+4) and the base's type_info (+8). The type_info
 * functions of single-inheritance classes call it on first use.
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled class name
 * @param baseType  the base class's type_info
 */
void __rtti_si(int *storage, int typeName, int baseType) {
    if (storage != 0) {
        storage[2] = baseType;
        storage[0] = typeName;
        storage[1] = (int)D_004999E8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_user);
#ifdef SKIP_ASM
extern int D_00499A00[];
/**
 * Constructs the type_info of a class without bases in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0) and the __user_type_info vtable D_00499A00 (+4). The type_info functions of classes without
 * bases call it on first use.
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled class name
 */
void __rtti_user(int *storage, int typeName) {
    if (storage != 0) {
        storage[0] = typeName;
        storage[1] = (int)D_00499A00;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __user_type_info_dcast);
#ifdef SKIP_ASM
/**
 * dynamic_cast step for a class without bases: returns the object pointer when the class is the desired type.
 *
 * Compares the type_infos with func_00414B98; fills the dcast slot (2) of the __user_type_info vtable D_00499A00.
 *
 * @param thisType  this class's type_info
 * @param desired   the type_info cast to
 * @param isPublic  unused here
 * @param objPtr    the object pointer
 * @return objPtr on a match, else 0
 */
int __user_type_info_dcast(int *thisType, int *desired, int isPublic, int objPtr) {
    if (func_00414B98(thisType, desired) == 0) objPtr = 0;
    return objPtr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __si_type_info_dcast);
#ifdef SKIP_ASM
/**
 * dynamic_cast step for a single-inheritance class: matches this class or recurses into the base.
 *
 * Returns objPtr when func_00414B98 says this class is the desired type; otherwise calls the dcast slot of the base
 * type_info (+8) through its vtable (old-style entry: pointer at +0x14, this-delta at +0x10) with the same arguments.
 * Fills the dcast slot of the __si_type_info vtable D_004999E8.
 *
 * @param thisType  this class's type_info
 * @param desired   the type_info cast to
 * @param isPublic  nonzero to allow only public bases
 * @param objPtr    the object pointer
 * @param subType   the static type of the source subobject, or null
 * @param subPtr    the source subobject pointer
 * @return the adjusted pointer, or 0 when the cast fails
 */
int __si_type_info_dcast(int *thisType, int desired, int isPublic, int objPtr, int subType, int subPtr) {
    int *baseType;
    short *vt;
    if (func_00414B98(thisType, (int *)desired) != 0)
        return objPtr;
    baseType = (int *)thisType[2];
    vt = (short *)baseType[1];
    return ((int (*)(int *, int, int, int, int, int))((int *)vt)[5])((int *)((char *)baseType + vt[8]), desired, isPublic, objPtr, subType, subPtr);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __class_type_info_dcast);
#ifdef SKIP_ASM
/* cp/tinfo.cc / cp/tinfo2.cc (GCC runtime, GPL v2+ with the runtime linking exception): needs the class
   declarations of include/seg/seg_315B68_tinfo.h (included at the top of the unit).
   g++ calls these under fixed names (type_info::operator==, the typeinfo functions typeid and dynamic_cast use,
   __dynamic_cast); bind each to the retail symbol (assembler assignments: the relocations then name the retail
   symbols). All of them are defined in this unit. */
__asm__("__eq__C9type_infoRC9type_info = func_00414B98\n"
        "__tf16__user_type_info = func_00415018\n"
        "__tf9type_info = func_004151F8\n"
        "__dynamic_cast = func_00415840\n"
        "__tfv = func_004158F0\n"
        "__tf16__func_type_info = func_00415DF8\n"
        "__tf16__attr_type_info = func_00415ED8\n"
        "__tf19__pointer_type_info = func_00415F48");

/* tinfo.cc __class_type_info::dcast (GCC runtime, GPL v2+ with the runtime linking exception) */
}  // extern "C"  (a member function definition cannot have C linkage)

/**
 * dynamic_cast step for a class with several (or virtual) bases: searches every base for the desired type.
 *
 * Returns objptr when this class is the desired type. Otherwise it walks the base list (skipping non-public bases when
 * is_public is set, following the pointer for virtual bases) and recurses into each base's dcast. Two different hits
 * make the cast ambiguous and return 0, unless sub/subptr show that the source subobject lies in exactly one of them,
 * which then wins. Fills the dcast slot of the __class_type_info vtable D_004999D0.
 *
 * @param desired    the type_info cast to
 * @param is_public  nonzero to allow only public bases
 * @param objptr     the object pointer
 * @param sub        the static type of the source subobject, or null
 * @param subptr     the source subobject pointer
 * @return the adjusted pointer, or 0 when the cast fails or is ambiguous
 */
void* __class_type_info::
dcast (const type_info& desired, int is_public, void *objptr,
       const type_info *sub, void *subptr) const
{
  if (*this == desired)
    return objptr;

  void *match_found = 0;
  for (size_t i = 0; i < n_bases; i++)
    {
      if (is_public && base_list[i].access != PUBLIC)
	continue;

      void *candidate = (char *)objptr + base_list[i].offset;
      if (base_list[i].is_virtual)
	candidate = *(void **)candidate;
      candidate = base_list[i].base->dcast (desired, is_public, candidate, sub, subptr);
      if (candidate)
	{
	  if (match_found == 0)
	    match_found = candidate;
	  else if (match_found != candidate)
	    {
	      if (sub)
		{
		  // Perhaps we're downcasting from *sub to desired; see if
		  // subptr is a subobject of exactly one of {match_found,p}.

		  const __user_type_info &desiredUser =
		    static_cast <const __user_type_info &> (desired);

		  void *os = desiredUser.dcast (*sub, 1, match_found);
		  void *ns = desiredUser.dcast (*sub, 1, candidate);

		  if (os == ns)
		    /* ambiguous -- subptr is a virtual base */;
		  else if (os == subptr)
		    continue;
		  else if (ns == subptr)
		    {
		      match_found = candidate;
		      continue;
		    }
		}

	      // base found at two different pointers,
	      // conversion is not unique
	      return 0;
	    }
	}
    }

  return match_found;
}

extern "C" {
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __class_type_info___class_type_info);
#ifdef SKIP_ASM
extern int D_004999D0[];
/**
 * Constructs a multiple-inheritance class type_info: name, vtable D_004999D0, base list and base count.
 *
 * @param typeInfo  the object
 * @param typeName  the mangled class name
 * @param baseList  the base_info array
 * @param baseCount  number of bases
 * @return the object
 */
int *__class_type_info___class_type_info(int *typeInfo, int typeName, int baseList, int baseCount) {
    typeInfo[0] = typeName;
    typeInfo[1] = (int)D_004999D0;
    typeInfo[2] = baseList;
    typeInfo[3] = baseCount;
    return typeInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F00);
#ifdef SKIP_ASM
extern int D_0053EA60[];
extern int D_00499A58[];
extern int D_0053EA40[];
extern int *func_00415018(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __class_type_info, building it on first use.
 *
 * When D_0053EA60 is still zero, builds the base's type_info (func_00415018) and constructs a single-inheritance
 * type_info in D_0053EA60 with __rtti_si, name "17__class_type_info" (D_00499A58) and base D_0053EA40.
 *
 * @return the type_info object D_0053EA60
 */
int *func_00414F00(void) {
    if (D_0053EA60[0] == 0) {
        func_00415018();
        __rtti_si(D_0053EA60, (int)D_00499A58, (int)D_0053EA40);
    }
    return D_0053EA60;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __class_type_info____class_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __class_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __class_type_info____class_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __si_type_info___si_type_info);
#ifdef SKIP_ASM
extern int D_004999E8[];
/**
 * Constructs a single-inheritance class type_info: name, vtable D_004999E8 and base type_info.
 *
 * @param typeInfo  the object
 * @param typeName  the mangled class name
 * @param baseType  the base class's type_info
 * @return the object
 */
int *__si_type_info___si_type_info(int *typeInfo, int typeName, int baseType) {
    typeInfo[0] = typeName;
    typeInfo[1] = (int)D_004999E8;
    typeInfo[2] = baseType;
    return typeInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00414F90);
#ifdef SKIP_ASM
extern int D_0053EA50[];
extern int D_00499A70[];
extern int D_0053EA40[];
extern int *func_00415018(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __si_type_info, building it on first use.
 *
 * When D_0053EA50 is still zero, builds the base's type_info (func_00415018) and constructs a single-inheritance
 * type_info in D_0053EA50 with __rtti_si, name "14__si_type_info" (D_00499A70) and base D_0053EA40.
 *
 * @return the type_info object D_0053EA50
 */
int *func_00414F90(void) {
    if (D_0053EA50[0] == 0) {
        func_00415018();
        __rtti_si(D_0053EA50, (int)D_00499A70, (int)D_0053EA40);
    }
    return D_0053EA50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __si_type_info____si_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __si_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __si_type_info____si_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __user_type_info___user_type_info);
#ifdef SKIP_ASM
extern int D_00499A00[];
/**
 * Constructs the type_info of a class without bases: name and vtable D_00499A00.
 *
 * @param typeInfo  the object
 * @param typeName  the mangled class name
 * @return the object
 */
int *__user_type_info___user_type_info(int *typeInfo, int typeName) {
    typeInfo[0] = typeName;
    typeInfo[1] = (int)D_00499A00;
    return typeInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415018);
#ifdef SKIP_ASM
extern int D_0053EA40[];
extern int D_00499A88[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __user_type_info, building it on first use.
 *
 * When D_0053EA40 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EA40 with __rtti_si, name "16__user_type_info" (D_00499A88) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EA40
 */
int *func_00415018(void) {
    if (D_0053EA40[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EA40, (int)D_00499A88, (int)D_004A5BB0);
    }
    return D_0053EA40;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __user_type_info____user_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __user_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __user_type_info____user_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", bad_typeid__bad_typeid);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
/**
 * Destroys a bad_typeid: puts back the base exception vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores D_004998B8 (exception's vtable) and calls operator_delete__FPi on flag & 1.
 * Fills the destructor slot of bad_typeid's vtable D_00499A18.
 *
 * @param excObject     the bad_typeid object
 * @param flag  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int bad_typeid__bad_typeid(int **excObject, int flag) {
    *excObject = D_004998B8;
    if (flag & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", bad_typeid_bad_typeid);
#ifdef SKIP_ASM
extern int D_00499A18[];
/**
 * Constructs a bad_typeid: stores its vtable (D_00499A18) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
int *bad_typeid_bad_typeid(int **excObject) {
    *excObject = D_00499A18;
    return (int*)excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tf10bad_typeid);
#ifdef SKIP_ASM
extern int D_0053EA20[];
extern int D_00499AA0[];
extern int D_004A5BA8[];
extern int *func_00412F18(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class bad_typeid, building it on first use.
 *
 * When D_0053EA20 is still zero, builds exception's type_info (func_00412F18) and constructs a single-inheritance
 * type_info in D_0053EA20 with __rtti_si, name "10bad_typeid" (D_00499AA0) and base D_004A5BA8.
 *
 * @return the type_info object D_0053EA20
 */
int *__tf10bad_typeid(void) {
    if (D_0053EA20[0] == 0) {
        func_00412F18();
        __rtti_si(D_0053EA20, (int)D_00499AA0, (int)D_004A5BA8);
    }
    return D_0053EA20;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", bad_cast__bad_cast);
#ifdef SKIP_ASM
extern int D_004998B8[];
extern int operator_delete__FPi(int *);
/**
 * Destroys a bad_cast: puts back the base exception vtable and frees the object when bit 0 of the flag is set.
 *
 * g++ deleting-destructor shape: stores D_004998B8 (exception's vtable) and calls operator_delete__FPi on flag & 1.
 * Fills the destructor slot of bad_cast's vtable D_00499A30.
 *
 * @param excObject     the bad_cast object
 * @param flag  destructor flags: bit 0 frees the storage
 * @return nothing meaningful (the C reconstruction declares int)
 */
int bad_cast__bad_cast(int **excObject, int flag) {
    *excObject = D_004998B8;
    if (flag & 1) return operator_delete__FPi((int *)excObject);
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", bad_cast_bad_cast);
#ifdef SKIP_ASM
extern int D_00499A30[];
/**
 * Constructs a bad_cast: stores its vtable (D_00499A30) and returns the object.
 *
 * @param excObject  the object to construct
 * @return the constructed object
 */
int *bad_cast_bad_cast(int **excObject) {
    *excObject = D_00499A30;
    return (int*)excObject;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tf8bad_cast);
#ifdef SKIP_ASM
extern int D_0053EA30[];
extern int D_00499AB0[];
extern int D_004A5BA8[];
extern int *func_00412F18(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class bad_cast, building it on first use.
 *
 * When D_0053EA30 is still zero, builds exception's type_info (func_00412F18) and constructs a single-inheritance
 * type_info in D_0053EA30 with __rtti_si, name "8bad_cast" (D_00499AB0) and base D_004A5BA8.
 *
 * @return the type_info object D_0053EA30
 */
int *__tf8bad_cast(void) {
    if (D_0053EA30[0] == 0) {
        func_00412F18();
        __rtti_si(D_0053EA30, (int)D_00499AB0, (int)D_004A5BA8);
    }
    return D_0053EA30;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151B8);
#ifdef SKIP_ASM
/**
 * Returns whether two type_info objects describe different types (the negation of func_00414B98).
 *
 * @param lhs  first type_info
 * @param rhs  second type_info
 * @return 1 when the types differ, else 0
 */
int func_004151B8(int *lhs, int *rhs) {
    return func_00414B98(lhs, rhs) ^ 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", type_info_name);
#ifdef SKIP_ASM
/**
 * Returns a type_info's name string (its first word).
 *
 * @param typeInfo  the type_info
 * @return the mangled type name
 */
int type_info_name(int *typeInfo) {
    return *typeInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", type_info_type_info);
#ifdef SKIP_ASM
extern int D_00499A48[];
/**
 * Constructs a type_info: stores the name and type_info's vtable (D_00499A48, at +4) and returns the object.
 *
 * @param typeInfo  the object
 * @param typeName  the mangled type name
 * @return the constructed object
 */
int *type_info_type_info(int *typeInfo, int typeName) {
    typeInfo[0] = typeName;
    typeInfo[1] = (int)D_00499A48;
    return typeInfo;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004151F8);
#ifdef SKIP_ASM
extern int D_00499AC0[];
extern int D_004A5BB0[];
extern void __rtti_user(int *, int);
/**
 * Returns the type_info object of class type_info itself, building it on first use.
 *
 * When D_004A5BB0 is still zero, constructs a type_info for a class without bases there with __rtti_user and the name
 * "9type_info" (D_00499AC0).
 *
 * @return the type_info object D_004A5BB0
 */
int *func_004151F8(void) {
    if (D_004A5BB0[0] == 0)
        __rtti_user(D_004A5BB0, (int)D_00499AC0);
    return D_004A5BB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", type_info_before);
#ifdef SKIP_ASM
extern int func_004165A8(int, int);
/**
 * Returns whether one type_info sorts before another (by strcmp of their names).
 *
 * @param lhs  first type_info
 * @param rhs  second type_info
 * @return 1 when lhs's name compares less than rhs's, else 0
 */
int type_info_before(int *lhs, int *rhs) {
    return (unsigned)func_004165A8(*lhs, *rhs) >> 31;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415260);
#ifdef SKIP_ASM
/* tinfo2.cc __throw_type_match_rtti (GCC runtime, GPL v2+ with the runtime linking exception) */
/* Low level match routine used by compiler to match types of catch
   variables and thrown objects.  */

extern "C" void*
__throw_type_match_rtti (const void *catch_type_r, const void *throw_type_r,
			 void *objptr) __asm__ ("func_00415260");
/**
 * Tests whether a thrown object can be caught by a handler of another type and returns the object pointer to bind.
 *
 * Equal types (func_00414B98) match as is. A class throw type is cast with its type_info's dcast, public bases only. A
 * pointer throw type needs a pointer catch type: cv-qualifiers (attr type_infos) are stripped from both pointees and
 * the catch side may not drop const or volatile; equal pointees match, a void pointee catches any non-function pointee,
 * a class pointee is cast with dcast, and multi-level pointers are compared level by level with the
 * qualification-conversion rule (a level may add const only when every outer level is const). The type tests are
 * dynamic_casts through func_00415840. __cplus_type_matcher and __check_eh_spec call it.
 *
 * @param catch_type_r  the handler's type_info
 * @param throw_type_r  the thrown object's type_info
 * @param objptr        the thrown object
 * @return the (adjusted) object pointer on a match, 0 otherwise
 */
extern "C" void*
__throw_type_match_rtti (const void *catch_type_r, const void *throw_type_r,
			 void *objptr)
{
  const type_info &catch_type = *(const type_info *)catch_type_r;
  const type_info &throw_type = *(const type_info *)throw_type_r;
  
  if (catch_type == throw_type)
    return objptr;
  
  /* (a disabled `#if 0` printf of the two type names was here) */

  void *new_objptr = 0;

  if (const __user_type_info *userType
      = dynamic_cast <const __user_type_info *> (&throw_type))
    {
      /* The 1 skips conversions to private bases. */
      new_objptr = userType->dcast (catch_type, 1, objptr);
    }
  else if (const __pointer_type_info *fr =
	   dynamic_cast <const __pointer_type_info *> (&throw_type))
    {
      const __pointer_type_info *to =
	   dynamic_cast <const __pointer_type_info *> (&catch_type);

      if (! to)
	goto fail;

      const type_info *subfr = &fr->type, *subto = &to->type;
      __attr_type_info::cv cvfrom, cvto;

      if (const __attr_type_info *attrType
	  = dynamic_cast <const __attr_type_info *> (subfr))
	{
	  cvfrom = attrType->attr;
	  subfr = &attrType->type;
	}
      else
	cvfrom = __attr_type_info::NONE;
      
      if (const __attr_type_info *attrType
	  = dynamic_cast <const __attr_type_info *> (subto))
	{
	  cvto = attrType->attr;
	  subto = &attrType->type;
	}
      else
	cvto = __attr_type_info::NONE;

      if (((cvfrom & __attr_type_info::CONST)
	   > (cvto & __attr_type_info::CONST))
	  || ((cvfrom & __attr_type_info::VOLATILE)
	      > (cvto & __attr_type_info::VOLATILE)))
	goto fail;

      if (*subto == *subfr)
	new_objptr = objptr;
      else if (*subto == typeid (void)
	       && dynamic_cast <const __func_type_info *> (subfr) == 0)
	new_objptr = objptr;
      else if (const __user_type_info *userType
	       = dynamic_cast <const __user_type_info *> (subfr))
	{
	  /* The 1 skips conversions to private bases. */
	  new_objptr = userType->dcast (*subto, 1, objptr);
	}
      else if (const __pointer_type_info *pfr
	       = dynamic_cast <const __pointer_type_info *> (subfr))
	{
	  // Multi-level pointer conversion.

	  const __pointer_type_info *pto
	    = dynamic_cast <const __pointer_type_info *> (subto);

	  if (! pto)
	    goto fail;
	    
	  bool constp = (cvto & __attr_type_info::CONST);
	  for (subto = &pto->type, subfr = &pfr->type; ;
	       subto = &pto->type, subfr = &pfr->type)
	    {
	      if (const __attr_type_info *attrType
		  = dynamic_cast <const __attr_type_info *> (subfr))
		{
		  cvfrom = attrType->attr;
		  subfr = &attrType->type;
		}
	      else
		cvfrom = __attr_type_info::NONE;
      
	      if (const __attr_type_info *attrType
		  = dynamic_cast <const __attr_type_info *> (subto))
		{
		  cvto = attrType->attr;
		  subto = &attrType->type;
		}
	      else
		cvto = __attr_type_info::NONE;

	      if (((cvfrom & __attr_type_info::CONST)
		   > (cvto & __attr_type_info::CONST))
		  || ((cvfrom & __attr_type_info::VOLATILE)
		      > (cvto & __attr_type_info::VOLATILE)))
		goto fail;

	      if (! constp
		  && (((cvfrom & __attr_type_info::CONST)
		       < (cvto & __attr_type_info::CONST))
		      || ((cvfrom & __attr_type_info::VOLATILE)
			  < (cvto & __attr_type_info::VOLATILE))))
		goto fail;

	      if (*subto == *subfr)
		{
		  new_objptr = objptr;
		  break;
		}

	      pto = dynamic_cast <const __pointer_type_info *> (subto);
	      pfr = dynamic_cast <const __pointer_type_info *> (subfr);
	      if (! pto || ! pfr)
		goto fail;		

	      if (! (cvto & __attr_type_info::CONST))
		constp = false;
	    }
	}
    }
 fail:

  /* (a disabled `#if 0` printf of the pointer delta was here) */
  return new_objptr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __is_pointer);
#ifdef SKIP_ASM
extern int *func_004151F8(void);
extern int *func_00415F48(void);
extern int func_00415840(int, char *, int, char *, char *, void *);
/**
 * Returns whether a type_info describes a pointer type.
 *
 * Null gives false. Otherwise it dynamic_casts the type_info to the pointer type_info class with func_00415840:
 * from-type is the object's own type_info function (vtable slot 0), to-type func_00415F48, public only, the address
 * adjusted by the slot's delta, sub-type func_004151F8. func_00412A28 uses it to decide whether the thrown object is
 * freed.
 *
 * @param typePtr  the type_info to test, or null
 * @return 1 for a pointer type_info, else 0
 */
int __is_pointer(int *typePtr) {
    int pointerType = 0;
    if (typePtr) {
        short *vt = (short *)typePtr[1];
        pointerType = func_00415840(((int *)vt)[1], (char *)func_00415F48, 1, (char *)typePtr + vt[0], (char *)func_004151F8, typePtr);
    }
    return pointerType != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_ptr);
#ifdef SKIP_ASM
extern int D_00499BC8[];
/**
 * Constructs a pointer type_info in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0), the __pointer_type_info vtable D_00499BC8 (+4) and the pointed-to type (+8).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 * @param pointeeType  the pointed-to type's type_info
 */
void __rtti_ptr(int *storage, int typeName, int pointeeType) {
    if (storage != 0) {
        storage[2] = pointeeType;
        storage[0] = typeName;
        storage[1] = (int)D_00499BC8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_attr);
#ifdef SKIP_ASM
extern int D_00499BB8[];
/**
 * Constructs a cv-qualified type's type_info in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0), the __attr_type_info vtable D_00499BB8 (+4), the unqualified type (+8) and the qualifier bits
 * (+0xC: 1 const, 2 volatile).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 * @param cvQualifiers  qualifier bits (1 const, 2 volatile)
 * @param baseType  the unqualified type's type_info
 */
void __rtti_attr(int *storage, int typeName, int cvQualifiers, int baseType) {
    if (storage != 0) {
        storage[3] = cvQualifiers;
        storage[0] = typeName;
        storage[2] = baseType;
        storage[1] = (int)D_00499BB8;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_func);
#ifdef SKIP_ASM
extern int D_00499B98[];
/**
 * Constructs the type_info of a function type in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0) and the __func_type_info vtable D_00499B98 (+4).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 */
void __rtti_func(int *storage, int typeName) {
    if (storage != 0) {
        storage[0] = typeName;
        storage[1] = (int)D_00499B98;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_ptmf);
#ifdef SKIP_ASM
extern int D_00499B88[];
/**
 * Constructs the type_info of a pointer-to-member-function type in caller-supplied storage (does nothing for null
 * storage).
 *
 * Stores the name (+0) and the __ptmf_type_info vtable D_00499B88 (+4).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 */
void __rtti_ptmf(int *storage, int typeName) {
    if (storage != 0) {
        storage[0] = typeName;
        storage[1] = (int)D_00499B88;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_ptmd);
#ifdef SKIP_ASM
extern int D_00499B78[];
/**
 * Constructs the type_info of a pointer-to-data-member type in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0) and the __ptmd_type_info vtable D_00499B78 (+4).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 */
void __rtti_ptmd(int *storage, int typeName) {
    if (storage != 0) {
        storage[0] = typeName;
        storage[1] = (int)D_00499B78;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __rtti_array);
#ifdef SKIP_ASM
extern int D_00499B68[];
/**
 * Constructs the type_info of an array type in caller-supplied storage (does nothing for null storage).
 *
 * Stores the name (+0) and the __array_type_info vtable D_00499B68 (+4).
 *
 * @param storage  the storage, or null
 * @param typeName  the mangled type name
 */
void __rtti_array(int *storage, int typeName) {
    if (storage != 0) {
        storage[0] = typeName;
        storage[1] = (int)D_00499B68;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415840);
#ifdef SKIP_ASM
/**
 * Performs a run-time checked cast: asks the source object's dynamic type to find the target type in it.
 *
 * Calls fromFn for the object's dynamic type_info, then that type_info's dcast (vtable slot 2: pointer at +0x14,
 * this-delta at +0x10) with toFn's type_info, requirePublic, the address, subFn's type_info and subPtr.
 *
 * @param fromFn         type_info function of the object's dynamic type
 * @param toFn           type_info function of the target type
 * @param requirePublic  nonzero to allow only public bases
 * @param address        the complete object
 * @param subFn          type_info function of the source expression's static type
 * @param subPtr         the source pointer
 * @return the adjusted pointer, or 0 when the cast fails
 */
int func_00415840(int fromFn, char *toFn, int requirePublic, char *address, char *subFn, void *subPtr) {
    char *fromType = ((char *(*)(void))fromFn)();
    int *vt = *(int **)(fromType + 4);
    char *th = fromType + *(short *)((char *)vt + 0x10);
    int toType;
    int subType;
    vt = (int *)((char *)vt + 0x10);
    toType = ((int (*)(void))toFn)();
    subType = ((int (*)(void))subFn)();
    return ((int (*)(char *, int, int, char *, int, void *))vt[1])(th, toType, requirePublic, address, subType, subPtr);
}
#endif

}  // extern "C"

// libstdc++-v2 tinfo2.cc, BUILTIN(m): __tf<m>() lazily placement-constructs the builtin type_info __ti<m> (its name
// is the mangled type letter, its vtable is __builtin_type_info's) and returns it. The objects, name strings and
// the vtable are retail data (D_*): only their addresses are referenced here.
typedef unsigned int size_t;
inline void* operator new(size_t, void* p) throw() { return p; }

extern "C" {
extern const int D_00499BA8[];
extern unsigned char D_00454158[8];
extern unsigned char D_00454160[8];
extern unsigned char D_00454168[8];
extern unsigned char D_00454170[8];
extern unsigned char D_00454178[8];
extern unsigned char D_00454180[8];
extern unsigned char D_00454188[8];
extern unsigned char D_00454190[8];
extern unsigned char D_00454198[8];
extern unsigned char D_004541A0[8];
extern unsigned char D_004541A8[8];
extern unsigned char D_004541B0[8];
extern unsigned char D_004541B8[8];
extern unsigned char D_004541C0[8];
extern unsigned char D_004541C8[8];
extern unsigned char D_004541D0[8];
extern unsigned char D_004541D8[8];
extern const char D_00499AE0[];
extern const char D_00499AE8[];
extern const char D_00499AF0[];
extern const char D_00499AF8[];
extern const char D_00499B00[];
extern const char D_00499B08[];
extern const char D_00499B10[];
extern const char D_00499B18[];
extern const char D_00499B20[];
extern const char D_00499B28[];
extern const char D_00499B30[];
extern const char D_00499B38[];
extern const char D_00499B40[];
extern const char D_00499B48[];
extern const char D_00499B50[];
extern const char D_00499B58[];
extern const char D_00499B60[];
}

class __builtin_ti {
    const char* _name;
    const void* vtbl_;
public:
    __builtin_ti(const char* n) : _name(n), vtbl_(D_00499BA8) {}
};

// (a non-polymorphic stand-in class that stores the retail vtable's address: a real polymorphic class would emit its
// own vtable, and the retail one is data we reference rather than regenerate)
#define BUILTIN(fn, ti, name)                  \
    extern "C" const __builtin_ti& fn(void) {  \
        if (*(void**)ti == 0)                  \
            new (ti) __builtin_ti(name);       \
        return *(const __builtin_ti*)ti;       \
    }

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_004158F0);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type void, building it on first use.
 *
 * When the first word of D_00454158 is zero, constructs a type_info there with the name "v" (D_00499AE0, g++'s mangled
 * code for void) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454158
 */
BUILTIN(func_004158F0, D_00454158, D_00499AE0)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfx);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type long long, building it on first use.
 *
 * When the first word of D_00454160 is zero, constructs a type_info there with the name "x" (D_00499AE8, g++'s mangled
 * code for long long) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454160
 */
BUILTIN(__tfx, D_00454160, D_00499AE8)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfl);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type long, building it on first use.
 *
 * When the first word of D_00454168 is zero, constructs a type_info there with the name "l" (D_00499AF0, g++'s mangled
 * code for long) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454168
 */
BUILTIN(__tfl, D_00454168, D_00499AF0)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfi);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type int, building it on first use.
 *
 * When the first word of D_00454170 is zero, constructs a type_info there with the name "i" (D_00499AF8, g++'s mangled
 * code for int) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454170
 */
BUILTIN(__tfi, D_00454170, D_00499AF8)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfs);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type short, building it on first use.
 *
 * When the first word of D_00454178 is zero, constructs a type_info there with the name "s" (D_00499B00, g++'s mangled
 * code for short) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454178
 */
BUILTIN(__tfs, D_00454178, D_00499B00)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfb);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type bool, building it on first use.
 *
 * When the first word of D_00454180 is zero, constructs a type_info there with the name "b" (D_00499B08, g++'s mangled
 * code for bool) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454180
 */
BUILTIN(__tfb, D_00454180, D_00499B08)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfc);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type char, building it on first use.
 *
 * When the first word of D_00454188 is zero, constructs a type_info there with the name "c" (D_00499B10, g++'s mangled
 * code for char) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454188
 */
BUILTIN(__tfc, D_00454188, D_00499B10)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfw);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type wchar_t, building it on first use.
 *
 * When the first word of D_00454190 is zero, constructs a type_info there with the name "w" (D_00499B18, g++'s mangled
 * code for wchar_t) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454190
 */
BUILTIN(__tfw, D_00454190, D_00499B18)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfr);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type long double, building it on first use.
 *
 * When the first word of D_00454198 is zero, constructs a type_info there with the name "r" (D_00499B20, g++'s mangled
 * code for long double) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_00454198
 */
BUILTIN(__tfr, D_00454198, D_00499B20)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfd);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type double, building it on first use.
 *
 * When the first word of D_004541A0 is zero, constructs a type_info there with the name "d" (D_00499B28, g++'s mangled
 * code for double) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541A0
 */
BUILTIN(__tfd, D_004541A0, D_00499B28)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tff);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type float, building it on first use.
 *
 * When the first word of D_004541A8 is zero, constructs a type_info there with the name "f" (D_00499B30, g++'s mangled
 * code for float) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541A8
 */
BUILTIN(__tff, D_004541A8, D_00499B30)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfUi);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type unsigned int, building it on first use.
 *
 * When the first word of D_004541B0 is zero, constructs a type_info there with the name "Ui" (D_00499B38, g++'s mangled
 * code for unsigned int) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541B0
 */
BUILTIN(__tfUi, D_004541B0, D_00499B38)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfUl);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type unsigned long, building it on first use.
 *
 * When the first word of D_004541B8 is zero, constructs a type_info there with the name "Ul" (D_00499B40, g++'s mangled
 * code for unsigned long) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541B8
 */
BUILTIN(__tfUl, D_004541B8, D_00499B40)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfUx);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type unsigned long long, building it on first use.
 *
 * When the first word of D_004541C0 is zero, constructs a type_info there with the name "Ux" (D_00499B48, g++'s mangled
 * code for unsigned long long) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541C0
 */
BUILTIN(__tfUx, D_004541C0, D_00499B48)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfUs);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type unsigned short, building it on first use.
 *
 * When the first word of D_004541C8 is zero, constructs a type_info there with the name "Us" (D_00499B50, g++'s mangled
 * code for unsigned short) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541C8
 */
BUILTIN(__tfUs, D_004541C8, D_00499B50)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfUc);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type unsigned char, building it on first use.
 *
 * When the first word of D_004541D0 is zero, constructs a type_info there with the name "Uc" (D_00499B58, g++'s mangled
 * code for unsigned char) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541D0
 */
BUILTIN(__tfUc, D_004541D0, D_00499B58)
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __tfSc);
#ifdef SKIP_ASM
/**
 * Returns the type_info object of the builtin type signed char, building it on first use.
 *
 * When the first word of D_004541D8 is zero, constructs a type_info there with the name "Sc" (D_00499B60, g++'s mangled
 * code for signed char) and the __builtin_type_info vtable D_00499BA8.
 *
 * @return the type_info object D_004541D8
 */
BUILTIN(__tfSc, D_004541D8, D_00499B60)
#endif

extern "C" {

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415CA8);
#ifdef SKIP_ASM
extern int D_0053EAA0[];
extern int D_00499BD8[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __array_type_info, building it on first use.
 *
 * When D_0053EAA0 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EAA0 with __rtti_si, name "17__array_type_info" (D_00499BD8) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EAA0
 */
int *func_00415CA8(void) {
    if (D_0053EAA0[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EAA0, (int)D_00499BD8, (int)D_004A5BB0);
    }
    return D_0053EAA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __array_type_info____array_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __array_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __array_type_info____array_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415D18);
#ifdef SKIP_ASM
extern int D_0053EAC0[];
extern int D_00499BF0[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __ptmd_type_info, building it on first use.
 *
 * When D_0053EAC0 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EAC0 with __rtti_si, name "16__ptmd_type_info" (D_00499BF0) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EAC0
 */
int *func_00415D18(void) {
    if (D_0053EAC0[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EAC0, (int)D_00499BF0, (int)D_004A5BB0);
    }
    return D_0053EAC0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __ptmd_type_info____ptmd_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __ptmd_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __ptmd_type_info____ptmd_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415D88);
#ifdef SKIP_ASM
extern int D_0053EA90[];
extern int D_00499C08[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __ptmf_type_info, building it on first use.
 *
 * When D_0053EA90 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EA90 with __rtti_si, name "16__ptmf_type_info" (D_00499C08) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EA90
 */
int *func_00415D88(void) {
    if (D_0053EA90[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EA90, (int)D_00499C08, (int)D_004A5BB0);
    }
    return D_0053EA90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __ptmf_type_info____ptmf_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __ptmf_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __ptmf_type_info____ptmf_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415DF8);
#ifdef SKIP_ASM
extern int D_0053EA70[];
extern int D_00499C20[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __func_type_info, building it on first use.
 *
 * When D_0053EA70 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EA70 with __rtti_si, name "16__func_type_info" (D_00499C20) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EA70
 */
int *func_00415DF8(void) {
    if (D_0053EA70[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EA70, (int)D_00499C20, (int)D_004A5BB0);
    }
    return D_0053EA70;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __func_type_info____func_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __func_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __func_type_info____func_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415E68);
#ifdef SKIP_ASM
extern int D_0053EAD0[];
extern int D_00499C38[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __builtin_type_info, building it on first use.
 *
 * When D_0053EAD0 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EAD0 with __rtti_si, name "19__builtin_type_info" (D_00499C38) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EAD0
 */
int *func_00415E68(void) {
    if (D_0053EAD0[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EAD0, (int)D_00499C38, (int)D_004A5BB0);
    }
    return D_0053EAD0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __builtin_type_info____builtin_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __builtin_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __builtin_type_info____builtin_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415ED8);
#ifdef SKIP_ASM
extern int D_0053EA80[];
extern int D_00499C50[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __attr_type_info, building it on first use.
 *
 * When D_0053EA80 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EA80 with __rtti_si, name "16__attr_type_info" (D_00499C50) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EA80
 */
int *func_00415ED8(void) {
    if (D_0053EA80[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EA80, (int)D_00499C50, (int)D_004A5BB0);
    }
    return D_0053EA80;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __attr_type_info____attr_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __attr_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __attr_type_info____attr_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", func_00415F48);
#ifdef SKIP_ASM
extern int D_0053EAB0[];
extern int D_00499C68[];
extern int D_004A5BB0[];
extern int *func_004151F8(void);
extern void __rtti_si(int *, int, int);
/**
 * Returns the type_info object of class __pointer_type_info, building it on first use.
 *
 * When D_0053EAB0 is still zero, builds type_info's own type_info (func_004151F8) and constructs a single-inheritance
 * type_info in D_0053EAB0 with __rtti_si, name "19__pointer_type_info" (D_00499C68) and base D_004A5BB0.
 *
 * @return the type_info object D_0053EAB0
 */
int *func_00415F48(void) {
    if (D_0053EAB0[0] == 0) {
        func_004151F8();
        __rtti_si(D_0053EAB0, (int)D_00499C68, (int)D_004A5BB0);
    }
    return D_0053EAB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_315B68_tinfo", __pointer_type_info____pointer_type_info);
#ifdef SKIP_ASM
/**
 * Destroys a __pointer_type_info by jumping to type_info's destructor (type_info__type_info).
 *
 * Declared without parameters: the object and destructor flags pass through in the argument registers.
 */
void __pointer_type_info____pointer_type_info(void) {
    ((void (*)(void))type_info__type_info)();
}
#endif

}  // extern "C"
