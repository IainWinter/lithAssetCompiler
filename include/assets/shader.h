#pragma once

#include "shader_spirv.h"
#include "enums.h"

struct ShaderAsset : Asset {
    struct PushConstant {
        ShaderStageBit stages;
        size_t size;
    };

    struct Descriptor {
        enum Type {
            Type_Sampler,
            Type_UniformBuffer
        };

        ShaderStageBit stages;
        Type type;
        uint32_t elementCount;
        uint32_t location;
        uint32_t set;
    };

    std::shared_ptr<ShaderSPIRVAsset> vertex;
    std::shared_ptr<ShaderSPIRVAsset> fragment;

    std::vector<PushConstant> pushConstants;
    std::vector<Descriptor> descriptors;
};