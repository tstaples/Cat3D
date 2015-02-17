#include "Precompiled.h"
#include "ID.h"

ID::ID()
: blob(U32_MAX)
{
}

ID::ID(Meta::Type type, u8 instance, u16 index)
{
    flags.type = (u8)type;
    flags.instance = instance;
    flags.index = index;
}