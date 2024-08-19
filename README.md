# Trekker

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

Trekker relies on [nibrary](https://github.com/nibrary/nibrary). To compile Trekker from source, ensure that nibrary is installed first. Then follow these steps to compile Trekker:

#### Step 1: Download the Source Code

```bash
git clone https://github.com/dmritrekker/trekker
```

#### Step 2: Modify and run the build script

- **For Linux:** Edit `build_Linux.sh`

**Note:** Trekker binaries can be compiled using the following version matching with [nibrary](https://github.com/nibrary/nibrary):

| Trekker version    | nibrary version|
|--------------------|-------------|
| [master](https://github.com/dmritrekker/trekker/tree/master)  | [main](https://github.com/nibrary/nibrary/tree/main) |
| [v1.0.0-rc1](https://github.com/dmritrekker/trekker/tree/v1.0.0-rc1)   | [v0.1.0](https://github.com/nibrary/nibrary/tree/v0.1.0) |
| [v1.0-beta](https://github.com/dmritrekker/trekker/tree/v1.0-beta)   | [v0.0.0](https://github.com/nibrary/nibrary/tree/v0.0.0) |

## Usage

Simply run `trekker` on the terminal. This will display help and the commands, which are listed below:

| Command    | Description |
|------------|-------------|
| **track**  | performs fiber tracking using parallel transport tractography (PTT) algorithm. PTT excels in reconstructing geometrically smooth and topographically organized fiber bundles. |
| **filter** | Trekker employs an intuitive set of pathway rules to filter tractograms. The rules can be specified with surface meshes, as well as with spheres, image masks and partial volume fractions. |
|**track2img**| maps tractogram features on an image |
|**track2surf**| maps tractogram features on a surface |
|**img2track**| maps image values on a tractogram |
|**select**| selects streamlines from a tractogram |
|**resample**| resamples streamlines in a tractogram |
|**convert**| converts tractogram file formats (.vtk, .tck, .trk) |
|**transform**| applies a transform on an input tractogram |
|**diff**| finds different streamlines between two tractograms |
|**merge**| merges two tractograms, optionally ignoring duplicates |
|**addColor**| adds colors to streamlines (.vtk only) |
|**fieldExport**| exports a selected vtk field from a tractogram (.vtk only) |
|**fieldImport**| adds a new vtk field with values read from a file (.vtk only) |
|**fieldRemove**| removes a vtk field from a tractogram (.vtk only) |


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
