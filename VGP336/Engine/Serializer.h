#ifndef INCLUDED_ENGINE_SERIALIZER_H
#define INCLUDED_ENGINE_SERIALIZER_H

class Serializer
{
public:
    template <typename T>
    static void Write(const T& data, u8* buffer, u32& offset)
    {
        //u8* handle = buffer + offset;
        memcpy(buffer + offset, &data, sizeof(data));
        offset += sizeof(data);
    }

    template <typename T>
    static void Read(const T& data, u8* buffer, u32& offset)
    {
        data = *reinterpret_cast<T*>(data + offset);
        offset += sizeof(data);
    }
};

class SerialWriter/* : public Serializer*/
{
public:
    SerialWriter(u8* buffer, u32 size)
        : mBufferHandle(buffer)
        , mBufferSize(size)
        , mWriteOffset(0)
    {
    }

	template <typename T>
	void Write(const T& data)
	{
		memcpy(mBufferHandle + mWriteOffset, &data, sizeof(data));
		mWriteOffset += sizeof(data);
		assert(mWriteOffset <= mBufferSize);
	}

private:
	u8* mBufferHandle;	    // Buffer we are reading/writing to
	u32 mWriteOffset;	// Position in buffer
	u32 mBufferSize;	    // Total size of buffer
};


class SerialReader/* : public Serializer*/
{
public:
    SerialReader(const u8* buffer, u32 size)
        : mBufferHandle(buffer)
        , mBufferSize(size)
        , mWriteOffset(0)
    {
    }

    template <typename T>
	void Read(T& data)
	{
		//data = *reinterpret_cast<T*>(mBufferHandle + mWriteOffset);
        memcpy(&data, mBufferHandle + mWriteOffset, sizeof(data));
		mWriteOffset += sizeof(data);
		assert(mWriteOffset <= mBufferSize);
	}

    // Reads sizeof(T) * length bytes from the buffer
    template <typename T>
	void Read(T& data, u32 length)
	{
        memcpy(&data, mBufferHandle + mWriteOffset, sizeof(data) * length);
		mWriteOffset += sizeof(data) * length;
		assert(mWriteOffset <= mBufferSize);
	}

    template <typename T>
	T Read()
    {
        T data = NULL;
        Read(data);
        return data;
    }

    // Reads n bytes from the buffer and returns it as a formatted string
	inline std::string ReadFormatted(u32 bytes)
	{
        const u8* buff = mBufferHandle + mWriteOffset;
        std::string str(buff, buff + bytes);
		mWriteOffset += bytes;
		assert(mWriteOffset <= mBufferSize);
        return str;
	}

    enum Origin
    {
        Beg,
        Current,
        End
    };

    inline void Seek(s32 offset, Origin origin)
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

private:
	const u8* mBufferHandle;	    // Buffer we are reading/writing to
	u32 mWriteOffset;	// Position in buffer
	u32 mBufferSize;	    // Total size of buffer
};

#endif // #ifndef INCLUDED_ENGINE_SERIALIZER_H