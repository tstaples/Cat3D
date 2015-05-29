#ifndef INCLUDED_ENGINE_JSONVALUE_H
#define INCLUDED_ENGINE_JSONVALUE_H

#include <json/json.h>

class JSONValue
{
public:
    typedef Json::Value::iterator iterator;
    typedef Json::Value::const_iterator const_iterator;

    JSONValue();
    JSONValue(const Json::Value val);
    JSONValue(s32 val);
    JSONValue(u32 val);
    JSONValue(s64 val);
    JSONValue(u64 val);
    JSONValue(f32 val);
    JSONValue(f64 val);
    JSONValue(bool val);
    JSONValue(const std::string& val);

    s32 asInt() const;
    u32 asUInt() const;
    s64 asInt64() const;
    u64 asUInt64() const;
    f32 asFloat() const;
    f64 asDouble() const;
    bool asBool() const;
    std::string asString() const;
    const char* asCString() const;

    bool IsNull() const;
    bool IsBool() const;
    bool IsInt() const;
    bool IsInt64() const;
    bool IsUInt() const;
    bool IsUInt64() const;
    bool IsIntegral() const;
    bool IsDouble() const;
    bool IsNumeric() const;
    bool IsString() const;
    bool IsArray() const;
    bool IsObject() const;

    JSONValue Get(u32 index, const JSONValue& defaultValue) const;
    JSONValue Get(const std::string& key, const JSONValue& defaultValue) const;

    u32 Size() const;

    JSONValue& operator=(Json::Value other);
    JSONValue& operator=(JSONValue other);

    JSONValue& operator[](u32 index);
    const JSONValue& operator[](u32 index) const;
    JSONValue& operator[](const std::string& key);
    const JSONValue& operator[](const std::string& key) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

private:
    Json::Value mValue;
};


#endif // #ifndef INCLUDED_ENGINE_JSONVALUE_H