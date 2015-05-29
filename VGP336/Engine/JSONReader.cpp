#include "Precompiled.h"
#include "JSONReader.h"

#include "IO.h"

#include <fstream>

bool JSONReader::Parse(const char* path, JSONValue& root)
{
    std::ifstream data(path);
    ASSERT(data.good(), "[JSONReader] Error loading file: %s", path);

    Json::Reader reader;
    Json::Value jroot;
    if (!reader.parse(data, jroot))
    {
        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
        return false;
    }
    root = jroot;
    return true;
}
