#include "TestApp.h"

void DrawSkeleton(const Transforms& toRootTransforms, const Model& model);

TestApp::TestApp()
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

	mInputSystem.Initialize(mWindow.GetWindowHandle());
	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);

	mCamera.Setup(Math::kPiByTwo, (f32)width / (f32)height, 0.01f, 10000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 0.0f, -10.0f));
    mCamera.SetLookAt(Math::Vector3(0.0f, 0.0f, 0.0f));
	mTimer.Initialize();

	mRenderer.Initialize(mGraphicsSystem);

    mAssetLoader.Initialize(mGraphicsSystem);
    mAssetLoader.LoadModel(L"../Data/Stuff/soldierv2.catm", mModel);

    mAnimationController.Initialize(mModel);
    AnimationClip tempclip;
    mAnimationController.StartClip(*mModel.mAnimations[0], true);
}

void TestApp::OnTerminate()
{
	mWindow.Terminate();
	mGraphicsSystem.Terminate();
	SimpleDraw::Terminate();
	mInputSystem.Terminate();
    mRenderer.Terminate();

    mModel.Unload();
    mAssetLoader.Terminate();
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
	const f32 kMoveSpeed = 10.0f;

	// Camera movement
	f32 mouseSensitivity = 0.25f;
	mCamera.Yaw((f32)(mInputSystem.GetMouseMoveX()) * mouseSensitivity);
	mCamera.Pitch((f32)(mInputSystem.GetMouseMoveY()) * mouseSensitivity);

	// Player movement
	if (mInputSystem.IsKeyDown(Keys::W))
		mCamera.Walk(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::A))
		mCamera.Strafe(-kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::D))
		mCamera.Strafe(kMoveSpeed * deltatime);
	else if (mInputSystem.IsKeyDown(Keys::S))
		mCamera.Walk(-kMoveSpeed * deltatime);

    
    const std::vector<Bone*>& bones = mModel.mBones;
    const Transforms& toRootTransforms = mAnimationController.ToRootTransforms(); 
    const Transforms& boneTransforms = mAnimationController.GetFinalTransforms();

    for (u32 i=0; i < mModel.mMeshes.size(); ++i)
    {
        Mesh* mesh = mModel.mMeshes[i];
        VertexWeights& vertexWeights = mesh->GetVertexWeights();
        if (!vertexWeights.empty())
        {
            const Mesh::Vertex* vertices = mesh->GetVertices();
            const u32 vertexCount = mesh->GetVertexCount();

            Mesh::Vertex* newVertices = new Mesh::Vertex[vertexCount];
            for (u32 j=0; j < vertexCount; ++j)
            {
                // Get the boneweights for this vertex
                const BoneWeights& boneWeights = vertexWeights[j];
                
                Math::Matrix transform = boneTransforms[boneWeights[0].boneIndex] * boneWeights[0].weight;
                for (u32 k=1; k < boneWeights.size(); ++k)
                {
                    transform += boneTransforms[boneWeights[k].boneIndex] * boneWeights[k].weight;
                }
                newVertices[j].position = Math::TransformCoord(vertices[j].position, transform);
                newVertices[j].normal = Math::TransformNormal(vertices[j].normal, transform);
                newVertices[j].texcoord = vertices[j].texcoord;
            }
            // Send the new vertex data to the GPU
            mModel.mMeshBuffers[i]->UpdateBuffer(mGraphicsSystem, newVertices, vertexCount);
            SafeDeleteArray(newVertices);
        }
    }

	// Render
	mGraphicsSystem.BeginRender();

	mRenderer.SetCamera(mCamera);

    DrawSkeleton(toRootTransforms, mModel);

    mModel.Render(mRenderer, Math::Matrix::Identity());
    
    SimpleDraw::Render(mCamera);

	mGraphicsSystem.EndRender();
}

void DrawSkeleton(const Transforms& toRootTransforms, const Model& model)
{
    for (u32 i=0; i < toRootTransforms.size(); ++i)
    {
        const Math::Matrix& transform = toRootTransforms[i];
        Math::Vector3 position = transform.GetTranslation();
        SimpleDraw::AddAABB(position, 0.02f, Color::Green());

        const Bone& bone = *model.mBones[i];
        for (u32 j=0; j < bone.children.size(); ++j)
        {
            const u32 childIndex = bone.childrenIndices[j];
            Math::Vector3 childPosition = toRootTransforms[childIndex].GetTranslation();
            SimpleDraw::AddLine(position, childPosition, Color::White());
        }
    }
}