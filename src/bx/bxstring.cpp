#include "bx/bxstring.h"

extern "C" void* memcpy(void* dst, const void* src, uint size);
extern "C" void* memset(void* dst, int c, uint size);
extern "C" void* operator_new(uint size, const char* name, uint flags, int align);
extern "C" void cMemMan_free(void* ptr);
extern "C" char* strchr(const char* str, int c);
// Not yet identified; from their use these look like strpbrk, strrchr,
// strstr, strspn and strcspn
extern "C" char* func_0041AC40(const char* str, const char* set);
extern "C" char* func_0041ACC0(const char* str, int c);
extern "C" char* func_0041AD80(const char* str, const char* sub);
extern "C" int func_0041AD10(const char* str, const char* set);
extern "C" int func_0041AB08(const char* str, const char* set);

// Allocation name for string buffers
extern const char D_0048D850[];
// newlib's character class table, _ctype_ + 1
extern const unsigned char D_00499C81[];

// The libc's ctype macros
#define islower(c) (D_00499C81[(unsigned)(c)] & 2)
#define isupper(c) (D_00499C81[(unsigned)(c)] & 1)
#define toupper(c) ({ int __x = (c); islower(__x) ? (__x - 'a' + 'A') : __x; })
#define tolower(c) ({ int __x = (c); isupper(__x) ? (__x - 'A' + 'a') : __x; })

#define min(a, b) ((a) < (b) ? (a) : (b))

cBXString::cBXString(const cBXString& src)
{
    if (src.GetData()->nRefs >= 0) {
        m_pchData = src.m_pchData;
        GetData()->nRefs++;
    } else {
        Init();
        *this = src.m_pchData;
    }
}

// Not matching: register allocation and store order after operator_new
INCLUDE_ASM("bx/bxstring", AllocBuffer__9cBXStringi);
#ifdef SKIP_ASM
void cBXString::AllocBuffer(int nLen)
{
    if (nLen == 0) {
        Init();
    } else {
        cBXStringData* pData = (cBXStringData*)operator_new(sizeof(cBXStringData) + nLen + 1, D_0048D850, 0x20000000, 0);
        m_pchData = pData->data();
        pData->nRefs = 1;
        m_pchData[nLen] = '\0';
        pData->nDataLength = nLen;
        pData->nAllocLength = nLen;
    }
}
#endif

void cBXString::Release()
{
    if (m_pchData != D_0048DCD4) {
        if (--GetData()->nRefs == 0) {
            cBXStringData* pData = GetData();
            if (pData)
                cMemMan_free(pData);
        }
        Init();
    }
}

// Not matching: our compiler turns two nearby negative offsets from one
// pointer (here -8 and -4 from m_pchData) into -32768 + 32760; the original
// compiler never does
INCLUDE_ASM("bx/bxstring", CopyBeforeWrite__9cBXString);
#ifdef SKIP_ASM
void cBXString::CopyBeforeWrite()
{
    if (GetData()->nRefs > 1) {
        char* pchOld = m_pchData;
        Release();
        AllocBuffer(((cBXStringData*)pchOld - 1)->nDataLength);
        memcpy(m_pchData, pchOld, ((cBXStringData*)pchOld - 1)->nDataLength + 1);
    }
}
#endif

void cBXString::AllocBeforeWrite(int nLen)
{
    if (GetData()->nRefs > 1 || nLen > GetAllocLength()) {
        Release();
        AllocBuffer(nLen);
    }
}

void cBXString::AllocCopy(cBXString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
{
    int nNewLen = nCopyLen + nExtraLen;

    if (nNewLen == 0) {
        dest.Init();
    } else {
        dest.AllocBuffer(nNewLen);
        memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen);
    }
}

cBXString::~cBXString()
{
    if (m_pchData != D_0048DCD4) {
        if (--GetData()->nRefs <= 0) {
            cBXStringData* pData = GetData();
            if (pData)
                cMemMan_free(pData);
        }
    }
}

cBXString::cBXString(const char* psz)
{
    Init();
    int nLen = SafeStrlen(psz);
    if (nLen != 0) {
        AllocBuffer(nLen);
        memcpy(m_pchData, psz, nLen);
    }
}

cBXString::cBXString(const char* pch, uint nLength)
{
    Init();
    nLength = min(nLength, strlen(pch));
    if (nLength != 0) {
        AllocBuffer(nLength);
        memcpy(m_pchData, pch, nLength);
    }
}

