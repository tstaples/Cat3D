#include "Precompiled.h"

#include "File.h"
#include <string.h>

namespace IO
{

bool File::SyncReadFile(const char* path, u8* buffer, size_t buffsize, size_t& bytesRead, const char* mode)
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

bool File::SyncReadFile(const char* path, u8* buffer, size_t buffsize, const char* mode)
{
    size_t read = 0;
    if (SyncReadFile(path, buffer, buffsize, read, mode))
    {
        return (read == buffsize);
    }
    return false;
}

bool File::SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, const char* mode)
{
    size_t written = 0;
    if (SyncWriteFile(path, buffer, buffsize, written, mode))
    {
        return (written == buffsize);
    }
    return false;
}

bool File::SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, size_t& bWritten, const char* mode)
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

size_t File::GetFileSize(const char* path)
{
    size_t size = 0;
    FILE* handle = fopen(path, Mode::Read);
    if (handle)
    {
        fseek(handle, 0, SEEK_END);
        size = ftell(handle);
        fseek(handle, 0, SEEK_SET);
        fclose(handle);
    }
    return size;
}

std::string File::GetExtension(const std::string& file)
{
    std::string extension;
    size_t pos = file.find_last_of('.');
    if (pos != std::string::npos)
    {
        extension.assign(file.begin() + pos + 1, file.end());
    }
    return extension;
}

std::string File::GetExtension(const wchar_t* file)
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

void File::ConvertMBToWChar(const std::string& buff, wchar_t* out)
{
    size_t numConverted = 0;
    //mbstowcs_s(&numConverted, out, buff.c_str(), 256);
    mbstowcs_s(&numConverted, out, 256, buff.c_str(), 256);
}

std::string File::GetLocation(const std::string& path)
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

} // namespace IO