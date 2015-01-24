#ifndef INCLUDED_ENGINE_IO_H
#define INCLUDED_ENGINE_IO_H

namespace IO
{

namespace Mode
{
    extern const char* Read;
    extern const char* ReadBinary;
    extern const char* Write;
    extern const char* WriteBinary;
    extern const char* ReadWrite;
    extern const char* ReadWriteBinary;
    extern const char* Append;
} // namespace Mode


// Synchronous read/write helpers
bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, const char* mode=Mode::ReadBinary);
bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, size_t& bytesRead, const char* mode=Mode::ReadBinary);
bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, const char* mode=Mode::WriteBinary);
bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, size_t& bytesWritten, const char* mode=Mode::WriteBinary);
    
// Returns the size of a file in bytes
size_t GetFileSize(const char* path);

// Returns the extension of a filename
std::string GetExtension(const std::string& file);
std::string GetExtension(const wchar_t* file);

// Gets the directory the file resides in
std::string GetLocation(const std::string& path);

// === UTIL ===

// Converts a char array to a wchar array
void CharToWChar(const std::string& buff, wchar_t* out, size_t outSize);
std::wstring CharToWChar(const std::string& buff);

void WCharToChar(const std::wstring& wbuff, char* out, size_t outSize);
std::string WCharToChar(const std::wstring& wbuff);

} // namespace IO

#endif // #ifndef INCLUDED_ENGINE_IO_H