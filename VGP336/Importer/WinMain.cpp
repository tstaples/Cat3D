#include "Importer.h"
#include "Exporter.h"

#include <Engine.h>
#include <iostream>

f32 GetScale(u32 args, char** argv);

int main(int argc, char** argv)
{
    const u32 minArgs = 2;
    const u32 args = argc - 1; // Exclude .exe
    if (args < minArgs)
    {
        // TODO: print usage/help page
        std::cout << "Invalid number of arguments\n";
        return 1;
    }

    // Store params
    // TODO: verify path
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const f32 scale = GetScale(args, argv);

    Importer importer;
    if (!importer.Load(inputFile, scale))
    {
        std::cout << "Failed to load file: " << inputFile << std::endl;
        return 1;
    }

    Exporter exporter;
    if (!exporter.Export(outputFile, importer.GetMeshes()))
    {
        std::cout << "Failed to export to: " << outputFile << std::endl;
        return 1;
    }

    return 0;
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