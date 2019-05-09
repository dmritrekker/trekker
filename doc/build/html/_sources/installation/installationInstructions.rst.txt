Installation instructions 
=========================================

Dependencies and configuration
------------------------------

.. _zlib: https://zlib.net/

- zlib_: Required to read compressed nifti files with extension :code:`.nii.gz`. This is enabled by default in the :code:`Makefile` with :code:`ENABLE_ZLIB=YES`. **zlib** typically exists in most systems. However if you do not have zlib or do not wish to use it, you may set :code:`ENABLE_ZLIB=NO`. In this case, you can use trekker with nifti files having :code:`.nii` extension only.


Installation
------------

Linux
+++++

The compilation is done only by running:

::

	make

This compiles the :code:`trekker` executable under the :code:`build/bin` folder.

.. note::

	- Trekker requires **g++-4.8** or above for compilation since some components require C++11 support.
	
Windows
+++++++

A Windows (test using Windows 10) compatible Trekker binary is pre-compiled using mingw and can be obtained from `https://github.com/dmritrekker/trekker/tree/v0.4/binaries/trekker_win_x64_v0.4.exe <https://github.com/dmritrekker/trekker/tree/v0.4/binaries/trekker_win_x64_v0.4.exe>`__.

.. note::

	- Support for Windows is experimental. The binary is tested in 64bit Windows 10 systems.
	- Windows executable may not be as fast as the Linux version.
	- The Windows binary is cross-compiled in a Debian 9 machine with g++-mingw-w64 (6.3.0).

MAC OS
++++++

Not tested.

