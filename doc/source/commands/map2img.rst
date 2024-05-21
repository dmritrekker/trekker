map2img
=======

maps tractogram features on an image

|

**Usage**

.. code-block:: bash

    ./trekker map2img [OPTIONS] <input tractogram> <output image>

|

**Positionals**

- `<input tractogram> TEXT:FILE REQUIRED`
  Input tractogram (.vtk, .tck)

- `<output image> TEXT REQUIRED`
  Output image name (.nii, .nii.gz)

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--feature TEXT</td>
           <td style="padding: 8px;">Name of output feature. Options are: "streamlineCount", "segmentLength" or "DEC_SegmentLength".</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--weights TEXT:FILE</td>
           <td style="padding: 8px;">A binary file for streamline weights. Values should have float datatype.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--anisotropicSmoothing [FLOAT,INT]</td>
           <td style="padding: 8px;">Standard deviation of the Gaussian kernel, and computation density for anisotropic smoothing. Default=0 0.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--SF INT</td>
           <td style="padding: 8px;">Spherical domain detail. This value should be a positive odd integer up to 31.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--SF_smoothing FLOAT</td>
           <td style="padding: 8px;">Apply spherical smoothing.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--SH INT</td>
           <td style="padding: 8px;">Order of spherical harmonics function. Only even orders are supported.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-t, --template TEXT:FILE</td>
           <td style="padding: 8px;">Input template image (.nii/.nii.gz).</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--templateUpsample FLOAT</td>
           <td style="padding: 8px;">Upsampling factor for template image.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-b, --bbox FLOAT ...</td>
           <td style="padding: 8px;">Bounding box for the output image.<br>Options are:<br>(i) "x_min x_max y_min y_max z_min z_max" (an isotropic voxel dimension of 1 will be assumed.)<br>(ii) "x_min x_max y_min y_max z_min z_max voxDim" (an isotropic voxel dimension of voxDim will be assumed.)<br>(iii) "x_min x_max y_min y_max z_min z_max xVoxDim yVoxDim zVoxDim"</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--voxDim FLOAT ...</td>
           <td style="padding: 8px;">Voxel dimension of the output image.<br>Options are:<br>(i) a single value that will be used for all dimensions<br>(ii) one value for each of the three dimensions separated by space.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--mask TEXT:FILE</td>
           <td style="padding: 8px;">Input mask image (.nii/.nii.gz).</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--maskLabel INT</td>
           <td style="padding: 8px;">Label to use in the mask image.</td>
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

|

**References**

For uses without spherical mapping, please cite [Calamante2010]_, which provides detailed information on Track-density imaging (TDI).

.. [Calamante2010] `Calamante, F.; Tournier, J.-D.; Jackson, G. D. & Connelly, A. "Track-density imaging (TDI): Super-resolution white matter imaging using whole-brain track-density mapping", NeuroImage, 2010, 53, 1233-1243. <https://www.sciencedirect.com/science/article/abs/pii/S1053811910009766>`__

For uses with spherical mapping, please cite [Dhollander2014]_, which explains Track Orientation Density Imaging (TODI) and Track Orientation Distribution (TOD) based tractography.

.. [Dhollander2014] `Dhollander, T., Emsell, L., Van Hecke, W., Maes, F., Sunaert, S., Suetens, P. "Track Orientation Density Imaging (TODI) and Track Orientation Distribution (TOD) based tractography", NeuroImage, 2014, 94, 312-336. <https://www.sciencedirect.com/science/article/pii/S1053811913012676>`__

