#ifndef INCLUDED_ENGINE_LEVELLOADER_H
#define INCLUDED_ENGINE_LEVELLOADER_H

//====================================================================================================
// Filename:	LevelLoader.h
// Created by:	Tyler Staples
// Description: Utility class for saving and loading levels.
//====================================================================================================

#define LEVEL_BUFFER_SIZE (1024 * 1024)

//====================================================================================================
// Includes
//====================================================================================================

#include "GameObject.h"
#include "GameSettings.h"
#include "IO.h"

//====================================================================================================
// Forward declarations
//====================================================================================================

struct GameSettings;

//====================================================================================================
// Structs
//====================================================================================================

struct Level
{
    GameSettings settings;
    const u8* buffer;
    u32 bufferSize;
    u32 numGameObjects;
};

//====================================================================================================
// Class declarations
//====================================================================================================

class LevelLoader
{
    typedef std::vector<GameObjectHandle> GameObjectHandles;

public:
    enum Mode : u32
    {
        Binary,
        JSON
    };

    LevelLoader();
    
    /* Loads the data for a level.
     * @param filename: path to the level file.
     * @param level: Level struct to ouput the data to.
     * Returns true if read level data in successfully.
     */
    bool Load(const char* filename, Level& level, Mode mode = Mode::Binary);

    /* Serializes out the level data to memory.
     * @param handles: static gameobjects to store in the level data.
     * @param settings: the settings for the level.
     * Returns true if saved level data successfully.
     */
    bool SaveLocal(const GameObjectHandles& handles, const GameSettings& settings);

    /* Serializes out the level data to a file.
     * @param filename: path to the level file.
     * @param handles: static gameobjects to store in the level data.
     * @param settings: the settings for the level.
     * Returns true if saved level data successfully.
     */
    bool SaveToFile(const char* filename, const GameObjectHandles& handles, const GameSettings& settings, Mode mode = Mode::Binary);

    /* Writes the current level data in the buffer to file.
     * @param filename: file to write to.
     * Returns true if data was written successfully.
     */
    bool WriteBufferToFile(const char* filename, Mode mode = Mode::Binary);

private:
    NONCOPYABLE(LevelLoader);

    u8 mBuffer[LEVEL_BUFFER_SIZE];
    u32 mOffset;
    bool mBufferHasData;
};

#endif // #ifndef INCLUDED_ENGINE_LEVELLOADER_H