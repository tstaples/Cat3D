#ifndef INCLUDED_ENGINE_INPUTMANAGER_H
#define INCLUDED_ENGINE_INPUTMANAGER_H

//====================================================================================================
// Includes
//====================================================================================================

#include "Delegate.h"
#include "InputEvent.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct InputData;

//====================================================================================================
// Macros
//====================================================================================================

#define MAKE_ACTION_DELEGATE(CLASS_TYPE_PARAM, CALLBACK_PARAM)\
    Delegate<bool>::Make<CLASS_TYPE_PARAM, CALLBACK_PARAM>(this)

#define MAKE_AXIS_DELEGATE(CLASS_TYPE_PARAM, CALLBACK_PARAM)\
    Delegate<bool, s32>::Make<CLASS_TYPE_PARAM, CALLBACK_PARAM>(this)

//====================================================================================================
// Class Declarations
//====================================================================================================

template <typename DelType>
class InputCallback
{
public:
    InputCallback(Input::Type type, DelType del)
        : type(type)
        , callback(del)
    {}

    Input::Type type;
    DelType callback;
};

//----------------------------------------------------------------------------------------------------

class InputManager
{
    typedef InputCallback<Delegate<bool>> ActionCallback;
    typedef InputCallback<Delegate<bool, s32>> AxisCallback;

public:
    void OnAction(const InputEvent& evt);
    void Update(const InputData& input);

    void BindAction(const u32 key, Input::Type type, Delegate<bool> callback)
    {
        mActionMap[key].push_back(ActionCallback(type, callback));
    }

    void BindAxis(const u32 key, Input::Type type, Delegate<bool, s32> callback)
    {
        mAxisMap[key].push_back(AxisCallback(type, callback));
    }

private:
    typedef std::vector<ActionCallback> ActionCallbacks;
    typedef std::vector<AxisCallback> AxisCallbacks;

    std::map<u32, ActionCallbacks> mActionMap;  // called when the corresponding input is fired
    std::map<u32, AxisCallbacks> mAxisMap;
};

#endif // #ifndef INCLUDED_ENGINE_INPUTMANAGER_H