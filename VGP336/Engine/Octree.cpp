#include "Precompiled.h"
#include "Octree.h"
#include "TransformComponent.h"

Octree::Octree(TransformRepository& transformRepo)
    : mTransformRepository(transformRepo)
    , mpParent(nullptr)
    , mActiveChildren(0)
{
}

Octree::Octree(TransformRepository& transformRepo, const Math::AABB& region)
    : mTransformRepository(transformRepo)
    , mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
{

}

Octree::Octree(TransformRepository& transformRepo, const Math::AABB& region, const GameObjects& objects)
    : mTransformRepository(transformRepo)
    , mAABB(region)
    , mpParent(nullptr)
    , mActiveChildren(0)
    , mObjects(objects)
{
}

Octree::~Octree()
{
}

void Octree::BuildTree()
{
    //if (!IsRegionValid(mAABB))
    //{
    //    return;
    //}

    //// Create 8 quads to fill this region.
    //// iter though objects and test which quad they fit in.
    //// iter though 8 quads and create child nodes for ones with an obj count > 0.
    //// mark which children are active.

    //const Math::Vector3 center(mAABB.center);       // Center of this region
    //const Math::Vector3 extend(mAABB.extend);
    //const Math::Vector3 half(extend / 2.0f);

    //// Clock-wise
    //Math::AABB octants[kNumChildren];
    //octants[0] = Math::AABB(center - half, extend); // bottom left
    //octants[1] = Math::AABB(center.x - half.x, center.y - half.y, center.z + half.z, extend);
    //octants[2] = Math::AABB(center.x + half.x, center.y - half.y, center.z + half.z, extend);
    //octants[3] = Math::AABB(center.x + half.x, center.y - half.y, center.z - half.z, extend);
    //octants[4] = Math::AABB(center + half, extend); // top left
    //octants[5] = Math::AABB(center.x - half.x, center.y + half.y, center.z + half.z, extend);
    //octants[6] = Math::AABB(center.x + half.x, center.y + half.y, center.z + half.z, extend);
    //octants[7] = Math::AABB(center.x + half.x, center.y + half.y, center.z - half.z, extend);

    //GameObjects octList[kNumChildren]; // Holds which objects fit in the corresponding octants

    //GameObjects::iterator objIter = mObjects.begin();
    //for (objIter; objIter != mObjects.end; ++objIter)
    //{
    //    GameObject& gameObject = *objIter;

    //    // See if this object has a transform
    //    ID transformComponentID = gameObject.FindComponentID(Meta::TransformComponentType);
    //    if (!transformComponentID.IsValid())
    //    {
    //        // Some objects may not have a position in the world; ignore them?
    //        continue;
    //    }
    //    TransformComponent& transform = mTransformRepository.GetItem(transformComponentID);
    //    const Math::Vector3& position = transform.GetPosition();

    //    // Find which octant the object belongs in
    //    for (u32 i=0; i < kNumChildren; ++i)
    //    {
    //        if (octants[i].Contains(position))
    //        {
    //            octList[i].push_back(gameObject);
    //            break;
    //        }
    //    }
    //}

    //mChildren.resize(kNumChildren, Octree(mTransformRepository));
    //for (u32 i=0; i < kNumChildren; ++i)
    //{
    //    // Check if this sub-region contains any objects
    //    if (octList[i].size())
    //    {
    //        mChildren[i].CreateNode(octants[i], octList[i]);
    //        mActiveChildren |= (1 << i); // set the corresponding flag
    //        mChildren[i].BuildTree();
    //    }
    //}
}

void Octree::CreateNode(const Math::AABB& region, const GameObjects& objs)
{

}

bool Octree::IsRegionValid(const Math::AABB& region)
{
    return (region.extend.x > kMinSize &&
            region.extend.y > kMinSize &&
            region.extend.z > kMinSize);
}