# Trekker.pyx

from libcpp.string cimport string
from libcpp.vector cimport vector
import numpy as np

cdef extern from "trekker.h":
    cdef cppclass Trekker:
        Trekker(string pathToFODimage);
        void set_seeds(vector[vector[double]]);
        vector[vector[vector[double]]] run();
        
        void set_numberOfThreads(int);
        void set_seed_maxTrials(int);
        void set_stepSize(double);
        void set_minFODamp(double);
        
        void set_probeQuality(int);

cdef class tracker:
    cdef Trekker *thisptr
    def __cinit__(self, pathToFODimage):
        self.thisptr = new Trekker(pathToFODimage)
    def __dealloc__(self):
        del self.thisptr
    
    def __reduce__(self):
        return tuple(self.thisptr.run())
        
    def set_seeds(self, coordinates):
        self.thisptr.set_seeds(coordinates)

    def run(self):
        return self.thisptr.run()

    def set_numberOfThreads(self, n):
        self.thisptr.set_numberOfThreads(n)

    def set_seed_maxTrials(self, n):
        self.thisptr.set_seed_maxTrials(n)
    
    def set_stepSize(self, stepSize):
        self.thisptr.set_stepSize(stepSize)

    def set_minFODamp(self, minFODamp):
        self.thisptr.set_minFODamp(minFODamp)
        
    def set_probeQuality(self, n):
        self.thisptr.set_probeQuality(n)
