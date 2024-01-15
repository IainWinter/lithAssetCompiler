#include "loaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

void loadImage(AssetPackage* package, const std::string& name, const std::string& path, std::string& err) {
	int width, height, channels, format;
	stbi_info(path.c_str(), &width, &height, &channels);

	switch (channels) {
        case 1: format = STBI_grey;       break;
        case 2: format = STBI_grey_alpha; break;
        case 3: format = STBI_rgb;        break;
        case 4: format = STBI_rgb_alpha;  break;
        default:
            err = "Failed to load image, invalid number of channels";
            return;
	}

	char* pixels = (char*)stbi_load(path.c_str(), &width, &height, &channels, format);

	if (!pixels) {
        err = "Failed to load image, no pixels";
        return;
	}

    size_t size = width * height * channels;

    ImageAsset image;
    image.pixels = FixedByteArray(pixels, size);
    image.width = width;
    image.height = height;
    image.channels = channels;

    package->add(make_asset<ImageAsset>(AssetType_Image, name, image));
    err = "";

    STBI_FREE(pixels);
}