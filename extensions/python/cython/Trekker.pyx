# Trekker.pyx

from libcpp.string cimport string
from libcpp.vector cimport vector
import numpy as np

cdef extern from "trekker.h":
    cdef cppclass Trekker:
        Trekker(string pathToFODimage);
        void set_seeds(vector[vector[double]]);
        vector[vector[vector[double]]] run();

cdef class tracker:
    cdef Trekker *thisptr
    def __cinit__(self, pathToFODimage):
        self.thisptr = new Trekker(pathToFODimage)
    def __dealloc__(self):
        del self.thisptr
    def set_seeds(self, coordinates):
        self.thisptr.set_seeds(coordinates)
    def run(self):
        return self.thisptr.run()
