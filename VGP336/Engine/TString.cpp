#include "Precompiled.h"
#include "TString.h"


String::String()
    : mString(nullptr)
    , mSize(0)
{
}

String::String(const char* str)
{
    ASSERT(str != nullptr, "String is unitialized");

    mSize = strlen(str);
    mString = new char[mSize];
    memcpy(mString, str, mSize);
}

String::String(const wchar_t* wstr)
{
    mSize = wcslen(wstr);
    mString = new char[mSize];

    size_t numConverted = 0;
    errno_t err = wcstombs_s(&numConverted, mString, mSize, wstr, mSize);

    ASSERT(err == 0, "Error converting wchar_t* to char*");
}

String::String(const String& rhs)
{
    // Call explicit = operator
    *this = rhs;
}

String& String::operator=(const String& rhs)
{
    if (this != &rhs)
    {
        Clear();

        mSize = rhs.mSize;
        mString = new char[mSize];
        strcpy_s(mString, mSize, rhs.mString);
    }
    return *this;
}

String::~String()
{
    Clear();
}

void String::Clear()
{
    if (mString)
    {
        delete [] mString;
        mString = nullptr;
    }
    mSize = 0;
}

size_t String::Size() const
{
    return mSize;
}

const char* String::CStr() const
{
    return mString;
}

const wchar_t* String::WCStr() const
{
    return nullptr;
}