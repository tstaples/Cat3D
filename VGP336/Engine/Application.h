#ifndef INCLUDED_ENGINE_APPLICATION_H
#define INCLUDED_ENGINE_APPLICATION_H

//====================================================================================================
// Filename:	Application.h
// Created by:	Peter Chan
// Description:	Class for an application.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct InputEvent;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Application
{
public:
	Application();
	virtual ~Application();

	void Initialize(HINSTANCE instance, LPCSTR appName, u32 width, u32 height);
	void Terminate();
	
	void HookWindow(HWND hWnd, bool owner=true);
	void UnhookWindow();

	void Update();
	
	bool IsRunning() const			{ return mRunning; }
    bool IsOwner() const            { return mOwner; }

    u32 GetScreenWidth() const      { return mWidth; }
    u32 GetScreenHeight() const     { return mHeight; }

    void ForwardInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HINSTANCE GetInstance() const	{ return mInstance; }
	HWND GetWindow() const			{ return mWindow; }
	const char* GetAppName() const	{ return mAppName.c_str(); }
	
	bool mRunning;

    u32 mWidth;
    u32 mHeight;

private:
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool ProcessInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, InputEvent& evt);

	virtual void OnInitialize(u32 width, u32 height) {}
	virtual void OnTerminate() {}
	virtual bool OnInput(const InputEvent& evt) { return false; }
	virtual void OnUpdate() {}
    virtual void OnResizeWindow() {}

	HINSTANCE mInstance;
	HWND mWindow;
    bool mOwner; // Flag representing if we own the window

	std::string mAppName;
};

#endif // #ifndef INCLUDED_ENGINE_APPLICATION_H