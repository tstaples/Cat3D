template <typename DataType>
MemoryPool<DataType>* MemHandle<DataType>::sMemoryPool = nullptr;

template <typename DataType>
MemHandle<DataType>::MemHandle()
    : mInstance(U16_MAX)
    , mIndex(U16_MAX)
{
}

template <typename DataType>
MemHandle<DataType>::MemHandle(u16 instance, u16 index)
    : mInstance(instance)
    , mIndex(index)
{
}

template <typename DataType>
DataType* MemHandle<DataType>::Get()
{
    DataType* data = nullptr;
    if (sMemoryPool)
    {
        data = sMemoryPool->Get(*this);
    }
    return data;
}

template <typename DataType>
const DataType* MemHandle<DataType>::Get() const
{
    DataType* data = nullptr;
    if (sMemoryPool)
    {
        data = sMemoryPool->Get(*this);
    }
    return data;
}

template <typename DataType>
bool MemHandle<DataType>::IsValid() const
{
    return sMemoryPool && sMemoryPool->IsValid(*this);
}

template <typename DataType>
void MemHandle<DataType>::Invalidate()
{
    mIndex = U16_MAX; mInstance = U16_MAX;
}