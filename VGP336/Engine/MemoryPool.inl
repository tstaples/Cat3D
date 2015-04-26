template<typename Handle, typename DataType>
MemoryPool<Handle, DataType>::MemoryPool(u16 capacity)
    : MemoryPoolBase(type)
    , mCapacity(capacity)
    , mSize(0)
    , mData(nullptr)
    , mInstanceCount(nullptr)
{
    // Using malloc because we don't want to construct the objects
    mData = static_cast<DataType*>(malloc(sizeof(DataType) * capacity));
    mInstanceCount = new u8[capacity];
    memset(mInstanceCount, 0, sizeof(mInstanceCount) * capacity);
    mFreeSlots.reserve(capacity);
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
MemoryPool<Handle, DataType>::~MemoryPool()
{
    SafeDeleteArray(mInstanceCount);
    free(mData);
    mSize = 0;
    mCapacity = 0;
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
MemHandle MemoryPool<Handle, DataType>::Allocate()
{
    ASSERT(!mFreeSlots.size() || mSize < mCapacity, "[MemoryPool] Failed to allocate.");

    // Additional check to prevent crashing during release
    if (mFreeSlots.size() && mSize >= mCapacity)
    {
        // Return an invalid MemHandle
        return MemHandle();
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
    new (&mData[index]) DataType();

    return MemHandle(mInstanceCount[index], index);
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
void MemoryPool<Handle, DataType>::Free(MemHandle& handle)
{
    if (IsValid(handle))
    {
        const u16 index = handle.GetIndex();

        // Free the slot
        mData[index].~DataType();
        ++mInstanceCount[index]; // Invalidate all existing handles to this slot
        mFreeSlots.push_back(index);
    }
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
void MemoryPool<Handle, DataType>::Flush()
{
    const u32 count = mSize;
    for (u32 i=0; i < count; ++i)
    {
        // Explicitly call the destructor of the data objects
        mData[i].~DataType();

        // Invalidate the MemHandle to this object by offsetting it
        ++mInstanceCount[i];
    }
    mFreeSlots.clear();
    mSize = 0;
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
DataType* MemoryPool<Handle, DataType>::GetItem(MemHandle handle)
{
    DataType* item = nullptr;
    if (IsValid(handle))
    {
        item = mData[handle.GetIndex()];
    }
    return item;
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
const DataType* MemoryPool<Handle, DataType>::GetItem(MemHandle handle) const
{
    const DataType* item = nullptr;
    if (IsValid(handle))
    {
        item = mData[handle.GetIndex()];
    }
    return item;
}
//----------------------------------------------------------------------------------------------------

template<typename Handle, typename DataType>
bool MemoryPool<Handle, DataType>::IsValid(MemHandle handle) const
{
    // Cache values for debug purpose. Possible todo: wrap with prepro build type (#if _DEBUG)
    const u16 instance = handle.GetInstance();
    const u16 index = handle.GetIndex();

    // Index cannot be the last slot since it is reserved for an "invalid" slot
    return ((index < (mSize - 1)) &&
            (instance == mInstanceCount[index]));
}