#ifndef INCLUDED_ENGINE_SERIALIZER_H
#define INCLUDED_ENGINE_SERIALIZER_H


class SerialWriter
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

    template <typename T>
    void WriteArray(const T* data, size_t size)
    {
        memcpy(mBufferHandle + mWriteOffset, data, size);
        mWriteOffset += size;
    }

private:
	u8* mBufferHandle;	// Buffer we are reading/writing to
	u32 mWriteOffset;	// Position in buffer
	u32 mBufferSize;	// Total size of buffer
};


class SerialReader
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
	void ReadArray(T* data, size_t size)
	{
        memcpy(data, mBufferHandle + mWriteOffset, size);
		mWriteOffset += size;
		assert(mWriteOffset <= mBufferSize);
	}

    template <typename T>
	void ReadVector(std::vector<T> vec, size_t size)
	{
        T* buffer = new T[size];
        ReadArray(buffer, size);
        for (u32 i=0; i < size; ++i)
        {
            vec.push_back(buffer[i]);
        }
        SafeDeleteArray(buffer);
	}

    template <typename T>
	T Read()
    {
        T data;
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