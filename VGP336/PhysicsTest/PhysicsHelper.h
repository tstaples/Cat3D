#ifndef INCLUDED_PHYSICSHELPER_H
#define INCLUDED_PHYSICSHELPER_H

class PhysicsWorld;

namespace PhysicsHelper
{
    void AddTetrahydron(PhysicsWorld& physicsworld);
    void AddCube(PhysicsWorld& physicsworld);
    void AddMesh(PhysicsWorld& physicsworld);
}

#endif // #ifndef INCLUDED_ENGINE_PHYSICSHELPER_H