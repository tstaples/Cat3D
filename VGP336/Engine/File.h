#ifndef INCLUDED_ENGINE_FILE_H
#define INCLUDED_ENGINE_FILE_H

#include "FileBuffer.h"

namespace IO
{

class File
{
public:
    File();
    File(const std::string& path);

private:
    FileBuffer mBuffer;
};

} // namespace IO

#endif // #ifndef INCLUDED_ENGINE_FILE_H