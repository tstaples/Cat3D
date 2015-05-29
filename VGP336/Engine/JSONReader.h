#ifndef INCLUDED_ENGINE_JSONREADER_H
#define INCLUDED_ENGINE_JSONREADER_H

#include "JSONValue.h"

class JSONReader
{
public:
    JSONReader();

    bool Parse(const char* path, JSONValue& root);

private:
    JSONValue mRoot;
};

#endif // #ifndef INCLUDED_ENGINE_JSONREADER_H