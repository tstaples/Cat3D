#ifndef INCLUDED_ENGINE_RESOURCE_H
#define INCLUDED_ENGINE_RESOURCE_H

class Resource
{
public:
    inline Resource()
        : mRefCount(0)
    {
    }
    
    // Enable polymorphism
    inline virtual ~Resource()
    {
        // Empty
    }

    inline void AddRef()
    {
        ++mRefCount;
    }

    inline void Release()
    {
        // Prevent refcount from wrapping back to max int
        ASSERT(mRefCount > 0, "[Resource] Cannot release resource with 0 refs");

        // Last reference to this object was released; free memory
        if ((--mRefCount) == 0)
        {
            delete this;
        }
    }

    inline u32 GetRefCount() const
    {
        return mRefCount;
    }

private:
    // TODO: make thread safe (atomic)
    u32 mRefCount;
};

#endif // #ifndef INCLUDED_ENGINE_RESOURCE_H