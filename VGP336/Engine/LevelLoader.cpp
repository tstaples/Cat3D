#include "Precompiled.h"
#include "LevelLoader.h"

#include "GameObject.h"
#include "MemoryPool.h"
#include "IO.h"
#include "SerialReader.h"
#include "SerialWriter.h"

LevelLoader::LevelLoader()
    : mBufferHasData(false)
    , mOffset(0)
{
    mBuffer = new u8[LEVEL_BUFFER_SIZE];
    memset(mBuffer, 0, LEVEL_BUFFER_SIZE);
}

//----------------------------------------------------------------------------------------------------

LevelLoader::~LevelLoader()
{
    SafeDeleteArray(mBuffer);
}

//----------------------------------------------------------------------------------------------------

/*  Format
    - Gamesettings block
    - num gameobjects
    - gameObject data
    .
    .
    .
*/

bool LevelLoader::Load(const char* filename, Level& level, Mode mode)
{
    memset(mBuffer, 0, LEVEL_BUFFER_SIZE);

    // Read in the file
    mBufferHasData = false;
    const u32 fileSize = IO::GetFileSize(filename);
    if (!IO::SyncReadFile(filename, mBuffer, fileSize))
    {
        return mBufferHasData;
    }

    // Extract the data from the buffer and store in the level object
    SerialReader reader(mBuffer, fileSize);
    level.settings = reader.Read<GameSettings>();
    level.numGameObjects = reader.Read<u32>();
    level.buffer = mBuffer + reader.GetOffset();
    level.bufferSize = fileSize - reader.GetOffset();
    
    // Track the offset and set that we have data in the temp buffer
    mOffset = fileSize;
    mBufferHasData = true;

    return mBufferHasData;
}

//----------------------------------------------------------------------------------------------------

bool LevelLoader::SaveLocal(const GameObjectHandles& handles, const GameSettings& settings)
{
    memset(mBuffer, 0, LEVEL_BUFFER_SIZE);

    // Write the settings and number of objects
    SerialWriter writer(mBuffer, LEVEL_BUFFER_SIZE);
    writer.Write(settings);
    writer.Write(handles.size());

    // Write out all the objects
    for (GameObjectHandle handle : handles)
    {
        GameObject* gameObject = handle.Get();
        gameObject->Serialize(writer);
    }
    
    // Track the offset and set that we have data in the temp buffer
    mOffset = writer.GetOffset();
    mBufferHasData = true;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool LevelLoader::SaveToFile(const char* filename, const GameObjectHandles& handles, const GameSettings& settings, Mode mode)
{
    if (SaveLocal(handles, settings))
    {
        return WriteBufferToFile(filename, mode);
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool LevelLoader::WriteBufferToFile(const char* filename, Mode mode)
{
    bool canWrite = mBufferHasData && (mOffset > 0);
    ASSERT(canWrite, "[LevelLoader] Error: no level data to write.");
    if (canWrite)
    {
        if (IO::SyncWriteFile(filename, mBuffer, mOffset))
        {
            return true;
        }
    }
    return false;
}