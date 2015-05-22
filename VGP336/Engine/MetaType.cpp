#include "Precompiled.h"
#include "MetaType.h"

#include "EngineMath.h"
#include "IO.h"
#include "Meta.h"

#include <json/json.h>

template <typename DataType>
const MetaType* DeduceDataType()
{
    // Tempalte hlper for automatic tpye discovery, default implemetation will return nothing.
    // Template specialization is required for each data type.
}

//----------------------------------------------------------------------------------------------------

#define META_REGISTER_TYPE(ENUM_TYPE, DATA_TYPE)\
    template <> const MetaType* DeduceDataType<DATA_TYPE>()\
    {\
        bool ispointer = std::is_pointer<DATA_TYPE>::value;\
        bool isarray = std::is_array<DATA_TYPE>::value;\
        u32 arrsize = (isarray) ? std::extent<DATA_TYPE>::value : 0;\
        static MetaType sMetaType(ENUM_TYPE, sizeof(DATA_TYPE), ispointer, isarray, arrsize);\
        return &sMetaType;\
    }

// Registered types
META_REGISTER_TYPE(MetaType::Int, s32);
META_REGISTER_TYPE(MetaType::UInt, u32);
META_REGISTER_TYPE(MetaType::Short, s16);
META_REGISTER_TYPE(MetaType::UShort, u16);
META_REGISTER_TYPE(MetaType::Char, s8);
META_REGISTER_TYPE(MetaType::UChar, u8);
META_REGISTER_TYPE(MetaType::Float, f32);
META_REGISTER_TYPE(MetaType::Double, f64);
META_REGISTER_TYPE(MetaType::Bool, bool);
META_REGISTER_TYPE(MetaType::String, std::string);
META_REGISTER_TYPE(MetaType::WString, std::wstring);
META_REGISTER_TYPE(MetaType::Path, wchar_t[MAX_PATH]);
META_REGISTER_TYPE(MetaType::Vector3, Math::Vector3);
META_REGISTER_TYPE(MetaType::Matrix, Math::Matrix);
META_REGISTER_TYPE(MetaType::AABB, Math::AABB);
//META_REGISTER_TYPE(MetaType::Array, std::vector); // Need to know vector type

//----------------------------------------------------------------------------------------------------

// Deserialize specializations
// Note: short/char unsupported by jsoncpp
template <typename DataType>
void Deserialize(const Json::Value& jvalue, void* data)
{
    ASSERT(false, "[MetaType] Error deserializing unsupported type");
}
template <> void Deserialize<s32>(const Json::Value& jvalue, void* data)
{
    s32 i = (s32)jvalue[0].asInt();
    memcpy(data, &i, sizeof(s32));
}
template <> void Deserialize<u32>(const Json::Value& jvalue, void* data)
{
    u32 i = (u32)jvalue[0].asUInt();
    memcpy(data, &i, sizeof(u32));
}
template <> void Deserialize<f64>(const Json::Value& jvalue, void* data)
{
    f64 i = (f64)jvalue[0].asDouble();
    memcpy(data, &i, sizeof(f64));
}
template <> void Deserialize<f32>(const Json::Value& jvalue, void* data)
{
    f32 i = (f32)jvalue[0].asFloat();
    memcpy(data, &i, sizeof(f32));
}
template <> void Deserialize<bool>(const Json::Value& jvalue, void* data)
{
    bool i = (bool)jvalue[0].asBool();
    memcpy(data, &i, sizeof(bool));
}
template <> void Deserialize<std::wstring>(const Json::Value& jvalue, void* data)
{
    std::wstring s = IO::CharToWChar(jvalue[0].asString());
    memcpy(data, &s, s.length());
}
template <> void Deserialize<std::string>(const Json::Value& jvalue, void* data)
{
    std::string s = jvalue[0].asString();
    memcpy((s8*)data, s.c_str(), s.length());
}
template <> void Deserialize<wchar_t[MAX_PATH]>(const Json::Value& jvalue, void* data)
{
    memset(data, 0, MAX_PATH * sizeof(wchar_t));
    std::wstring s = IO::CharToWChar(jvalue[0].asString());
    memcpy(data, s.c_str(), s.length() * sizeof(wchar_t));
}
template <> void Deserialize<Math::Vector3>(const Json::Value& jvalue, void* data)
{
    Math::Vector3 v;
    v.x = (f32)jvalue[0].asFloat();
    v.y = (f32)jvalue[1].asFloat();
    v.z = (f32)jvalue[2].asFloat();
    memcpy(data, &v, sizeof(Math::Vector3));
}
template <> void Deserialize<Math::Matrix>(const Json::Value& jvalue, void* data)
{
    f32 m[16] = { 0.0f };
    for (u32 i=0; i < 16; ++i)
    {
        m[i] = (f32)jvalue[i].asFloat();
    }
    memcpy(data, m, sizeof(f32) * 16);
}
template <> void Deserialize<Math::AABB>(const Json::Value& jvalue, void* data)
{
    // Note: I doubt this will work
    Deserialize<Math::Vector3>(jvalue, data);
    const Json::Value* v = &jvalue[3];
    u8* p = (u8*)data;
    Deserialize<Math::Vector3>(*v, p + sizeof(Math::Vector3));
}

//----------------------------------------------------------------------------------------------------

typedef void (*DeserializeFunc)(const Json::Value&, void*);
static std::map<MetaType::Type, DeserializeFunc> sDeserializeMap = 
{
    { MetaType::Int, Deserialize<s32> },
    { MetaType::UInt, Deserialize<u32> },
    { MetaType::Float, Deserialize<f32> },
    { MetaType::Double, Deserialize<f64> },
    { MetaType::Bool, Deserialize<bool> },
    { MetaType::String, Deserialize<std::string> },
    { MetaType::WString, Deserialize<std::wstring> },
    { MetaType::Path, Deserialize<wchar_t[MAX_PATH]> },
    { MetaType::Vector3, Deserialize<Math::Vector3> },
    { MetaType::Matrix, Deserialize<Math::Matrix> },
    { MetaType::AABB, Deserialize<Math::AABB> }
};

//----------------------------------------------------------------------------------------------------

MetaType::MetaType(Type type, u32 size, bool ispointer, bool isArray, u32 arrsize)
    : mType(type)
    , mSize(size)
    , mIsPointer(ispointer)
    , mIsArray(isArray)
    , mArrLength(arrsize)
{
}

//----------------------------------------------------------------------------------------------------

void MetaType::Deserialize(const Json::Value& jsonValue, void* data) const
{
    sDeserializeMap[mType](jsonValue, data);
}