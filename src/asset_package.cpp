#include "asset_package.h"

#include "assets/font.h"
#include "assets/image.h"
#include "assets/shader_spirv.h"

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <ostream>
#include <istream>

void AssetPackage::add(const std::shared_ptr<Asset>& asset) {
    assets.push_back(asset);
}

std::shared_ptr<Asset> AssetPackage::get(const UUID& uuid) {
    for (const std::shared_ptr<Asset>& asset : assets) {
        if (asset->metadata.uuid == uuid) {
            return asset;
        }
    }

    return nullptr;
}

void AssetPackage::serialize(AssetSerializer& out) {
    // Assets depend on other assets, but if I force this to be consistant by type,
    // it can be as simple as ordering the types of assets
    std::unordered_map<AssetType, std::vector<std::shared_ptr<Asset>>> assetsByType;
    
    for (std::shared_ptr<Asset> asset : assets) {
        assetsByType[asset->metadata.type].push_back(asset);
    }

    std::vector<AssetType> order = {
        AssetType_Image,
        AssetType_Font,
        AssetType_ShaderSPIRV
    };

    out << assets.size();

    for (AssetType type : order) {
        for (const std::shared_ptr<Asset>& asset : assetsByType.at(type)) {
            out << asset->metadata;
            asset->serialize(out);
        }
    }
}

void AssetPackage::deserialize(AssetDeserializer& in) {
    size_t numAssets = 0;
    in >> numAssets;

    for (size_t i = 0; i < numAssets; i++) {
        std::shared_ptr<Asset> asset;

        AssetMetadata metadata;
        in >> metadata;

        switch (metadata.type) {
            case AssetType_Image: {
                asset = std::static_pointer_cast<Asset>(std::make_shared<ImageAsset>());
                break;
            }
            case AssetType_Font: {
                asset = std::static_pointer_cast<Asset>(std::make_shared<FontAsset>());
                break;
            }
            case AssetType_ShaderSPIRV: {
                asset = std::static_pointer_cast<Asset>(std::make_shared<ShaderSPIRVAsset>());
                break;
            }
        }

        asset->metadata = metadata;
        asset->deserialize(in);

        assets.push_back(asset);
    }
}