#ifndef INCLUDED_ENGINE_OCTREE_H
#define INCLUDED_ENGINE_OCTREE_H

#include "EngineMath.h"

class Octree
{
public:
    typedef std::vector<Math::AABB> Objects; // Temp until able to access transform from gameobject
    //typedef std::vector<Octree> Children;

public:
    static const u16 kNumChildren = 8;
    static const u16 kMinSize = 1;  // Smallest is 1x1x1 cube
    
    Octree(const Math::AABB& region, s32 maxDepth=10);
    Octree(const Math::AABB& region, const Math::AABB& point, s32 maxDepth=10);
    Octree(const Math::AABB& region, const Objects& objects, s32 maxDepth=10);
    ~Octree();

    void Insert(const Math::AABB& point);
    void Destroy();

    // Returns true if any objects are found intersecting with the ray, and are stored objects vector
    bool GetIntersectingObjects(const Math::Ray& ray, std::vector<Math::AABB>& objects);

    const Math::AABB& GetBoundingBox() const { return mAABB; }

private:
    void Insert(const Math::AABB& point, s32 depth);
    bool IsLeaf();
    bool GetIntersectingObjects(const Math::Ray& ray, std::vector<Math::AABB>& objects, s32 depth);

private:
    Math::AABB mAABB;   // Bounding volume
    s32 mMaxDepth;       // Max recursion depth TODO: set based on total objects

    Objects mObjects;    // Objects in this volume

    Octree* mpParent;

    /*
	    Children follow a predictable pattern to make accesses simple.
	    Here, - means less than 'origin' in that dimension, + means greater than.
	    child:	0 1 2 3 4 5 6 7
	    x:      - - - - + + + +
	    y:      - - + + - - + +
	    z:      - + - + - + - +
	*/
    Octree* mpChildren[kNumChildren];
    u8 mActiveChildren; // Bitmask indicating which children are active
};

#endif // #ifndef INCLUDED_ENGINE_OCTTREE_H