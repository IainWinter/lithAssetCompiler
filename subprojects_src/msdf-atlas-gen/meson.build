headers = [
    'msdfgen/msdfgen-ext.h',
    'msdfgen/msdfgen.h'
]

sources = [
    'msdfgen/core/contour-combiners.cpp',
    'msdfgen/core/Contour.cpp',
    'msdfgen/core/edge-coloring.cpp',
    'msdfgen/core/edge-segments.cpp',
    'msdfgen/core/edge-selectors.cpp',
    'msdfgen/core/EdgeHolder.cpp',
    'msdfgen/core/equation-solver.cpp',
    'msdfgen/core/msdf-error-correction.cpp',
    'msdfgen/core/MSDFErrorCorrection.cpp',
    'msdfgen/core/msdfgen.cpp',
    'msdfgen/core/Projection.cpp',
    'msdfgen/core/rasterization.cpp',
    'msdfgen/core/render-sdf.cpp',
    'msdfgen/core/save-bmp.cpp',
    'msdfgen/core/save-tiff.cpp',
    'msdfgen/core/Scanline.cpp',
    'msdfgen/core/sdf-error-estimation.cpp',
    'msdfgen/core/shape-description.cpp',
    'msdfgen/core/Shape.cpp',
    'msdfgen/core/ShapeDistanceFinder.hpp',
    'msdfgen/core/SignedDistance.cpp',
    'msdfgen/core/Vector2.cpp',
    'msdfgen/ext/import-font.cpp',
    'msdfgen/ext/import-svg.cpp',
    'msdfgen/ext/resolve-shape-geometry.cpp',
    'msdfgen/ext/save-png.cpp',
]

deps = [
    dependency('freetype2'),
    dependency('tinyxml2', default_options: { 'tests': false })
]

include = include_directories(['msdfgen'])

target = static_library('msdfgen', headers + sources, include_directories: include, dependencies: deps)

msdfgen_dep = declare_dependency(include_directories: include, link_with: target)

# also build msdf-atlas-gen

sources = [
    'msdf-atlas-gen/artery-font-export.cpp',
    'msdf-atlas-gen/bitmap-blit.cpp',
    'msdf-atlas-gen/charset-parser.cpp',
    'msdf-atlas-gen/Charset.cpp',
    'msdf-atlas-gen/csv-export.cpp',
    'msdf-atlas-gen/FontGeometry.cpp',
    'msdf-atlas-gen/glyph-generators.cpp',
    'msdf-atlas-gen/GlyphGeometry.cpp',
    'msdf-atlas-gen/image-encode.cpp',
    'msdf-atlas-gen/json-export.cpp',
    'msdf-atlas-gen/RectanglePacker.cpp',
    'msdf-atlas-gen/shadron-preview-generator.cpp',
    'msdf-atlas-gen/size-selectors.cpp',
    'msdf-atlas-gen/TightAtlasPacker.cpp',
    'msdf-atlas-gen/utf8.cpp',
    'msdf-atlas-gen/Workload.cpp'
]

deps = [
    msdfgen_dep
]

msdf_atlas_args = ['-DMSDF_ATLAS_PUBLIC=', '-DMSDF_ATLAS_NO_ARTERY_FONT=1']

include = include_directories(['msdf-atlas-gen'])

target = static_library('msdf-atlas-gen', sources, include_directories: include, dependencies: deps, cpp_args: msdf_atlas_args)

msdf_atlas_gen_dep = declare_dependency(include_directories: include, link_with: target)
