trekker
=============================================

Synopsis
--------

diffusion MRI based fiber tracking using parallel transport tractography

Usage
--------

::

    trekker [ options ]


Description
-----------

Trekker expects at least the following options to be specified:

- **-fod**: Trekker generates streamlines from a fiber orientation distribution (FOD) image provided by the user with this option.
- **-seed_image** or **-seed_coordinates**: Trekker expects the user to provide seeds using either of these options.
- **-seed_count** or **-seed_countPerVoxel** or **-timeLimit**: If a seed image is provided, the user can provide the number of streamlines to be generated either with the option *-seed_count* or *-seed_countPerVoxel*. If *-seed_coordinates* options is used, trekker will generate one streamline for each specified coordinate. Alternatively the *-timeLimit* option can be used to stop tracking after a certain amount of time.
- **-output**: Lastly the user has to provide the output path with *-output* followed by the path and name of the output file. trekker's output file format is \*.vtk. An additional .json formatted metadata is also saved.
- All unspecified parameters will be set to trekker's default values.

::

    # Example 1
    ./trekker -fod FOD.nii.gz \
              -seed_image SEED.nii.gz \
              -seed_count 1000 \
              -output OUTPUT.vtk

::

    # Example 2
    ./trekker -fod FOD.nii.gz \
              -seed_coordinates COORDINATES \
              -output OUTPUT.vtk

::

    # Example 3
    ./trekker -fod FOD.nii.gz \
              -seed_image SEED.nii.gz \
              -timeLimit 15 \ # time limit is in minutes
              -output OUTPUT.vtk



Options
-------

Tracking options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**-fod <nifti image of spherical harmonic coefficients>** or **-fod <nifti image of spherical function values> <list of spherical coordinates> <either \"sym\" or \"asym\">** Starting with version 0.5, trekker supports both symmetric and asymmetric FODs that can be represented with even or odd ordered spherical harmonics.

There are two ways to provide the input FOD image:

**1.** as spherical harmonic coefficients

The FOD image stores each coefficient as a separate volume with respect to spherical harmonic degree, l and m, as shown in the tables below. Based on the number of volumes in the image, trekker automatically identifies if the FOD is symmetric or asymmetric.

.. table:: For symmetric FOD

	+---+------+------+------+------+------+------+------+------+------+-----+
	|   | vol1 | vol2 | vol3 | vol4 | vol5 | vol6 | vol7 | vol8 | vol9 | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+
	| l | 0    | 2    | 2    | 2    | 2    | 2    | 4    | 4    | 4    | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+
	| m | 0    | -2   | -1   | 0    | 1    | 2    | -4   | -3   | -2   | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+


.. table:: For asymmetric FOD

	+---+------+------+------+------+------+------+------+------+------+-----+
	|   | vol1 | vol2 | vol3 | vol4 | vol5 | vol6 | vol7 | vol8 | vol9 | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+
	| l | 0    | 1    | 1    | 1    | 2    | 2    | 2    | 2    | 2    | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+
	| m | 0    | -1   | 0    | 1    | -2   | -1   | 0    | 1    | 2    | ... |
	+---+------+------+------+------+------+------+------+------+------+-----+

**2.** as a spherical function defined on provided vertices of a sphere

In this case, FOD image stores a spherical function at each voxel. Volume *n* of the FOD image corresponds to the value of the spherical function at the coordinates provided in line *n* of the vertices of the sphere that should be provided after the image. Additionally user should specify if the function is symmetric or asymmetric.

::

    # Example 4
    ./trekker -fod FOD_spherical_function_values.nii.gz FOD_spherical_function_domain.txt asym \
              -seed_image SEED.nii.gz \
              -seed_count 1000 \
              -output OUTPUT.vtk

**-orderOfDirections <one of the 48 direction options>** Switches direction of FOD. By default XYZ is assumed (no change). A lower case can be used to switch the direction, e.g., xYZ switches the sign of the first component. The order of components can be changed as well, e.g., yXz switches the sign of the second component while replacing it with the first, at the same time the sign of the last component is also changed.

