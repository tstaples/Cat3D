#ifndef INCLUDED_ENGINE_CONSTRAINTS_H
#define INCLUDED_ENGINE_CONSTRAINTS_H

//====================================================================================================
// Filename:	Constraints.h
// Created by:	Tyler Staples
// Description:	Header containing various particle constraints.
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

class Constraint
{
public:
    virtual ~Constraint() {}

    virtual void Apply() = 0;
    virtual void Render() {}
};

class Spring : public Constraint
{
public:
    // Rest length default as -1 to set current distance between a and b as rest length
    Spring(Particle* a, Particle* b, f32 restLength = -1.0f);

    // Implements Constraint
    virtual void Apply();
    virtual void Render();

private:
    // Weak ptr
    Particle* mpParticleA;
    Particle* mpParticleB;

    f32 mRestLength;
};

#endif // #ifndef INCLUDED_ENGINE_CONSTRAINTS_H