#include "SimpleDraw.h"
#include "Color.h"

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
   inline  std::vector<Math::AABB> BuildRegions(const Math::AABB& region)
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

    inline u32 GetOctantContainingPoint(const Math::Vector3& origin, const Math::Vector3& point)
    {
        // How the child nodes are ordered allows us to get the index with 3 simple checks
		u32 oct = 0;
		if (point.x >= origin.x) oct |= 4;
		if (point.y >= origin.y) oct |= 2;
		if (point.z >= origin.z) oct |= 1;
		return oct;
	}

    inline bool IsRegionValid(const Math::AABB& region, u16 minSize)
    {
        return (region.extend.x > minSize &&
                region.extend.y > minSize &&
                region.extend.z > minSize);
    }
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline Octree<T>::Octree(const Math::AABB& octRegion, u32 maxElements, s32 maxDepth)
    : mAABB(octRegion)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mMaxDepth(maxDepth)
    , mMaxElements(maxElements)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline Octree<T>::Octree(const Math::AABB& octRegion, T& gameObject, const Math::AABB& objRegion, u32 maxElements, s32 maxDepth)
    : mAABB(octRegion)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mMaxDepth(maxDepth)
    , mMaxElements(maxElements)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
    mObjects.insert(std::make_pair(&gameObject, objRegion));
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline Octree<T>::Octree(const Math::AABB& octRegion, Objects& objects, u32 maxElements, s32 maxDepth)
    : mAABB(octRegion)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mObjects(objects)
    , mMaxDepth(maxDepth)
    , mMaxElements(maxElements)
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        mpChildren[i] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline Octree<T>::~Octree()
{
    ASSERT(mActiveChildren == 0 && mObjects.empty(), "[Ocree] Tree was not properly destroyed");
    //Destroy();
}

//----------------------------------------------------------------------------------------------------

//void Octree<T>::Insert(const Math::AABB& point)
template <typename T>
inline void Octree<T>::Insert(T& object, const Math::AABB& region)
{
    s32 depth = 0;
    Insert(object, region, depth);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void Octree<T>::Insert(T& object, const Math::AABB& region, s32 depth)
{
    // Region cannot be split any further
    if (depth >= mMaxDepth || !IsRegionValid(mAABB, kMinSize))
    {
        // TODO: What happens with the point then?
        return;
    }

    if (IsLeaf())
    {
        mObjects.insert(std::make_pair(&object, region));

        // Check if this node is full
        const u32 numObjects = mObjects.size();
        if (numObjects > mMaxElements)
        {
            // Split the region into octants
            std::vector<Math::AABB> octants = BuildRegions(mAABB);

            // Push all the objects down the tree
            for (auto objectIter : mObjects)
            {
                // Find which octant the point belongs and and create a new child there
                const s32 index = GetOctantContainingPoint(mAABB.center, objectIter.second.center);
                if (mpChildren[index] == nullptr)
                {
                    // Create a new child if it doesn't exist already
                    mpChildren[index] = new Octree(octants[index], *objectIter.first, objectIter.second);
                    mActiveChildren |= index; // Set the flag for the new child
                }
                else
                {
                    // Child already exists; insert
                    mpChildren[index]->Insert(object, region, depth + 1);
                }
            }
            // This node no longer contains any objects
            mObjects.clear();
        }
    }
    else
    {
        // Not a leaf, traverse recursivley
        const u32 index = GetOctantContainingPoint(mAABB.center, region.center);
        if (mpChildren[index] == nullptr)
        {
            std::vector<Math::AABB> octants = BuildRegions(mAABB);
            mpChildren[index] = new Octree(octants[index], object, region);
            mActiveChildren |= index; // Set the flag for the new child
        }
        else
        {
            mpChildren[index]->Insert(object, region, depth + 1);
        }
    }
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool Octree<T>::IsLeaf()
{
    // A leaf may contain 1 or 0 objects and has no children
    return (mActiveChildren == 0);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void Octree<T>::Destroy()
{
    for (u32 i=0; i < kNumChildren; ++i)
    {
        // Destroy children recursively
        Octree* child = mpChildren[i];
        if (child)
        {
            child->Destroy();
            SafeDelete(mpChildren[i]);
        }
    }
    mObjects.clear();
    mActiveChildren = 0;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool Octree<T>::GetIntersectingObjects(const Math::Ray& ray, std::vector<T*>& objects)
{
    // Early check to check if it's within the root region
    if (!Math::Intersect(ray, mAABB))
    {
        return false;
    }

    s32 depth = 0;
    return GetIntersectingObjects(ray, objects, depth);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool Octree<T>::GetIntersectingObjects(const Math::Ray& ray, std::vector<T*>& objects, s32 depth)
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
                    if (child->GetIntersectingObjects(ray, objects, depth + 1))
                    {
                        // return if this was the correct child, otherwise continue checking the rest
                        return true;
                    }
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
        for (auto object : mObjects)
        {
            if (Math::Intersect(ray, object.second))
            {
                objects.push_back(object.first);
                anyIntersect = true;
                break;
            }
        }
        return anyIntersect; // May not have intersected with any objects
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool Octree<T>::GetIntersectingObjects(std::vector<T*>& objects)
{
    s32 depth = 0;
    return GetIntersectingObjects(objects, depth);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool Octree<T>::GetIntersectingObjects(std::vector<T*>& objects, s32 depth)
{
    // TODO
    if (IsLeaf())
    {
        // Nothing to intersect with
        if (mObjects.size() <= 1)
        {
            return false;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void Octree<T>::SetBoundingRegion(const Math::AABB& region)
{
    // TODO: update child oct bounds when region is set
    mAABB = region;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void Octree<T>::Debug_DrawTree()
{
    SimpleDraw::AddAABB(mAABB, Color::Green());
    for (u32 i=0; i < kNumChildren; ++i)
    {
        if (mpChildren[i] != nullptr)
        {
            mpChildren[i]->Debug_DrawTree();
        }
    }
}