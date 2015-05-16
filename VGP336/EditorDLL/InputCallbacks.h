#ifndef INCLUDED_INPUTCALLBACKS_H
#define INCLUDED_INPUTCALLBACKS_H

class EditorApp;

class InputCallbacks
{
public:
    InputCallbacks(EditorApp& owner);
    void RegisterCallbacks();

    bool OnSelectObject();
    bool OnZoom();

    // Input Axis events
    bool OnPanCamera(s32 val);
    bool OnCameraLook(s32 val);
    bool OnCamerWalkForward(s32 val);
    bool OnMouseDrag(s32 val);

private:
    EditorApp& mOwner;
};

#endif // #ifndef INCLUDED_INPUTCALLBACKS_H