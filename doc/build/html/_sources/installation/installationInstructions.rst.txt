Installation instructions
=========================================

Dependencies
------------
- cmake-3.15 or newer


Installation
------------

Linux and Windows
+++++++++++++++++

Open the *build_Linux.sh* or *build_Windows.bat* file, change paths for *cmake* and *python* if necessary.

For Linux:

::

	$ sh build_Linux.sh

For Windows:

::

	> build_Windows.bat


- :code:`trekker` executable will be installed under :code:`build/Linux/install/bin` or :code:`build/Windows/install/bin` depending on the OS.

- Static and shared libraries are installed under :code:`build/Linux/install/lib` with library header :code:`build/Linux/install/include/trekker.h`.

- A python package will be prepared and installed under :code:`build/Linux/install/extensions/python/dist`.


MAC OS
++++++

Not tested.
