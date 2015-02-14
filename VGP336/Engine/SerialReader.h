#ifndef INCLUDED_ENGINE_SERIALREADER_H
#define INCLUDED_ENGINE_SERIALREADER_H

//====================================================================================================
// Filename:	SerialReader.h
// Created by:	Tyler Staples
// Description:	Utility object for reading serialized data from an external buffer.
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class SerialReader
{
public:
    /*  Constructor
    *   @param buffer: external buffer to read from.
    *   @param size: size in bytes of the buffer.
    */
    SerialReader(const u8* buffer, u32 size);

    enum Origin
    {
        Beg,
        Current,
        End
    };

    template <typename T>
	void Read(T& data);

    /*  Reads size bytes from the buffer.
    *   @param data: buffer read data is copied into.
    *   @param size: size (bytes) to read from the buffer.
    */
    template <typename T>
	void ReadArray(T* data, size_t size);

    /*  Reads data from the buffer into a vector.
    *   @param vec: vector to store the read data.
    *   @param size: number of elements to read.
    */
    template <typename T>
	void ReadVector(std::vector<T>& vec, size_t size);

    /*  Reads sizeof(T) bytes and returns it.
    *   Returns: data read from buffer.
    */
    template <typename T>
	T Read();

    /*  Reads data from the buffer into a formatted string.
    *   @param bytes: number of bytes to read from the buffer.
    *   Returns: the formatted string.
    */
	inline std::string ReadFormatted(u32 bytes);

    /*  Reads a length encoded string from the buffer.
    *   Assumes next byte in the buffer is the length of the string.
    */
    inline std::string ReadLengthEncodedString();

    /*  Seeks to a certain position in the buffer.
    *   @param offset: number of bytes to move the buffer handle from the origin.
    *   @param origin: position where the offset is applied from.
    */
    inline void Seek(s32 offset, Origin origin);

private:
	const u8* mBufferHandle;    // Buffer we are reading/writing to
	u32 mWriteOffset;	        // Position in buffer
	u32 mBufferSize;	        // Total size of buffer
};

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "SerialReader.inl"

#endif // #ifndef INCLUDED_ENGINE_SERIALREADER_H