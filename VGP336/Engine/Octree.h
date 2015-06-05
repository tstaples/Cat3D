#ifndef INCLUDED_ENGINE_OCTREE_H
#define INCLUDED_ENGINE_OCTREE_H

#include "EngineMath.h"
#include "GameObject.h"

template <typename T>
class Octree
{
public:
    typedef std::map<T*, Math::AABB> Objects;

public:
    static const u16 kNumChildren = 8;
    static const u16 kMinSize = 1;  // Smallest is 1x1x1 cube
    
    Octree(const Math::AABB& octRegion, u32 maxElements = 1, s32 maxDepth=10);
    Octree(const Math::AABB& octRegion, T& object, const Math::AABB& objRegion, u32 maxElements = 1, s32 maxDepth=10);
    Octree(const Math::AABB& octRegion, Objects& objects, u32 maxElements = 1, s32 maxDepth=10);
    ~Octree();

    void Insert(T& object, const Math::AABB& region);
    void Destroy();

    // Returns true if any objects are found intersecting with the ray, and are stored objects vector
    bool GetIntersectingObjects(const Math::Ray& ray, std::vector<T*>& objects);
    // Returns true if any objects in the tree have intersecting regions.
    bool GetIntersectingObjects(std::vector<T*>& objects);

    // TODO: update child oct bounds when region is set
    void SetBoundingRegion(const Math::AABB& region);
    const Math::AABB& GetBoundingBox() const    { return mAABB; }
    void SetMaxDepth(s32 depth)                 { mMaxDepth = depth; }

    void Debug_DrawTree();

private:
    void Insert(T& object, const Math::AABB& region, s32 depth);
    bool IsLeaf();
    bool GetIntersectingObjects(const Math::Ray& ray, std::vector<T*>& objects, s32 depth);
    bool GetIntersectingObjects(std::vector<T*>& objects, s32 depth);

private:
    Math::AABB mAABB;   // Bounding volume
    s32 mMaxDepth;      // Max recursion depth TODO: set based on total objects
    u32 mMaxElements;   // Max number of elements a leaf node can contain

    Objects mObjects;   // Objects in this volume

    Octree* mpParent;
    Octree* mpChildren[kNumChildren];
    u8 mActiveChildren; // Bitmask indicating which children are active
};

#include "Octree.inl"

#endif // #ifndef INCLUDED_ENGINE_OCTTREE_H