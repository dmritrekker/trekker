Trekker
=======

Trekker implements a state-of-the-art tractography algorithm, parallel transport tractography (PTT). PTT is capable of reconstructing geometrically smooth and topographically organized fiber bundles. Click `here <./publications/publications.html>`__ to learn how cite PTT in your publications.

Trekker software has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **.vtk** output, that is compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.


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


**Result of the above command, click to enlarge**

|pic1| |pic2|
|pic3| |pic4|

.. |pic1| image:: demo_1.png
	:width: 49%

.. |pic2| image:: demo_2.png
	:width: 49%
	
.. |pic3| image:: demo_3.png
	:width: 49%
	
.. |pic4| image:: demo_4.png
	:width: 49%

------------


.. toctree::
   :maxdepth: 2 
   :caption: Download and install
   :hidden:

   installation/howToGetAndCite
   installation/installationInstructions

.. toctree::
   :maxdepth: 2 
   :caption: Get started
   :hidden:

   tutorials/tutorial_1
   tutorials/tutorial_2


.. toctree::
   :maxdepth: 2
   :caption: Manual
   :hidden:

   manual/trekker

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

