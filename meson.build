project(
    'trekker',
    'cpp', 'c',
    version: '0.4'
)

cc = meson.get_compiler('c')

cpp_args = []
multithreading_feature = not get_option('NO_MULTITHREADING')
if multithreading_feature
    cpp_args='-D ENABLE_MULTITHREADING'
endif
thread_dep = dependency('threads')

incdir = include_directories(
    'src/base',
    'src/math',
    'src/image',
    'src/tracker',
    'src/config',
    'src/nifticlib-2.0.0/znzlib',
    'src/nifticlib-2.0.0/niftilib'
)

m_dep = cc.find_library('m', required : false)

zdep = dependency(
    'zlib',
    required: false
)

libznz = library(
    'znzlib',
    sources: 'src/nifticlib-2.0.0/znzlib/znzlib.c',
    include_directories: incdir,
)

libnifti = library(
    'libnifti',
    sources: 'src/nifticlib-2.0.0/niftilib/nifti1_io.c',
    include_directories: incdir,
    link_with: libznz,
    dependencies: [m_dep, zdep]
)

sources = [
    'src/trekker.cpp',
    'src/image/image.cpp',
    'src/image/voxel.cpp',
    'src/image/fod_image.cpp',
    'src/image/roi_image.cpp',
    'src/base/streamline.cpp',
    'src/base/tractogram.cpp',
    'src/base/inputParser.cpp',
    'src/base/algorithm.cpp',
    'src/math/math_aux.cpp',
    'src/math/coordinate.cpp',
    'src/math/sphericalHarmonics.cpp',
    'src/math/doRandomThings.cpp',
    'src/tracker/tracker_thread.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic_initialize.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic.cpp',
    'src/tracker/algorithms/local_probabilistic/streamline_local_probabilistic.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic_defaults.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic_estimatePosteriorMax.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic_propagate.cpp',
    'src/tracker/algorithms/local_probabilistic/tractogram_local_probabilistic.cpp',
    'src/tracker/algorithms/local_probabilistic/algorithm_local_probabilistic_rejectionSample.cpp',
    'src/tracker/algorithms/ptt/streamline_ptt.cpp',
    'src/tracker/algorithms/ptt/fast_PTF_discretizer.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_rejectionSample.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_defaults.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_propagate.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_estimatePosteriorMax.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_initialize.cpp',
    'src/tracker/algorithms/ptt/fast_PTF.cpp',
    'src/tracker/algorithms/ptt/tractogram_ptt.cpp',
    'src/tracker/algorithms/ptt/algorithm_ptt_likelihoodandposterior.cpp',
    'src/config/config_tracker.cpp',
    'src/config/config_seeding.cpp',
    'src/config/config_general.cpp',
    'src/config/config_pathway.cpp',
    'src/config/config_output.cpp',
]

executable(
    'trekker',
    sources,
    include_directories: incdir,
    link_with: [libnifti, libznz],
    dependencies: thread_dep,
    cpp_args: cpp_args
)
status = [
        '@0@ @1@'.format(meson.project_name(), meson.project_version()),
        'MULTITHREADING:                 @0@'.format(multithreading_feature),
]

message('\n         '.join(status))
