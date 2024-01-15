#include "loaders.h"

#include "msdf-atlas-gen.h"

void loadFont(AssetPackage* package, const std::string& name, const std::string& path, std::string& err) {
	//
	// config. not all of these are independent
	//

	// eventually this can use an image atlas instead of an image and
	// storing the uvs in the glyphs

	constexpr bool expensiveColoring = true;
	constexpr float angleThreshold = 3.0f;
	constexpr uint64_t logMultiplier = 6364136223846793005ull;
	constexpr uint64_t logIncrement = 1442695040888963407ull;
	constexpr uint32_t threadCount = 8;

	constexpr auto edgeColoringFunction = msdfgen::edgeColoringInkTrap;
	constexpr auto generatorFunction = msdf_atlas::msdfGenerator;

	constexpr int channelCount = (int)3;

	using storage_t = msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, channelCount>;
	using bitmap_t = msdfgen::BitmapConstRef<msdf_atlas::byte, channelCount>;
	using generator_t = msdf_atlas::ImmediateAtlasGenerator<float, channelCount, generatorFunction, storage_t>;

	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
	if (!ft) {
        err = "Failed to initialize freetype";
		return;
	}

	msdfgen::FontHandle* fontHandle = msdfgen::loadFont(ft, path.c_str());
	if (!fontHandle) {
		msdfgen::deinitializeFreetype(ft);

        err = "Failed to load font";
		return;
	}

	std::vector<msdf_atlas::GlyphGeometry> glyphs;

	msdf_atlas::FontGeometry fontGeometry(&glyphs);
	fontGeometry.loadCharset(fontHandle, 1.0, *msdf_atlas::Charset::ASCII);

	if (expensiveColoring) {
		uint64_t coloringSeed = 0; // what does this do?
		auto work = [edgeColoringFunction, &coloringSeed, &glyphs = glyphs](int i, int threadNo) -> bool {
			uint64_t glyphSeed = (logMultiplier * (coloringSeed ^ i) + logIncrement) * !!coloringSeed;
			glyphs[i].edgeColoring(edgeColoringFunction, angleThreshold, glyphSeed);
			return true;
		};

		msdf_atlas::Workload(work, glyphs.size()).finish(threadCount);
	}

	else {
		uint64_t glyphSeed = 0ull;
		for (msdf_atlas::GlyphGeometry& glyph : glyphs) {
			glyphSeed *= logMultiplier;
			glyph.edgeColoring(edgeColoringFunction, angleThreshold, glyphSeed);
		}
	}

	int width = 0;
	int height = 0;
	
	msdf_atlas::GeneratorAttributes attributes;
	attributes.config.overlapSupport = true;
	attributes.scanlinePass = true;

	msdf_atlas::TightAtlasPacker packer;
	packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_SQUARE);
	packer.setMinimumScale(32); // should allow user to provide
	packer.setPixelRange(2.0);
	packer.setMiterLimit(1.0);
	packer.pack(glyphs.data(), glyphs.size());
	packer.getDimensions(width, height);        // sets width and height with pass-by-ref

	generator_t generator(width, height);
	generator.setAttributes(attributes);
	generator.setThreadCount(threadCount);
	generator.generate(glyphs.data(), glyphs.size());
	
	bitmap_t bitmap = generator.atlasStorage();

    FontAsset font;

	for (const auto& g : fontGeometry.getGlyphs()) {
		double quadLeft, quadBottom, quadRight, quadTop;
		g.getQuadPlaneBounds(quadLeft, quadBottom, quadRight, quadTop);

		double uvLeft, uvBottom, uvRight, uvTop;
		g.getQuadAtlasBounds(uvLeft, uvBottom, uvRight, uvTop);

		vec2 posMin = vec2(quadLeft, quadBottom);
		vec2 posMax = vec2(quadRight, quadTop);
		vec2 uvMin = vec2(uvLeft, uvBottom) / vec2(width, height);
		vec2 uvMax = vec2(uvRight, uvTop) / vec2(width, height);

		float advance = g.getAdvance();
		int index = g.getIndex();
		uint32_t code = g.getCodepoint();

		FontAsset::Glyph glyph;
		glyph.posMin = posMin;
		glyph.posMax = posMax;
		glyph.uvMin = uvMin;
		glyph.uvMax = uvMax;
		glyph.advance = advance;
		glyph.index = index;
		glyph.character = code;

		font.glyphs.push_back(glyph);
	}

	for (const auto& kern : fontGeometry.getKerning()) {
		FontAsset::Kerning kerning;
		kerning.left = kern.first.first;
		kerning.right = kern.first.second;
		kerning.amount = (float)kern.second;

		font.kerning.push_back(kerning);
	}

	const auto& metrics = fontGeometry.getMetrics();
	font.topHeight = metrics.ascenderY;// + config.linePaddingTop;
	font.bottomHeight = -metrics.descenderY;// + config.linePaddingBottom;
	font.lineHeight = metrics.lineHeight;

	font.spaceAdvance = font.getGlyph(' ').advance;

	msdfgen::destroyFont(fontHandle);
	msdfgen::deinitializeFreetype(ft);

	size_t size = bitmap.width * bitmap.height * channelCount;

	ImageAsset img;
	img.pixels = FixedByteArray((const char*)bitmap.pixels, size);
	img.width = bitmap.width;
	img.height = bitmap.height;
	img.channels = channelCount;

	font.atlas = make_asset<ImageAsset>(AssetType_Image, name + ".atlas", img);

	package->add(make_asset<FontAsset>(AssetType_Font, name, font));
	package->add(font.atlas);

    err = {};
}