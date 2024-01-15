#pragma once

#include "asset_metadata.h"
#include "fixed_byte_array.h"
#include <vector>

// A serialized asset and the UUIDs of the assets which it depends on
struct SerializedAsset {
    AssetMetadata metadata;
    FixedByteArray data;
    std::vector<UUID> dependencies;
};