#include "Precompiled.h"
#include "Octree.h"

namespace
{
    /*
	    Children follow a predictable pattern to make accesses simple.
	    Here, - means less than 'origin' in that dimension, + means greater than.
	    child:	0 1 2 3 4 5 6 7
	    x:      - - - - + + + +
	    y:      - - + + - - + +
	    z:      - + - + - + - +
	*/
    void BuildRegions(const Math::AABB& region, Math::AABB* octants)
    {
        const Math::Vector3 center(region.center);          // Center of this region
        const Math::Vector3 extend(region.extend / 2.0f);   // Half the size of the region

        octants[0] = Math::AABB(center - extend, extend);
        octants[1] = Math::AABB(center.x - extend.x, center.y - extend.y, center.z + extend.z, extend);
        octants[2] = Math::AABB(center.x - extend.x, center.y + extend.y, center.z - extend.z, extend);
        octants[3] = Math::AABB(center.x - extend.x, center.y + extend.y, center.z + extend.z, extend);
        octants[4] = Math::AABB(center.x + extend.x, center.y - extend.y, center.z - extend.z, extend);
        octants[5] = Math::AABB(center.x + extend.x, center.y - extend.y, center.z + extend.z, extend);
        octants[6] = Math::AABB(center.x + extend.x, center.y + extend.y, center.z - extend.z, extend);
        octants[7] = Math::AABB(center.x + extend.x, center.y + extend.y, center.z + extend.z, extend);
    }

    u32 GetOctantContainingPoint(const Math::Vector3& origin, const Math::Vector3& point)
    {
        // How the child nodes are ordered allows us to get the index with 3 simple checks
		u32 oct = 0;
		if (point.x >= origin.x) oct |= 4;
		if (point.y >= origin.y) oct |= 2;
		if (point.z >= origin.z) oct |= 1;
		return oct;
	}

    bool IsRegionValid(const Math::AABB& region, u16 minSize)
    {
        return (region.extend.x > minSize &&
                region.extend.y > minSize &&
                region.extend.z > minSize);
    }
}

Octree::Octree()
    : mpParent(nullptr)
    , mActiveChildren(0)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

Octree::Octree(const Math::AABB& region)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

Octree::Octree(const Math::AABB& region, const Math::Vector3& point)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
    mObjects.push_back(point);
}

Octree::Octree(const Math::AABB& region, const Points& objects)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mObjects(objects)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

Octree::~Octree()
{
    Destroy();
}

void Octree::Insert(const Math::Vector3& point)
{
    // Region cannot be split any further
    if (!IsRegionValid(mAABB, kMinSize))
    {
        // TODO: What happens with the point then?
        return;
    }

    if (IsLeaf())
    {
        mObjects.push_back(point);

        // Check if this node currently contains anything
        const u32 numObjects = mObjects.size();
        if (numObjects > 1)
        {
            // Split the region into octants
            Math::AABB octants[kNumChildren];
            BuildRegions(mAABB, octants);

            // We're only going to ever have 2 objects, but it may be modified in the future
            for (u32 i=0; i < numObjects; ++i)
            {
                // Find which octant the point belongs and and create a new child there
                const s32 index = GetOctantContainingPoint(mAABB.center, mObjects[i]);
                mpChildren[index] = new Octree(octants[index], mObjects[i]);
                mActiveChildren |= index; // Set the flag for the new child
            }
        }
    }
    else
    {
        // Not a leaf, traverse recursivley
        const u32 index = GetOctantContainingPoint(mAABB.center, point);
        ASSERT(mpChildren[index] != nullptr, "[Octree] Invalid child index");
        mpChildren[index]->Insert(point);
    }
}

bool Octree::IsLeaf()
{
    // A leaf may contain 1 or 0 objects and has no children
    return (mActiveChildren == 0);
}

void Octree::Destroy()
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        delete mpChildren[i];
    }
}