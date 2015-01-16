#ifndef INCLUDED_ENGINE_FILE_H
#define INCLUDED_ENGINE_FILE_H

namespace IO
{

namespace Mode
{
    static const char* Read = "r";
    static const char* ReadBinary = "rb";
    static const char* Write = "w";
    static const char* WriteBinary = "wb";
    static const char* ReadWrite = "w+";
    static const char* ReadWriteBinary = "wb+";
    static const char* Append = "a";
}

class File
{
public:
    static bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, const char* mode=Mode::ReadBinary);
    static bool SyncReadFile(const char* path, u8* buffer, size_t buffsize, size_t& bytesRead, const char* mode=Mode::ReadBinary);
    static bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, const char* mode=Mode::WriteBinary);
    static bool SyncWriteFile(const char* path, const u8* buffer, size_t buffsize, size_t& bytesWritten, const char* mode=Mode::WriteBinary);
    static size_t GetFileSize(const char* path);

    static std::string GetExtension(const std::string& file);
};

} // namespace IO

#endif // #ifndef INCLUDED_ENGINE_FILE_H