::

    # Example 5
    ./trekker -fod FOD.nii.gz \
              -orderOfDirections Zyx \
              -seed_image SEED.nii.gz \
              -seed_count 1000 \
              -output OUTPUT.vtk


**-dontDiscretizeFod** Turns of FOD discretization. For faster tracking, trekker by default will discretize the FOD on a sphere (using 2076 points for symmetric functions and 1004 points for asymmetric functions).

**-stepSize <positive real number>** Step size. Default=1/40 x the smallest of the FOD voxel dimensions.

**-minRadiusOfCurvature <positive real number>** Minimum radius of curvature. Default=1/2 x the smallest of the FOD voxel dimensions.

**-minFODamp <positive real number>** Threshold for minimum data support. For PTT algorithm this is minimum FOD threshold. Default=0.05.

**-dataSupportExponent <positive real number>** Data support to the power dataSupportExponent is used for rejection sampling. Default=1.

**-minLength <positive real number>** Minimum length of the tracks. Default=0.

**-maxLength <positive real number>** Maximum length of the tracks. Default=infinite.

**-atMaxLength <either \"stop\" or \"discard\">** If set to \"stop\", when maxLength is reached the propagation stops and the streamline is written in the output. If set to \"discard\", the streamline is not written in the output. Default=discard.

**-writeInterval <positive integer>** Because stepSize might be very small, it might not be desirable to save each step of propagation in the output. writeInterval enables skipping of a number of steps before saving them. This parameter does not change the propagation algorithm, internally trekker always walks with the specified *-stepSize*. *-writeInterval* by Default: is set, so that the distance between the output tracks is 0.5 x the smallest of the FOD voxel dimensions.

**-directionality <either \"one_sided\" or \"two_sided\">** If set to \"two_sided\", tracking is done along both directions starting from the seed point. If set to \"one_sided\", tracking is only done along the other way. Default=two_sided.

**-maxEstInterval <positive integer>** For random sampling, an estimate for the maximum posterior probability is needed. If set to *n*, this estimate is computed at every *n* \ :sup:`th` step. Default=1.

**-initMaxEstTrials <positive integer>** Number of trials done for the estimation of maximum posterior probability during initialization. Default: is set adaptively based on previous trials.

**-propMaxEstTrials <positive integer>** Number of trials done for the estimation of maximum posterior probability during propagation. Default: is set adaptively based on previous trials.

**-maxSamplingPerStep <positive integer>** Maximum number of random sampling to select an acceptable candidate for propagation. Tracking stops if a suitable candidate cannot be found after this many attempts. Default: 1000.

**-useBestAtInit** If used, during initialization, random sampling is skipped and the candidate that has the largest data support is used for propagation. Default: off

::

    # Example 6
    ./trekker -fod FOD.nii.gz \
              -initMaxEstTrials 500 \
              -useBestAtInit \ # for initialization, out of 500 candidates, the one with the highest data support will be used
              -seed_image SEED.nii.gz \
              -seed_count 1000 \
              -output OUTPUT.vtk


Probe options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Trekker uses probes for estimating future propagation steps. A probe is a short, cylinderical model of the connecting segment.

**-probeLength <positive real number>** Length of the probe. Default=1/4 x the smallest of the FOD voxel dimensions.

**-probeRadius <positive real number>** Radius of the probe. Default=0.

**-probeCount <integer between 1 and 100>** probeCount determines the number of parallel lines used to model the cylinder. Default: 1 if probeRadius=0, 4 if probeRadius≠0.

**-probeQuality <integer between 1 and 100>** This parameter sets the number of segments to split the cylinder along the length of the probe. Default=4.

**-ignoreWeakLinks <positive real number>** If the individual data support (FOD amplitude along the direction of the segment) for any of the points in the probe is lower than what is specified with this parameter, this candidate probe is ignored. Default: 0.


