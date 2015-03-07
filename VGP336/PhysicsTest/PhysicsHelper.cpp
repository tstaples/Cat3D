#include "PhysicsHelper.h"

#include <Engine.h>
#include "Random.h"

namespace PhysicsHelper
{
    const f32 kTimeStep = 1.0f / 60.0f;

    void AddTetrahydron(PhysicsWorld& physicsworld)
    {
        Particle* p0 = new Particle();
        Particle* p1 = new Particle();
        Particle* p2 = new Particle();
        Particle* p3 = new Particle();

        p0->SetPosition(-0.5f, 5.0f, 0.5f);
        p1->SetPosition(0.5f, 5.0f, 0.0f);
        p2->SetPosition(0.5f, 5.0f, 0.5f);
        p3->SetPosition(0.5f, 5.5f, 0.5f);

        p0->SetVelocity(Random::GetF(-1.0f, 1.0f) * kTimeStep, 
                        Random::GetF(1.0f, 2.0f)  * kTimeStep, 
                        Random::GetF(-1.0f, 1.0f) * kTimeStep);
        p0->radius = 0.05f;
        p1->radius = 0.05f;
        p2->radius = 0.05f;
        p3->radius = 0.05f;

        physicsworld.AddParticle(p0);
        physicsworld.AddParticle(p1);
        physicsworld.AddParticle(p2);
        physicsworld.AddParticle(p3);

        Spring* spring1 = new Spring(p0, p1);
        Spring* spring2 = new Spring(p1, p2);
        Spring* spring3 = new Spring(p2, p0);
        Spring* spring4 = new Spring(p0, p3);
        Spring* spring5 = new Spring(p1, p3);
        Spring* spring6 = new Spring(p2, p3);

        physicsworld.AddConstraint(spring1);
        physicsworld.AddConstraint(spring2);
        physicsworld.AddConstraint(spring3);
        physicsworld.AddConstraint(spring4);
        physicsworld.AddConstraint(spring5);
        physicsworld.AddConstraint(spring6);
    }

    void AddCube(PhysicsWorld& physicsworld)
    {
        Particle* p1 = new Particle();
        Particle* p2 = new Particle();
        Particle* p3 = new Particle();
        Particle* p4 = new Particle();

        Particle* p5 = new Particle();
        Particle* p6 = new Particle();
        Particle* p7 = new Particle();
        Particle* p8 = new Particle();

        p1->SetPosition(-0.5f, 5.0f, 0.5f);     // top left
        p2->SetPosition(0.5f, 5.0f, 0.5f);      // top right
        p3->SetPosition(0.5f, 5.0f, -0.5f);     // bottom right
        p4->SetPosition(-0.5f, 5.0f, -0.5f);    // bottom left

        p5->SetPosition(-0.5f, 6.0f, 0.5f);
        p6->SetPosition(0.5f, 6.0f, 0.5f);
        p7->SetPosition(0.5f, 6.0f, -0.5f);
        p8->SetPosition(-0.5f, 6.0f, -0.5f);

        p1->SetVelocity(Random::GetF(-1.0f, 1.0f) * kTimeStep, 
                        Random::GetF(1.0f, 2.0f)  * kTimeStep, 
                        Random::GetF(-1.0f, 1.0f) * kTimeStep);

        p1->radius = 0.05f;
        p2->radius = 0.05f;
        p3->radius = 0.05f;
        p4->radius = 0.05f;
        p5->radius = 0.05f;
        p6->radius = 0.05f;
        p7->radius = 0.05f;
        p8->radius = 0.05f;

        physicsworld.AddParticle(p1);
        physicsworld.AddParticle(p2);
        physicsworld.AddParticle(p3);
        physicsworld.AddParticle(p4);
        physicsworld.AddParticle(p5);
        physicsworld.AddParticle(p6);
        physicsworld.AddParticle(p7);
        physicsworld.AddParticle(p8);
        
        std::vector<Spring*> springs;
        // Bottom face
        springs.push_back(new Spring(p1, p2));
        springs.push_back(new Spring(p2, p3));
        springs.push_back(new Spring(p3, p4));
        springs.push_back(new Spring(p4, p1));
        springs.push_back(new Spring(p2, p4));
        springs.push_back(new Spring(p1, p3));

        // Top face
        springs.push_back(new Spring(p5, p6));
        springs.push_back(new Spring(p6, p7));
        springs.push_back(new Spring(p7, p8));
        springs.push_back(new Spring(p8, p5));
        springs.push_back(new Spring(p5, p7));
        springs.push_back(new Spring(p6, p8));

        // Connecting top to bottom
        springs.push_back(new Spring(p1, p5));
        springs.push_back(new Spring(p2, p6));
        springs.push_back(new Spring(p3, p7));
        springs.push_back(new Spring(p4, p8));

        // Cross side faces
        springs.push_back(new Spring(p1, p6));
        springs.push_back(new Spring(p2, p5));
        springs.push_back(new Spring(p2, p7));
        springs.push_back(new Spring(p3, p6));

        // Cross side faces
        springs.push_back(new Spring(p3, p8));
        springs.push_back(new Spring(p4, p7));
        springs.push_back(new Spring(p4, p5));
        springs.push_back(new Spring(p1, p8));

        for (auto spring : springs)
        {
            physicsworld.AddConstraint(spring);
        }
    }

    typedef std::vector<Particle*> Particles;

    void AddMesh(PhysicsWorld& physicsworld)
    {
        f32 x = 0;
        f32 y = 0;

        std::vector<Particles> particles(4);
        for (u32 i=0; i < 4; ++i)
        {
            particles[i].resize(4);
            for (u32 j=0; j < 4; ++j)
            {
                particles[i][j] = new Particle(x, y, 0.0f, 0.05f, 0.0f);
                physicsworld.AddParticle(particles[i][j]);
                x += 0.5f;

                //if (j > 0)
                //{
                //    Spring* spring = new Spring(particles[i][j-1], particles[i][j]);
                //    physicsworld.AddConstraint(spring);
                //}
            }
            y += 0.5f;
            x = 0.0f;
        }
    }
}