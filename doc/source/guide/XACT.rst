XACT
====

.. Note::

    - eXtended Anatomically Constrained Tractography (XACT) is currently an experimental feature in Trekker.

    - Please submit an issue in our `GitHub repository <https://github.com/dmritrekker/trekker/issues>`__ or send an e-mail to `baran.aydogan@uef.fi <mailto:baran.aydogan@uef.fi>`__ if you encounter any problems or have suggestions.

XACT leverages surface meshes as anatomical constraints together with extended options to the conventional ACT [Smith2012]_ [Yeh2017]_.

To create XACT surfaces from a Freesurfer folder, please use the `prepXact <../commands/prepXact.html>`__ command. The generated XACT file can then be used for fiber tracking using the `track <../commands/track.html>`__ command with the ``--xact`` option.

The XACT file contains multiple surfaces that are used to apply anatomical constraints during tractography. These surfaces include the following 13 regions:

.. table:: XACT surface labels and descriptions

+-----+-------------------------+--------------+
|Label|Description              | Abbreviation |
+-----+-------------------------+--------------+
|1    |Left white matter        | L_WM         |
+-----+-------------------------+--------------+
|2    |Right white matter       | R_WM         |
+-----+-------------------------+--------------+
|3    |Left gray matter         | L_GM         |
+-----+-------------------------+--------------+
|4    |Right gray matter        | R_GM         |
+-----+-------------------------+--------------+
|5    |Left subcortical         | L_SUB        |
+-----+-------------------------+--------------+
|6    |Right subcortical        | R_SUB        |
+-----+-------------------------+--------------+
|7    |Cerebrospinal fluid      | CSF          |
+-----+-------------------------+--------------+
|8    |Cerebellar white matter  | CER_WM       |
+-----+-------------------------+--------------+
|9    |Cerebellar gray matter   | CER_GM       |
+-----+-------------------------+--------------+
|10   |Brain stem               | BS           |
+-----+-------------------------+--------------+
|11   |Inferior brain stem      | I_BS         |
+-----+-------------------------+--------------+
|12   |Abnormality              | ABN          |
+-----+-------------------------+--------------+
|13   |Background               | BG           |
+-----+-------------------------+--------------+

.. Important::

    **Known issue:** The surfaces created by ``prepXact`` are known to intersect each other. This may cause problems for some applications. 
    
    While we are still working on improvements for future releases, we advise to check the surfaces carefully before use to ensure they are adequate for the application in question. 
    
    An example showing the intersecting surfaces is shown below:

    .. figure:: xact_overlap.png
        :scale: 25 %
        :alt: prepXact generated XACT surfaces showing overlaps


**Common rules**

When the ``--xact`` option is used, the following rules are **always** applied:

- **Seeding**: Seeds are randomly generated in white matter (L_WM, R_WM, CER_WM) and brain stem (BS).
- **Allowed endpoints**: Streamlines are required to end inside gray matter (L_GM, R_GM, CER_GM), subcortical structures (L_SUB, R_SUB), brain stem (BS), abnormality (ABN), or background (BG).
- **Exclusion**: Streamlines are discarded if they enter cerebrospinal fluid (CSF).

|

**Default behavior**

In addition to the common rules, Trekker applies specific rules for different regions by default. These rules control seeding within these regions and how streamlines behave upon entering or exiting them.

.. raw:: html

   <table style="width: 100%; border-collapse: collapse;">
       <tr style="background-color: #f2f2f2;">
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Region</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Seeding</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Stop after entry</th>
           <th style="padding: 8px; border-bottom: 1px solid #000; border-top: 1px solid #000;">Stop before exit</th>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">Cortex (L_GM, R_GM, CER_GM)</td>
           <td style="padding: 8px;">OFF</td>
           <td style="padding: 8px;">ON</td>
           <td style="padding: 8px;">OFF</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">Background (BG)</td>
           <td style="padding: 8px;">OFF</td>
           <td style="padding: 8px;">ON</td>
           <td style="padding: 8px;">OFF</td>
       </tr>
       <tr>
           <td style="padding: 8px; font-weight: 500;">Subcortex (L_SUB, R_SUB)</td>
           <td style="padding: 8px;">ON</td>
           <td style="padding: 8px;">OFF</td>
           <td style="padding: 8px;">OFF</td>
       </tr>
       <tr style="background-color: #f2f2f2;">
           <td style="padding: 8px; font-weight: 500;">Abnormality (ABN)</td>
           <td style="padding: 8px;">ON</td>
           <td style="padding: 8px;">OFF</td>
           <td style="padding: 8px;">OFF</td>
       </tr>
   </table>

|

**Modifying behavior**

The default behavior can be modified using specific flags. These flags adjust the region-specific rules to accommodate different tracking requirements.

.. table:: XACT behavior modification flags

    +-------------------------------+-------------------------------------------------------+------------------------------------------------------------------+
    | Flag                          | Description                                           | Effect                                                           |
    +===============================+=======================================================+==================================================================+
    | ``--xact_intracortical``      | Performs tractography also within the intracortical   | Enables seeding in cortex, disables stopping after entry,        |
    |                               | regions defined in the XACT file (L_GM + R_GM +       | and enables stopping before exit.                                |
    |                               | CER_GM).                                              |                                                                  |
    +-------------------------------+-------------------------------------------------------+------------------------------------------------------------------+
    | ``--xact_cranial``            | Performs tractography also within the cranial region  | Enables seeding in background, disables stopping after entry,    |
    |                               | defined in the XACT file (BG).                        | and enables stopping before exit.                                |
    +-------------------------------+-------------------------------------------------------+------------------------------------------------------------------+
    | ``--xact_subcortical_deadend``| Streamlines are truncated before they exit            | Enables stopping before exit for subcortical regions.            |
    |                               | subcortical regions (L_SUB + R_SUB).                  |                                                                  |
    +-------------------------------+-------------------------------------------------------+------------------------------------------------------------------+
    | ``--xact_abnormality_deadend``| Streamlines are truncated before they exit            | Enables stopping before exit for abnormality regions.            |
    |                               | abnormality regions (ABN).                            |                                                                  |
    +-------------------------------+-------------------------------------------------------+------------------------------------------------------------------+

.. note::

    - Inferior brain stem (I_BS) is considered part of the brain stem (BS) and is not currently used as a separate label during tractography.

    - Abnormality (ABN) region is optional. This region can be included to represent tumours, lesions or other abnormalities if they are present. Please use ``prepXact``'s ``--abnormality`` option to include this region in the XACT file. If not provided, the related rules are ignored.

**References**

.. [Smith2012] `Smith RE, Tournier JD, Calamante F, Connelly A. "Anatomically-constrained tractography: improved diffusion MRI streamlines tractography through effective use of anatomical information." Neuroimage. 2012 Sep 1;62(3):1924-38. <https://doi.org/10.1016/j.neuroimage.2012.06.005>`__
.. [Yeh2017] `Yeh, C.H., Smith, R.E., Dhollander, T. and Connelly, A. "Mesh-based anatomically-constrained tractography for effective tracking termination and structural connectome construction."" In Proc ISMRM (Vol. 58). <https://cds.ismrm.org/protected/17MProceedings/PDFfiles/0058.html>`__