#include "loaders.h"
#include "util/load_file.h"

#include <cstdlib>
#include <filesystem>

void loadShaderSPIRV(AssetPackage* package, const std::string& name, const std::string& path, std::string& err) {
    // Can this not make files?
    
    std::string spvPath = path + ".spv";
    std::string command = "glslc " + path + " -o " + spvPath;

    if (system(command.c_str()) != 0) {
        err = "Failed to compile shader";
        std::filesystem::remove(spvPath);
        return;
    }

    ShaderSPIRVAsset shader;
    shader.bytecode = loadFile(spvPath, err);

    std::filesystem::remove(spvPath);

    if (err != "") {
        // bubble up load file error
        return;
    }

    package->add(make_asset<ShaderSPIRVAsset>(AssetType_ShaderSPIRV, name, shader));
    err = "";
}