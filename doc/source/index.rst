Trekker
=======

Trekker offers state-of-the-art fiber **tracking** and **filtering** algorithms:

- **Fiber Tracking:** Trekker uses the Parallel Transport Tractography (PTT) algorithm. PTT excels in reconstructing geometrically smooth and topographically organized fiber bundles.

- **Fiber Filtering:** Trekker employs an intuitive set of pathway rules, which can be specified using surface meshes, as well as commonly used image masks and partial volume fraction images.

Click `here <./publications/publications.html>`__ to learn how cite PTT in your publications.

Trekker has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Support for asymmetric FODs** provides a more flexible option for fiber tracking.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **.vtk** and **.tck** input/output, making the results compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.

.. tip::

  - For latest stand-alone executables, click `here <https://github.com/dmritrekker/trekker/releases>`__.
  - |githublogo| Go to Trekker's source code by clicking `here <https://github.com/dmritrekker/trekker>`__.


.. |githublogo| image:: GitHub-Mark-32px.png
  :scale: 75 %

**A quick example**

::

   # Clean whole brain tractography with intuitive options

   ./trekker --input FOD.nii.gz \
          --seed WHITE_MATTER_SURFACE.gii \
          --seed_surf_useSurfNorm \
          --seed_count 100000 \
          --pathway discard_if_ends_inside WHITE_MATTER.nii.gz \
          --pathway discard_if_enters CSF.nii.gz \
          --minLength 20 \
          --maxLength 300 \
          --output OUT_TRACK.vtk

------------


.. toctree::
   :maxdepth: 2
   :caption: Download and install
   :hidden:

   installation/howToGetAndCite

.. toctree::
   :maxdepth: 2
   :caption: Get started
   :hidden:

   tutorials/tutorial_1
   tutorials/tutorial_2
   tutorials/tutorial_3

.. toctree::
   :maxdepth: 2
   :caption: Manual
   :hidden:

   manual/trekker


.. toctree::
   :maxdepth: 2
   :caption: Tips
   :hidden:

   tips/tip1


.. toctree::
   :maxdepth: 2
   :caption: Publications
   :hidden:

   publications/publications

.. toctree::
   :maxdepth: 2
   :caption: About
   :hidden:

   about/licenseAndWarranty
   about/acknowledgements
   about/contact
