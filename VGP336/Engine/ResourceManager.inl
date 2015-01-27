template<typename T>
ResourceManager<T>::ResourceManager()
{
}
//----------------------------------------------------------------------------------------------------

template<typename T>
ResourceManager<T>::~ResourceManager()
{
    ASSERT(mInventory.size() == 0, "[ResourceManager] Inventory is not empty!");
}
//----------------------------------------------------------------------------------------------------

template<typename T>
void ResourceManager<T>::Terminate()
{
    // Make sure we are holding the last reference of each resource
    Inventory::iterator item = mInventory.begin();
    for (item; item != mInventory.end(); ++item)
    {
        T* resource = item->second;

        // Ensure we are the soul owner of this object
        ASSERT(resource->GetRefCount() == 1, "[ResourceManager] resource was not released!");
        CleanupResource(resource);
        SafeRelease(resource);
    }
    mInventory.clear();
}
//----------------------------------------------------------------------------------------------------

template<typename T>
T* ResourceManager<T>::GetResource(const wchar_t* pfilename)
{
    // Check if the resource already exists
    u64 hash = GetHash(pfilename); // TODO: overload hash for wchar
    Inventory::iterator it = mInventory.find(hash);
    if (it != mInventory.end())
    {
        // Increase refcount and return the object
        it->second->AddRef();
        return it->second;
    }

    // Create a new entry in our inventory
    T* newResource = new T();
    newResource->AddRef(); // Resource Manager's ref
    InitResource(newResource, pfilename);
    mInventory.insert(std::make_pair(hash, newResource));

    // Add ref count for the object requesting it
    newResource->AddRef();
    return newResource;
}