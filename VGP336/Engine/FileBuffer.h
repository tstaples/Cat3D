#ifndef INCLUDED_ENGINE_FILEBUFFER_H
#define INCLUDED_ENGINE_FILEBUFFER_H

#include "Serializer.h"

class FileBuffer
{
public:
    // Doesn't init buffer
    FileBuffer();
    // Loads the file into the buffer
    FileBuffer(const char* pFilename);
    // Inits a buffer of size
    FileBuffer(const size_t size);
    ~FileBuffer();

    // Writes the buffer to the file
    bool WriteBufferToFile(const char* pFilename);

    // Init and load the buffer from a file
    bool LoadBuffer(const char* pFilename);
    bool LoadBuffer(const char* pFilename, size_t fileSize);

    // Wipes the buffer
    void Clear();

    // Returns true if the buffer is initialized
    bool Initialized();

    // Writes data to the buffer
    template <typename T>
	void Write(const T& data);

    // Reads sizeof(T) bytes from the buffer into data
    template <typename T>
	void Read(T& data);

    const u8* GetBuffer() const     { return mBuffer; }
    const size_t Size() const       { return mBufferSize; }
    const size_t GetOffset() const  { return mWriteOffset; }

private:
	u8* mBuffer;
    size_t mBufferSize;     // mBuffer's capacity
	size_t mWriteOffset;	// Position in buffer
};

//====================================================================================================
// Inline definitions
//====================================================================================================

template <typename T>
void FileBuffer::Write(const T& data)
{
    Serializer::Write(data, mBuffer, mWriteOffset);
    ASSERT(mWriteOffset <= mBufferSize, "Writing to invalid memory");
}

template <typename T>
void FileBuffer::Read(T& data)
{
    Serializer::Read(data, mBuffer, mWriteOffset);
    ASSERT(mWriteOffset <= mBufferSize, "Writing to invalid memory");
}

#endif // #ifndef INCLUDED_ENGINE_FILEBUFFER_H