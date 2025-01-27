#pragma once

#include "base/nibr.h"
#include "base/config.h"
#include "base/stringOperations.h"

// dMRI
#include "dMRI/gradient/dMRI_grad.h"
#include "dMRI/imageTypes/fod_image.h"
#include "dMRI/reconstruction/transhi2015/recon_transhi2015.h"

#include "dMRI/tractography/trekker.h"

#include "dMRI/tractography/algorithms/tractographyAlgorithm.h"
#include "dMRI/tractography/algorithms/ptt/ptf.h"
#include "dMRI/tractography/algorithms/ptt/algorithm_ptt.h"
#include "dMRI/tractography/algorithms/ptt/algorithm_ptt_params.h"

#include "dMRI/tractography/connectivity/tractogramConn_imageIndexer.h"
#include "dMRI/tractography/connectivity/tractogramConn_surfaceIndexer.h"

#include "dMRI/tractography/io/tractogramField.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "dMRI/tractography/io/tractogramWriter.h"

#include "dMRI/tractography/mappers/gridder_4DECsegmentLength.h"
#include "dMRI/tractography/mappers/gridder_4dMRIsim_stickZeppelinWM_GM_CSF.h"
#include "dMRI/tractography/mappers/gridder_4mask.h"
#include "dMRI/tractography/mappers/gridder_4segmentLength.h"
#include "dMRI/tractography/mappers/gridder_4streamlineCount.h"
#include "dMRI/tractography/mappers/gridder_4streamlineIdAndGaussianWeightedLength.h"
#include "dMRI/tractography/mappers/gridder_4streamlineIdAndLength.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"
#include "dMRI/tractography/mappers/tractogram2surfaceMapper.h"
#include "dMRI/tractography/mappers/tractogramMap_imageIndexer.h"
#include "dMRI/tractography/mappers/tractogramMap_surfaceIndexer.h"

#include "dMRI/tractography/pathway/pathway.h"
#include "dMRI/tractography/pathway/parseRule.h"

#include "dMRI/tractography/seed/seed.h"

#include "dMRI/tractography/tracker/tracker.h"
#include "dMRI/tractography/tracker/trackerThread.h"

#include "dMRI/tractography/utility/pathFilter.h"
#include "dMRI/tractography/utility/parallelStreamlineGenerator.h"
#include "dMRI/tractography/utility/resampleStreamline.h"
#include "dMRI/tractography/utility/segmentOperators.h"
#include "dMRI/tractography/utility/streamline_operators.h"
#include "dMRI/tractography/utility/tractogram_operators.h"

// EEG

// fMRI

// image
#include "image/image.h"
#include "image/image_operators.h"
#include "image/image_morphological.h"
#include "image/image_marchingCubes.h"
#include "image/sf_image.h"

// math
#include "math/sphere.h"
#include "math/conn3D.h"
#include "math/sphericalFunctions.h"
#include "math/sphericalHarmonics.h"
#include "math/triangle.h"
#include "math/randomThings.h"
#include "math/gaussian.h"
#include "math/reorient.h"
#include "math/PDE/FDM.h"

// surface
#include "surface/findSegmentTriangleIntersection.h"
#include "surface/surface2imageMapper.h"
#include "surface/surface_operators.h"
#include "surface/surface.h"

// utility
#include "utility/seeder/seeder.h"
#include "utility/seeder/seedingMethods.h"
#include "utility/seeder/seedImage.h"
#include "utility/seeder/seedSphere.h"
#include "utility/seeder/seedList.h"
#include "utility/seeder/seedSurface.h"
#include "utility/seeder/seedInsideSurface.h"

// cuda
#ifdef HAVE_CUDA
#include "base/cudaHandler.cuh"
#include "image/imageCuda.cuh"
#include "image/interpolationCuda.cuh"
#include "image/image_operatorsCuda.cuh"
#endif
