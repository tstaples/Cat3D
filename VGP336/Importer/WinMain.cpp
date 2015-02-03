#include "Importer.h"
#include "Exporter.h"

#include <Engine.h>
#include <iostream>
#include "Options.h"

void PrintUsage();
f32 GetScale(u32 args, char** argv);
u32 ParseOptions(int numArgs, char** args);
u32 DefaultFlags();


int main(int argc, char** argv)
{
    const u32 minArgs = 2;
    const u32 numArgs = argc - 1; // Exclude .exe
    if (numArgs < minArgs)
    {
        // TODO: print usage/help page
        PrintUsage();
        return 1;
    }

    // Store params TODO: verify path
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const f32 scale = GetScale(numArgs, argv);
    u32 flags = DefaultFlags(); // Temp

    // Import the model and convert it to native format
    Importer importer;
    if (!importer.Load(inputFile, scale, flags))
    {
        std::cout << "Failed to load file: " << inputFile << std::endl;
        return 1;
    }

    // Export the asset in our native format
    Exporter exporter;
    if (!exporter.Export(outputFile, importer.GetMeshes(), importer.GetTexturePaths(), importer.GetBones()))
    {
        std::cout << "Failed to export to: " << outputFile << std::endl;
        return 1;
    }

    importer.Cleanup();

    return 0;
}

u32 ParseOptions(int numArgs, char** args)
{
    u32 flags = 0;
    if (args == nullptr)
    {
        // If there are no 
        return DefaultFlags();
    }

    for (int i=0; i < numArgs; ++i)
    {
        const char* arg = args[i];

        // Check the flag groups
        auto groupIter = Options::flagGroups.find(arg);
        if (groupIter != Options::flagGroups.end())
        {
            // OR the corresponding flag
            flags |= groupIter->second;
        }

        // Check for individual options
        auto flagIter = Options::flagMap.find(arg);
        if (flagIter!= Options::flagMap.end())
        {
            flags |= flagIter->second;
        }
    }
    return flags;
}

u32 DefaultFlags()
{
    u32 flags   = aiProcess_Triangulate
			    | aiProcess_JoinIdenticalVertices
			    | aiProcess_SortByPType
			    | aiProcess_FlipUVs;
    return flags;
}

f32 GetScale(u32 args, char** argv)
{
    f32 scale = 1.0f; // default
    if (args == 3)
    {
        std::string line(argv[3]);
        if (line.find("scale") == std::string::npos)
        {
            // TODO: throw exception printing usage
            return scale;
        }

        u32 equalIndex = line.find('=');
        if (equalIndex == std::string::npos)
        {
            // TODO: throw exception printing usage
            return scale;
        }
        std::string val(line.begin() + (equalIndex + 1), line.end());
        scale = (f32)atof(val.data());
    }
    return scale;
}

void PrintUsage()
{
    // TODO: print actual usage
    // usage: inputpath outputpath options
    std::cout << "you fucked up\n";
}