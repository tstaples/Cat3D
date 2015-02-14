inline SerialReader::SerialReader(const u8* buffer, u32 size)
    : mBufferHandle(buffer)
    , mBufferSize(size)
    , mWriteOffset(0)
{
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SerialReader::Read(T& data)
{
    memcpy(&data, mBufferHandle + mWriteOffset, sizeof(data));
	mWriteOffset += sizeof(data);
	assert(mWriteOffset <= mBufferSize);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SerialReader::ReadArray(T* data, size_t size)
{
    memcpy(data, mBufferHandle + mWriteOffset, size);
	mWriteOffset += size;
	assert(mWriteOffset <= mBufferSize);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SerialReader::ReadVector(std::vector<T>& vec, size_t size)
{
    // TODO: Use static buffer of n bytes
    T* buffer = new T[size];
    ReadArray(buffer, size * sizeof(T));
    for (u32 i=0; i < size; ++i)
    {
        vec.push_back(buffer[i]);
    }
    SafeDeleteArray(buffer);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline T SerialReader::Read()
{
    T data;
    Read(data);
    return data;
}

//----------------------------------------------------------------------------------------------------

// Reads n bytes from the buffer and returns it as a formatted string
inline std::string SerialReader::ReadFormatted(u32 bytes)
{
    const u8* buff = mBufferHandle + mWriteOffset;
    std::string str(buff, buff + bytes);
	mWriteOffset += bytes;
	assert(mWriteOffset <= mBufferSize);
    return str;
}

//----------------------------------------------------------------------------------------------------

inline std::string SerialReader::ReadLengthEncodedString()
{
    const u32 len = Read<u32>();
    char buffer[2048];
    ReadArray(buffer, len);
    if (len < 2048)
    {
        buffer[len] = '\0';
    }
    return std::string(buffer);
}

//----------------------------------------------------------------------------------------------------

inline void SerialReader::Seek(s32 offset, Origin origin)
{
    switch (origin)
    {
    case Beg:
        mWriteOffset = offset;
        break;
    case Current:
        mWriteOffset += offset;
        break;
    case End:
        mWriteOffset = mBufferSize - offset;
    }
}