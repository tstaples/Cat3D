#ifndef INCLUDED_ENGINE_OCTREE_H
#define INCLUDED_ENGINE_OCTREE_H

#include "EngineMath.h"

class Octree
{
public:
    typedef std::vector<Math::Vector3> Points; // Temp until able to access transform from gameobject
    //typedef std::vector<Octree> Children;

public:
    static const u16 kNumChildren = 8;
    static const u16 kMinSize = 1;  // Smallest is 1x1x1 cube
    
    Octree();
    Octree(const Math::AABB& region);
    Octree(const Math::AABB& region, const Math::Vector3& point);
    Octree(const Math::AABB& region, const Points& objects);
    ~Octree();

    void Insert(const Math::Vector3& point);
    void Destroy();

    const Math::AABB& GetBoundingBox() const { return mAABB; }

private:
    //void BuildTree();
    bool IsLeaf();

private:
    Math::AABB mAABB; // Bounding volume

    Points mObjects; // Objects in this volume

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