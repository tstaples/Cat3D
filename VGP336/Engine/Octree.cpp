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
    std::vector<Math::AABB> BuildRegions(const Math::AABB& region)
    {
        const Math::Vector3 center(region.center);          // Center of this region
        const Math::Vector3 extend(region.extend / 2.0f);   // Half the size of the region

        std::vector<Math::AABB> octants(8);
        octants[0] = Math::AABB(center - extend, extend);
        octants[1] = Math::AABB(center.x - extend.x, center.y - extend.y, center.z + extend.z, extend);
        octants[2] = Math::AABB(center.x - extend.x, center.y + extend.y, center.z - extend.z, extend);
        octants[3] = Math::AABB(center.x - extend.x, center.y + extend.y, center.z + extend.z, extend);
        octants[4] = Math::AABB(center.x + extend.x, center.y - extend.y, center.z - extend.z, extend);
        octants[5] = Math::AABB(center.x + extend.x, center.y - extend.y, center.z + extend.z, extend);
        octants[6] = Math::AABB(center.x + extend.x, center.y + extend.y, center.z - extend.z, extend);
        octants[7] = Math::AABB(center.x + extend.x, center.y + extend.y, center.z + extend.z, extend);
        return octants;
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

//Octree::Octree(u8 maxDepth)
//    : mpParent(nullptr)
//    , mActiveChildren(0)
//    , mMaxDepth(maxDepth)
//{
//    for (u32 i=0; i < kNumChildren; ++i)
//    {
//        mpChildren[i] = nullptr;
//    }
//}

Octree::Octree(const Math::AABB& region, s32 maxDepth)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mMaxDepth(maxDepth)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

Octree::Octree(const Math::AABB& region, const Math::AABB& point, s32 maxDepth)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mMaxDepth(maxDepth)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
    mObjects.push_back(point);
}

Octree::Octree(const Math::AABB& region, const Objects& objects, s32 maxDepth)
    : mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mObjects(objects)
    , mMaxDepth(maxDepth)
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

void Octree::Insert(const Math::AABB& point)
{
    s32 depth = 0;
    Insert(point, depth);
}

void Octree::Insert(const Math::AABB& object, s32 depth)
{
    // Region cannot be split any further
    if (depth >= mMaxDepth || !IsRegionValid(mAABB, kMinSize))
    {
        // TODO: What happens with the point then?
        return;
    }

    if (IsLeaf())
    {
        mObjects.push_back(object);

        // Check if this node currently contains anything
        const u32 numObjects = mObjects.size();
        if (numObjects > 1)
        {
            // Split the region into octants
            std::vector<Math::AABB> octants = BuildRegions(mAABB);

            // We're only going to ever have 2 objects, but it may be modified in the future
            for (u32 i=0; i < numObjects; ++i)
            {
                // Find which octant the point belongs and and create a new child there
                const s32 index = GetOctantContainingPoint(mAABB.center, mObjects[i].center);
                mpChildren[index] = new Octree(octants[index], mObjects[i]);
                //mpChildren[index]->Insert(object, depth + 1);
                mActiveChildren |= index; // Set the flag for the new child
                break;
            }
        }
    }
    else
    {
        // Not a leaf, traverse recursivley
        const u32 index = GetOctantContainingPoint(mAABB.center, object.center);
        ASSERT(mpChildren[index] != nullptr, "[Octree] Invalid child index");
        mpChildren[index]->Insert(object);
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
        SafeDelete(mpChildren[i]);
    }
    mActiveChildren = 0;
}

bool Octree::GetIntersectingObjects(const Math::Ray& ray, std::vector<Math::AABB>& objects)
{
    // Early check to check if it's within the root region
    if (!Math::Intersect(ray, mAABB))
    {
        return false;
    }

    s32 depth = 0;
    return GetIntersectingObjects(ray, objects, depth);
}

bool Octree::GetIntersectingObjects(const Math::Ray& ray, std::vector<Math::AABB>& objects, s32 depth)
{
    if (depth >= mMaxDepth)
    {
        return false;
    }

    if (!IsLeaf())
    {
        // Find which child the ray intersects with
        for (u32 i=0; i < kNumChildren; ++i)
        {
            Octree* child = mpChildren[i];
            if (child != nullptr)
            {
                const Math::AABB& region = child->GetBoundingBox();
                if (Math::Intersect(ray, region))
                {
                    // Traverse that child's tree
                    return child->GetIntersectingObjects(ray, objects, depth + 1);
                }
            }
        }
    }
    else
    {
        // Check if we have any data
        const u32 numObjects = mObjects.size();
        if (numObjects == 0)
        {
            // No data to test against
            return false;
        }
        
        // See which objects the ray intersects with and add them to the list
        bool anyIntersect = false;
        for (u32 i=0; i < numObjects; ++i)
        {
            if (Math::Intersect(ray, mObjects[i]))
            {
                objects.push_back(mObjects[i]);
                anyIntersect = true;
                break;
            }
        }
        return anyIntersect; // May not have intersected with any objects
    }
    return false;
}