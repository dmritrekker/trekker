# Trekker

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) [![Shared build](https://github.com/baranaydogan/trekker/actions/workflows/build_shared.yml/badge.svg)](https://github.com/baranaydogan/trekker/actions/workflows/build_shared.yml) [![Static build](https://github.com/baranaydogan/trekker/actions/workflows/build_static.yml/badge.svg)](https://github.com/baranaydogan/trekker/actions/workflows/build_static.yml)


<img src="doc/source/_static/logo_github.png" alt="Trekker Logo" align="center" width="150">

Trekker offers state-of-the-art tractography tools to study the structural connectivity of the brain. Trekker aims to improve fiber tracking pipelines by offering features like:

- **Well-organized, clean, probabilistic streamline tractography** using the parallel transport tractography (PTT) algorithm.
- A comprehensive set of **tractogram filtering** options, including support for anatomical constraints using surface meshes.
- Compatibility with **.vtk** format, to enable the use of powerful third-party 3D rendering software, such as [Paraview](https://www.paraview.org/), for easy and high-quality visualizations.
- **Support for asymmetric FODs**, providing greater flexibility in fiber tracking.
- Several features designed for high-performance computing (HPC), including **multithreading**, **time-limited tracking**, and a user-friendly **command line interface**.

🏆 **Awards and Recognitions:**

Tractography pipelines based on Trekker have earned awards in every international tractography challenge since 2017. Click [here](https://dmritrekker.github.io/about/awards.html) to read more about Trekker's awards.

For complete documentation, tutorials and examples, visit https://dmritrekker.github.io/.

## Installation

**Stand-alone executables:** Please find the stand-alone executables under [Releases](https://github.com/dmritrekker/trekker/releases).


### Building from Source

#### Step 1: Download the Source Code

```bash
git clone https://github.com/dmritrekker/trekker
```

#### Step 2: Modify and run the build script

```bash
sh build.sh
```


### Examples

**Fiber tracking example:**
```bash
./trekker track \
          FOD.nii.gz \
          --seed WHITE_MATTER_SURFACE.gii \
          --seed_surf_useSurfNorm \
          --seed_count 100000 \
          --pathway discard_if_ends_inside WHITE_MATTER.nii.gz \
          --pathway discard_if_enters CSF.nii.gz \
          --minLength 20 \
          --maxLength 300 \
          --output OUT_TRACK.vtk
```

**Fiber filtering example:**
```bash
# You can use the --seed option when filtering too.
./trekker filter \
          INP_TRACK.vtk \
          --seed WHITE_MATTER_SURFACE.gii \
          --pathway require_end_inside LEFT_THAL.nii.gz \
          OUT_TRACK.vtk
```


Publications:
------------

If you use Trekker, please cite [Aydogan2021], which provides a detailed explanation of the algorithm and the results obtained from extensive experiments. A precursory abstract was initially published during ISMRM 2019, Montreal [Aydogan2019], which can additionally be cited:

- [[Aydogan2021](https://ieeexplore.ieee.org/abstract/document/9239977/)] Aydogan D.B., Shi Y., "Parallel transport tractography", in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038.

- [[Aydogan2019](https://www.researchgate.net/publication/336847169_A_novel_fiber-tracking_algorithm_using_parallel_transport_frames)] Aydogan D.B., Shi Y., "A novel fiber tracking algorithm using parallel transport frames", Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019