void cBXString::AssignCopy(int nSrcLen, const char* pszSrcData)
{
    AllocBeforeWrite(nSrcLen);
    if (m_pchData != D_0048DCD4) {
        memcpy(m_pchData, pszSrcData, nSrcLen);
        GetData()->nDataLength = nSrcLen;
        m_pchData[nSrcLen] = '\0';
    }
}

const cBXString& cBXString::operator=(const cBXString& src)
{
    if (m_pchData != src.m_pchData) {
        if ((GetData()->nRefs < 0 && m_pchData != D_0048DCD4) || src.GetData()->nRefs < 0) {
            AssignCopy(src.GetLength(), src.m_pchData);
        } else {
            Release();
            m_pchData = src.m_pchData;
            GetData()->nRefs++;
        }
    }
    return *this;
}

const cBXString& cBXString::operator=(const char* psz)
{
    AssignCopy(SafeStrlen(psz), psz);
    return *this;
}

void cBXString::ConcatCopy(int nSrc1Len, const char* pszSrc1Data, int nSrc2Len, const char* pszSrc2Data)
{
    int nNewLen = nSrc1Len + nSrc2Len;

    if (nNewLen != 0) {
        AllocBuffer(nNewLen);
        memcpy(m_pchData, pszSrc1Data, nSrc1Len);
        memcpy(m_pchData + nSrc1Len, pszSrc2Data, nSrc2Len);
    }
}

cBXString operator+(const cBXString& string1, const cBXString& string2)
{
    cBXString s;
    s.ConcatCopy(string1.GetLength(), string1.m_pchData, string2.GetLength(), string2.m_pchData);
    return s;
}

// Not matching: our compiler turns two nearby negative offsets from one
// pointer (here -8 and -4 from m_pchData) into -32768 + 32760; the original
// compiler never does
INCLUDE_ASM("bx/bxstring", __pl__FRC9cBXStringPCc);
#ifdef SKIP_ASM
cBXString operator+(const cBXString& string, const char* psz)
{
    cBXString s;
    s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, cBXString::SafeStrlen(psz), psz);
    return s;
}
#endif

cBXString operator+(const char* psz, const cBXString& string)
{
    cBXString s;
    s.ConcatCopy(cBXString::SafeStrlen(psz), psz, string.GetLength(), string.m_pchData);
    return s;
}

cBXString operator+(const cBXString& string, char ch)
{
    cBXString s;
    s.ConcatCopy(string.GetLength(), string.m_pchData, 1, &ch);
    return s;
}

cBXString operator+(char ch, const cBXString& string)
{
    cBXString s;
    s.ConcatCopy(1, &ch, string.GetLength(), string.m_pchData);
    return s;
}

// Not matching: our compiler turns two nearby negative offsets from one
// pointer (here -8 and -4 from m_pchData) into -32768 + 32760; the original
// compiler never does
INCLUDE_ASM("bx/bxstring", ConcatInPlace__9cBXStringiPCc);
#ifdef SKIP_ASM
void cBXString::ConcatInPlace(int nSrcLen, const char* pszSrcData)
{
    if (GetData()->nRefs > 1 || GetLength() + nSrcLen > GetAllocLength()) {
        // Holds a reference so the old buffer outlives Release
        cBXString old(*this);
        cBXStringData* pOldData = GetData();
        Release();
        ConcatCopy(pOldData->nDataLength, pOldData->data(), nSrcLen, pszSrcData);
    } else {
        memcpy(m_pchData + GetLength(), pszSrcData, nSrcLen);
        GetData()->nDataLength += nSrcLen;
        m_pchData[GetLength()] = '\0';
    }
}
#endif

const cBXString& cBXString::operator+=(const char* psz)
{
    ConcatInPlace(SafeStrlen(psz), psz);
    return *this;
}

const cBXString& cBXString::operator+=(char ch)
{
    ConcatInPlace(1, &ch);
    return *this;
}

const cBXString& cBXString::operator+=(const cBXString& string)
{
    ConcatInPlace(string.GetLength(), string.m_pchData);
    return *this;
}

char* cBXString::GetBuffer(int nMinBufLength)
{
    if (GetData()->nRefs > 1 || nMinBufLength > GetAllocLength()) {
        cBXString old(*this);
        int nOldLen = old.GetLength();
        Release();
        AllocBuffer(nOldLen < nMinBufLength ? nMinBufLength : nOldLen);
        memcpy(m_pchData, old.m_pchData, nOldLen);
        GetData()->nDataLength = nOldLen;
        m_pchData[nOldLen] = '\0';
    }
    return m_pchData;
}

