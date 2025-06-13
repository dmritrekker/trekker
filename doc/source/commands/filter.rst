filter
======

filters tractograms

|

**Usage**

.. code-block:: bash

    ./trekker filter [OPTIONS] <input tractogram>

|

**Positionals**

- `<input tractogram> FILE REQUIRED`
  Input tractogram (.vtk, .tck)

|

.. tip::

    Please check the documentation for `pathways <../guide/pathways.html>`__ for a detailed explanation.
    
|   

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-o,--output FILE REQUIRED</td>
           <td style="padding: 8px;">Output tractogram (.vtk, .tck)</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-p, --pathway TEXT ...</td>
           <td style="padding: 8px;">Pathway rule.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-s, --seed TEXT ...</td>
           <td style="padding: 8px;">Seed definition.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--discard_seed TEXT ...</td>
           <td style="padding: 8px;">Discard seed definition.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--minlength FLOAT</td>
           <td style="padding: 8px;">Minimum length of the tracks. Default=0.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--maxlength FLOAT</td>
           <td style="padding: 8px;">Maximum length of the tracks. Default=infinite.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--stopAtMax</td>
           <td style="padding: 8px;">If set to "stop", when `maxLength` is reached, the propagation stops, and the streamline is written in the output. If set to "discard", the streamline is not written in the output. Default=discard.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--oneSided</td>
           <td style="padding: 8px;">If enabled, tracking is done only towards one direction.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--skipSeed</td>
           <td style="padding: 8px;">Does not output the points that are within the seed region.</td>
       </tr>
       <!--
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--allowEdgeSeeds</td>
           <td style="padding: 8px;">Allows seeding at the edges of pathway rules. Default: false.</td>
       </tr>
       -->
       <tr>
           <td style="padding: 8px; font-weight: 500;">--seed_trials INT</td>
           <td style="padding: 8px;">Number of random trials for assigning seed. Default: 0.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--inOrder</td>
           <td style="padding: 8px;">If enabled, all pathway requirements are going to be satisfied in the order that they are given. All pathway options should be defined for `pathway_A/pathway_B` in order to use this option.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--maxOut INT</td>
           <td style="padding: 8px;">Maximum number of output streamlines.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-a, --ascii</td>
           <td style="padding: 8px;">Write ASCII output (.vtk only).</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">--saveDiscarded TEXT</td>
           <td style="padding: 8px;">Path for saving discarded streamlines.</td>
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
