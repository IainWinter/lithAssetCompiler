#pragma once

#include "uuid.h"
#include <string>

enum AssetType {
    AssetType_Image,
    AssetType_ShaderSPIRV,
    AssetType_Font
};

// Track info about an asset, not stored in the asset itself
struct AssetMetadata {
    AssetType type;
    UUID uuid;
    std::string name;
};