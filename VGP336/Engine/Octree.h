#ifndef INCLUDED_ENGINE_OCTREE_H
#define INCLUDED_ENGINE_OCTREE_H

#include "EngineMath.h"
#include "GameObject.h"
#include "RepositoryTypes.h"

class Octree
{
public:
    typedef std::vector<GameObject> GameObjects;
    typedef std::vector<Octree> Children;

public:
    static const u16 kNumChildren = 8;
    static const u16 kMinSize = 1;  // Smallest is 1x1x1 cube
    
    Octree(TransformRepository& transformRepo);
    Octree(TransformRepository& transformRepo, const Math::AABB& region);
    Octree(TransformRepository& transformRepo, const Math::AABB& region, const GameObjects& objects);
    ~Octree();

    const Math::AABB& GetBoundingBox() const { return mAABB; }

private:
    void BuildTree();
    void CreateNode(const Math::AABB& region, const GameObjects& objs);
    bool IsRegionValid(const Math::AABB& region);

private:
    Math::AABB mAABB; // Bounding volume

    GameObjects mObjects; // Objects in this volume
    TransformRepository& mTransformRepository;

    Octree* mpParent;

    Children mChildren;
    u8 mActiveChildren; // Bitmask indicating which children are active
};

#endif // #ifndef INCLUDED_ENGINE_OCTTREE_H