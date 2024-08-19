track
=====

fiber tracker

|

**Usage**

.. code-block:: bash

    ./trekker track <FOD image> [OPTIONS]

|

**Positionals**

- `<FOD image> FILE:FILE ... REQUIRED`
  Input FOD image (.nii, .nii.gz). Trekker supports both symmetric and asymmetric FODs, i.e. spherical harmonics with both even and odd orders.

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-o, --output FILE REQUIRED</td>
           <td style="padding: 8px;">Output tractogram (.vtk, .tck).</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-a, --ascii</td>
           <td style="padding: 8px;">Write ASCII output (.vtk only).</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-w, --writeStepSize FLOAT</td>
           <td style="padding: 8px;">Because stepSize might be very small, it might not be desirable to save each step of propagation in the output. writeStepSize enables skipping of a number of steps before saving them. This parameter does not change the propagation algorithm; internally Trekker always walks with the specified --stepSize. Default: the distance between the output tracks is 0.5 x the smallest of the FOD voxel dimensions.</td>
       </tr>
   </table>

|

**Algorithm parameters**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--stepSize FLOAT</td>
           <td style="padding: 8px;">Step size. Default=1/40 x the smallest of the FOD voxel dimensions.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--minRadiusOfCurvature FLOAT</td>
           <td style="padding: 8px;">Minimum radius of curvature. Default=1/2 x the smallest of the FOD voxel dimensions.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--minDataSupport FLOAT</td>
           <td style="padding: 8px;">Threshold for minimum data support. For PTT algorithm this is minimum FOD threshold. Default=0.05.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--dataSupportExponent FLOAT</td>
           <td style="padding: 8px;">Data support to the power dataSupportExponent is used for rejection sampling. Default=1.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--orderOfDirections TEXT</td>
           <td style="padding: 8px;">Switches direction of FOD. By default XYZ is assumed (no change). A lower case can be used to switch the direction, e.g., xYZ switches the sign of the first component. The order of components can be changed as well, e.g., yXz switches the sign of the second component while replacing it with the first, at the same time the sign of the last component is also changed.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--dontDiscretizeFod</td>
           <td style="padding: 8px;">Turns off FOD discretization. For faster tracking, Trekker by default will discretize the FOD on a sphere (using 2076 points for symmetric functions and 1004 points for asymmetric functions).</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--runTimeLimit INT</td>
           <td style="padding: 8px;">Sets the maximum allowed duration in minutes for Trekker to continue tracking. Tracking stops when time limit is reached. When tracking stops due to time limit, all the streamlines that are computed so far are written to the disk.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--idleTimeLimit INT</td>
           <td style="padding: 8px;">Sets the maximum allowed duration in minutes from the computation of the last successful streamline, i.e., if no new streamlines were computed within this time, Trekker stops.</td>
       </tr>
   </table>

|

**Sampling parameters**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--maxEstInterval INT</td>
           <td style="padding: 8px;">For random sampling, an estimate for the maximum posterior probability is needed. If set to n, this estimate is computed at every n^th step. Default=1.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--initMaxEstTrials INT</td>
           <td style="padding: 8px;">Number of trials done for the estimation of maximum posterior probability during initialization. Default: is set adaptively based on previous trials.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--propMaxEstTrials INT</td>
           <td style="padding: 8px;">Number of trials done for the estimation of maximum posterior probability during propagation. Default: is set adaptively based on previous trials.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--maxSamplingPerStep INT</td>
           <td style="padding: 8px;">Maximum number of random sampling to select an acceptable candidate for propagation. Tracking stops if a suitable candidate cannot be found after this many attempts. Default: 1000.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--useBestAtInit</td>
           <td style="padding: 8px;">If used, during initialization, random sampling is skipped and the candidate that has the largest data support is used for propagation. Default: off.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--useLegacySampling</td>
           <td style="padding: 8px;">If used, during propagation, random sampling will be done using rejection sampling. Default: off.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--samplingQuality INT</td>
           <td style="padding: 8px;">Integer value between 1 and 7 that controls the quality of sampling. Default=2.</td>
       </tr>
   </table>

|

