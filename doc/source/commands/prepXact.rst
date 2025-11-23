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
  xact output (.vtk). This single file will contains all surfaces combined.

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--fslFirst TEXT</td>
           <td style="padding: 8px;">Use FSL First results for subcortical and brainstem surfaces</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--abnormality TEXT</td>
           <td style="padding: 8px;">Path to abnormality mask/surface to include in the XACT output. If an image is used, it will be converted to a surface.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--cereDistThresh FLOAT</td>
           <td style="padding: 8px;">Distance threshold from the brain stem to separate cerebellar white matter. Default: 0.5 mm</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--enlargeBrainStem FLOAT</td>
           <td style="padding: 8px;">Enlarge or shrink brain stem. Default: 5 mm</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--inferiorBrainStemCutLevel FLOAT</td>
           <td style="padding: 8px;">Cut level for inferior brain stem. Default: 20 mm</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--faceArea FLOAT</td>
           <td style="padding: 8px;">Average area of output triangles. Default: 0.25 mm^2</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_l_wm TEXT</td>
           <td style="padding: 8px;">Optional left white matter output. (Label = 1)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_r_wm TEXT</td>
           <td style="padding: 8px;">Optional right white matter output. (Label = 2)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_l_gm TEXT</td>
           <td style="padding: 8px;">Optional left gray matter output. (Label = 3)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_r_gm TEXT</td>
           <td style="padding: 8px;">Optional right gray matter output. (Label = 4)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_l_sub TEXT</td>
           <td style="padding: 8px;">Optional left subcortical output. (Label = 5)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_r_sub TEXT</td>
           <td style="padding: 8px;">Optional right subcortical output. (Label = 6)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_csf TEXT</td>
           <td style="padding: 8px;">Optional cerebrospinal fluid output. (Label = 7)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_cer_wm TEXT</td>
           <td style="padding: 8px;">Optional cerebellar white matter output. (Label = 8)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_cer_gm TEXT</td>
           <td style="padding: 8px;">Optional cerebellar gray matter output. (Label = 9)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_bs TEXT</td>
           <td style="padding: 8px;">Optional brain stem output. (Label = 10)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_i_bs TEXT</td>
           <td style="padding: 8px;">Optional inferior brain stem output. (Label = 11)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--out_abn TEXT</td>
           <td style="padding: 8px;">Optional abnormality output. (Label = 12)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--out_bg TEXT</td>
           <td style="padding: 8px;">Optional background output. (Label = 13)</td>
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
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-n, --numberOfThreads INT</td>
           <td style="padding: 8px;">Number of threads.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-v, --verbose TEXT</td>
           <td style="padding: 8px;">Verbose level. Options are "quiet", "fatal", "error", "warn", "info", and "debug". Default=info.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500; border-bottom: 1px solid #000;">-f, --force</td>
           <td style="padding: 8px; border-bottom: 1px solid #000;">Force overwriting of existing file.</td>
       </tr>
   </table>
