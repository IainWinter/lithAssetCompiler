#include "asset_serializer.h"
#include "asset_package.h"

AssetSerializer::AssetSerializer(std::ostream& out) 
    : m_out (out)
{}

AssetSerializer& AssetSerializer::operator<<(int value) {
    m_out.write(reinterpret_cast<const char*>(&value), sizeof(int));
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(size_t value) {
    m_out.write(reinterpret_cast<const char*>(&value), sizeof(size_t));
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(float value) {
    m_out.write(reinterpret_cast<const char*>(&value), sizeof(float));
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(vec2 value) {
    *this << value.x << value.y;
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(const UUID& value) {
    m_out.write(reinterpret_cast<const char*>(&value), sizeof(UUID));
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(const std::string& value) {
    *this << value.size();
    m_out.write(reinterpret_cast<const char*>(value.data()), value.size());
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(const FixedByteArray& value) {
    *this << value.size();
    m_out.write(reinterpret_cast<const char*>(value.data()), value.size());
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(const AssetMetadata& value) {
    *this << static_cast<int>(value.type) << value.uuid << value.name;
    return *this;
}

AssetSerializer& AssetSerializer::operator<<(const std::shared_ptr<Asset>& value) {
    *this << value->metadata.uuid;
    return *this;
}

AssetDeserializer::AssetDeserializer(std::istream& in, AssetPackage* package) 
    : m_in      (in)
    , m_package (package)
{}

AssetDeserializer& AssetDeserializer::operator>>(int& value) {
    m_in.read(reinterpret_cast<char*>(&value), sizeof(int));
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(size_t& value) {
    m_in.read(reinterpret_cast<char*>(&value), sizeof(size_t));
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(float& value) {
    m_in.read(reinterpret_cast<char*>(&value), sizeof(float));
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(vec2& value) {
    *this >> value.x >> value.y;
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(UUID& value) {
    m_in.read(reinterpret_cast<char*>(&value), sizeof(UUID));
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(std::string& value) {
    size_t size;
    *this >> size;
    value.resize(size);
    m_in.read(reinterpret_cast<char*>(value.data()), value.size());

    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(FixedByteArray& value) {
    size_t size;
    *this >> size;
    value.resize(size);
    m_in.read(reinterpret_cast<char*>(value.data()), value.size());

    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(AssetMetadata& value) {
    *this >> reinterpret_cast<int&>(value.type) >> value.uuid >> value.name;
    return *this;
}

AssetDeserializer& AssetDeserializer::operator>>(std::shared_ptr<Asset>& value) {
    UUID uuid;
    *this >> uuid;
    value = m_package->get(uuid);
    return *this;
}