Seeding options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**-seed_image <nifti image> {optional:labelNo}** Path to seed image and optionally a label no. If label is not specified all voxels with values larger than zero are used to seed points. Accepted file extensions are: nii, nii.gz.

**-seed_count <positive integer>** Number of seeds to randomly sample from the seed image. Trekker tries to generate a single streamline from each seed. For that it tries what is set by *seed_maxTrials*. If an acceptable streamline cannot be generated then it is skipped. If this happens, there will be less streamlines in the output tractogram than what is defined with *seed_count*.

**-seed_countPerVoxel <positive integer>** Number of seeds to randomly sample per each voxel of the seed image.

**-seed_coordinates <coordinate list file> {optional:initial direction list file}** Path to an ASCII formatted list of the physical locations of the seed coordinates. This text file for the coordinates can optionally be followed with another ASCII formatted list that specifies the initial directions for propagation. Each of the listed seeds is used to generate a single streamline, seed_count and seed_countPerVoxels options do not have any affect when seed_coordinates is used. Example for the contents of a COORDINATES file is given below:

::

   0 0 0
   0 0 0
   0 0 0
   1 1 1
   1 1 1
   0.121323 0.21323 0.123213123
   1.231232.1.2313213 2.5545435
   12.2323213 213.2321323 235.2321321

::

    # Example 7
    ./trekker -fod FOD.nii.gz \
              -seed_coordinates COORDINATES \
              -output OUTPUT.vtk

::

    # Example 8
    ./trekker -fod FOD.nii.gz \
              -seed_coordinates COORDINATES DIRECTIONS \
              -output OUTPUT.vtk

**-seed_maxTrials <positive integer>** Sets the maximum number of attempts to generate streamline from the seed point. Default=100000.

Output options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**-output <vtk file>** Output file path. Trekker saves tractograms in .vtk format. An additional output file with .json extension is also saved which contains metadata about tracking parameters and information about the results.

**-enableOutputOverwrite** If used, output file will be overwritten in the case that it already exists in the specified path. Default=off.

**-writeSeedCoordinates** If used, seed coordinates of each streamline is written in the output.

**-dontWriteSegmentsInSeedROI** If used, seeding is done using the provided seed image but streamlines are saved starting from the surface of the seed ROI. The seed coordinates with this option are also written starting from the seed surface.

**-writeColors** If used, direction coded colors are written for each segment in the output tractogram. Default=off. (If this is enabled, you can visualize the tracks in the familiar tractography RGB type coloring in Paraview. For that load the .vtk file in Paraview, choose "colors" as the coloring option and make sure that "Map Scalars" option is not selected.)

**-writeFODamp**, **-writeTangents**, **-writek1axes**, **-writek2axes**, **-writek1s**, **-writek2s**, **-writeCurvatures**, **-writeLikelihoods** options can be used separately to write these values for each segment in the output. Likelihoods are the computed data supports.



