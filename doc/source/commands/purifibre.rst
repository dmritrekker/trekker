purifibre
=========

removes spurious streamlines from tractograms

Purifibre computes a smoothed Track Orientation Density Image (TODI) to measure how well each streamline aligns with the collective organization of its neighbors, assigning a Fiber Coupling (FICO) score based on the streamline's least aligned segment (minimum SECO). It then filters the tractogram by removing a user-defined percentage of streamlines with the lowest FICO scores, thereby eliminating spatially incoherent "spurious" fibers.

**References**

[Aydogan2022] Aydogan D.B., "Fiber coupling (FICO) measure using anisotropic smoothing of track orientation density images for tractogram filtering", ISMRM 2022

|

**Usage**

.. code-block:: bash

    ./trekker purifibre [OPTIONS] <input tractogram> <output tractogram>

|

**Positionals**

- `<input tractogram> TEXT:FILE REQUIRED`
  Input tractogram (.vtk, .tck, .trk)

- `<output tractogram> TEXT REQUIRED`
  Output tractogram (.vtk, .tck)

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-t, --trim FLOAT</td>
           <td style="padding: 8px;">Trim excludes ends of streamlines from being analyzed. E.g., when trim is 10, 90% of the streamline is analyzed. 5% of the streamline length from each end is excluded from the computation. Default: 10.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-p, --purify FLOAT</td>
           <td style="padding: 8px;">Percentage of streamlines to remove from the tractogram. Default: 5.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--voxDim FLOAT</td>
           <td style="padding: 8px;">Isotropic voxel dimension for sTODI computation. Default: 4.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--anisotropicSmoothing FLOAT INT</td>
           <td style="padding: 8px;">Standard deviation of the Gaussian kernel (in mm), and computation density for anisotropic smoothing (number of streamlines). E.g. when set to 2 100, smoothing is done using 100 streamlines randomly distributed around each streamline using a Gaussion distribution with standard deviation of 2 mm. Default=0 0.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--sphericalSmoothing FLOAT</td>
           <td style="padding: 8px;">Amount of sTODI spherical smoothing. Default: 15.</td>
           </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--save_fico TEXT</td>
           <td style="padding: 8px;">Saves a .vtk formatted copy of input tractogram with FICO values written as a field.</td>
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
