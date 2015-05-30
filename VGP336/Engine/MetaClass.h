#ifndef INCLUDED_ENGINE_METACLASS_H
#define INCLUDED_ENGINE_METACLASS_H

//====================================================================================================
// Filename:	MetaClass.h
// Created by:	Tyler Staples
// Description: Class for storing meta info about a class.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MetaType.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class MetaField;
class MetaDependency;

//====================================================================================================
// Class Declarations
//====================================================================================================

class MetaClass : public MetaType
{
public:
    typedef void* (*CreateFunc)();
    typedef void (*DestroyFunc)(void*);

    // Constructor
    // @param name: name of the class.
    // @param classSize: sizeof the class.
    // @param fields: pointer to a static array of fields that represent the class members.
    // @param numFields: number of fields in the array.
    // @param create: function pointer running the logic to create a new instance of the class.
    // @param destroy: unction pointer running the logic to destroy the instance of the class.
    MetaClass(const char* name, 
              u32 classSize, 
              const MetaField* fields, 
              u32 numFields,
              const MetaDependency* dependencies,
              u32 numDependencies,
              bool ispointer, 
              CreateFunc create, 
              DestroyFunc destroy);

    // Creates an instance of the class.
    // Returns a void pointer to the class.
    void* Create() const;

    // Destroys the instance of a class.
    // @param data: void pointer to the class.
    void Destroy(void* data) const;

    // Looks up a field by name.
    // @param name: name of the meta field.
    // Returns a pointer to the MetaField with that name, or nullptr if the field was not found.
    const MetaField* FindField(const char* name) const;

    // Gets the field associated with the index.
    // @param index: index of the field in mFields.
    // Returns a pointer to the MetaField.
    const MetaField* GetFieldAtIndex(u32 index) const;

    const MetaDependency* GetDependencyAtIndex(u32 index) const;

    const char* GetName() const     { return mName; }
    u32 GetNumFields() const        { return mNumFields; }
    u32 GetNumDependencies() const  { return mNumDependencies; }

private:
    // Will point to literal constant, so shallow copy is fine.
    const char* mName;

    const MetaField* mFields;
    const u32 mNumFields;

    const MetaDependency* mDependencies;
    const u32 mNumDependencies;

    CreateFunc mConstruct;
    DestroyFunc mDestruct;
};

#endif // #ifndef INCLUDED_ENGINE_METACLASS_H