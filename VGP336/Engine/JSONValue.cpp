#include "Precompiled.h"
#include "JSONValue.h"

JSONValue::JSONValue() {}
JSONValue::JSONValue(const Json::Value val) : mValue(val) {}
JSONValue::JSONValue(s32 val) : mValue(val) {}
JSONValue::JSONValue(u32 val) : mValue(val) {}
JSONValue::JSONValue(s64 val) : mValue(val) {}
JSONValue::JSONValue(u64 val) : mValue(val) {}
JSONValue::JSONValue(f32 val) : mValue(val) {}
JSONValue::JSONValue(f64 val) : mValue(val) {}
JSONValue::JSONValue(bool val) : mValue(val) {}
JSONValue::JSONValue(const std::string& val) : mValue(val) {}

s32 JSONValue::asInt() const                { return (s32)mValue.asInt(); }
u32 JSONValue::asUInt() const               { return (u32)mValue.asUInt(); }
s64 JSONValue::asInt64() const              { return (s64)mValue.asInt64(); }
u64 JSONValue::asUInt64() const             { return (u64)mValue.asUInt64(); }
f32 JSONValue::asFloat() const              { return (f32)mValue.asFloat(); }
f64 JSONValue::asDouble() const             { return (f64)mValue.asDouble(); }
bool JSONValue::asBool() const              { return mValue.asBool(); }
std::string JSONValue::asString() const     { return mValue.asString(); }
const char* JSONValue::asCString() const    { return mValue.asCString(); }

bool JSONValue::IsNull() const      { return mValue.isNull(); }
bool JSONValue::IsBool() const      { return mValue.isBool(); }
bool JSONValue::IsInt() const       { return mValue.isInt(); }
bool JSONValue::IsInt64() const     { return mValue.isInt64(); }
bool JSONValue::IsUInt() const      { return mValue.isUInt(); }
bool JSONValue::IsUInt64() const    { return mValue.isUInt64(); }
bool JSONValue::IsIntegral() const  { return mValue.isIntegral(); }
bool JSONValue::IsDouble() const    { return mValue.isDouble(); }
bool JSONValue::IsNumeric() const   { return mValue.isNumeric(); }
bool JSONValue::IsString() const    { return mValue.isString(); }
bool JSONValue::IsArray() const     { return mValue.isArray(); }
bool JSONValue::IsObject() const    { return mValue.isObject(); }

JSONValue JSONValue::Get(u32 index, const JSONValue& defaultValue) const
{
    return JSONValue(mValue.get(index, defaultValue.mValue));
}

JSONValue JSONValue::Get(const std::string& key, const JSONValue& defaultValue) const
{
    return JSONValue(mValue.get(key, defaultValue.mValue));
}

u32 JSONValue::Size() const
{
    return mValue.size();
}

JSONValue& JSONValue::operator=(Json::Value other)
{
    mValue = other;
    return *this;
}

JSONValue& JSONValue::operator=(JSONValue other)
{
    mValue = other.mValue;
    return *this;
}

JSONValue& JSONValue::operator[](u32 index)
{
    return JSONValue(mValue[index]);
}

const JSONValue& JSONValue::operator[](u32 index) const
{
    return JSONValue(mValue[index]);
}

JSONValue& JSONValue::operator[](const std::string& key)
{
    return JSONValue(mValue[key]);
}

const JSONValue& JSONValue::operator[](const std::string& key) const
{
    return JSONValue(mValue[key]);
}

JSONValue::iterator JSONValue::begin()
{
    return mValue.begin();
}

JSONValue::iterator JSONValue::end()
{
    return mValue.end();
}

JSONValue::const_iterator JSONValue::begin() const
{
    return mValue.begin();
}

JSONValue::const_iterator JSONValue::end() const
{
    return mValue.end();
}