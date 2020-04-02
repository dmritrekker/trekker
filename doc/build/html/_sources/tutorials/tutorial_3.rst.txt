Tutorial 3: Python package usage
================================

Use :code:`pip install Trekker` to install the trekker python 3.7 package in Linux or Windows.

::

    # Tutorial 3 - Example 1
    import Trekker

    myTrekker=Trekker.initialize(b"Fod.nii")
    myTrekker.seed_image(b"Seed.nii")
    myTrekker.seed_count(1000)

    streamlines = myTrekker.run();

All parameters explained in the `manual <../manual/trekker.html>`__ can be set as demonstrated in the example above. Exceptions to these are the output options which can't be specified yet, i.e., only the output streamline coordinates are returned, and it is not at the moment possible to save colors or any other information associated with the streamlines.

**Notes on initialization**

Python package of Trekker allows one to *initialize* only a single FOD image at a time. Initialization can be done with 1, 3 or 5 arguments as explained below:

- *myTrekker=Trekker.initialize(b"Fod.nii")* : FOD image will be descretized and the default orientation of "XYZ" will be used.

- *myTrekker=Trekker.initialize(b"Fod.nii", orientation=b"xYZ", discretization=False)* : The default parameters for orientation and discretization can be changed.

- *myTrekker=Trekker.initialize(b"FOD_spherical_function_values.nii", b"FOD_spherical_function_domain.txt", b"asym",orientation=b"xYZ", discretization=True)* : A spherical function instead of spherical harmonic coefficients can be used as input.
