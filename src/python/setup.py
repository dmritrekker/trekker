import glob
import os
import platform
import shutil
from pathlib import Path

from Cython.Build import cythonize
from Cython.Distutils import build_ext
from setuptools import setup
from setuptools.extension import Extension


class TrekkerBuildExt(build_ext):
    def run(self):
        build_ext.run(self)
        target_dir = Path(self.build_lib)
        os.makedirs(str(target_dir / "Trekker"))
        for file in glob.glob(str(Path("cython/*"))):
            shutil.copy(file, str(target_dir / "Trekker"))
        shutil.copyfile(
            str(Path("build/cython") / "Trekker.cpp"),
            str(target_dir / "Trekker/Trekker.cpp"),
        )


LIB_DIRS = ["cython/"]
INC_DIRS = [
    "cython/",
    "../",
    "../nifticlib-2.0.0/znzlib",
    "../nifticlib-2.0.0/niftilib",
    "../base",
    "../math",
    "../image",
    "../config",
    "../tracker",
]

cpp_sources = [str(i) for i in (Path("../").glob("**/*.cpp"))]
c_sources = [str(i) for i in (Path("../").glob("**/*.c"))]
sources = cpp_sources + c_sources
sources = [
    source
    for source in sources
    if not source.endswith("cmd.cpp") and not source.lower().endswith("trekker.cpp")
]
sources = sources + ["cython/Trekker.pxd", "cython/Trekker.pyx", "../trekker.cpp"]

print(f"\n\n\n{sources=}\n\n\n")

if platform.system() == "Windows":
    LIBS = []
    EXTRA_COMPILE_ARGS = ["/DBUILD_FOR_WINDOWS"]
elif platform.system() == "Darwin":
    LIBS = ["z"]
    EXTRA_COMPILE_ARGS = ["-std=c++11","-D BUILD_FOR_MACOS", "-D HAVE_ZLIB"]
else:
    LIBS = ["z"]
    EXTRA_COMPILE_ARGS = ["-std=c++11", "-D HAVE_ZLIB"]

with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name="Trekker",
    description="Python package for parallel transport tractography (PTT)",
    long_description=long_description,
    long_description_content_type="text/markdown",
    version="0.9",
    setup_requires=["setuptools>=18.0", "cython>=0.28.4", "wheel"],
    ext_modules=cythonize(
        [
            Extension(
                "Trekker",
                sources=sources,
                libraries=LIBS,
                library_dirs=LIB_DIRS,
                include_dirs=INC_DIRS,
                language="c++",
                extra_compile_args=EXTRA_COMPILE_ARGS,
            )
        ],
        language_level=3,
        build_dir="build",
        compiler_directives=dict(always_allow_keywords=True),
    ),
    cmdclass=dict(build_ext=TrekkerBuildExt),
    author="Dogu Baran Aydogan",
    author_email="baran.aydogan@aalto.fi",
    url="https://dmritrekker.github.io/",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.6",
    license="BSD 2-Clause License",
    platforms=["linux", "win"],
)
