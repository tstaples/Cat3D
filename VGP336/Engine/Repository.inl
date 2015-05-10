template<typename T>
Repository<T>::Repository(Meta::Type type, u16 capacity)
    : RepositoryBase(type)
    , mCapacity(capacity)
    , mSize(0)
    , mData(nullptr)
    , mInstanceCount(nullptr)
{
    // Using malloc because we don't want to construct the objects
    mData = static_cast<T*>(malloc(sizeof(T) * capacity));
    mInstanceCount = new u8[capacity];
    memset(mInstanceCount, 0, sizeof(mInstanceCount));
    mFreeSlots.reserve(capacity);
}
//----------------------------------------------------------------------------------------------------

template<typename T>
Repository<T>::~Repository()
{
    free(mData);
    SafeDeleteArray(mInstanceCount);
    mSize = 0;
    mCapacity = 0;
}
//----------------------------------------------------------------------------------------------------

template<typename T>
ID Repository<T>::Allocate()
{
    ASSERT(!mFreeSlots.size() || mSize < mCapacity, "[Repository(type=%u)] Failed to allocate.", GetType());

    // Additional check to prevent crashing during release
    if (mFreeSlots.size() && mSize >= mCapacity)
    {
        // Return an invalid id
        return ID();
    }

    u16 index = 0;
    if (!mFreeSlots.empty())
    {
        // Get the index from the back of the list
        index = mFreeSlots.back();
        mFreeSlots.pop_back();
    }
    else
    {
        // No free slots; append
        index = mSize;
        ++mSize;
    }

    // Note: placement new skips allocation if it's already allocated
    // and just does the construction part.
    //new (&mData[index]) T();

    return ID(GetType(), mInstanceCount[index], index);
}
//----------------------------------------------------------------------------------------------------

template<typename T>
void Repository<T>::Free(ID& id)
{
    if (IsValid(id))
    {
        const u16 index = id.GetIndex();

        // Free the slot
        mData[index].~T();
        ++mInstanceCount[index]; // Invalidate all existing ID's
        mFreeSlots.push_back(index);

        // Invalidate this id
        id.Invalidate();
    }
}
//----------------------------------------------------------------------------------------------------

template<typename T>
void Repository<T>::Flush()
{
    const u32 count = mSize;
    for (u32 i=0; i < count; ++i)
    {
        // Explicitly call the destructor of the data objects
        mData[i].~T();

        // Invalidate the ID to this object by offsetting it
        ++mInstanceCount[i];
    }
    mFreeSlots.clear();
    mSize = 0;
}
//----------------------------------------------------------------------------------------------------

template<typename T>
T& Repository<T>::GetItem(ID id)
{
    ASSERT(IsValid(id), "[Repository(type = %u)] Failed to get item with id(%u/%u/%u).", GetType(), GetType(), id.GetInstance(), id.GetIndex());
    return mData[id.GetIndex()];
}
//----------------------------------------------------------------------------------------------------

template<typename T>
const T& Repository<T>::GetItem(ID id) const
{
    ASSERT(IsValid(id), "[Repository(type = %u)] Failed to get item with id(%u/%u/%u).", GetType(), GetType(), id.GetInstance(), id.GetIndex());
    return mData[id.GetIndex()];
}
//----------------------------------------------------------------------------------------------------

template<typename T>
T* Repository<T>::FindItem(ID id)
{
    T* item = nullptr;
    if (IsValid(id))
    {
        item = mData[id.GetIndex()];
    }
    return item;
}
//----------------------------------------------------------------------------------------------------

template<typename T>
const T* Repository<T>::FindItem(ID id) const
{
    const T* item = nullptr;
    if (IsValid(id))
    {
        item = mData[id.GetIndex()];
    }
    return item;
}
//----------------------------------------------------------------------------------------------------

//template<typename T>
//Component* Repository<T>::GetComponent(ID id)
//{
//    return GetItem(id);
//}

//----------------------------------------------------------------------------------------------------

template<typename T>
bool Repository<T>::IsValid(ID id) const
{
    // Cache values for debug purpose. Possible todo: wrap with prepro build type (#if _DEBUG)
    const Meta::Type type = id.GetType();
    const u8 instance = id.GetInstance();
    const u16 index = id.GetIndex();

    return (type == GetType()
        && (index < mSize)
        && (instance == mInstanceCount[index]));
}