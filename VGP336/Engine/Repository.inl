template<typename T>
Repository<T>::Repository(Meta::Type type, u16 capacity)
: RepositoryBase(type)
{
    mData.reserve(capacity);
    mInstanceCount.resize(capacity, 0);
    mFreeSlots.reserve(capacity);
}
//----------------------------------------------------------------------------------------------------

template<typename T>
ID Repository<T>::Allocate()
{
    ASSERT(!mFreeSlots.size() || mData.size() < mData.capacity(), "[Repository(type=%u)] Failed to allocate.", mType);

    // Additional check to prevent crashing during release
    if (mFreeSlots.size() && mData.size() >= mData.capacity())
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
        index = (u16)mData.size();
        mData.push_back(T());
    }
    return ID(GetType(), mInstanceCount[index], index);
}
//----------------------------------------------------------------------------------------------------

template<typename T>
void Repository<T>::Free(ID& id)
{
    if (IsValid(id))
    {
        const u16 index = id.GetIndex();

        // Free the data by overwriting it with a default object
        mData[index] = T();
        ++mInstanceCount[index]; // Invalidate all existing ID's
        mFreeSlots.push_back(index);

        // Invalidate this id
        id.Invalidate();
    }
}
//----------------------------------------------------------------------------------------------------

template<typename T>
T& Repository<T>::GetItem(ID id)
{
    ASSERT(IsValid(id), "[Repository(type = %u)] Failed to get item with id(%u/%u/%u).", mType, mType, id.GetInstance(), id.GetIndex());
    return mData.at(id.GetIndex());
}
//----------------------------------------------------------------------------------------------------

template<typename T>
const T& Repository<T>::GetItem(ID id) const
{
    ASSERT(IsValid(id), "[Repository(type = %u)] Failed to get item with id(%u/%u/%u).", mType, mType, id.GetInstance(), id.GetIndex());
    return mData.at(id.GetIndex());
}
//----------------------------------------------------------------------------------------------------

template<typename T>
T* Repository<T>::FindItem(ID id)
{
    T* item = nullptr;
    if (IsValid(id))
    {
        item = mData.at(id.GetIndex());
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
        item = mData.at(id.GetIndex());
    }
    return item;
}
//----------------------------------------------------------------------------------------------------

template<typename T>
bool Repository<T>::IsValid(ID id) const
{
    // Cache values for debug purpose. Possible todo: wrap with prepro build type (#if _DEBUG)
    const Meta::Type type = id.GetType();
    const u8 instance = id.GetInstance();
    const u16 index = id.GetIndex();

    return (type == GetType()
        && (index < mData.size())
        && (instance == mInstanceCount.at(index));
}