Trekker
=======

Trekker implements a state-of-the-art tractography algorithm, parallel transport tractography (PTT). PTT is capable of reconstructing geometrically smooth and topographically organized fiber bundles.

Trekker software has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Support for asymmetric FODs** provides a more flexible option for fiber tracking.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **Python package**, wrapped using Cython from Trekker's C/C++ code which provides easy access to PTT algorithm for python lovers.
- **.vtk** output, that is compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.

For complete documentation, tutorials and examples, visit https://dmritrekker.github.io/.

**Example:**

    #Clean whole brain tractography with intuitive options

    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 100000 \
              -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \
              -pathway=discard_if_enters CSF.nii.gz \
              -minLength 20 \
              -maxLength 300 \
              -output OUTPUT.vtk


Installation
------------


**Stand-alone executables:**

Latest binaries are under: https://github.com/baranaydogan/trekker/tree/master/binaries


**Building from source:**

Step 1. Download the source code:

	git clone https://github.com/dmritrekker/trekker

Step 2. Modify the first few lines in the build script (make sure to have cmake-3.15 or newer):

- For Linux -> build_Linux.sh
- For Windows -> build_Windows.bat

Step 3. Run the build script. This will build Trekker under:

- For Linux -> <TrekkerFolder>/build/Linux/install
- For Windows -> <TrekkerFolder>/build/Windows/install

**Python package:**

A python package can be built from source. You can also download a built distributions (.whl) from the /binaries folder.

To install the python package use:

    pip install Trekker-<VERSION>.whl


Demo
----

To test your installation, please use the scripts under -> <TrekkerFolder>/demo


Publications:
------------

If you use Trekker, please cite [Aydogan2021], which provides a detailed explanation of the algorithm and the results obtained from extensive experiments. A precursory abstract was initially published during ISMRM 2019, Montreal [Aydogan2019], which can additionally be cited:

- [[Aydogan2021](https://ieeexplore.ieee.org/abstract/document/9239977/)] Aydogan D.B., Shi Y., "Parallel transport tractography", in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038.

- [[Aydogan2019](https://www.researchgate.net/publication/336847169_A_novel_fiber-tracking_algorithm_using_parallel_transport_frames)] Aydogan D.B., Shi Y., "A novel fiber tracking algorithm using parallel transport frames", Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019
