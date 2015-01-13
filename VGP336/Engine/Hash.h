#ifndef INCLUDED_ENGINE_HASH_H
#define INCLUDED_ENGINE_HASH_H

//====================================================================================================
// Filename:	Hash.h
// Created by:	Tyler Staples
// Description:	Helper functions for generating a hash;
//====================================================================================================

//====================================================================================================
// Function Declarations
//====================================================================================================

// Assumes string passed in is null terminated
u64 GetHash(const char* str);

#endif // #ifndef INCLUDED_ENGINE_HASH_H