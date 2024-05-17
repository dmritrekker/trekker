Tutorial 2: Trekker's pathways
==============================

This tutorial shows how to specify pathway rules in Trekker. Before describing the rules in detail, please check the illustration below for definitions that Trekker use.

.. figure:: tut2_0.svg
	:alt: Trekker's pathways

	Trekker regards each streamline to be composed of two sides which are called **pathway_A** and **pathway_B**. Pathway rules can be specified for each side separately or the complete **pathway** as a whole.

Trekker automatically determines the sides as **pathway_A** or **pathway_B** based on how they meet the user defined pathway constraints, i.e., if the first side meets the criteria for **B** then trekker assigns it as pathway **B**.

.. note::

	You can also disable the default \"two-sided\" tracking by setting **-directionality=one_sided** . In this case, you can only specify rules using the **-pathway=** option.



Defining pathway rules
----------------------

Trekker provides powerful options for specifying pathways. Rules can be defined using one of:

- **-pathway=**
- **-pathway_A=**
- **-pathway_B=**

followed by one of:

- **require_entry**
- **stop_at_entry**
- **require_exit**
- **stop_at_exit**
- **discard_if_enters**
- **discard_if_exits**
- **discard_if_ends_inside**


Pathway rules specify clear instructions for how streamlines in the output tractogram should be.

1. If a rule starts with **require**, all streamlines in the tractogram will satisfy this condition.
2. If a rule starts with **discard_if**, no streamline meeting this specification will be included in the tractogram.
3. If a rule starts with **stop_at_**, streamlines will terminate with respect to this specification.

The **-pathway=** option specifies rules for the whole connection. Following examples will generate similar tractograms where one end of streamlines will reach the gray matter while the other end may or may not terminate at the gray matter:

::

   # Tutorial 2 - Example 1
   ./trekker -fod FOD.nii.gz \                            # Input FOD image
             -seed_image THAL.nii.gz \                    # Input seed image
             -seed_count 1000 \                           # Number of seeds
             -pathway=require_entry GRAYMATTER.nii.gz \   # A self descriptive pathway rule
             -output OUTPUT.vtk                           # Output

::

   # Tutorial 2 - Example 2
   ./trekker -fod FOD.nii.gz \                            # Input FOD image
             -seed_image THAL.nii.gz \                    # Input seed image
             -seed_count 1000 \                           # Number of seeds
             -pathway_A=require_entry GRAYMATTER.nii.gz \ # A self descriptive pathway rule
             -output OUTPUT.vtk                           # Output

::

   # Tutorial 2 - Example 3
   ./trekker -fod FOD.nii.gz \                            # Input FOD image
             -seed_image THAL.nii.gz \                    # Input seed image
             -seed_count 1000 \                           # Number of seeds
             -pathway_B=require_entry GRAYMATTER.nii.gz \ # A self descriptive pathway rule
             -output OUTPUT.vtk                           # Output

.. note::

	- If you choose to specify a rule using **-pathway=**, no other rule can start with **-pathway_A=** or **-pathway_B=**. (pathway=satisfy_requirements_in_order is an exception to this.)


Whole brain tractography
------------------------

Pathway rules provide useful options for obtaining clean whole brain tractograms.

The following example will generate streamlines where no end will be inside the white matter:

::

   # Tutorial 2 - Example 4
   ./trekker -fod FOD.nii.gz \                                    # Input FOD image
             -seed_image WHITEMATTER.nii.gz \                     # Input seed image
             -seed_count 1000 \                                   # Number of seeds
             -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \ # A self descriptive pathway rule
             -output OUTPUT.vtk                                   # Output

Or even better:

::

   # Tutorial 2 - Example 5
   ./trekker -fod FOD.nii.gz \                                    # Input FOD image
             -seed_image WHITEMATTER.nii.gz \                     # Input seed image
             -seed_count 1000 \                                   # Number of seeds
             -pathway=discard_if_ends_inside WHITEMATTER.nii.gz \ # A self descriptive pathway rule
             -pathway=discard_if_enters CSF.nii.gz \              # A self descriptive pathway rule
             -output OUTPUT.vtk                                   # Output


The following will create only gray matter to gray matter connections and nothing else:

::

   # Tutorial 2 - Example 6
   ./trekker -fod FOD.nii.gz \                                    # Input FOD image
             -seed_image WHITEMATTER.nii.gz \                     # Input seed image
             -seed_count 1000 \                                   # Number of seeds
             -pathway_A=require_entry GRAYMATTER.nii.gz \         # A self descriptive pathway rule
             -pathway_A=stop_at_exit GRAYMATTER.nii.gz \          # A self descriptive pathway rule
             -pathway_B=require_entry GRAYMATTER.nii.gz \         # A self descriptive pathway rule
             -pathway_B=stop_at_exit GRAYMATTER.nii.gz \          # A self descriptive pathway rule
             -output OUTPUT.vtk                                   # Output


Using atlases or labeled images in pathway rules
------------------------------------------------

Labels in segmented images can be used when defining the seed region and/or pathway rules:

::

   # Tutorial 2 - Example 7
   ./trekker -fod FOD.nii.gz \                                    # Input FOD image
             -seed_image aseg.nii.gz 16 \                         # Aseg label 16, brain stem, will be used as seed
             -seed_count 1000 \                                   # Number of seeds
             -pathway=require_entry aseg.nii.gz 3 \               # Aseg label 3: left cerebral cortex
             -pathway=discard_if_enters aseg.nii.gz 41 \          # Aseg label 41: right cerebral white matter
             -output OUTPUT.vtk                                   # Output


Tracking in order
-----------------

Trekker features the option, **-pathway=satisfy_requirements_in_order**, that generates streamlines which satisfy pathway rules in the order that they are defined.

Following example only generates those streamlines that go from the start of ROI_1 till the end of ROI_5 *in order* where ROI_3 is the seed.

::

   # Tutorial 2 - Example 8
   ./trekker -fod testFOD.nii.gz \                    # Input FOD image
             -seed_image ROI_3.nii.gz \               # Input seed image
             -seed_count 1000 \                       # Number of seeds
             -pathway=satisfy_requirements_in_order \ # A self descriptive pathway rule
             -pathway_A=require_entry ROI_4.nii.gz \  # A self descriptive pathway rule
             -pathway_A=require_exit ROI_5.nii.gz \   # A self descriptive pathway rule
             -pathway_B=require_entry ROI_2.nii.gz \  # A self descriptive pathway rule
             -pathway_B=require_exit ROI_1.nii.gz \   # A self descriptive pathway rule
             -output OUTPUT.vtk                       # Output

.. note::

	If you use the **-pathway=satisfy_requirements_in_order** option, you can define pathway rules only using **-pathway_A=** or **-pathway_B=**.
