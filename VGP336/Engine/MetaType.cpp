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
        static MetaType sMetaType(MetaType::ENUM_TYPE, sizeof(DATA_TYPE));\
        return &sMetaType;\
    }

// Registered types
META_REGISTER_TYPE(Int, int);
META_REGISTER_TYPE(Float, float);
META_REGISTER_TYPE(Bool, bool);
META_REGISTER_TYPE(Vector3, Math::Vector3);
META_REGISTER_TYPE(Matrix, Math::Matrix);