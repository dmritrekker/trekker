dMRI
====

Trekker currently only has a single dMRI command.

dMRI signal reconstruction based on [TranShi2015]_.

|

**Usage**

.. code-block:: bash

    ./trekker dMRI recon tran_shi_2015 [OPTIONS] <input dMRI image> <output FOD> <output tissue map>

|

**Positionals**
    
- `<input dMRI image> TEXT:FILE REQUIRED`           
   Input diffusion MRI image (.nii, .nii.gz)

- `<output FOD> TEXT REQUIRED`                      
   Output FOD image (.nii, .nii.gz)

- `<output tissue map> TEXT REQUIRED`
   Output tissue map image (.nii, .nii.gz)

|


**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-m, --mask TEXT:FILE REQUIRED</td>
           <td style="padding: 8px;">Brain mask (.nii/.nii.gz)</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-g, --gradient [TEXT,TEXT]:FILE REQUIRED</td>
           <td style="padding: 8px;">bvals and bvecs text files given in FSL format, e.g., -g bvals bvecs.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-o, --order INT</td>
           <td style="padding: 8px;">Spherical harmonic order. Default: 8</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-s, --deltaStep FLOAT</td>
           <td style="padding: 8px;">Gradient descent step size. Default: 0.0000001</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--D_inAx FLOAT</td>
           <td style="padding: 8px;">Intra-axonal diffusivity. Default: 0.0017 mm^2/s</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--D_trapped FLOAT</td>
           <td style="padding: 8px;">Trapped water diffusivity. Default: 0 mm^2/s</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--init_D_exAx_iso FLOAT</td>
           <td style="padding: 8px;">Initial value for extra-axonal diffusivity. Default: 0.001 mm^2/s</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--init_minNumConstraint INT</td>
           <td style="padding: 8px;">Initial number of constraints. Default: 100</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--constraintUpdateCount INT</td>
           <td style="padding: 8px;">Update count for constraints. Default: 1</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--disableFastOptimization</td>
           <td style="padding: 8px;">Disable fast optimization by fully solving the system at each iteration.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--bValLow FLOAT</td>
           <td style="padding: 8px;">Lowest b-value to include. Default: 10 s/mm^2</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--bValHigh FLOAT</td>
           <td style="padding: 8px;">Highest b-value to include. Default: 1000000000000 s/mm2</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--maxIter INT</td>
           <td style="padding: 8px;">Number of optimization steps. Default: 1000</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--xi_init FLOAT</td>
           <td style="padding: 8px;">Initial regularization value. Default: 0.04</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--xi_step FLOAT</td>
           <td style="padding: 8px;">Regularization step size. Default: 0.02</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--xi_stepCount INT</td>
           <td style="padding: 8px;">Number of regularization steps. Default: 100</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-c, --maxCrossings INT</td>
           <td style="padding: 8px;">Maximum number of fiber maxCrossings. Default: 4</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--noiseFloor FLOAT</td>
           <td style="padding: 8px;">Noise floor. Default: 0</td>
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


**References**

This method is based on incorporating a compartment model into a spherical deconvolution framework, directly optimizing spherical harmonics coefficients, for fiber orientation distribution (FOD) reconstruction, using an adaptively constrained energy minimization approach for efficient computation. This allows for sharper and cleaner FODs, better modeling of fiber crossings, and reliable estimation of compartment parameters.

.. [TranShi2015] `Tran, G. Shi, Y. "Fiber orientation and compartment parameter estimation from multi-shell diffusion imaging." IEEE Transactions on Medical Imaging, 2015, 34(11), pp.2320-2332. <https://ieeexplore.ieee.org/abstract/document/7103314>`__