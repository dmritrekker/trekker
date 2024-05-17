Trekker
=======

<img src="doc/source/_static/logo.svg" alt="Trekker Logo" align="center" width="150">

Trekker offers state-of-the-art fiber **tracking** and **filtering** algorithms:

- **Fiber Tracking:** Trekker uses the Parallel Transport Tractography (PTT) algorithm. PTT excels in reconstructing geometrically smooth and topographically organized fiber bundles.

- **Fiber Filtering:** Trekker employs an intuitive set of pathway rules, which can be specified using surface meshes, as well as commonly used image masks and partial volume fraction images.


Trekker has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Support for asymmetric FODs** provides a more flexible option for fiber tracking.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **.vtk** and **.tck** input/output, making the results compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.

For complete documentation, tutorials and examples, visit https://dmritrekker.github.io/.

**Fiber tracking example:**
```bash
./trekker --input FOD.nii.gz \
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
./trekker --input INP_TRACK.vtk \
          --seed WHITE_MATTER_SURFACE.gii \
          --pathway require_end_inside LEFT_THAL.nii.gz \
          --output OUT_TRACK.vtk

# Yes! You can use the --seed option when filtering too.
```


Installation
------------

**Stand-alone executables:**

Please find the stand-alone executables under [Releases](https://github.com/dmritrekker/trekker/releases).


## Building from Source

Trekker relies on [nibrary](https://github.com/nibrary/nibrary). To compile Trekker from source, please ensure nibrary is installed first. Follow these steps to compile Trekker:

#### Step 1: Download the Source Code

```bash
git clone https://github.com/dmritrekker/trekker
```

#### Step 2: Modify and run the build script

Make sure you have CMake 3.15 or newer installed.

- **For Linux:** Edit `build_Linux.sh`
- **For Windows:** Edit `build_Windows.bat`

Demo
----

To test your installation, please use the scripts under -> <TrekkerFolder>/demo


Publications:
------------

If you use Trekker, please cite [Aydogan2021], which provides a detailed explanation of the algorithm and the results obtained from extensive experiments. A precursory abstract was initially published during ISMRM 2019, Montreal [Aydogan2019], which can additionally be cited:

- [[Aydogan2021](https://ieeexplore.ieee.org/abstract/document/9239977/)] Aydogan D.B., Shi Y., "Parallel transport tractography", in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038.

- [[Aydogan2019](https://www.researchgate.net/publication/336847169_A_novel_fiber-tracking_algorithm_using_parallel_transport_frames)] Aydogan D.B., Shi Y., "A novel fiber tracking algorithm using parallel transport frames", Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019
