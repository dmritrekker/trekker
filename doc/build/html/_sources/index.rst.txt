Trekker
=======

Trekker implements a state-of-the-art tractography algorithm, parallel transport tractography (PTT). PTT is capable of reconstructing geometrically smooth and topographically organized fiber bundles. Click `here <./publications/publications.html>`__ to learn how cite PTT in your publications.

Trekker software has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Support for asymmetric FODs** provides a more flexible option for fiber tracking.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **Python package**, wrapped using Cython from Trekker's C/C++ code which provides easy access to PTT algorithm for python lovers.
- **.vtk** output, that is compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.

.. tip::

  - For latest stand-alone executables, click `here <https://github.com/dmritrekker/trekker/tree/master/binaries>`__.
  - |githublogo| Go to Trekker's source code by clicking `here <https://github.com/dmritrekker/trekker>`__.


.. |githublogo| image:: GitHub-Mark-32px.png
  :scale: 75 %

**A quick example**

::

   # Clean whole brain tractography with intuitive options
   ./trekker -fod FOD.nii.gz \
             -seed_image WHITEMATTER.nii.gz \
             -seed_count 100000 \
             -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \
             -pathway=discard_if_enters CSF.nii.gz \
             -minLength 20 \
             -maxLength 300 \
             -output OUTPUT.vtk

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
