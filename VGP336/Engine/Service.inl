//====================================================================================================
// Filename:	Service.inl
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Class Definitions
//====================================================================================================

template<typename T>
Service<T>::Service()
{
}

// ---------------------------------------------------------------------------------------------------

template<typename T>
Service<T>::~Service()
{
}

// ---------------------------------------------------------------------------------------------------

template<typename T>
void Service<T>::Subscribe(ID ObjId)
{
    ASSERT(mSubscribers.find(ObjId) == mSubscribers.end(), 
        "[Service] GameObject %u/%u is already registered.",
        ObjId.GetIndex(), ObjId.GetInstance());

    T info;
    OnSubscribe(ObjId, info);
    mSubscribers.insert(std::make_pair(ObjId, info));
}

// ---------------------------------------------------------------------------------------------------

template<typename T>
void Service<T>::UnSubscribe(ID ObjId)
{
    auto iter = mSubscribers.find(ObjId);

    ASSSERT(iter != mSubscribers.end(), 
        "[Service] Cannot unsubscribe from service the object is not subscribed to");

    if (iter != mSubscribers.end())
    {
        mSubscribers.erase(iter);
    }
}