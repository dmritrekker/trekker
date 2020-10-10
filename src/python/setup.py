from setuptools import setup
from setuptools.extension import Extension

from Cython.Build import cythonize
from Cython.Distutils import build_ext

from pathlib import Path
import shutil
import os
import platform
import glob

class TrekkerBuildExt(build_ext):
    def run(self):
        build_ext.run(self)
        target_dir = Path(self.build_lib)
        os.makedirs(str(target_dir / 'Trekker'))
        for file in glob.glob(str(Path('cython/*'))):
           shutil.copy(file, str(target_dir / 'Trekker'))
        shutil.copyfile(str(Path('build/cython') / 'Trekker.cpp'), str(target_dir / 'Trekker/Trekker.cpp'))


LIB_DIRS          = ['cython/']
INC_DIRS          = ['cython/']

if (platform.system()=="Linux"):
    LIBS              = ["Trekker","z","pthread"]
if (platform.system()=="Windows"):
    LIBS              = ["Trekker"]

with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name='Trekker',
    description='Python package for parallel transport tractography (PTT)',
    long_description=long_description,
    long_description_content_type="text/markdown",
    version='0.7',
    setup_requires=[
        'setuptools>=18.0',
        'cython>=0.28.4',
    ],
    ext_modules=cythonize(
            [
                Extension(
                'Trekker',
                sources=['cython/Trekker.pxd','cython/Trekker.pyx'],
                libraries=LIBS,
                library_dirs=LIB_DIRS,
                include_dirs=INC_DIRS,
                language="c++",)
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
    python_requires='>=3.6',
    license='BSD 2-Clause License',
    platforms=['linux','win'],
)