**PTT probe parameters**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--probeLength FLOAT</td>
           <td style="padding: 8px;">Length of the probe. Default=1/4 x the smallest of the FOD voxel dimensions.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--probeRadius FLOAT</td>
           <td style="padding: 8px;">Radius of the probe. Default=0.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--probeCount INT</td>
           <td style="padding: 8px;">probeCount determines the number of parallel lines used to model the cylinder. Default: 1 if probeRadius=0, 4 if probeRadius≠0.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--probeQuality INT</td>
           <td style="padding: 8px;">This parameter sets the number of segments to split the cylinder along the length of the probe. Default=4.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--ignoreWeakLinks FLOAT</td>
           <td style="padding: 8px;">If the individual data support (FOD amplitude along the direction of the segment) for any of the points in the probe is lower than what is specified with this parameter, this candidate probe is ignored. Default: 0.</td>
       </tr>
   </table>

|

**Seeding parameters**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-s, --seed TEXT ...</td>
           <td style="padding: 8px;">Seed definition.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--discard_seed TEXT ...</td>
           <td style="padding: 8px;">If a seed point falls into this region, it will be discarded.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--skipSeed</td>
           <td style="padding: 8px;">Does not output the points that are within seed region.</td>
       </tr>
       <!--
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--allowEdgeSeeds</td>
           <td style="padding: 8px;">Allows seeding at the edges of pathway rules. Default: false.</td>
       </tr>
       -->
       <tr>
           <td style="padding: 8px; font-weight: 500;">--seed_trials INT</td>
           <td style="padding: 8px;">Sets the maximum number of attempts to generate streamline from the seed point. Default=1.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--seed_count INT</td>
           <td style="padding: 8px;">Number of seeds. Trekker tries to generate a single streamline from each seed. For that it makes maximum amount of "trials". If an acceptable streamline cannot be generated then it is skipped. If this happens, there will be fewer streamlines in the output tractogram than what is defined with "count".</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--seed_density FLOAT</td>
           <td style="padding: 8px;">Density of seeds. If seed is an image, density is the number of seeds per mm^3. If seed is a surface mesh, density is the number of seeds per mm^2.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--seed_surf_faceDensity TEXT ...</td>
           <td style="padding: 8px;">A text file containing density information for each face of the input seed surface.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--seed_surf_vertDensity TEXT ...</td>
           <td style="padding: 8px;">A text file containing density information for each vertex of the input seed surface.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--seed_surf_fieldDensity TEXT</td>
           <td style="padding: 8px;">The given field in the input seed surface will be used to set seed density.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--seed_surf_useSurfNorm</td>
           <td style="padding: 8px;">Surface normals will be used as the initial direction.</td>
       </tr>
   </table>

|

**Pathway options**

.. tip::

    Please check the documentation for `pathways <../guide/pathways.html>`__ for a detailed explanation.

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-p, --pathway TEXT ...</td>
           <td style="padding: 8px;">Pathway rules.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--minlength FLOAT</td>
           <td style="padding: 8px;">Minimum length of output streamlines. Default=0.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--maxlength FLOAT</td>
           <td style="padding: 8px;">Maximum length of output streamlines. Default=infinite.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--oneSided</td>
           <td style="padding: 8px;">If enabled, tracking is done only towards one direction. Default=OFF.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--stopAtMax</td>
           <td style="padding: 8px;">If used, propagation stops when maxLength is reached. By default, streamlines are discarded when propagation reaches maxLength.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--inOrder</td>
           <td style="padding: 8px;">If enabled, all pathway requirements are going to be satisfied in the order that they are input to Trekker. All pathway options should be defined for pathway_A/pathway_B in order to use this option.</td>
       </tr>
   </table>

|

**General options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-h, --help</td>
           <td style="padding: 8px;">Print this help message and exit.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-n, --numberOfThreads INT</td>
           <td style="padding: 8px;">Number of threads.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-v, --verbose TEXT</td>
           <td style="padding: 8px;">Verbose level. Options are "quiet", "fatal", "error", "warn", "info", and "debug". Default=info.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-f, --force</td>
           <td style="padding: 8px;">Force overwriting of existing file.</td>
       </tr>
   </table>


|

**References**

Please cite [Aydogan2021]_, which provides a detailed explanation of the algorithm and the results obtained from extensive experiments. A precursory abstract was initially published during ISMRM 2019, Montreal [Aydogan2019]_, which can additionally be cited.

.. [Aydogan2021] `Aydogan D.B., Shi Y., "Parallel transport tractography", in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038. <https://ieeexplore.ieee.org/abstract/document/9239977/>`__

.. [Aydogan2019] `Aydogan D.B., Shi Y., "A novel fiber tracking algorithm using parallel transport frames", Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019 <https://www.researchgate.net/publication/336847169_A_novel_fiber-tracking_algorithm_using_parallel_transport_frames>`__
