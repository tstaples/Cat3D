#include "Precompiled.h"
#include "MetaType.h"

#include "Meta.h"
#include "EngineMath.h"

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