Trekker
=======

Trekker implements a state-of-the-art tractography algorithm, parallel transport tractography (PTT). PTT is capable of reconstructing geometrically smooth and topographically organized fiber bundles.

Trekker software has the following features:

- **Clean probabilistic tractography** with PTT.
- **Pathway rules** that offer flexible options to reconstruct intricate connections.
- **Time limiting** enables tracking for a given length of duration, specification of streamline count is optional.
- **Multithreading** reconstructs streamlines using multiple cores of a CPU.
- **Command line interface**, an essential requirement to write scripts for processing *big data* in computer grids.
- **.vtk** output, that is compatible with a large number of third party 3D rendering software, for easy and high quality visualizations.

For complete documentation, tutorials and examples, visit https://dmritrekker.github.io/.



Installation
------------

**Building from source:**

	git clone https://github.com/dmritrekker/trekker
	
	cd trekker

	make


**Stand-alone executables:**

For Linux: https://github.com/dmritrekker/trekker/tree/v0.4/binaries/trekker_linux_x64_v0.4

For Windows: https://github.com/dmritrekker/trekker/tree/v0.4/binaries/trekker_win_x64_v0.4.exe
