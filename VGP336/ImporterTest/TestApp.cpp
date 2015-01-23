#include "TestApp.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	mTimer.Initialize();

    mTexture.Initialize(mGraphicsSystem, L"../Engine/Images/metalpanel.dds");
    mTexture.AddRef();

	mRenderer.Initialize(mGraphicsSystem);
    mRenderer.SetTexture(mTexture);

    AssetLoader::LoadModel("../Data/Models/duck.catm", mGraphicsSystem, mModel);
}

void TestApp::OnTerminate()
{
	mWindow.Terminate();
	mGraphicsSystem.Terminate();
	SimpleDraw::Terminate();
	mInputSystem.Terminate();
	
    mTexture.Terminate();
    mTexture.Release();
    
    mRenderer.Terminate();

    mModel.Destroy();
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

	// Render
	mGraphicsSystem.BeginRender();

	mRenderer.SetCamera(mCamera);
	for (u32 i = 0; i < (u32)mModel.mMeshBuffers.size(); ++i)
	{
		mRenderer.Render(*mModel.mMeshBuffers[i], Math::Matrix::Identity());
	}

	mGraphicsSystem.EndRender();
}


//void TestApp::LoadModel(const char* filename, Model& model)
//{
//	Assimp::Importer importer;
//	u32 flags =	  aiProcess_Triangulate
//				| aiProcess_JoinIdenticalVertices
//				| aiProcess_SortByPType
//				| aiProcess_FlipUVs;
//
//	const aiScene* scene = importer.ReadFile(filename, flags);
//	ASSERT(scene, "Failed to load model - %s", importer.GetErrorString());
//
//	if(scene->HasMeshes())
//	{
//		for(u32 i = 0; i < scene->mNumMeshes; ++i)
//		{
//			aiMesh* aimesh = scene->mMeshes[i];
//			Mesh* mesh = new Mesh();
//
//			Mesh::Vertex* vertices = new Mesh::Vertex[aimesh->mNumVertices];
//			Mesh::Vertex* vertexIter = vertices;
//
//			// Copy positions
//			for(u32 j = 0; j < aimesh->mNumVertices; ++j)
//			{
//				// Hack: scale down mesh for now.
//				// TODO: make scaling an optional param
//				vertexIter->position.x = aimesh->mVertices[j].x * 0.1f;
//				vertexIter->position.y = aimesh->mVertices[j].y * 0.1f;
//				vertexIter->position.z = aimesh->mVertices[j].z * 0.1f;
//				++vertexIter;
//			}
//
//			// Copy normals if it has any
//			if(aimesh->HasNormals())
//			{
//				vertexIter = vertices;
//				for(u32 j = 0; j < aimesh->mNumVertices; ++j)
//				{
//					vertexIter->normal.x = aimesh->mNormals[j].x;
//					vertexIter->normal.y = aimesh->mNormals[j].y;
//					vertexIter->normal.z = aimesh->mNormals[j].z;
//					++vertexIter;
//				}
//			}
//
//			u16* indices = new u16[aimesh->mNumFaces * 3];
//			u16* indexIter = indices;
//			for(u32 j = 0; j < aimesh->mNumFaces; ++j)
//			{
//				indexIter[0] = aimesh->mFaces[j].mIndices[0];
//				indexIter[1] = aimesh->mFaces[j].mIndices[1];
//				indexIter[2] = aimesh->mFaces[j].mIndices[2];
//				indexIter += 3;
//			}
//
//			MeshBuilder::GenerateMesh(*mesh, vertices, aimesh->mNumVertices, indices, aimesh->mNumFaces * 3);
//			mModel.mMeshes.push_back(mesh);
//
//			SafeDeleteArray(vertices);
//			SafeDeleteArray(indices);
//
//			MeshBuffer* meshBuffer = new MeshBuffer();
//			meshBuffer->Initialize(mGraphicsSystem, Mesh::GetVertexFormat(), *mesh);
//			mModel.mMeshBuffers.push_back(meshBuffer);
//		}
//	}
//}