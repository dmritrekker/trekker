# Trekker

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) [![Shared build](https://github.com/dmritrekker/trekker/actions/workflows/build_shared.yml/badge.svg?branch=build)](https://github.com/dmritrekker/trekker/actions/workflows/build_shared.yml) [![Static build](https://github.com/dmritrekker/trekker/actions/workflows/build_static.yml/badge.svg?branch=build)](https://github.com/dmritrekker/trekker/actions/workflows/build_static.yml) [![Test](https://github.com/baranaydogan/trekker/actions/workflows/test.yml/badge.svg?branch=test)](https://github.com/baranaydogan/trekker/actions/workflows/test.yml)

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

### Stand-alone executables

Download the appropriate stand-alone executable for your operating system:

*   **Linux:** :download: [linux_binary.zip](https://github.com/dmritrekker/trekker/releases/download/v1.0.0-rc4/linux_binary.zip)
*   **macOS:** :download: [macOS-universal_binary.zip](https://github.com/dmritrekker/trekker/releases/download/v1.0.0-rc4/macOS-universal_binary.zip)
*   **Windows:** :download: [windows_binary.zip](https://github.com/dmritrekker/trekker/releases/download/v1.0.0-rc4/windows_binary.zip)

After downloading and extracting the archive, you can run the executable.

**Testing:**

- Download and extract the datasets and scripts we prepared for testing by clicking here, [tests.zip](https://github.com/dmritrekker/trekker/releases/download/v1.0.0-rc4/tests.zip).
- Copy paste your executable under the ``testScripts`` folder.
- Run the provided test script for your operating system.


**Troubleshooting:** 

- *permission denied error*: If you encounter this when trying to run the executable on Linux or macOS, open a terminal, navigate to the directory where you extracted the file, and run the following command:

```bash
chmod +x <executable name>
```

Replace ``<executable name>`` with the actual name of the executable file (e.g., ``trekker_linux`` or ``trekker_macOS``).  Then, try running the executable again.

- *macOS errors related to llvm and libomp*: If you encounter errors related to missing OpenMP or libomp, please see the provided test script for macOS in this [link](https://github.com/dmritrekker/trekker/blob/main/tests/testScripts/test_macOS.sh), which gives an example for how to set these paths and install dependencies if needed.



### Building from source

Trekker is natively supported on Linux, Windows, and macOS. Trekker can be installed with a minimal set of standard development tools.

*   **CMake**: min v3.15.0
*   **OpenMP**
*   **C/C++ Compiler**: The following compilers have been tested and are known to work:
    *   **GCC** (min v9.0)
    *   **Clang**: v18.0.0 (v19 is known not to work)
    *   **Microsoft Visual C++ (MSVC)**: Visual Studio 2022

To install Trekker, first clone the repository: 

```bash
git clone https://github.com/dmritrekker/trekker.git
```

Then follow the instructions below.


#
#### Linux

#### 1. Install dependencies:

Debian/Ubuntu:
```bash
sudo apt install cmake libomp-dev build-essential
```

Fedora/CentOS/RHEL:
```bash
sudo dnf install cmake openmp-devel
sudo dnf group install "Development Tools"
```

Arch Linux/Manjaro:
```bash
sudo pacman -S cmake openmp
sudo pacman -S base-devel
```

#### 2. Run the build script:

The following will install a statically built executable under the `build-static` folder using the default compiler in the system.
```bash
cd trekker
sh build_linux.sh
```


Edit the `build_linux.sh` script to customize your installation.

#
#### macOS

The provided installation script for macOS, `build_mac.sh`, will install the dependencies, set the environment variables, compile and install trekker under the `build-static` folder.

```bash
cd trekker
sh build_macOS.sh
```

Note that trekker requires `llvm` and `libomp` in macOS. We have successfully tested `llvm` v18 to build nibrary; however, compilation with the more recent version, v19, fails. The provided build script installs `llvm@18` and `libomp` in the system. However, it does not permanently set environment variables for future use. The `build_mac.sh` script contains information about how to permanently set these environment variables if needed.

Edit the `build_mac.sh` script to customize your installation.

#
#### Windows

Install Visual Studio 2022 (other versions might work too but they have not been tested). Open command window and use the following to install a statically built library under the `build-static` folder:

```cmd
cd trekker
call build_win.bat
```

Edit the `build_win.bat` script to customize your installation.

#
### Testing

You can test your executable using the test scripts and data provided under the `tests` folder. For that:

1. copy paste your executable under the `tests/testScripts`
2. rename your executable as:
    - For Linux: `trekker_linux`
    - For macOS: `trekker_macOS` 
    - For Windows: `trekker_win.exe`
3. Run the test script:
    - For Linux: `sh test_linux.sh`
    - For macOS: `sh test_macOS.sh`
    - For Windows: `call test_win.bat`

**Note:** The same testing protocol is used under the `test` branch of this repository before sharing the stand-alone-executables.

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
          --output OUT_TRACK.vtk
```


Publications:
------------

If you use Trekker, please cite [Aydogan2021], which provides a detailed explanation of the algorithm and the results obtained from extensive experiments. A precursory abstract was initially published during ISMRM 2019, Montreal [Aydogan2019], which can additionally be cited:

- [[Aydogan2021](https://ieeexplore.ieee.org/abstract/document/9239977/)] Aydogan D.B., Shi Y., "Parallel transport tractography", in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038.

- [[Aydogan2019](https://www.researchgate.net/publication/336847169_A_novel_fiber-tracking_algorithm_using_parallel_transport_frames)] Aydogan D.B., Shi Y., "A novel fiber tracking algorithm using parallel transport frames", Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019