void cBXString::ReleaseBuffer(int nNewLength)
{
    CopyBeforeWrite();
    if (nNewLength == -1)
        nNewLength = strlen(m_pchData);
    if (m_pchData != D_0048DCD4) {
        GetData()->nDataLength = nNewLength;
        m_pchData[nNewLength] = '\0';
    }
}

char* cBXString::GetBufferSetLength(int nNewLength)
{
    GetBuffer(nNewLength);
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
    return m_pchData;
}

// Not matching: our compiler turns two nearby negative offsets from one
// pointer (here -8 and -4 from m_pchData) into -32768 + 32760; the original
// compiler never does
INCLUDE_ASM("bx/bxstring", FreeExtra__9cBXString);
#ifdef SKIP_ASM
void cBXString::FreeExtra()
{
    if (GetLength() != GetAllocLength()) {
        cBXString old(*this);
        int nLen = old.GetLength();
        Release();
        AllocBuffer(nLen);
        memcpy(m_pchData, old.m_pchData, nLen);
    }
}
#endif

char* cBXString::LockBuffer()
{
    char* psz = GetBuffer(0);
    GetData()->nRefs = -1;
    return psz;
}

void cBXString::UnlockBuffer()
{
    if (m_pchData != D_0048DCD4)
        GetData()->nRefs = 1;
}

int cBXString::Find(char ch, int nStart) const
{
    char* psz = strchr(m_pchData + nStart, ch);
    return psz == 0 ? -1 : (int)(psz - m_pchData);
}

int cBXString::FindOneOf(const char* pszCharSet) const
{
    char* psz = func_0041AC40(m_pchData, pszCharSet);
    return psz == 0 ? -1 : (int)(psz - m_pchData);
}

int cBXString::ReverseFind(char ch) const
{
    char* psz = func_0041ACC0(m_pchData, ch);
    return psz == 0 ? -1 : (int)(psz - m_pchData);
}

int cBXString::Find(const char* pszSub, int nStart) const
{
    char* psz = func_0041AD80(m_pchData + nStart, pszSub);
    return psz == 0 ? -1 : (int)(psz - m_pchData);
}

void cBXString::MakeUpper()
{
    CopyBeforeWrite();
    char* p = m_pchData;
    int i = GetLength();
    while (--i >= 0) {
        *p = toupper(*p);
        p++;
    }
}

void cBXString::MakeLower()
{
    CopyBeforeWrite();
    char* p = m_pchData;
    int i = GetLength();
    while (--i >= 0) {
        *p = tolower(*p);
        p++;
    }
}

void cBXString::Replace(char chOld, char chNew)
{
    CopyBeforeWrite();
    char* p = m_pchData;
    int i = GetLength();
    while (--i >= 0) {
        if (*p == chOld)
            *p = chNew;
        p++;
    }
}

void cBXString::SetAt(int nIndex, char ch)
{
    CopyBeforeWrite();
    m_pchData[nIndex] = ch;
}

cBXString::cBXString(char ch, int nRepeat)
{
    Init();
    if (nRepeat >= 1) {
        AllocBuffer(nRepeat);
        memset(m_pchData, ch, nRepeat);
    }
}

const cBXString& cBXString::operator=(char ch)
{
    AssignCopy(1, &ch);
    return *this;
}

cBXString cBXString::Mid(int nFirst) const
{
    return Mid(nFirst, GetLength() - nFirst);
}

cBXString cBXString::Mid(int nFirst, int nCount) const
{
    if (nFirst < 0)
        nFirst = 0;
    if (nCount < 0)
        nCount = 0;
    if (nFirst + nCount > GetLength())
        nCount = GetLength() - nFirst;
    if (nFirst > GetLength())
        nCount = 0;

    cBXString dest;
    AllocCopy(dest, nCount, nFirst, 0);
    return dest;
}

cBXString cBXString::Right(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    else if (nCount > GetLength())
        nCount = GetLength();

    cBXString dest;
    AllocCopy(dest, nCount, GetLength() - nCount, 0);
    return dest;
}

cBXString cBXString::Left(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    else if (nCount > GetLength())
        nCount = GetLength();

    cBXString dest;
    AllocCopy(dest, nCount, 0, 0);
    return dest;
}

cBXString cBXString::SpanIncluding(const char* pszCharSet) const
{
    return Left(func_0041AD10(m_pchData, pszCharSet));
}

cBXString cBXString::SpanExcluding(const char* pszCharSet) const
{
    return Left(func_0041AB08(m_pchData, pszCharSet));
}
