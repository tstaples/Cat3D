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
class Constraint;

//====================================================================================================
// Struct Declarations
//====================================================================================================

struct PhysicsSettings
{
    PhysicsSettings(const Math::Vector3& gravity, f32 timestep, f32 drag, u32 resCount)
        : gravity(gravity)
        , drag(drag)
        , timeStep(timestep)
        , resolutionCount(resCount)
    {}

    PhysicsSettings()
        : gravity(0.0f, -9.8f, 0.0f)
        , drag(0.0f)
        , timeStep(1.0f / 60.0f)
        , resolutionCount(1)
    {}

    Math::Vector3 gravity;
    f32 drag;
    f32 timeStep;  // Fixed interval
    u32 resolutionCount;
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class PhysicsWorld
{
public:
    PhysicsWorld();
    PhysicsWorld(const PhysicsSettings& settings);
    ~PhysicsWorld();

    void Setup(const PhysicsSettings& settings) { mSettings = settings; }
    void StepSimulation(f32 deltatime);

    void AddConstraint(Constraint* c);
    void AddParticle(Particle* p);
    void AddOBB(const Math::OBB& obb);

    void Purge();

    void Render();

    f32 GetTimeStep() const { return mSettings.timeStep; }

private:
    void AccumulateForces();
    void Integrate();
    void SatisfyConstraints();

private:
    std::vector<Particle*> mParticles;
    std::vector<Constraint*> mConstraints;
    std::vector<Math::OBB> mOBBs;

    PhysicsSettings mSettings;
    f32 mTimer;
};

#endif // #ifndef INCLUDED_ENGINE_PHYSICSWORLD_H