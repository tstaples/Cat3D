#ifndef INCLUDED_ENGINE_INPUTDATA_H
#define INCLUDED_ENGINE_INPUTDATA_H

//====================================================================================================
// Filename:	InputData.h
// Created by:	Tyler Staples
// Description:	Struct for an input event.
//====================================================================================================

#define WHEEL_SCROLL_UP 1
#define WHEEL_SCROLL_DOWN -1

//====================================================================================================
// Structs
//====================================================================================================

struct InputData
{
    bool keyStates[256];
    bool mouseStates[4];
    s32 mouseX;
    s32 mouseY;
    f32 mouseMoveX;
    f32 mouseMoveY;
    s8 mouseScrollDelta;
};

#endif // #ifndef INCLUDED_ENGINE_INPUTDATA_H