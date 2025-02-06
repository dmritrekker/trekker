Trekker
=======

.. note::

    February 6\ :sup:`th`, 2025

    **UPDATE:** Trekker v1.0.0-rc3 is out with major improvements and bug fixes! See below for a list of changes.

Trekker offers state-of-the-art tractography tools to study the structural connectivity of the brain. Trekker aims to improve fiber tracking pipelines by offering features like:

- **Well-organized, clean, probabilistic streamline tractography** using the parallel transport tractography (PTT) algorithm.
- A comprehensive set of **tractogram filtering** options, including support for anatomical constraints using surface meshes.
- Compatibility with **.vtk** format, to enable the use of powerful third-party 3D rendering software, such as `Paraview <https://www.paraview.org/>`__, for easy and high-quality visualizations.
- Support for asymmetric FODs, providing greater flexibility in fiber tracking.
- Several features designed for high-performance computing (HPC), including multithreading, time-limited tracking, and a user-friendly command line interface.
- Since 2017, algorithms and pipelines based on Trekker have earned awards in every international tractography competition, each featuring different themes.


.. seealso::

    🏆 **Awards and Recognitions**

    Tractography pipelines based on Trekker have earned awards in every international tractography challenge since 2017. Click `here <about/awards.html>`__ to learn more about Trekker's awards.



Installation
------------

Option 1: Stand-alone executables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Please find the stand-alone executables for Linux, Windows and macOS under `Releases <https://github.com/dmritrekker/trekker/releases>`_.

Option 2: Building from source
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Trekker is natively supported on Linux, Windows, and macOS. Trekker can be installed with a minimal set of standard development tools.

For detailed instructions to build from source, please check the `Readme <https://github.com/dmritrekker/trekker/tree/main>`_ file in GitHub.

Shortly, if your system has CMake (min version 3.15), OpenMP, and a modern compiler. The following should be sufficient for the compilation.

*Note*: clang-v19 is known NOT to work with this version of Trekker due to a conflict with an internal dependency. Please use clang-v18 instead.

**Step 1: Download the Source Code**

.. code-block:: bash

   git clone https://github.com/dmritrekker/trekker.git

**Step 2: Modify and run the build script**

- **For Linux:** Edit ``build_Linux.sh``
- **For Windows:** Edit ``build_win.sh``
- **For macOS:** Edit ``build_macOS.sh``



Usage
-----

Simply run ``trekker`` on the terminal. This will display help and the commands, which are listed below:

.. raw:: html


   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Command</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/info.html">info</a></strong></td>
           <td style="padding: 8px;">Displays information about input file.</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/track.html">track</a></strong></td>
           <td style="padding: 8px;">Performs fiber tracking using parallel transport tractography (PTT) algorithm. PTT excels in reconstructing geometrically smooth and topographically organized fiber bundles.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/filter.html">filter</a></strong></td>
           <td style="padding: 8px;">Trekker employs an intuitive set of pathway rules to filter tractograms. The rules can be specified with surface meshes, as well as with spheres, image masks and partial volume fractions.</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/track2img.html">track2img</a></strong></td>
           <td style="padding: 8px;">Maps tractogram features on an image.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/track2surf.html">track2surf</a></strong></td>
           <td style="padding: 8px;">Maps tractogram features on a surface.</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/img2track.html">img2track</a></strong></td>
           <td style="padding: 8px;">Maps image values on a tractogram.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/select.html">select</a></strong></td>
           <td style="padding: 8px;">Selects streamlines from a tractogram.</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/resample.html">resample</a></strong></td>
           <td style="padding: 8px;">Resamples streamlines in a tractogram.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/convert.html">convert</a></strong></td>
           <td style="padding: 8px;">Converts tractogram file formats (.vtk, .tck, .trk).</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/transform.html">transform</a></strong></td>
           <td style="padding: 8px;">Applies a transform on an input tractogram.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/diff.html">diff</a></strong></td>
           <td style="padding: 8px;">Finds different streamlines between two tractograms.</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/merge.html">merge</a></strong></td>
           <td style="padding: 8px;">Merges two tractograms, optionally ignoring duplicates.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/addColor.html">addColor</a></strong></td>
           <td style="padding: 8px;">Adds colors to streamlines (.vtk only).</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/fieldExport.html">fieldExport</a></strong></td>
           <td style="padding: 8px;">Exports a selected vtk field from a tractogram (.vtk only).</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px;"><strong><a href="../commands/fieldImport.html">fieldImport</a></strong></td>
           <td style="padding: 8px;">Adds a new vtk field with values read from a file (.vtk only).</td>
       </tr>
       <tr>
           <td style="padding: 8px;"><strong><a href="../commands/fieldRemove.html">fieldRemove</a></strong></td>
           <td style="padding: 8px;">Removes a vtk field from a tractogram (.vtk only).</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; border-bottom: 1px solid #000;"><strong><a href="../commands/dMRI.html">dMRI</a></strong></td>
           <td style="padding: 8px; border-bottom: 1px solid #000;">dMRI commands</td>
       </tr>
   </table>



