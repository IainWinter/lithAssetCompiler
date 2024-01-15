#pragma once

#include "asset_metadata.h"
#include "fixed_byte_array.h"
#include <ostream>
#include <istream>

#include "glm/vec2.hpp"
using namespace glm;

struct Asset;
struct AssetPackage;

// Writes assets to a vector of bytes
// If an Asset<T> is written, write its UUID and track it has a dependency
class AssetSerializer {
public:
    AssetSerializer(std::ostream& out);

    AssetSerializer& operator<<(int value);
    AssetSerializer& operator<<(size_t value);
    AssetSerializer& operator<<(float value);
    AssetSerializer& operator<<(vec2 value);
    AssetSerializer& operator<<(const UUID& value);
    AssetSerializer& operator<<(const std::string& value);
    AssetSerializer& operator<<(const FixedByteArray& value);
    AssetSerializer& operator<<(const AssetMetadata& value);
    AssetSerializer& operator<<(const std::shared_ptr<Asset>& value);

    // this seems to need to be a template cus the implicit conversion from
    // std::shared_ptr<T> to std::shared_ptr<Asset> doesn't work
    template<typename _t>
    AssetSerializer& operator<<(const std::shared_ptr<_t>& value) {
        *this << reinterpret_cast<const std::shared_ptr<Asset>&>(value);
        return *this;
    }

private:
    std::ostream& m_out;
};

class AssetDeserializer {
public:
    AssetDeserializer(std::istream& in, AssetPackage* package);

    AssetDeserializer& operator>>(int& value);
    AssetDeserializer& operator>>(size_t& value);
    AssetDeserializer& operator>>(float& value);
    AssetDeserializer& operator>>(vec2& value);
    AssetDeserializer& operator>>(UUID& value);
    AssetDeserializer& operator>>(std::string& value);
    AssetDeserializer& operator>>(FixedByteArray& value);
    AssetDeserializer& operator>>(AssetMetadata& value);
    AssetDeserializer& operator>>(std::shared_ptr<Asset>& value);

    // this seems to need to be a template cus the implicit conversion from
    // std::shared_ptr<T> to std::shared_ptr<Asset> doesn't work
    template<typename _t>
    AssetDeserializer& operator>>(std::shared_ptr<_t>& value) {
        *this >> reinterpret_cast<std::shared_ptr<Asset>&>(value);
        return *this;
    }

private:
    std::istream& m_in;
    AssetPackage* m_package;
};