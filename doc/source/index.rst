Trekker
=======

.. note::


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





Trekker offers state-of-the-art tractography tools to study the structural connectivity of the brain. Trekker aims to improve fiber tracking pipelines by offering features like:

- **Well-organized, clean, probabilistic streamline tractography** using the parallel transport tractography (PTT) algorithm.
- A comprehensive set of **tractogram filtering** options, including support for anatomical constraints using surface meshes.
- Compatibility with **.vtk** format, to enable the use of powerful third-party 3D rendering software, such as `Paraview <https://www.paraview.org/>`__, for easy and high-quality visualizations.
- Support for asymmetric FODs, providing greater flexibility in fiber tracking.
- Several features designed for high-performance computing (HPC), including multithreading, time-limited tracking, and a user-friendly command line interface.



Installation
------------

Option 1: Stand-alone executables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Please find the stand-alone executables under `Releases <https://github.com/dmritrekker/trekker/releases>`_.

Option 2: Building from source
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Trekker relies on `nibrary <https://github.com/nibrary/nibrary>`_. To compile Trekker from source, ensure that nibrary is installed first. Then follow these steps to compile Trekker:

**Step 1: Download the Source Code**

.. code-block:: bash

   git clone https://github.com/dmritrekker/trekker

**Step 2: Modify and run the build script**

- **For Linux:** Edit ``build_Linux.sh``
- **For Windows:** Edit ``build_Windows.bat``



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
           <td style="padding: 8px; border-bottom: 1px solid #000;"><strong><a href="../commands/fieldRemove.html">fieldRemove</a></strong></td>
           <td style="padding: 8px; border-bottom: 1px solid #000;">Removes a vtk field from a tractogram (.vtk only).</td>
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
             OUT_TRACK.vtk




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


.. toctree::
   :maxdepth: 2
   :caption: About
   :hidden:

   about/licenseAndWarranty
   about/acknowledgements
   about/contact
