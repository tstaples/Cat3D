inline SerialWriter::SerialWriter(u8* buffer, u32 size)
    : mBufferHandle(buffer)
    , mBufferSize(size)
    , mWriteOffset(0)
{
    ASSERT(buffer != nullptr, "[SerialWriter] Error: buffer is uninitialized");
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SerialWriter::Write(const T& data)
{
	memcpy(mBufferHandle + mWriteOffset, &data, sizeof(data));
	mWriteOffset += sizeof(data);
	assert(mWriteOffset <= mBufferSize);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SerialWriter::WriteArray(const T* data, size_t size)
{
    memcpy(mBufferHandle + mWriteOffset, data, size);
    mWriteOffset += size;
	assert(mWriteOffset <= mBufferSize);
}

//----------------------------------------------------------------------------------------------------

inline void SerialWriter::WriteLengthEncodedString(const std::string& str)
{
    u32 len = str.length();
    Write(len);
    WriteArray(str.c_str(), len);
}

//----------------------------------------------------------------------------------------------------

inline void SerialWriter::WriteLengthEncodedString(const std::wstring& str)
{
    u32 len = str.length();
    Write(len);
    WriteArray(str.c_str(), len);
}