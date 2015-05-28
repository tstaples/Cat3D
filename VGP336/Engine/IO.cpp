#include "Precompiled.h"
#include "IO.h"

namespace IO
{

namespace Mode
{
    const char* Read = "r";
    const char* ReadBinary = "rb";
    const char* Write = "w";
    const char* WriteBinary = "wb";
    const char* ReadWrite = "w+";
    const char* ReadWriteBinary = "wb+";
    const char* Append = "a";
} // namespace Mode

bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, size_t& bytesRead, const char* mode)
{
    FILE* handle = fopen(path, mode);
    if (handle)
    {
        // Blocking operation
        size_t bytesread = fread(buffer, 1, buffsize, handle);
        fclose(handle);
    
        int err = ferror(handle);
        if (err == 0)
        {
            bytesRead = bytesread;
            return true;
        }
    }
    bytesRead = 0;
    return false;
}

//----------------------------------------------------------------------------------------------------

bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, const char* mode)
{
    size_t read = 0;
    if (SyncReadFile(path, buffer, buffsize, read, mode))
    {
        return (read == buffsize);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, const char* mode)
{
    size_t written = 0;
    if (SyncWriteFile(path, buffer, buffsize, written, mode))
    {
        return (written == buffsize);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, size_t& bWritten, const char* mode)
{
    FILE* handle = fopen(path, mode);
    if (handle)
    {
        size_t bytesWritten = fwrite(buffer, sizeof(u8), buffsize, handle);
        fclose(handle);

        int err = ferror(handle);
        if (err == 0)
        {
            bWritten = bytesWritten;
            return true;
        }
    }
    bWritten = 0;
    return false;
}

//----------------------------------------------------------------------------------------------------

u32 GetFileSize(const char* path)
{
    u32 size = 0;
    FILE* handle = fopen(path, Mode::Read);
    if (handle)
    {
        size = GetFileSize(handle);
        fclose(handle);
    }
    return size;
}

//----------------------------------------------------------------------------------------------------

u32 GetFileSize(FILE* handle)
{
    fseek(handle, 0, SEEK_END);
    u32 size = (u32)ftell(handle);
    fseek(handle, 0, SEEK_SET);
    return size;
}

//----------------------------------------------------------------------------------------------------

std::string GetExtension(const std::string& file)
{
    std::string extension;
    size_t pos = file.find_last_of('.');
    if (pos != std::string::npos)
    {
        extension.assign(file.begin() + pos + 1, file.end());
    }
    return extension;
}

//----------------------------------------------------------------------------------------------------

std::string GetExtension(const wchar_t* file)
{
    std::string extension;

    std::wstring path(file);
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
    {
        extension.assign(path.begin() + pos + 1, path.end());
    }
    return extension;
}

//----------------------------------------------------------------------------------------------------

std::string GetLocation(const std::string& path)
{
    std::string locationDir;

    // Try forward slash
    u32 slashIndex = path.find_last_of('/');
    if (slashIndex != std::string::npos)
    {
        // +1 to include the trailing slash
        locationDir.assign(path.begin(), path.begin() + slashIndex + 1);
    }
    else
    {
        // Check if path is formatted with backslashed
        slashIndex = path.find_last_of('\\');

        // +1 to include the trailing slash
        locationDir.assign(path.begin(), path.begin() + slashIndex + 1);
    }
    return locationDir;
}

//----------------------------------------------------------------------------------------------------

void CharToWChar(const std::string& buff, wchar_t* out, size_t outSize)
{
    const size_t size = buff.size();
    if (size > 0)
    {
        size_t numConverted = 0;
        errno_t err = mbstowcs_s(&numConverted, out, size * 2, buff.c_str(), size);
        ASSERT(err == 0, "Error converting char* to wchar_t*");
    }
}

//----------------------------------------------------------------------------------------------------

std::wstring CharToWChar(const std::string& buff)
{
    const size_t size = buff.size();

    ASSERT(size < 2048, "Buffer overflow");
    wchar_t wbuffer[2048] = { 0 };  // hack: use temp buffer of some fixed size. hopefully it's big enough.

    CharToWChar(buff, wbuffer, 2048);
    return std::wstring(wbuffer);
}

//----------------------------------------------------------------------------------------------------

void WCharToChar(const std::wstring& wbuff, char* out, size_t outSize)
{
    const size_t size = wbuff.size();
    if (size > 0)
    {
        size_t numConverted = 0;
        errno_t err = wcstombs_s(&numConverted, out, outSize, wbuff.c_str(), size);
        ASSERT(err == 0, "Error converting wchar_t* to char*");
    }
}

//----------------------------------------------------------------------------------------------------

std::string WCharToChar(const std::wstring& wbuff)
{
    const size_t size = wbuff.size();

    ASSERT(size < 2048, "Buffer overflow");
    char buffer[2048] = { 0 };  // hack: use temp buffer of some fixed size. hopefully it's big enough.

    WCharToChar(wbuff, buffer, 2048);
    return std::string(buffer);
}

} // namespace IO