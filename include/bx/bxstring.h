#ifndef BXSTRING_H
#define BXSTRING_H

#include "common.h"

extern "C" uint strlen(const char* str);

// Sits directly in front of every string buffer
struct cBXStringData {
    int nRefs;          // Below zero when locked or static: never shared
    int nDataLength;
    int nAllocLength;

    char* data() { return (char*)(this + 1); }
};

// Points at D_0048DCD4, the data of the static empty string
extern char* D_004A3E90;
extern char D_0048DCD4[];

// Reference-counted, copy-on-write string. Its interface and behaviour
// follow MFC's CString closely.
class cBXString {
public:
    void Init() { m_pchData = D_004A3E90; }
    cBXStringData* GetData() const { return (cBXStringData*)m_pchData - 1; }
    int GetLength() const { return ((cBXStringData*)m_pchData)[-1].nDataLength; }
    int GetAllocLength() const { return ((cBXStringData*)m_pchData)[-1].nAllocLength; }
    static int SafeStrlen(const char* psz) { return psz ? strlen(psz) : 0; }

    // Not Init(): GCC 2.95 won't inline a call nested in an inline constructor
    cBXString() { m_pchData = D_004A3E90; }
    cBXString(const cBXString& src);
    cBXString(const char* psz);
    cBXString(const char* pch, uint nLength);
    cBXString(char ch, int nRepeat);
    ~cBXString();

    const cBXString& operator=(const cBXString& src);
    const cBXString& operator=(const char* psz);
    const cBXString& operator=(char ch);
    const cBXString& operator+=(const char* psz);
    const cBXString& operator+=(char ch);
    const cBXString& operator+=(const cBXString& string);

    char* GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength);
    char* GetBufferSetLength(int nNewLength);
    void FreeExtra();
    char* LockBuffer();
    void UnlockBuffer();

    int Find(char ch, int nStart) const;
    int FindOneOf(const char* pszCharSet) const;
    int ReverseFind(char ch) const;
    int Find(const char* pszSub, int nStart) const;

    void MakeUpper();
    void MakeLower();
    void Replace(char chOld, char chNew);
    void SetAt(int nIndex, char ch);

    cBXString Mid(int nFirst) const;
    cBXString Mid(int nFirst, int nCount) const;
    cBXString Right(int nCount) const;
    cBXString Left(int nCount) const;
    cBXString SpanIncluding(const char* pszCharSet) const;
    cBXString SpanExcluding(const char* pszCharSet) const;

    void AllocBuffer(int nLen);
    void Release();
    void CopyBeforeWrite();
    void AllocBeforeWrite(int nLen);
    void AllocCopy(cBXString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
    void AssignCopy(int nSrcLen, const char* pszSrcData);
    void ConcatCopy(int nSrc1Len, const char* pszSrc1Data, int nSrc2Len, const char* pszSrc2Data);
    void ConcatInPlace(int nSrcLen, const char* pszSrcData);

    char* m_pchData;
};

cBXString operator+(const cBXString& string1, const cBXString& string2);
cBXString operator+(const cBXString& string, const char* psz);
cBXString operator+(const char* psz, const cBXString& string);
cBXString operator+(const cBXString& string, char ch);
cBXString operator+(char ch, const cBXString& string);

#endif
