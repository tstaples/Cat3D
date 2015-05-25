#ifndef INCLUDED_ENGINE_METADEPENDENCY_H
#define INCLUDED_ENGINE_METADEPENDENCY_H

//====================================================================================================
// Filename:	MetaDependency.h
// Created by:	Tyler Staples
// Description: Class for storing meta info about a class dependency.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaClass;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaDependency
{
public:
    MetaDependency(const char* name)
        : mName(name)
    {
    }

    const char* GetName() const     { return mName; }
    //const MetaClass* GetMetaClass() const { return mMetaClass; }

private:
    const char* mName;
    //const MetaClass* mMetaClass;
};

#endif // #ifndef INCLUDED_ENGINE_METADEPENDENCY_H