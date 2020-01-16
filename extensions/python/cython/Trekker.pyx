# Trekker.pyx

from libcpp.string cimport string
from libcpp.vector cimport vector
import numpy as np

cdef extern from "trekker.h":
    cdef cppclass Trekker:
        Trekker(string pathToFODimage);
        vector[vector[vector[double]]] run();
        
        void printParameters();
        
        # Global config
        void numberOfThreads(int);
        void timeLimit(int);
        
        # Tracker config
        void checkWeakLinks(bool);
        void stepSize(double);
        void minRadiusOfCurvature(double);
        void minFODamp(double);
        void maxEstInterval(int);
        void dataSupportExponent(double);
        void minLength(double);
        void maxLength(double);
        void atMaxLength(string);
        void writeInterval(int);
        void directionality(string);
        void maxSamplingPerStep(int);
        void initMaxEstTrials(int);
        void propMaxEstTrials(int);
        void useBestAtInit(bool);
        
        void probeLength(double);
        void probeRadius(double);
        void probeCount(int);
        void probeQuality(int);

        # Seed config
        void seed_image(string)
        void seed_image_using_label(string,int)
        void seed_coordinates(vector[vector[double]]);
        void seed_coordinates_with_directions(vector[vector[double]],vector[vector[double]]);
        void seed_count(int);
        void seed_countPerVoxel(int);
        void seed_maxTrials(int);
        
        

cdef class tracker:
    cdef Trekker *thisptr
    def __cinit__(self, pathToFODimage):
        self.thisptr = new Trekker(pathToFODimage)
    def __dealloc__(self):
        del self.thisptr
    
    def __reduce__(self):
        return tuple(self.thisptr.run())

    def run(self):
        return self.thisptr.run()
    
    def printParameters(self):
        return self.thisptr.printParameters()

    # Global config
    def numberOfThreads(self, n):
        self.thisptr.numberOfThreads(n)
        
    def timeLimit(self, n):
        self.thisptr.timeLimit(n)

    # Tracker config
    def checkWeakLinks(self, q):
        self.thisptr.writeInterval(q)
        
    def stepSize(self, x):
        self.thisptr.stepSize(x)
        
    def minRadiusOfCurvature(self, x):
        self.thisptr.minRadiusOfCurvature(x)
    
    def minFODamp(self, x):
        self.thisptr.minFODamp(x)
        
    def maxEstInterval(self, n):
        self.thisptr.maxEstInterval(n)
        
    def dataSupportExponent(self, x):
        self.thisptr.dataSupportExponent(x)
        
    def minLength(self, x):
        self.thisptr.minLength(x)
        
    def maxLength(self, x):
        self.thisptr.maxLength(x)
        
    def atMaxLength(self, s):
        self.thisptr.atMaxLength(s)
        
    def writeInterval(self, n):
        self.thisptr.writeInterval(n)
    
    def directionality(self, s):
        self.thisptr.directionality(s)
        
    def maxSamplingPerStep(self, n):
        self.thisptr.maxSamplingPerStep(n)
        
    def initMaxEstTrials(self, n):
        self.thisptr.initMaxEstTrials(n)
        
    def propMaxEstTrials(self, n):
        self.thisptr.propMaxEstTrials(n)
        
    def useBestAtInit(self, n):
        self.thisptr.useBestAtInit(n)    
    
    def probeLength(self, x):
        self.thisptr.probeLength(x)
        
    def probeRadius(self, x):
        self.thisptr.probeLength(x)
    
    def probeCount(self, n):
        self.thisptr.probeCount(n)
    
    def probeQuality(self, n):
        self.thisptr.probeQuality(n)
    
    # Seed config
    def seed_image(self, s):
        self.thisptr.seed_image(s)
        
    def seed_image_using_label(self, s, l):
        self.thisptr.seed_image_using_label(s,l)
        
    def seed_coordinates(self, c):
        self.thisptr.seed_coordinates(c)
        
    def seed_coordinates_with_directions(self, c, d):
        self.thisptr.seed_coordinates_with_directions(c, d)
        
    def seed_count(self, n):
        self.thisptr.seed_count(n)
        
    def seed_countPerVoxel(self, n):
        self.thisptr.seed_countPerVoxel(n)
        
    def seed_maxTrials(self, n):
        self.thisptr.seed_maxTrials(n)
