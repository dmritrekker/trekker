fieldImport
===========

adds a new field with values read from a file (.vtk only)

|

**Usage**

.. code-block:: bash

    ./trekker fieldImport [OPTIONS] <input tractogram> <input field data> <input field owner> <input field data type> <input field data dimension> <field name>

|

**Positionals**

- `<input tractogram> TEXT:FILE REQUIRED`
  Input tractogram

- `<input field data> TEXT:FILE REQUIRED`
  Input field data

- `<input field owner> TEXT REQUIRED`
  Can be either "POINT" or "STREAMLINE"

- `<input field data type> TEXT REQUIRED`
  Can be either "float" or "int"

- `<input field data dimension> INT REQUIRED`
  Dimension of the field data

- `<field name> TEXT REQUIRED`
  Field name to write in the surface

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
