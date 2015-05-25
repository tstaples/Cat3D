#include "Precompiled.h"
#include "FileBuffer.h"
#include "IO.h"

namespace IO
{

FileBuffer::FileBuffer(const size_t size)
    : mBuffer(nullptr)
    , mBufferSize(size)
    , mWriteOffset(0)
{
    mBuffer = new u8[mBufferSize];
    //memset(mBuffer, 0, sizeof(u8) * mBufferSize);
}

FileBuffer::FileBuffer(const char* pFilename)
    : mBuffer(nullptr)
    , mBufferSize(0)
    , mWriteOffset(0)
{
    LoadBuffer(pFilename);
}

FileBuffer::FileBuffer()
    : mBuffer(nullptr)
    , mBufferSize(0)
    , mWriteOffset(0)
{
}

FileBuffer::~FileBuffer()
{
    Clear();
}

bool FileBuffer::WriteBufferToFile(const char* pFilename)
{
    ASSERT(mBuffer, "Buffer unitialized");
    size_t bwritten = 0;
    return IO::SyncWriteFile(pFilename, mBuffer, mBufferSize, bwritten);
}

bool FileBuffer::LoadBuffer(const char* pFilename)
{
    // Clear any existing data
    Clear();

    // Create a buffer the size of the file
    mBufferSize = IO::GetFileSize(pFilename);
    mBuffer = new u8[mBufferSize];
    
    size_t bRead = 0;
    return IO::SyncReadFile(pFilename, mBuffer, mBufferSize, bRead);
}

bool FileBuffer::LoadBuffer(const char* pFilename, size_t fileSize)
{
    // Clear any existing data
    Clear();

    // Create a buffer the size of the file
    mBufferSize = fileSize;
    mBuffer = new u8[mBufferSize];
    
    size_t bRead = 0;
    return IO::SyncReadFile(pFilename, mBuffer, mBufferSize, bRead);
}

void FileBuffer::Clear()
{
    SafeDeleteArray(mBuffer);
    mWriteOffset = 0;
    mBufferSize = 0;
}

bool FileBuffer::Initialized()
{
    return (mBuffer && mBufferSize);
}

} // namespace IO