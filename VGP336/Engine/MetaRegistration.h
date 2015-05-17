#ifndef INCLUDED_ENGINE_METAREGISTRATION_H
#define INCLUDED_ENGINE_METAREGISTRATION_H

//====================================================================================================
// Filename:	MetaRegistration.h
// Created by:	Tyler Staples
// Description: Function to register all meta classes.
//====================================================================================================

namespace Meta
{
    // Must be called before using any MetaClass
    void MetaRegistration();
}

#endif // #ifndef INCLUDED_ENGINE_METAREGISTRATION_H