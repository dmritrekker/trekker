fieldExport
===========

exports a field from a tractogram (.vtk only)

|

**Usage**

.. code-block:: bash

    ./trekker fieldExport [OPTIONS] <input tractogram> <output file>

|

**Positionals**

- `<input tractogram> TEXT:FILE REQUIRED`
  Input tractogram

- `<output file> TEXT REQUIRED`
  Output file name (.csv)

|

**Options**

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Option</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Description</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">-s, --skipFieldNames</td>
           <td style="padding: 8px;">Skip writing of field names in the first row of output.</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">--fields TEXT ... REQUIRED</td>
           <td style="padding: 8px;">Names of the fields to export that are separated with comma, e.g., field1,field2,field4.</td>
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
           <td style="padding: 8px; font-weight: 500;">-v, --verbose TEXT</td>
           <td style="padding: 8px;">Verbose level. Options are "quiet", "fatal", "error", "warn", "info", and "debug". Default=info.</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500; border-bottom: 1px solid #000;">-f, --force</td>
           <td style="padding: 8px; border-bottom: 1px solid #000;">Force overwriting of existing file.</td>
       </tr>
   </table>
