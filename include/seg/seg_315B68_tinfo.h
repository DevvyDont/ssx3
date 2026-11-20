// The type_info classes of libstdc++-v2's RTTI (seg/seg_315B68_tinfo.cpp): <typeinfo>'s type_info, cp/tinfo.h
// (__user_type_info, __si_type_info, __class_type_info) and the classes cp/tinfo2.cc declares for itself
// (__pointer_type_info, __attr_type_info, ...). GCC runtime, GPL v2+ with the runtime linking exception:
//   Copyright (C) 1994, 1995, 1996, 1998 Free Software Foundation (tinfo.h); 1994, 96-97, 1998 (tinfo2.cc).
//
// tinfo.cc (`#pragma implementation "typeinfo"`) and tinfo2.cc emitted these classes' vtables, typeinfo functions
// (__tf...) and out-of-line destructors; in this unit those are retail asm (INCLUDE_ASM) and data (D_...). Declaring
// the classes under `#pragma interface` makes the C bodies only reference them, as for seg_313800_exception.h.
// g++ 2.9 runs without -fhonor-std, so std:: is the global namespace.
#pragma interface

extern "C++" {

// <typeinfo>
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

inline bool type_info::
operator!= (const type_info& arg) const
{
  return !operator== (arg);
}

typedef __SIZE_TYPE__ size_t;

// cp/tinfo.h: class declarations shared between the typeinfo implementation files.

// type_info for a class with no base classes (or an enum).

struct __user_type_info : public type_info {
  __user_type_info (const char *n) : type_info (n) {}

  // If our type can be converted to the desired type,
  // return the pointer, adjusted accordingly; else return 0.
  virtual void* dcast (const type_info &, int, void *,
		       const type_info * = 0, void * = 0) const;
};

// type_info for a class with one public, nonvirtual base class.

class __si_type_info : public __user_type_info {
  const __user_type_info &base;

public:
  __si_type_info (const char *n, const __user_type_info &b)
    : __user_type_info (n), base (b) { }

  virtual void *dcast (const type_info &, int, void *,
		       const type_info * = 0, void * = 0) const;
};

// type_info for a general class.

typedef unsigned int USItype	__attribute__ ((mode (SI)));

struct __class_type_info : public __user_type_info {
  enum access { PUBLIC = 1, PROTECTED = 2, PRIVATE = 3 };

  struct base_info {
    const __user_type_info *base;
    USItype offset: 29;
    bool is_virtual: 1;
    enum access access: 2;
  };

  const base_info *base_list;
  size_t n_bases;

  __class_type_info (const char *name, const base_info *bl, size_t bn)
    : __user_type_info (name), base_list (bl), n_bases (bn) {}

  // This is a little complex.
  // (retail symbol: the body is landed C in seg_315B68_tinfo.cpp)
  virtual void* dcast (const type_info &, int, void *,
		       const type_info * = 0, void * = 0) const __asm__ ("__class_type_info_dcast");
};

// cp/tinfo2.cc

// type info for pointer type.

struct __pointer_type_info : public type_info {
  const type_info& type;

  __pointer_type_info (const char *n, const type_info& ti)
    : type_info (n), type (ti) {}
};

// type info for attributes

struct __attr_type_info : public type_info {
  enum cv { NONE = 0, CONST = 1, VOLATILE = 2, CONSTVOL = 1 | 2 };

  const type_info& type;
  cv attr;

  __attr_type_info (const char *n, cv a, const type_info& t)
    : type_info (n), type (t), attr (a) {}
};

// type_info for builtin type

struct __builtin_type_info : public type_info {
  __builtin_type_info (const char *n): type_info (n) {}
};

// type info for function.

struct __func_type_info : public type_info {
  __func_type_info (const char *n) : type_info (n) {}
};

// type info for pointer to member function.

struct __ptmf_type_info : public type_info {
  __ptmf_type_info (const char *n) : type_info (n) {}
};

// type info for pointer to data member.

struct __ptmd_type_info : public type_info {
  __ptmd_type_info (const char *n): type_info (n) {}
};

// type info for array.

struct __array_type_info : public type_info {
  __array_type_info (const char *n): type_info (n) {}
};

}
