#pragma once

#include "asset.h"

struct ImageAsset : Asset {
    int width;
    int height;
    int channels;

    FixedByteArray pixels;

    void serialize(AssetSerializer& out) const override {
        out << width
            << height
            << channels
            << pixels;
    }

    void deserialize(AssetDeserializer& in) override {
        in >> width
           >> height
           >> channels
           >> pixels;
    }
};