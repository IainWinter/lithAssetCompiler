#pragma once

#include "asset_package.h"
#include "assets/image.h"
#include "assets/shader_spirv.h"
#include "assets/font.h"

#include <string>

void loadImage      (AssetPackage* package, const std::string& name, const std::string& path, std::string& err);
void loadShaderSPIRV(AssetPackage* package, const std::string& name, const std::string& path, std::string& err);
void loadFont       (AssetPackage* package, const std::string& name, const std::string& path, std::string& err);