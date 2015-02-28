//====================================================================================================
// Filename:	Constraints.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "Constraints.h"

#include "Color.h"
#include "Particle.h"
#include "SimpleDraw.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Spring::Spring(Particle* a, Particle* b, f32 restLength)
    : mpParticleA(a)
    , mpParticleB(b)
    , mRestLength(restLength)
{
    if (mRestLength <= 0.0f)
    {
        mRestLength = Math::Distance(a->pos, b->pos);
    }
}

//----------------------------------------------------------------------------------------------------

void Spring::Apply()
{
    Math::Vector3 delta = mpParticleB->pos - mpParticleA->pos;
    f32 dist = Math::Magnitude(delta);
    f32 diff = (dist - mRestLength) / (dist * (mpParticleA->invMass + mpParticleB->invMass));
    mpParticleA->pos += delta * diff * mpParticleA->invMass;
    mpParticleB->pos -= delta * diff * mpParticleB->invMass;
}

//----------------------------------------------------------------------------------------------------

void Spring::Render()
{
    SimpleDraw::AddLine(mpParticleA->pos, mpParticleB->pos, Color::Blue());
}