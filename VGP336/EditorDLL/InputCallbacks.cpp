#include "Precompiled.h"

#include <Engine.h>

#include "EditorApp.h"
#include "InputCallbacks.h"
#include "Gizmo.h"

InputCallbacks::InputCallbacks(EditorApp& owner)
    : mOwner(owner)
{
}

//----------------------------------------------------------------------------------------------------

void InputCallbacks::RegisterCallbacks()
{
    // Bind controls
    InputManager& inputManager = mOwner.mInputManager;
    inputManager.BindAction(Mouse::LBUTTON, Input::MouseDown, MAKE_ACTION_DELEGATE(InputCallbacks, &InputCallbacks::OnSelectObject));
    inputManager.BindAction(Mouse::SCROLL, Input::MouseScroll, MAKE_ACTION_DELEGATE(InputCallbacks, &InputCallbacks::OnZoom));

    inputManager.BindAxis(Mouse::LBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(InputCallbacks, &InputCallbacks::OnMouseDrag));
    inputManager.BindAxis(Mouse::RBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(InputCallbacks, &InputCallbacks::OnCameraLook));
    inputManager.BindAxis(Mouse::MBUTTON, Input::MouseDown, MAKE_AXIS_DELEGATE(InputCallbacks, &InputCallbacks::OnPanCamera));
}

//----------------------------------------------------------------------------------------------------

bool InputCallbacks::OnCameraLook(s32 val)
{
    if (val == 0)
    {
        return false;
    }

    // TODO: Smoothing by interpolating between desired angle and current
    // (reference steering behaviours)
	const f32 lookSensitivity = 0.25f;

    InputData& input = mOwner.mInputData;
    mOwner.mCamera.Yaw(input.mouseMoveX * lookSensitivity);
    mOwner.mCamera.Pitch(input.mouseMoveY * lookSensitivity);

    // Zero out mouse move so the camera doesn't continue to rotate
    input.mouseMoveX = 0.0f;
    input.mouseMoveY = 0.0f;

    return true;
}

//----------------------------------------------------------------------------------------------------

bool InputCallbacks::OnZoom()
{
    const f32 zoomDistance = 2.0f;
    s8 delta = mOwner.mInputData.mouseScrollDelta;
    if (delta == WHEEL_SCROLL_UP)
    {
        mOwner.mCamera.Walk(zoomDistance);
    }
    else if (delta == WHEEL_SCROLL_DOWN)
    {
        mOwner.mCamera.Walk(-zoomDistance);
    }
    // Reset to prevent infinite zoom
    delta = 0;

    return true;
}

//----------------------------------------------------------------------------------------------------

bool InputCallbacks::OnPanCamera(s32 val)
{
    if (val == 0)
    {
        return false;
    }

    const f32 moveSensitivity = 0.25f;
    InputData& input = mOwner.mInputData;
    mOwner.mCamera.Strafe((-input.mouseMoveX) * moveSensitivity);
    mOwner.mCamera.Rise(input.mouseMoveY * moveSensitivity);

    input.mouseMoveX = 0.0f;
    input.mouseMoveY = 0.0f;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool InputCallbacks::OnSelectObject()
{
    bool handled = true;
    InputData& input = mOwner.mInputData;
    std::vector<EditorObject*>& selectedObjs = mOwner.mSelectedObjects;

    Math::Ray ray = mOwner.mCamera.GetMouseRay(input.mouseX, input.mouseY, mOwner.mWidth, mOwner.mHeight);
    if (!mOwner.mpGizmo->IsSelected(selectedObjs, ray))
    {
        // De-select the objects and remove any existing objects
        // TODO: check if shift or w/e is pressed for multiple selection
        for (auto object : selectedObjs)
        {
            object->DeSelect();
        }
        selectedObjs.clear();

        // Convert the mouse click into a ray cast in world space and test collision
        if (!mOwner.mOctree.GetIntersectingObjects(ray, selectedObjs))
        {
            return handled;
        }
        
        // Invert the selection flag so previously selected objects are now de-selected
        std::vector<EditorObject*>::iterator it = selectedObjs.begin();
        for (it; it != selectedObjs.end(); ++it)
        {
            EditorObject* object = *it;

            // Select the object if it hasn't been already
            if (!object->IsSelected())
            {
                object->Select();
            }
        }
    }
    return handled;
}

//----------------------------------------------------------------------------------------------------

bool InputCallbacks::OnMouseDrag(s32 val)
{
    if (val == 0)
    {
        return false;
    }
    mOwner.mpGizmo->Update(mOwner.mSelectedObjects, mOwner.mInputData, mOwner.mWidth, mOwner.mHeight);
    return true;
}