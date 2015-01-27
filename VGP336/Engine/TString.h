#ifndef INCLUDED_ENGINE_STRING_H
#define INCLUDED_ENGINE_STRING_H

// TODO
// Large string table allocated/managed by engine
// new names are appended, deliminated by '\0'.
// Name also holds hash of the string to prevent lookups for comparisons
/*
struct name
{
    const char* strPtr;
    u32 key;
};
*/

// TODO: Use small block allocator
// Create IAllocator interface for a system similar to Repository

class String
{
public:
    String();
    String(const char* str);
    String(const wchar_t* wstr);
    String(const String& rhs);
    String& operator=(const String& rhs);
    ~String();

    // Free's the memory occupied by the string
    void Clear();

    // Returns true if size is 0
    bool Empty() const;

    // Returns the number of characters in the string (not including null)
    size_t Size() const;

    const char* CStr() const;
    const wchar_t* WCStr() const;

    void Append(const String& str);

    String& operator+(const String& rhs);

private:
    char* mString;
    size_t mSize;
};

#endif // #ifndef INCLUDED_ENGINE_STRING_H