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

setup(
    name='Trekker',
    version='0.5',
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
            build_dir="build",
            compiler_directives=dict(always_allow_keywords=True),
        ),
    cmdclass=dict(build_ext=TrekkerBuildExt),
    author="Dogu Baran Aydogan",
    author_email="baran.aydogan@aalto.fi",
    description="Trekker",
)
