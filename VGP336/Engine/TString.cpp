#include "Precompiled.h"
#include "TString.h"

String::String()
    : mString(nullptr)
    , mSize(0)
{
}

//----------------------------------------------------------------------------------------------------

String::String(const char* str)
{
    ASSERT(str != nullptr, "String is unitialized");

    mSize = strlen(str);
    mString = new char[mSize];
    memcpy(mString, str, mSize);
}

//----------------------------------------------------------------------------------------------------

String::String(const wchar_t* wstr)
{
    mSize = wcslen(wstr);
    mString = new char[mSize];

    size_t numConverted = 0;
    errno_t err = wcstombs_s(&numConverted, mString, mSize, wstr, mSize);

    ASSERT(err == 0, "Error converting wchar_t* to char*");
}

//----------------------------------------------------------------------------------------------------

String::String(const String& rhs)
{
    // Call explicit = operator
    *this = rhs;
}

//----------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------

String::~String()
{
    Clear();
}

//----------------------------------------------------------------------------------------------------

void String::Clear()
{
    if (mString)
    {
        delete [] mString;
        mString = nullptr;
    }
    mSize = 0;
}

//----------------------------------------------------------------------------------------------------

size_t String::Size() const
{
    return mSize;
}

//----------------------------------------------------------------------------------------------------

bool String::Empty() const
{
    return (mSize == 0);
}

//----------------------------------------------------------------------------------------------------

const char* String::CStr() const
{
    return mString;
}

//----------------------------------------------------------------------------------------------------

const wchar_t* String::WCStr() const
{
    // TODO: make wstring version of this class and create an instance of it here
    return nullptr;
}

//----------------------------------------------------------------------------------------------------

void String::Append(const String& str)
{
    // Get combined size of the strings
    size_t fullsize = mSize + str.mSize;
 
    // Create a new buffer to hold both strings
    char* concatBuffer = new char[fullsize];
    strcpy(concatBuffer, mString);
    strcpy(concatBuffer + mSize, str.mString);

    // Clear existing string
    Clear();
        
    // Assign the new buffer to this and update the size
    mString = concatBuffer;
    mSize = fullsize;
}

//----------------------------------------------------------------------------------------------------

String& String::operator+(const String& rhs)
{
    // TODO
    return String();
}