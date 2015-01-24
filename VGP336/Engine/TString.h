#ifndef INCLUDED_ENGINE_STRING_H
#define INCLUDED_ENGINE_STRING_H

class String
{
public:
    String();
    String(const char* str);
    String(const wchar_t* wstr);
    String(const String& rhs);
    String& operator=(const String& rhs);
    ~String();

    void Clear();

    size_t Size() const;
    const char* CStr() const;
    const wchar_t* WCStr() const;

private:
    char* mString;
    size_t mSize;
};

#endif // #ifndef INCLUDED_ENGINE_STRING_H