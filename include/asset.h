#pragma once

#include "asset_serializer.h"
#include <string>
#include <memory>

struct Asset {
    AssetMetadata metadata;

    virtual ~Asset() = default;

    virtual void serialize(AssetSerializer& out) const = 0;
    virtual void deserialize(AssetDeserializer& in) = 0;
};

template<typename _t>
std::shared_ptr<_t> make_asset(AssetType type, const std::string& name, const _t& data) {
    AssetMetadata metadata;
    metadata.type = type;
    metadata.name = name;
    metadata.uuid = UUID::random();

    std::shared_ptr<_t> asset = std::make_shared<_t>(data);
    asset->metadata = metadata;

    return asset;
}