#ifndef INCLUDED_ENGINE_METADB_H
#define INCLUDED_ENGINE_METADB_H

//====================================================================================================
// Filename:	MetaDB.h
// Created by:	Tyler Staples
// Description: Helper class for tracking MetaClasses.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaClass;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaDB
{
public:
    static void Register(const MetaClass* metaClass);
    static const MetaClass* GetMetaClass(const char* className);

private:
    // No need to ever create this class
    MetaDB();
};

#endif // #ifndef INCLUDED_ENGINE_METADB_H