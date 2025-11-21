prepXact (experimental)
=======================

creates XACT surfaces for whole-brain tractography

XACT output can be used for fiber tracking using the `track` command with the `xact` option for anatomically constrained whole-brain tractography.

|

**Usage**

.. code-block:: bash

    ./trekker prepXact [OPTIONS] <FreesurferFolder> <Output_xact_file>

|

**Positionals**

- `<Freesurfer_folder> TEXT:DIR REQUIRED`
  Path to Freesurfer aseg file

- `<Output_xact_file> FILE REQUIRED`
  xact output (.vtk)

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--l_wm TEXT</td>
           <td style="padding: 8px;">Left white matter. (Label = 1)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--r_wm TEXT</td>
           <td style="padding: 8px;">Right white matter. (Label = 2)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--l_gm TEXT</td>
           <td style="padding: 8px;">Left gray matter. (Label = 3)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--r_gm TEXT</td>
           <td style="padding: 8px;">Right gray matter. (Label = 4)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--l_sub TEXT</td>
           <td style="padding: 8px;">Left subcortical. (Label = 5)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--r_sub TEXT</td>
           <td style="padding: 8px;">Right subcortical. (Label = 6)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--csf TEXT</td>
           <td style="padding: 8px;">Cerebrospinal fluid. (Label = 7)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--cer_wm TEXT</td>
           <td style="padding: 8px;">Cerebellar white matter. (Label = 8)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--cer_gm TEXT</td>
           <td style="padding: 8px;">Cerebellar gray matter. (Label = 9)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--bs TEXT</td>
           <td style="padding: 8px;">Brain stem. (Label = 10)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--i_bs TEXT</td>
           <td style="padding: 8px;">Inferior brain stem. (Label = 11)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--bg TEXT</td>
           <td style="padding: 8px;">Background. (Label = 12)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--fslFirst TEXT</td>
           <td style="padding: 8px;">Use FSL First results for subcortical and brainstem surfaces</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--cereDistThresh FLOAT</td>
           <td style="padding: 8px;">Distance threshold from the brain stem to separate cerebellar white matter. Default: 0.5 mm</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--enlargeBrainStem FLOAT</td>
           <td style="padding: 8px;">Enlarge or shrink brain stem. Default: 5 mm</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--inferiorBrainStemCutLevel FLOAT</td>
           <td style="padding: 8px;">Cut level for inferior brain stem. Default: 20 mm</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--faceArea FLOAT</td>
           <td style="padding: 8px;">Average area of output triangles. Default: 0.25 mm^2</td>
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
           <td style="padding: 8px; font-weight: 500; border-bottom: 1px solid #000;">-f, --force</td>
           <td style="padding: 8px; border-bottom: 1px solid #000;">Force overwriting of existing file.</td>
       </tr>
   </table>