Examples
--------

**Fiber tracking example:**

.. code-block:: bash

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

**Fiber filtering example:**

.. code-block:: bash

   # You can use the --seed option when filtering too.
   ./trekker filter \
             INP_TRACK.vtk \
             --seed WHITE_MATTER_SURFACE.gii \
             --pathway require_end_inside LEFT_THAL.nii.gz \
             --output OUT_TRACK.vtk

Release notes for Trekker-v1.0.0-rc3
------------------------------------

    - **New feature:** `track` can now save seed indices as a vtk field.

    - **Bug fix 1**: Fixed an issue when stopping rule was ignored when a seed was places on edges.

    - **Bug fix 2**: Fixed an issue with segment surface intersection.
    
    - **Bug fix 3**: `select` indexing fixed.
    
    - **Bug fix 4**: Fixed an issue preventing streamline truncation at max length in some cases.

    - **Bug fix 5**: Fixed MacOS support.

    - **Bug fix 6**: Fixed Windows support.

    - **Bug fix 7**: `tran_shi_2015` model no more gets stuck when running.

    - **Improvement 1**: Added stricter pathway rule validation for `track` and `filter`.

    - **Improvement 2**: In `filter`, changed option name `--seedTrials` to `--seed_trials` to be consistent with `track`. Also added `--output, -o` option for the same reason.

    - **Improvement 3**: Seedless filtering now works independent from online filtering and it works faster.

    - **Improvement 4**: Upgraded to CLI11-v2.4.2

    - **Improvement 5**: Provided stand-alone executables for macOS and Windows.

    - **Improvement 6**: Provided basic test routines.


Release notes for Trekker-v1.0.0-rc2
------------------------------------

    - **Bug fix 1**: Fixed a bug which prevented using negative labels for selecting surface ROIs.

    - **Bug fix 2**: Fixed a bug in `track2surf` that was introduced as a result of the major updates in rc1, which led to the saving of wrong field information for mapped streamlines on surface meshes.

    - **Improvement 1**: Added a new command `info`, which prints basic information about input files.

    - **Improvement 2**: Added a new command `dMRI recon tran_shi_2015`, which fits dMRI data onto a compartment model proposed in [TranShi2015]_.

    - **Improvement 3**: Added option to flip surface normals when defining pathway rules.

    - **Improvement 4**: Added option to use only the surface boundary when the provided mesh is closed for defining pathway rules.

    - **Improvement 5**: (*As a solution to the nibrary issue submitted by Xi Zhu*) Added option to use ascii (text) files for defining seed density for fiber tracking with surface-based seeding.

    - **Improvement 6**: Random seeds are now sampled using an approach based on cumulative distribution function instead of rejection sampling.


