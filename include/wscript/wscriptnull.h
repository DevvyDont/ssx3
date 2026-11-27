#ifndef WSCRIPT_WSCRIPTNULL_H
#define WSCRIPT_WSCRIPTNULL_H

// WScriptNullType: the empty end marker of every world-script parameter block (On Tour struct WScriptNullType, 1 byte,
// no members; the last member `null` of tMakeAnimDeltaData, tMakeCounterData, tAddFadeData ... and the trailing
// `const WScriptNullType& _null` of each block's constructor). The script functions (seg/seg_1FBE38) build their
// defaults in a guarded static-local init, and retail copies this byte through a stack temp there (lbu; sb back;
// sb 0(sp)): modeled as one byte with a user copy constructor, so `d.null = WScriptNullType(d.null)` compiles to that.

struct WScriptNullType {
    char c;                         // 0x0 the byte the copy moves (On Tour: none; an empty struct is 1 byte)
    WScriptNullType() {}
    WScriptNullType(const WScriptNullType& o) : c(o.c) {}
};

#endif /* WSCRIPT_WSCRIPTNULL_H */
