#pragma once

#include "image.h"

#include "glm/vec2.hpp"
using namespace glm;

struct FontAsset : Asset {
	struct Glyph {
		vec2 posMin;
		vec2 posMax;
		vec2 uvMin;
		vec2 uvMax;

		float advance;
		int index;
		int character;
	};

	struct Kerning {
		int left;
		int right;
		float amount;
	};

	std::shared_ptr<ImageAsset> atlas;

	std::vector<Glyph> glyphs;
	std::vector<Kerning> kerning;
	
	float spaceAdvance;
	float lineHeight;
	float topHeight;
	float bottomHeight;

	Glyph getGlyph(int character) const {
		for (const auto& glyph : glyphs) {
			if (glyph.character == character) {
				return glyph;
			}
		}

		return {};
	}

	float getKerning(int leftCharacter, int rightCharacter) const {
		for (const auto& kerning : kerning) {
			if (kerning.left == leftCharacter && kerning.right == rightCharacter) {
				return kerning.amount;
			}
		}

		return 0.0f;
	}

	void serialize(AssetSerializer& out) const override {
		out << atlas
			<< spaceAdvance
			<< lineHeight
			<< topHeight
			<< bottomHeight;

		out << glyphs.size();
		for (const auto& glyph : glyphs) {
			out << glyph.posMin
				<< glyph.posMax
				<< glyph.uvMin
				<< glyph.uvMax
				<< glyph.advance
				<< glyph.index
				<< glyph.character;
		}

		out << kerning.size();
		for (const auto& kerning : kerning) {
			out << kerning.left
				<< kerning.right
				<< kerning.amount;
		}
	}

	void deserialize(AssetDeserializer& in) override {
		in >> atlas
		   >> spaceAdvance
		   >> lineHeight
		   >> topHeight
		   >> bottomHeight;

		size_t numGlyphs = 0;
		in >> numGlyphs;
		glyphs.resize(numGlyphs);
		for (auto& glyph : glyphs) {
			in >> glyph.posMin
			   >> glyph.posMax
			   >> glyph.uvMin
			   >> glyph.uvMax
			   >> glyph.advance
			   >> glyph.index
			   >> glyph.character;
		}

		size_t numKerning = 0;
		in >> numKerning;
		kerning.resize(numKerning);
		for (auto& kerning : kerning) {
			in >> kerning.left
			   >> kerning.right
			   >> kerning.amount;
		}
	}
};