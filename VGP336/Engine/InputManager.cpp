#include "Precompiled.h"

#include "InputManager.h"
#include "InputEvent.h"
#include "InputData.h"

void InputManager::OnAction(const InputEvent& evt)
{
    // See if this input is bound
    auto actionIter = mActionMap.find(evt.value);
    if (actionIter != mActionMap.end())
    {
        // Iterate through all registered callbacks
        for (auto callbackIter : actionIter->second)
        {
            // Ensure the input is the correct type
            Input::Type type = callbackIter.type;
            if (evt.type == type)
            {
                // If the input is handled by this callback, break out
                if (callbackIter.callback())
                {
                    break;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void InputManager::Update(const InputData& input)
{
    // Iter through all bound keys
    for (auto it : mAxisMap)
    {
        // Iter through all corresponding callbacks for this key
        for (auto callbackIter : it.second)
        {
            // Look up the corrext state based on the type
            const u32 key = it.first;
            const Input::Type type = callbackIter.type;
            // Entries in Input::Type enum above 2 are mouse related.
            bool state = (type > 2) ? input.mouseStates[key] : input.keyStates[key];
        
            // If the callback handles the input then move on to the next key
            if (callbackIter.callback((s32)state))
            {
                break;
            }
        }
    }
}