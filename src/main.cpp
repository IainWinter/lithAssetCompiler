#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "loaders.h"
#include "asset_serializer.h"

// point this at a directory
// and it will print out all the files in it

static std::unordered_map<std::string, AssetType> s_extToAssetType = {
    { ".png", AssetType_Image },
    { ".jpeg", AssetType_Image },
    { ".jpg", AssetType_Image },
    { ".vert", AssetType_ShaderSPIRV },
    { ".frag", AssetType_ShaderSPIRV },
    { ".ttf", AssetType_Font }
};

static std::unordered_map<AssetType, std::string> s_assetTypeNames = {
    { AssetType_Image, "Image" },
    { AssetType_ShaderSPIRV, "ShaderSPIRV" },
    { AssetType_Font, "Font" }
};

int main() {
    std::string rootPath = "C:\\dev\\lithAssetCompiler2\\test";

    // iterate recursively through the directory
    // for each file, print out the name

    AssetPackage package;
    std::string err;

    for (std::filesystem::path p : std::filesystem::recursive_directory_iterator(rootPath)) {
        if (std::filesystem::is_regular_file(p)) {
            std::string path = p.string();

            std::string name = std::filesystem::relative(p, rootPath).string();
            std::replace(name.begin(), name.end(), '\\', '/');

            std::string ext = p.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

            if (s_extToAssetType.count(ext) == 0) {
                continue;
            }

            std::cout << "Processing File" << std::endl;

            AssetType type = s_extToAssetType[ext];
            std::string assetTypeName = s_assetTypeNames[type];

            std::cout << "\tPath: " << path << std::endl;
            std::cout << "\tName: " << name << std::endl;
            std::cout << "\tAsset type: " << assetTypeName << std::endl;

            switch (type) {
                case AssetType_Image: {
                    loadImage(&package, name, path, err);
                    break;
                }
                case AssetType_ShaderSPIRV: {
                    loadShaderSPIRV(&package, name, path, err);
                    break;
                }
                case AssetType_Font: {
                    loadFont(&package, name, path, err);
                    break;
                }
            }

            if (err == "") {
                std::cout << "\tSuccess" << std::endl;
            }

            else {
                std::cout << "\tError: " << err << std::endl;
                std::cout << "Aborting..." << std::endl;
                break;
            }
        }
    }

    {
        std::ofstream out("assets.bin", std::ios::binary);
        AssetSerializer serializer(out);
        package.serialize(serializer);
    }

    {
        AssetPackage package1;
        std::ifstream in("assets.bin", std::ios::binary);

        if (!in.is_open()) {
            std::cout << "Failed to open file" << std::endl;
            return 1;
        }

        AssetDeserializer deserializer(in, &package1);
        package1.deserialize(deserializer);
    }
}