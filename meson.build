project('lithAssetCompiler', 'cpp', default_options: ['cpp_std=c++20'])

subdir('subprojects_src/msdf-atlas-gen')

sources = [
	'src/main.cpp',
	'src/asset_package.cpp',
	'src/asset_serializer.cpp',
	'src/loaders/font_loader.cpp',
	'src/loaders/image_loader.cpp',
	'src/loaders/shader_spirv_loader.cpp',
]

includes = [
	include_directories('include'),
	include_directories('src'),
]

deps = [
	msdf_atlas_gen_dep,
	msdfgen_dep,

	dependency('glm')
]

args = msdf_atlas_args

executable('lithAssetCompiler', sources, include_directories: includes, dependencies: deps, cpp_args: args)

#
# Compile just the asset importing, not loading from file, part into a static library, may move
#

sources_importer = [
	'src/asset_package.cpp',
	'src/asset_serializer.cpp',
]

deps_importer = [
	dependency('glm')
]

asset_compiler_lib = static_library('lithAssetImporter', sources_importer, include_directories: includes, dependencies: deps_importer)