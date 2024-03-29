#ifndef INCLUDED_ENGINE_INPUTEVENT_H
#define INCLUDED_ENGINE_INPUTEVENT_H

//====================================================================================================
// Filename:	InputEvent.h
// Created by:	Peter Chan
// Description:	Struct for an input event.
//====================================================================================================

//====================================================================================================
// Macros
//====================================================================================================

#define WHEEL_SCROLL_UP 1
#define WHEEL_SCROLL_DOWN -1

//====================================================================================================
// Enums
//====================================================================================================

namespace Input
{
    enum Type
	{
		Character,
		KeyUp,
		KeyDown,
		MouseUp,
		MouseDown,
		MouseMove,
        MouseScroll,
	};
}

//====================================================================================================
// Structs
//====================================================================================================

struct InputEvent
{
    InputEvent()
    : x(0), y(0), wheeldelta(0)
    {}

	Input::Type type;
	u32 value;
	s32 x;
	s32 y;
    s8 wheeldelta; // 120 = scroll up, -120 = scroll down
};

#endif // #ifndef INCLUDED_ENGINE_INPUTEVENT_H