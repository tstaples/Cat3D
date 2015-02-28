#ifndef INCLUDED_ENGINE_PHYSICSWORLD_H
#define INCLUDED_ENGINE_PHYSICSWORLD_H

//====================================================================================================
// Filename:	PhysicsWorld.h
// Created by:	Tyler Staples
// Description:	Class for simulating a physics world.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Particle;

//====================================================================================================
// Class Declarations
//====================================================================================================

class PhysicsWorld
{
public:
    PhysicsWorld(const Math::Vector3& gravity, f32 timestep);
    ~PhysicsWorld();

    void StepSimulation(f32 deltatime);

    void AddParticle(Particle* p);
    void Purge();

    void Render();

private:
    void AccumulateForces();
    void Integrate(f32 deltatime);
    void SatisfyConstraints();

private:
    std::vector<Particle*> mParticles;

    Math::Vector3 mGravity;

    f32 mTimer;
    f32 mTimeStep;  // Fixed interval
};

#endif // #ifndef INCLUDED_ENGINE_PHYSICSWORLD_H