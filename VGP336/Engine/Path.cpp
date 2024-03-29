#include "Precompiled.h"
#include "Path.h"

#include "IO.h"

namespace IO
{

Path::Path()
{
}

//----------------------------------------------------------------------------------------------------

Path::Path(const std::string& path)
{
    mPath = IO::CharToWChar(path);
    ConvertBackslashToForwardslash();
}

//----------------------------------------------------------------------------------------------------

Path::Path(const std::wstring& wpath)
: mPath(wpath)
{
    ConvertBackslashToForwardslash();
}

//----------------------------------------------------------------------------------------------------

std::wstring Path::GetExtension() const
{
    std::wstring extension;

    size_t pos = mPath.find_last_of('.');
    if (pos != std::string::npos)
    {
        extension.assign(mPath.begin() + pos + 1, mPath.end());
    }
    return extension;
}

//----------------------------------------------------------------------------------------------------

std::wstring Path::GetFileName() const
{
    char slashChar = (mFormat == Forward) ? '/' : '\\';
    size_t lastSlashPos = mPath.find_last_of(slashChar);

    return std::wstring(mPath.begin() + lastSlashPos + 1, mPath.end());
}

//----------------------------------------------------------------------------------------------------

std::wstring Path::GetFileNameWithoutExtension() const
{
    char slashChar = (mFormat == Forward) ? '/' : '\\';
    size_t lastSlashPos = mPath.find_last_of(slashChar);
    size_t dotPos = mPath.find_last_of('.');

    return std::wstring(mPath.begin() + lastSlashPos + 1, mPath.begin() + dotPos);
}

//----------------------------------------------------------------------------------------------------

std::string Path::GetExtensionA() const
{
    return IO::WCharToChar(GetExtension());
}

//----------------------------------------------------------------------------------------------------

std::string Path::GetFileNameA() const
{
    return IO::WCharToChar(GetFileName());
}

//----------------------------------------------------------------------------------------------------

std::string Path::GetFileNameWithoutExtensionA() const
{
    return IO::WCharToChar(GetFileNameWithoutExtension());
}

//----------------------------------------------------------------------------------------------------

std::wstring Path::GetFullPath() const
{
    // Convert to TCHAR array
    TCHAR* tcharPath = (TCHAR*)mPath.c_str();

    TCHAR buffer[MAX_PATH];
    GetFullPathName(tcharPath, MAX_PATH, buffer, NULL);
    
    return std::wstring(buffer);
}

//----------------------------------------------------------------------------------------------------

std::string Path::GetFullPathA() const
{
    return IO::WCharToChar(GetFullPath());
}

//----------------------------------------------------------------------------------------------------

std::wstring Path::GetPath() const
{
    return mPath;
}

//----------------------------------------------------------------------------------------------------

std::string Path::GetPathA() const
{
    return IO::WCharToChar(mPath);
}

//----------------------------------------------------------------------------------------------------

size_t Path::Length() const
{
    return mPath.size();
}

//----------------------------------------------------------------------------------------------------

void Path::ConvertBackslashToForwardslash()
{
    for (auto c : mPath)
    {
        if (c == '\\')
        {
            c = '/';
        }
    }
    mFormat = Forward;
}

//----------------------------------------------------------------------------------------------------

void Path::ConvertForwardslashToBackslash()
{
    for (auto c : mPath)
    {
        if (c == '/')
        {
            c = '\\';
        }
    }
    mFormat = Back;
}

//----------------------------------------------------------------------------------------------------

Path& Path::operator=(const std::wstring& rhs)
{
    mPath = rhs;
    return *this;
}

//----------------------------------------------------------------------------------------------------

Path& Path::operator=(const std::string& rhs)
{
    mPath = IO::CharToWChar(rhs);
    return *this;
}

} // namespace IO