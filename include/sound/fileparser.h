#ifndef SOUND_FILEPARSER_H
#define SOUND_FILEPARSER_H

// FileParser: the sound code's .ini/.inf text parser (sections "[NAME]", "KEY = value" lines, '#'/';' comments).
// The constructor loads the whole file into a heap buffer; the parser walks it with mPtr and returns tokens in
// mTempStr. EA name from SSX On Tour's DWARF (class FileParser, size 0x80C, in On Tour's sound/all_in_one_sound.cpp,
// just before cPSPSoundManager, as retail's methods sit just before the cMidiSys / cPS2SoundManager code).
// Retail methods: sound/soundsys unit 0x2A77F8..0x2A8138 (19, On Tour's methods except Reset, which retail inlines as
// `mPtr = mBasePtr`). EA's source file is unknown (no asserts): the header path is ours.
// Size 0x80C [O]; [R] every user builds it on the stack (data/config/crowd.inf, speech.inf, music.inf, the SFX and mix
// tables of cSSXAudio) and reserves 0x810 (the next local starts 0x810 after it, e.g. func_002B0088). No vtable.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructor FileParser_FileParser (FileParser(const char* Filename, int MemType)) and destructor FileParser__FileParser;
//   [O] On Tour's FileParser (same three members + mTempStr[2048], same offsets);
//   [U] how the retail methods use the field.

#include "layout.h"

struct FileParser {
    int mFileSize;       // 0x000 [O]; [R] ctor = BXFILE_size(Filename); [U] IsEOF (FileParser_IsEOF) stops at mPtr - mBasePtr >= mFileSize
    char* mBasePtr;      // 0x004 [O]; [R] ctor operator new(mFileSize + 1, Filename, MemType), loads the file and 0-terminates it; dtor cMemMan_free()s it
    char* mPtr;          // 0x008 [O]; [R] ctor = mBasePtr; [U] GetChar (FileParser_GetChar__FPv) returns *mPtr++; every scan method reads *mPtr
    char mTempStr[2048]; // 0x00C [O]; [U] FindNextSection / GetString / GetName copy the token here and return it; GetFloat/GetInt sscanf it; Assign* compare the key in it

    // Methods bound to their retail symbols by asm labels.
    bool SkipComment() __asm__("FileParser_SkipComment");
    void SkipSectionNames() __asm__("FileParser_SkipSectionNames");
    bool FindEOL() __asm__("FileParser_FindEOL");
    char* FindNextSection() __asm__("FileParser_FindNextSection");
    bool FindSection(const char* name) __asm__("FileParser_FindSection");
    bool FindPartialSection(char* name) __asm__("FileParser_FindPartialSection");
    char* GetString() __asm__("FileParser_GetString");
    float GetFloat() __asm__("FileParser_GetFloat");
    int GetInt() __asm__("FileParser_GetInt");
    int ExpectToken(const char* name) __asm__("FileParser_ExpectToken");
    int IsEOF() __asm__("FileParser_IsEOF");
    bool AssignString(const char* name, char* out) __asm__("FileParser_AssignString");
    bool AssignFloat(const char* name, float* out) __asm__("FileParser_AssignFloat");
    bool AssignInt(const char* name, int* out) __asm__("FileParser_AssignInt");
    signed char GetChar() __asm__("FileParser_GetChar__FPv");
    int SkipSpace() __asm__("FileParser_SkipSpace");
    char* GetName() __asm__("FileParser_GetName");
};

CHECK_OFFSET(FileParser, mFileSize, 0x000);
CHECK_OFFSET(FileParser, mBasePtr, 0x004);
CHECK_OFFSET(FileParser, mPtr, 0x008);
CHECK_OFFSET(FileParser, mTempStr, 0x00C);
CHECK_SIZE(FileParser, 0x80C);

#endif /* SOUND_FILEPARSER_H */
