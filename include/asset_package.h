#pragma once

#include "asset.h"
#include <vector>

struct AssetPackage {
    std::vector<std::shared_ptr<Asset>> assets;

    void add(const std::shared_ptr<Asset>& asset);
    std::shared_ptr<Asset> get(const UUID& uuid);

    void serialize(AssetSerializer& out);
    void deserialize(AssetDeserializer& in);
};