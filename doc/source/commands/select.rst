select
======

selects streamlines from a tractogram

|

**Usage**

.. code-block:: bash

    ./trekker select [OPTIONS] <input tractogram> <output tractogram>

|

**Positionals**

- `<input tractogram> TEXT:FILE REQUIRED`
  Input tractogram (.vtk, .tck, .trk)

- `<output tractogram> TEXT REQUIRED`
  Output tractogram (.vtk, .tck, .trk)

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-s, --selection TEXT</td>
           <td style="padding: 8px;">File with binary values that mark selected streamlines with 1 and others with 0.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">-r, --random INT</td>
           <td style="padding: 8px;">Random tractogram file creating. One input required, total count for random lines.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-o, --ordered INT x 2</td>
           <td style="padding: 8px;">Ordered tractogram file creating. Two inputs required, begin and end index.</td>
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
