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
#include "Particle.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

PhysicsWorld::PhysicsWorld(const Math::Vector3& gravity, f32 timestep)
    : mGravity(gravity)
    , mTimeStep(timestep)
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
    if (mTimer >= mTimeStep)
    {
        AccumulateForces();
        Integrate(mTimeStep);   // verlet
        SatisfyConstraints();

        // Reset timer
        mTimer -= mTimeStep;
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
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::Render()
{
    for (auto particle : mParticles)
    {
        particle->Render();
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::AccumulateForces()
{
    const u32 numParticle = mParticles.size();
    for (u32 i=0; i < numParticle; ++i)
    {
        Particle* particle = mParticles[i];
        particle->accel = mGravity;
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::Integrate(f32 deltatime)
{
    const u32 numParticle = mParticles.size();
    for (u32 i=0; i < numParticle; ++i)
    {
        Particle* p = mParticles[i];

        // cache current position
        Math::Vector3 oldPos(p->pos);

        // verlet formula: p2 = 2p1 - p0 + at^2
        p->pos = (p->pos * 2.0f) - p->posOld + (p->accel * Math::Sqr(deltatime));
        p->posOld = oldPos;
    }
}

//----------------------------------------------------------------------------------------------------

void PhysicsWorld::SatisfyConstraints()
{
}