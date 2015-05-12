#ifndef INCLUDED_ENGINE_SERIALWRITER_H
#define INCLUDED_ENGINE_SERIALWRITER_H

//====================================================================================================
// Filename:	SerialWriter.h
// Created by:	Tyler Staples
// Description:	Utility object for serializing data to an external buffer.
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class SerialWriter
{
public:
    /*  Constructor
    *   @param buffer: external buffer to write to.
    *   @param size: size in bytes of the buffer.
    */
    SerialWriter(u8* buffer, u32 size);

    /*  Writes data to the buffer.
    *   @param data: the data to write.
    */
	template <typename T>
	void Write(const T& data);

    /*  Writes an array of bytes to the buffer.
    *   @param data: array to be written to the buffer.
    *   @param size: size in bytes of the data.
    */
    template <typename T>
    void WriteArray(const T* data, size_t size);

    /*  Length encodes and writes a string to the buffer.
    *   @param str: string to write.
    */
    void WriteLengthEncodedString(const std::string& str);

    u32 GetSize() const     { return mBufferSize; }
    u32 GetOffset() const   { return mWriteOffset; }

private:
	u8* mBufferHandle;	// Buffer we are reading/writing to
	u32 mWriteOffset;	// Position in buffer
	u32 mBufferSize;	// Total size of buffer
};

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "SerialWriter.inl"

#endif // #ifndef INCLUDED_ENGINE_SERIALWRITER_H