How to set parameters?
======================

.. warning::

  **Parallel transport tractography (PTT) works different than other probabilistic techniques**

  This implies that your experience with other algorithms do not directly translate to Trekker's approach. Read below for more information.


.. tip::

  **Default streamline maxLength in Trekker is infinite**

  Before telling about PTT related parameters, it is important to note that in Trekker we set **maxLength** to infinite by default. This was done simply because it is not possible to estimate the maximum streamline length from input images. So please always adjust this parameter according to your application.


Basic parameter setting tips
----------------------------

To save time, start with tracking a small number of streamlines with default parameters. You can see the default values for your dataset on your terminal screen or in the .json file saved in the output.

Trekker's default parameter combinations generally result with good coverage of fiber bundles with well organized streamlines. However by adjusting the parameters you can improve the results. Basically you can go towards two directions:

  | **A.** make the streamlines more organized, this could come with loss of bundle coverage
  | **B.** cover more of the bundle, then you could lose the organization of streamlines

To go towards direction A:

  | **1.** decrease *stepSize*
  | **2.** increase *probeLength*
  | **3.** increase *minRadiusOfCurvature*
  | **4.** increase *minFODamp*

Doing the opposite will go towards direction **B**. Try small adjustments for all the parameters rather than making a large modification for a single parameter.



Advanced parameter setting tips
-------------------------------

Below are some tips for tracking connections in healthy adult human brains that are typically acquired with voxel dimensions around 1.25-2 mm isotropic. (The values mentioned below might not apply to other types of brains but the relationships between parameters still hold.)

- Try to use small a **stepSize**. This value can be 0.05 or below. With higher values, bundle organization will be compromised.

- **minRadiusOfCurvature** values between 0.5-1 mm are generally sufficient for tracking connections in the adult human brain. While the default *minRadiusOfCurvature* is half the voxel size, you might want to go lower than this if you are after bundles which have sharp turns.

- In PTT algorithm, **probeLength** is a very important parameter. Try to make sure that this value does not exceed *minRadiusOfCurvature*. Notice that 2*pi*minRadiusOfCurvature is the smallest circle one can track and *probeLength* shouldn't trace a long part of it. A very long *probeLength* could lead the tracker to make loops or take sharp, incorrect turns at crossing fiber arrangements. A *probeLength* value about half of *minRadiusOfCurvature* is adequate.

- We designed PTT to preserve topographic organization of connections. So one can use pretty low **minFODamp** values such as 0.025 or even 0.01 without losing much from streamline organization. This allows to get better coverage of the fiber bundles or help track intricate and challenging connections. You may however need to provide good anatomical constraints to reduce false positives.

- If you have problems with data quality, set the **probeRadius** to the value used for *minRadiusOfCurvature*. Using a topographically organized cylindrical probe model is particularly effective to mitigate noise in FOD. However, this will slow down the tracking.

- If you have luxurious computational resources, you can increase **probeQuality** and **probeCount** to improve the probe model and fiber tracking results.
