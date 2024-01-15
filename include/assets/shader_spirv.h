#pragma once

#include "asset.h"

struct ShaderSPIRVAsset : Asset {
    FixedByteArray bytecode;

    void serialize(AssetSerializer& out) const override {
        out << bytecode;
    }

    void deserialize(AssetDeserializer& in) override {
        in >> bytecode;
    }
};