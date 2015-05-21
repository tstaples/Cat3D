#include "Precompiled.h"
#include "MetaType.h"

#include "Meta.h"
#include "EngineMath.h"

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
        static MetaType sMetaType(ENUM_TYPE, sizeof(DATA_TYPE));\
        return &sMetaType;\
    }

// Registered types
META_REGISTER_TYPE(MetaType::Int, int);
META_REGISTER_TYPE(MetaType::Float, float);
META_REGISTER_TYPE(MetaType::Bool, bool);
META_REGISTER_TYPE(MetaType::String, std::string);
META_REGISTER_TYPE(MetaType::Vector3, Math::Vector3);
META_REGISTER_TYPE(MetaType::Matrix, Math::Matrix);
META_REGISTER_TYPE(MetaType::AABB, Math::AABB);
//META_REGISTER_TYPE(MetaType::Array, std::vector); // Need to know vector type

//----------------------------------------------------------------------------------------------------

// Deserialize specializations
template <typename DataType>
void Deserialize(const Json::Value& jvalue, void* data)
{
    // Empty
}
template <>
void Deserialize<int>(const Json::Value& jvalue, void* data)
{
    int i = (int)jvalue[0].asInt();
    memcpy(data, &i, sizeof(int));
}
template <>
void Deserialize<float>(const Json::Value& jvalue, void* data)
{
    float i = (float)jvalue[0].asFloat();
    memcpy(data, &i, sizeof(float));
}
template <>
void Deserialize<bool>(const Json::Value& jvalue, void* data)
{
    bool i = (bool)jvalue[0].asBool();
    memcpy(data, &i, sizeof(bool));
}
template <>
void Deserialize<std::string>(const Json::Value& jvalue, void* data)
{
    std::string s = jvalue[0].asString();
    memcpy(data, s.c_str(), s.length());
}
template <>
void Deserialize<Math::Vector3>(const Json::Value& jvalue, void* data)
{
    Math::Vector3 v;
    v.x = (f32)jvalue[0].asFloat();
    v.y = (f32)jvalue[1].asFloat();
    v.z = (f32)jvalue[2].asFloat();
    memcpy(data, &v, sizeof(Math::Vector3));
}
template <>
void Deserialize<Math::Matrix>(const Json::Value& jvalue, void* data)
{
    f32 m[16] = { 0.0f };
    for (u32 i=0; i < 16; ++i)
    {
        m[i] = (f32)jvalue[i].asFloat();
    }
    memcpy(data, m, sizeof(f32) * 16);
}
template <>
void Deserialize<Math::AABB>(const Json::Value& jvalue, void* data)
{
    // Note: I doubt this will work
    Deserialize<Math::Vector3>(jvalue, data);
    const Json::Value* v = &jvalue[3];
    Deserialize<Math::Vector3>(v, ((u8*)data) + sizeof(Math::Vector3));
}

//----------------------------------------------------------------------------------------------------

typedef void (*DeserializeFunc)(const Json::Value&, void*);
static std::map<MetaType::Type, DeserializeFunc> sDeserializeMap = 
{
    { MetaType::Int, Deserialize<int> },
    { MetaType::Float, Deserialize<float> },
    { MetaType::Bool, Deserialize<bool> },
    { MetaType::String, Deserialize<std::string> },
    { MetaType::Vector3, Deserialize<Math::Vector3> },
    { MetaType::Matrix, Deserialize<Math::Matrix> },
    { MetaType::AABB, Deserialize<Math::AABB> }
};

//----------------------------------------------------------------------------------------------------

MetaType::MetaType(Type type, u32 size)
    : mType(type)
    , mSize(size)
{
}

//----------------------------------------------------------------------------------------------------

void MetaType::Deserialize(const Json::Value& jsonValue, void* data) const
{
    sDeserializeMap[mType](jsonValue, data);
}