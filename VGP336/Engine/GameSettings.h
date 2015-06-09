#ifndef INCLUDED_ENGNIE_GAMESETTINGS_H
#define INCLUDED_ENGNIE_GAMESETTINGS_H

#include "EngineMath.h"

//====================================================================================================
// Struct Declarations
//====================================================================================================

struct GameSettings
{
    u16 startingLevelIndex;

    f32 timeStep;
    Math::Vector3 gravity;
};

#endif // #ifndef INCLUDED_ENGNIE_GAMESETTINGS_H