Pathway options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Starting from the seed point, trekker first propagates along one direction to find one end of the connection. Propagation is then done towards the opposition direction from the seed to find the other end of the connection. (This \"two_sided\" tracking is the default option in trekker and can be changed to \"one_sided\" using the **-directionality** option.) The two different sides of propagation are named as **A** and **B** in trekker.

Trekker allows two options to define rules for pathways:

1. Rules can be defined for each propagation direction separately, using options **-pathway_A=** and **-pathway_B=**.
2. Rules can be defined for either propagation direction, using options starting with **-pathway=**. These options define rules for \"A or B\" not for \"A and B\".

Notes:

- Trekker accepts multiple pathway rules which are either defined using option 1 or 2, i.e.: if a pathway rule is defined using **-pathway=**, all other rules have to start with **-pathway=**. Or, if a pathway rule is defined using **-pathway_A=**, all other rules have to start either with **-pathway_A=** or **-pathway_B=**.

- Rules for **-pathway_A=** and **-pathway_B=** are considered separate. Therefore they are treated independent of each other. Rules can be defined using only **-pathway_A=** or **-pathway_B=**.

- Users do not need to think about which side trekker considers to be side A or B. The decision is done automatically based on the first rule that is satisfied. For example, consider the case **-seed_image THAL -pathway_A=require_entry GRAYMATTER -pathway_B=stop_at_exit THAL**. Here, starting from the seed point, trekker propagates towards one direction which will soon exit THAL. But because this is a stopping condition for side B, propagation will stop here and this part will be considered as side B. Trekker then will return back to seed point and start propagating towards the other side which is from now on considered as side A.


Trekker allows the specification of the following pathway rules:

- *=require_entry*
- *=stop_at_entry*
- *=require_exit*
- *=stop_at_exit*
- *=discard_if_enters*
- *=discard_if_exits*
- *=discard_if_ends_inside*


**-pathway=require_entry <nifti image> {optional:labelNo}** All tracks are required to enter the ROI present in the specified image.

**-pathway_A=require_entry <nifti image> {optional:labelNo}** One side of all tracks are required to enter the ROI present in the specified image.

**-pathway_B=require_entry <nifti image> {optional:labelNo}** The other side of all tracks are required to enter the ROI present in the specified image.

Example: The following will generate connections from left precentral gyrus (Left_PCG) back to Left_PCG.

::

    # Example 9
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway_A=require_entry Left_PCG.nii.gz \
              -pathway_B=require_entry Left_PCG.nii.gz \
              -output OUTPUT.vtk


**-pathway=stop_at_entry <nifti image> {optional:labelNo}** All tracks will stop propagating if they enter the ROI present in the specified image.

**-pathway_A=stop_at_entry <nifti image> {optional:labelNo}** One side of all tracks will stop propagating if they enter the ROI present in the specified image.

**-pathway_B=stop_at_entry <nifti image> {optional:labelNo}** The other side of all tracks will stop propagating if they enter the ROI present in the specified image.

Example: The following will generate connections from left precentral gyrus (Left_PCG) back to Left_PCG. Tracking stops at both ends when they enter the Left_PCG ROI.

::

    # Example 10
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway_A=require_entry Left_PCG.nii.gz \
              -pathway_A=stop_at_entry Left_PCG.nii.gz \
              -pathway_B=require_entry Left_PCG.nii.gz \
              -pathway_B=stop_at_entry Left_PCG.nii.gz \
              -output OUTPUT.vtk

**-pathway=require_exit <nifti image> {optional:labelNo}** All tracks are required to enter and exit the ROI present in the specified image.

**-pathway_A=require_exit <nifti image> {optional:labelNo}** One side of all tracks are required to enter and exit the ROI present in the specified image.

**-pathway_B=require_exit <nifti image> {optional:labelNo}** The other side of all tracks are required to enter and exit the ROI present in the specified image.

Example: At least one end of each streamline will end outside white matter.

::

    # Example 11
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway=require_exit WHITEMATTER.nii.gz \
              -output OUTPUT.vtk

Example: All streamline end points will be outside the white matter.

::

    # Example 12
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway_A=require_exit WHITEMATTER.nii.gz \
              -pathway_B=require_exit WHITEMATTER.nii.gz \
              -output OUTPUT.vtk

**-pathway=stop_at_exit <nifti image> {optional:labelNo}** All tracks that enter the ROI present in the specified image will stop propagating if they exit the ROI.

**-pathway_A=stop_at_exit <nifti image> {optional:labelNo}** One side of all tracks that enter the ROI present in the specified image will stop propagating if they exit the ROI.

**-pathway_B=stop_at_exit <nifti image> {optional:labelNo}** The other side of all tracks that enter the ROI present in the specified image will stop propagating if they exit the ROI.

Example: None of the streamlines will terminate inside the white matter and all streamlines will stop if they reach the end of gray matter.

::

    # Example 13
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway_A=discard_if_ends_inside WHITEMATTER.nii.gz \
              -pathway_A=stop_at_exit GRAYMATTER.nii.gz \
              -pathway_B=discard_if_ends_inside WHITEMATTER.nii.gz \
              -pathway_B=stop_at_exit GRAYMATTER.nii.gz \
              -output OUTPUT.vtk

**-pathway=discard_if_enters <nifti image> {optional:labelNo}** Tracks are not allowed to enter the ROI present in the specified image.

**-pathway_A=discard_if_enters <nifti image> {optional:labelNo}** One side of tracks are not allowed to enter the ROI present in the specified image.

**-pathway_B=discard_if_enters <nifti image> {optional:labelNo}** The other side of tracks are not allowed to enter the ROI present in the specified image.

Example: None of the streamlines generated by the following example ends inside the white matter or projects to CSF.

::

    # Example 14
    ./trekker -fod FOD.nii.gz \
              -seed_image WHITEMATTER.nii.gz \
              -seed_count 1000 \
              -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \
              -pathway=discard_if_enters CSF.nii.gz \
              -output OUTPUT.vtk


**-pathway=discard_if_exits <nifti image> {optional:labelNo}** Tracks are allowed to enter but not to exit the ROI present in the specified image.

**-pathway_A=discard_if_exits <nifti image> {optional:labelNo}** One side of tracks are allowed to enter but not to exit the ROI present in the specified image.

**-pathway_B=discard_if_exits <nifti image> {optional:labelNo}** The other side of tracks are allowed to enter but not to exit the ROI present in the specified image.

Example: All connections within hippocampus. Nothing projects inside or outside.

::

    # Example 15
    ./trekker -fod FOD.nii.gz \
              -seed_image HIPPOCAMPUS.nii.gz \
              -seed_count 1000 \
              -pathway_A=discard_if_exits HIPPOCAMPUS.nii.gz \
              -pathway_B=discard_if_exits HIPPOCAMPUS.nii.gz \
              -output OUTPUT.vtk

**-pathway=discard_if_ends_inside <nifti image> {optional:labelNo}** Tracks are not allowed to stop inside the ROI present in the specified image.

**-pathway_A=discard_if_ends_inside <nifti image> {optional:labelNo}** One side of tracks are not allowed to stop inside the ROI present in the specified image.

**-pathway_B=discard_if_ends_inside <nifti image> {optional:labelNo}** The other side of tracks are not allowed to stop inside the ROI present in the specified image.

Example: None of the streamlines will terminate inside the white matter.

::

      # Example 16
      ./trekker -fod FOD.nii.gz \
                -seed_image WHITEMATTER.nii.gz \
                -seed_count 1000 \
                -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \
                -output OUTPUT.vtk


**-pathway=satisfy_requirements_in_order** If enabled all pathway requirements are going to be satisfied in the order that they are input to trekker. All pathway options should be defined for pathway_A/pathway_B in order to use this option.


Example: Only generates those streamlines that go from the start of ROI_1 till the end of ROI_5 *in order* where ROI_3 is the seed.

::

      # Example 17
      ./trekker -fod FOD.nii.gz \
                -seed_image ROI_3.nii.gz \
                -seed_count 1000 \
                -pathway=satisfy_requirements_in_order \
                -pathway_A=require_entry ROI_4.nii.gz \
                -pathway_A=require_exit ROI_5.nii.gz \
                -pathway_B=require_entry ROI_2.nii.gz \
                -pathway_B=require_exit ROI_1.nii.gz \
                -output OUTPUT.vtk

Other options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**-numberOfThreads <positive integer>** Number of parallel threads to run during tracking. Default=number of CPU cores of the local processor

**-timeLimit <positive real number>** Sets the maximum allowed duration in minutes for trekker to continue tracking. If any of the *-seed_coordinates*, *-seed_count* or *-seed_countPerVoxel* options are used together with *-timeLimit*, tracking stops either when the required number of streamlines are reconstructed or the time limit is hit, whichever comes first. When tracking stops due to time limit, all the streamlines that are computed so far are written to the disk.

**-verboseLevel <integer between 0 and 4>** Determines the level of information displayed on the terminal. Verbose level must be between 0-4. No information is displayed at level 0. Default=1.

**-help** Displays this manual.
