//====================================================================================================
// Filename:	PhysicsWorld.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "PhysicsWorld.h"

#include "Constraints.h"
#include "Color.h"
#include "Particle.h"
#include "SimpleDraw.h"

//====================================================================================================
// local Definitions
//====================================================================================================

namespace
{
    void DrawOBB(const Math::OBB& obb)
    {
        // Build the transform matrix to bring the local space points into the world
        Math::Matrix matTrans = Math::Matrix::Translation(obb.center);
        Math::Matrix matRot = Math::Convert(obb.rot);
        Math::Matrix toWorld = matRot * matTrans;

        std::vector<Math::Vector3> points;

        // Create 8 points in the OBB's local space
        f32 y = obb.extend.y / 2.0f;
        points.push_back(Math::Vector3(-obb.extend.x, -y, -obb.extend.z)); // top left
        points.push_back(Math::Vector3(obb.extend.x,  -y, -obb.extend.z)); // top right
        points.push_back(Math::Vector3(obb.extend.x,  -y, obb.extend.z));  // bottom right
        points.push_back(Math::Vector3(-obb.extend.x, -y, obb.extend.z));  // bottom left

        y *= 2;
        points.push_back(Math::Vector3(-obb.extend.x, y, -obb.extend.z));
        points.push_back(Math::Vector3(obb.extend.x,  y, -obb.extend.z));
        points.push_back(Math::Vector3(obb.extend.x,  y, obb.extend.z));
        points.push_back(Math::Vector3(-obb.extend.x, y, obb.extend.z));

        // Transform the points into world space
        for (u32 i=0; i < points.size(); ++i)
        {
            points[i] = Math::TransformCoord(points[i], toWorld);
        }

        // Draw the OBB

        // Bottom face
        SimpleDraw::AddLine(points[0], points[1], Color::Red());
        SimpleDraw::AddLine(points[1], points[2], Color::Red());
        SimpleDraw::AddLine(points[2], points[3], Color::Red());
        SimpleDraw::AddLine(points[3], points[0], Color::Red());

        // Top face
        SimpleDraw::AddLine(points[4], points[5], Color::Red());
        SimpleDraw::AddLine(points[5], points[6], Color::Red());
        SimpleDraw::AddLine(points[6], points[7], Color::Red());
        SimpleDraw::AddLine(points[7], points[4], Color::Red());

        // Connect bottom to top
        SimpleDraw::AddLine(points[0], points[4], Color::Red());
        SimpleDraw::AddLine(points[1], points[5], Color::Red());
        SimpleDraw::AddLine(points[2], points[6], Color::Red());
        SimpleDraw::AddLine(points[3], points[7], Color::Red());
    }
}

//====================================================================================================
// Class Definitions
//====================================================================================================

PhysicsWorld::PhysicsWorld()
    : mTimer(0.0f)
{
}

PhysicsWorld::PhysicsWorld(const PhysicsSettings& settings)
    : mSettings(settings)
    , mTimer(0.0f)
{
}

//----------------------------------------------------------------------------------------------------

PhysicsWorld::~PhysicsWorld()
{
    Purge();
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::StepSimulation(f32 deltatime)
{
    // Check if enough time has passed
    mTimer += deltatime;
    if (mTimer >= mSettings.timeStep)
    {
        AccumulateForces();
        Integrate();   // verlet
        SatisfyConstraints();

        // Reset timer
        mTimer -= mSettings.timeStep;
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::AddConstraint(Constraint* c)
{
    mConstraints.push_back(c);
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::AddParticle(Particle* p)
{
    mParticles.push_back(p);
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::AddOBB(const Math::OBB& obb)
{
    mOBBs.push_back(obb);
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::Purge()
{
    for (u32 i=0; i < mParticles.size(); ++i)
    {
        SafeDelete(mParticles[i]);
    }
    mParticles.clear();

    for (u32 i=0; i < mConstraints.size(); ++i)
    {
        SafeDelete(mConstraints[i]);
    }
    mConstraints.clear();

    mOBBs.clear();
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::Render()
{
    for (auto particle : mParticles)
    {
        particle->Render();
    }

    for (auto constraint : mConstraints)
    {
        constraint->Render();
    }

    for (auto obb : mOBBs)
    {
        DrawOBB(obb);
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::AccumulateForces()
{
    const u32 numParticle = mParticles.size();
    for (u32 i=0; i < numParticle; ++i)
    {
        Particle* particle = mParticles[i];
        particle->accel = mSettings.gravity;
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::Integrate()
{
    const f32 kInvdrag = 1.0f - mSettings.drag;
    const f32 kTimeStep = Math::Sqr(mSettings.timeStep);
    const u32 numParticle = mParticles.size();
    for (u32 i=0; i < numParticle; ++i)
    {
        Particle* p = mParticles[i];

        // cache current position
        Math::Vector3 oldPos(p->pos);

        // verlet formula: p2 = 2p1 - p0 + at^2
        p->pos  = p->pos 
                + ((p->pos - p->posOld) * kInvdrag) 
                + (p->accel * kTimeStep);
        p->posOld = oldPos;
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::SatisfyConstraints()
{
    const u32 kNumParticles = mParticles.size();
    const u32 kNumOBBs = mOBBs.size();

    for (u32 n=0; n < mSettings.resolutionCount; ++n)
    {
        for (auto constraint : mConstraints)
        {
            constraint->Apply();
        }

        // Hack
        for (u32 i=0; i < kNumParticles; ++i)
        {
            Particle* p = mParticles[i];
            if (p->pos.y < 0.0f)
            {
                p->pos.y *= -1.0f;
                p->posOld.y *= -1.0f;
            }
        }
    }
}