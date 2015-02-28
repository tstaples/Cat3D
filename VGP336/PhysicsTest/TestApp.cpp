#include "TestApp.h"

#include "Random.h"

namespace
{
	const f32 kMoveSpeed = 10.0f;
    const f32 kTimeStep = 1.0f / 60.0f;

    void DrawGroundPlane()
    {
        const s32 groundSize = 25;
        for (s32 x = -groundSize; x <= groundSize; ++x)
        {
            Math::Vector3   a((f32)x, 0.0f, (f32)-groundSize),
                            b((f32)x, 0.0f, (f32)groundSize);
            SimpleDraw::AddLine(a, b, Color::White());
        }
        for (s32 z = -groundSize; z <= groundSize; ++z)
        {
            Math::Vector3   a((f32)-groundSize, 0.0f, (f32)z),
                            b((f32)groundSize, 0.0f, (f32)z);
            SimpleDraw::AddLine(a, b, Color::White());
        }
    }
}

TestApp::TestApp()
: mPhysicsWorld(Math::Vector3(0.0f, -10.0f, 0.0f), kTimeStep)
{
}

TestApp::~TestApp()
{
}

void TestApp::OnInitialize(u32 width, u32 height)
{
	// Init the window
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mTimer.Initialize();
	mInputSystem.Initialize(mWindow.GetWindowHandle());

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem, 1000000);

	mCamera.Setup(Math::kPi / 6.0f, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));
}

void TestApp::OnTerminate()
{
	mInputSystem.Terminate();
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

bool TestApp::OnInput(const InputEvent& evt)
{
	bool handled = false;
	if (mInputSystem.IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
		handled = true;
	}
	return handled;
}

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
		return;
	}

	mTimer.Update();
	mInputSystem.Update();
	const f32 deltatime = mTimer.GetElapsedTime();

    UpdateCamera(deltatime);

    // Spawn particles
    if (mInputSystem.IsKeyPressed(Keys::SPACE))
    {
        mPhysicsWorld.Purge();
        //for (u32 i=0; i < 1000; ++i)
        //{
        //    Particle* p = new Particle(0.0f, 1.0f, 0.0f, 0.01f, 1.0f);
        //    p->SetVelocity( Random::GetF(-1.0f, 1.0f) * kTimeStep, 
        //                    Random::GetF(1.0f, 2.0f)  * kTimeStep, 
        //                    Random::GetF(-1.0f, 1.0f) * kTimeStep);
        //    mPhysicsWorld.AddParticle(p);
        //}

        for (u32 i=0; i < 1; ++i)
        {
            Particle* p0 = new Particle();
            Particle* p1 = new Particle();
            Particle* p2 = new Particle();
            Particle* p3 = new Particle();

            p0->SetPosition(0.0f, 5.0f, 0.0f);
            p1->SetPosition(1.0f, 4.5f, 0.0f);
            p2->SetPosition(0.0f, 5.0f, 1.0f);
            p3->SetPosition(1.0f, 4.5f, 0.0f);

            p0->SetVelocity(Random::GetF(-1.0f, 1.0f) * kTimeStep, 
                            Random::GetF(1.0f, 2.0f)  * kTimeStep, 
                            Random::GetF(-1.0f, 1.0f) * kTimeStep);
            p0->radius = 0.05f;
            p1->radius = 0.05f;
            p2->radius = 0.05f;
            p3->radius = 0.05f;

            mPhysicsWorld.AddParticle(p0);
            mPhysicsWorld.AddParticle(p1);
            mPhysicsWorld.AddParticle(p2);
            mPhysicsWorld.AddParticle(p3);

            Spring* spring1 = new Spring(p0, p1);
            Spring* spring2 = new Spring(p1, p2);
            Spring* spring3 = new Spring(p2, p3);
            Spring* spring4 = new Spring(p3, p2);

            mPhysicsWorld.AddConstraint(spring1);
            mPhysicsWorld.AddConstraint(spring2);
            mPhysicsWorld.AddConstraint(spring3);
            mPhysicsWorld.AddConstraint(spring4);
        }
    }

    mPhysicsWorld.StepSimulation(deltatime);

	// Render
	mGraphicsSystem.BeginRender();

    DrawGroundPlane();

    mPhysicsWorld.Render();

	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::XAxis(), Color::Red());
	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::YAxis(), Color::Green());
	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::ZAxis(), Color::Blue());
    SimpleDraw::AddAABB(Math::Vector3::Zero(), 5.0f, Color::Blue());

	SimpleDraw::Render(mCamera);

	mGraphicsSystem.EndRender();
}

void TestApp::UpdateCamera(f32 deltatime)
{
    // Camera movement
	f32 mouseSensitivity = 0.25f;
	mCamera.Yaw((f32)(mInputSystem.GetMouseMoveX()) * mouseSensitivity);
	mCamera.Pitch((f32)(mInputSystem.GetMouseMoveY()) * mouseSensitivity);

	// Camera movement
	if (mInputSystem.IsKeyDown(Keys::W))
		mCamera.Walk(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::A))
		mCamera.Strafe(-kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::D))
		mCamera.Strafe(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::S))
		mCamera.Walk(-kMoveSpeed * deltatime);
}