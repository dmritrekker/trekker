Tutorial 3: Python usage
========================

Use :code:`pip install trekker` to install the trekker python 3.7 package in Linux or Windows.

::

    # Tutorial 3 - Example 1
    import Trekker

    myTrekker=Trekker.tracker(b"Fod.nii");
    myTrekker.seed_image(b"Seed.nii");
    myTrekker.seed_count(1000);

    streamlines = myTrekker.run();

Trekker's python package is still under active development and it does not support all the functionality. Below are examples for how to use the currently available options: (unset parameters are set to default values explained in the `manual <../manual/trekker.html>`__)

**Global options**

- numberOfThreads(8);
- timeLimit(10);

**Tracker options**

- orderOfDirections(b"xZy");
- algorithm(b"ptt C1");
- stepSize(0.01);
- minRadiusOfCurvature(0.5);
- minFODamp(0.05);
- dataSupportExponent(1);
- minLength(30);
- maxLength(200);
- atMaxLength(b"discard");
- writeInterval(50);
- directionality(b"one_sided");
- maxEstInterval(1)
- initMaxEstTrials(50);
- propMaxEstTrials(20);
- maxSamplingPerStep(10000);
- useBestAtInit(false);

**Probe options**

- probeLength(0.25);
- probeRadius(0);
- probeCount(1);
- probeQuality(5);
- ignoreWeakLinks(0.005);

**Seeding options**

- seed_image(b"WM.nii")
- seed_image_using_label(b"aseg.nii",41)
- seed_coordinates(coords); # *coords is a 2D numpy array with dimensions (N,3)*
- seed_coordinates_with_directions(coords,dirs); # *dirs is a 2D numpy array with dimensions (N,3)*
- seed_count(1000);
- seed_countPerVoxel(10);
- seed_maxTrials(1000);