Release notes for Trekker-v1.0.0-rc1
------------------------------------

    - **Bug fix 1**: Fixed a bug in `track` where streamlines were not stopped as expected when a pathway rule was set. This issue occurred because `track` allowed stop options unsupported by nibrary. The fix involved providing nibrary with the necessary stopping features during tracking.
    
    - **Bug fix 2**: (*Thanks to John Kruper for spotting this one!*) Fixed a bug in `track` related to the computation of data support in parallel transport tracking. Previously, the contribution from the last part of the probe was reused as the first part's contribution in the next iteration to speed up computation. This fix ensures the data support contribution is computed only once for the first part and reused for other candidate probes. Although this bug has existed since v0.5, it should not significantly affect results with default probe quality (4) and length (0.25 x voxel size). However, it may impact streamlines computed with longer probe lengths and lower quality probes.

    - **Bug fix 3**: Fixed a bug affecting both `track` and `filter`, where streamlines were not following the `require_end_inside` rule. The issue occured because this rule was commented out during a part of the development, and it was forgotten to be restored.
    
    - **Improvement 1**: Reorganized the code to harmonize the handling of segment-surface interactions.

    - **Improvement 2**: Switched to the *double* datatype in certain functions to better manage edge cases.

    With the beta release (May 21\ :sup:`st`, 2024), Trekker has gone through significant changes:

    - Powered by `nibrary <https://github.com/nibrary/nibrary>`_, Trekker now features:
        - Support for surface based tracking and filtering
        - Advanced pathway rules for filtering streamlines
        - Subcommands to perform various tractogram operations
        - Improved command line interface
    - While the core fiber tracking algorithm (PTT) works the same, the new version of Trekker is not compatible with the previous versions due to the above changes.
        - *Remark:* Your previous code (script) written for earlier versions of Trekker need to be modified to work with the latest version.
    - Some existing features in earlier versions of Trekker are currently missing in the latest version:
        - Windows compatibility
        - Python and Matlab support
        - Exporting of a `.json` formatted tracking report

    Enjoy the new Trekker!

    ..
        December 19\ :sup:`th`, 2024

        **UPDATE:** Trekker v1.0.0-rc2 now out! See below for a list of major changes.

        August 19\ :sup:`th`, 2024

        **UPDATE:** We are excited to announce the first release candidate. Trekker v1.0.0-rc1 includes bug fixes and improvements, particularly in surface handling. See below for a list of major changes.

        May 21\ :sup:`st`, 2024

        **MAJOR UPDATE!** We are excited to announce the release of Trekker v1.0 beta, now available for testing.

        This version includes significant changes and improvements.

        - Powered by `nibrary <https://github.com/nibrary/nibrary>`_, Trekker now features:
            - Support for surface based tracking and filtering
            - Advanced pathway rules for filtering streamlines
            - Subcommands to perform various tractogram operations
            - Improved command line interface
        - While the core fiber tracking algorithm (PTT) works the same, the new version of Trekker is not compatible with the previous versions due to the above changes.
            - *Remark:* Your previous code (script) written for earlier versions of Trekker need to be modified to work with the latest version.
        - Some existing features in earlier versions of Trekker are currently missing in the latest version:
            - Windows compatibility
            - Python and Matlab support
            - Exporting of a `.json` formatted tracking report

        Enjoy the new Trekker!

------------

.. toctree::
   :maxdepth: 2
   :caption: Guide
   :hidden:

   guide/tutorial.rst
   guide/pathways.rst
   guide/FOD.rst


.. toctree::
   :maxdepth: 2
   :caption: Commands
   :hidden:

   commands/info.rst
   commands/track.rst
   commands/filter.rst
   commands/track2img.rst
   commands/track2surf.rst
   commands/img2track.rst
   commands/select.rst
   commands/resample.rst
   commands/convert.rst
   commands/transform.rst
   commands/diff.rst
   commands/merge.rst
   commands/addColor.rst
   commands/fieldExport.rst
   commands/fieldImport.rst
   commands/fieldRemove.rst
   commands/dMRI.rst


.. toctree::
   :maxdepth: 2
   :caption: About
   :hidden:

   about/awards
   about/licenseAndWarranty
   about/acknowledgements
   about/contact
