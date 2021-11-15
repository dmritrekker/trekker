# distutils: language = c++

import sys
from Trekker cimport Trekker

cdef class T:

    cdef Trekker *_ptr

    def __cinit__(self):
        self._ptr = NULL

    def __dealloc__(self):
        if self._ptr is not NULL:
            del self._ptr
            self._ptr = NULL

    def __reduce__(self):
        return tuple(self._ptr.run())

    def run(self):
        return self._ptr.run()

    def printParameters(self):
        return self._ptr.printParameters()

    def resetParameters(self):
        return self._ptr.resetParameters()


    # Global config
    def numberOfThreads(self, n):
        self._ptr.numberOfThreads(n)

    def timeLimit(self, n):
        self._ptr.timeLimit(n)


    # Tracker config

    def orderOfDirections(self, s):
        self._ptr.orderOfDirections(s)

    def stepSize(self, x):
        self._ptr.stepSize(x)

    def minRadiusOfCurvature(self, x):
        self._ptr.minRadiusOfCurvature(x)

    def minFODamp(self, x):
        self._ptr.minFODamp(x)

    def maxEstInterval(self, n):
        self._ptr.maxEstInterval(n)

    def dataSupportExponent(self, x):
        self._ptr.dataSupportExponent(x)

    def minLength(self, x):
        self._ptr.minLength(x)

    def maxLength(self, x):
        self._ptr.maxLength(x)

    def atMaxLength(self, s):
        self._ptr.atMaxLength(s)

    def writeInterval(self, n):
        self._ptr.writeInterval(n)

    def directionality(self, s):
        self._ptr.directionality(s)

    def maxSamplingPerStep(self, n):
        self._ptr.maxSamplingPerStep(n)

    def initMaxEstTrials(self, n):
        self._ptr.initMaxEstTrials(n)

    def propMaxEstTrials(self, n):
        self._ptr.propMaxEstTrials(n)

    def useBestAtInit(self, q):
        self._ptr.useBestAtInit(q)

    def probeLength(self, x):
        self._ptr.probeLength(x)

    def probeRadius(self, x):
        self._ptr.probeLength(x)

    def probeCount(self, n):
        self._ptr.probeCount(n)

    def probeQuality(self, n):
        self._ptr.probeQuality(n)

    def ignoreWeakLinks(self, x):
        self._ptr.ignoreWeakLinks(x)




    # Seed config
    def seed_image(self, s, l=None):
        if l is None:
            self._ptr.seed_image(s)
        else:
            self._ptr.seed_image(s,l)

    def seed_coordinates(self, c):
        self._ptr.seed_coordinates(c)

    def seed_coordinates_with_directions(self, c, d):
        self._ptr.seed_coordinates_with_directions(c, d)

    def seed_count(self, n):
        self._ptr.seed_count(n)

    def seed_countPerVoxel(self, n):
        self._ptr.seed_countPerVoxel(n)

    def seed_maxTrials(self, n):
        self._ptr.seed_maxTrials(n)




    # Pathway config
    def clearPathwayRules(self):
        self._ptr.clearPathwayRules()

    def pathway_require_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_require_entry(s)
        else:
           self._ptr.pathway_require_entry(s,l)

    def pathway_A_require_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_require_entry(s)
        else:
           self._ptr.pathway_A_require_entry(s,l)

    def pathway_B_require_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_require_entry(s)
        else:
           self._ptr.pathway_B_require_entry(s,l)

    def pathway_require_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_require_exit(s)
        else:
           self._ptr.pathway_require_exit(s,l)

    def pathway_A_require_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_require_exit(s)
        else:
           self._ptr.pathway_A_require_exit(s,l)

    def pathway_B_require_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_require_exit(s)
        else:
           self._ptr.pathway_B_require_exit(s,l)



    def pathway_stop_at_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_stop_at_entry(s)
        else:
           self._ptr.pathway_stop_at_entry(s,l)

    def pathway_A_stop_at_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_stop_at_entry(s)
        else:
           self._ptr.pathway_A_stop_at_entry(s,l)

    def pathway_B_stop_at_entry(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_stop_at_entry(s)
        else:
           self._ptr.pathway_B_stop_at_entry(s,l)

    def pathway_stop_at_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_stop_at_exit(s)
        else:
           self._ptr.pathway_stop_at_exit(s,l)

    def pathway_A_stop_at_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_stop_at_exit(s)
        else:
           self._ptr.pathway_A_stop_at_exit(s,l)

    def pathway_B_stop_at_exit(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_stop_at_exit(s)
        else:
           self._ptr.pathway_B_stop_at_exit(s,l)



    def pathway_discard_if_enters(self, s, l=None):
        if l is None:
           self._ptr.pathway_discard_if_enters(s)
        else:
           self._ptr.pathway_discard_if_enters(s,l)

    def pathway_A_discard_if_enters(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_discard_if_enters(s)
        else:
           self._ptr.pathway_A_discard_if_enters(s,l)

    def pathway_B_discard_if_enters(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_discard_if_enters(s)
        else:
           self._ptr.pathway_B_discard_if_enters(s,l)

    def pathway_discard_if_exits(self, s, l=None):
        if l is None:
           self._ptr.pathway_discard_if_exits(s)
        else:
           self._ptr.pathway_discard_if_exits(s,l)

    def pathway_A_discard_if_exits(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_discard_if_exits(s)
        else:
           self._ptr.pathway_A_discard_if_exits(s,l)

    def pathway_B_discard_if_exits(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_discard_if_exits(s)
        else:
           self._ptr.pathway_B_discard_if_exits(s,l)



    def pathway_discard_if_ends_inside(self, s, l=None):
        if l is None:
           self._ptr.pathway_discard_if_ends_inside(s)
        else:
           self._ptr.pathway_discard_if_ends_inside(s,l)

    def pathway_A_discard_if_ends_inside(self, s, l=None):
        if l is None:
           self._ptr.pathway_A_discard_if_ends_inside(s)
        else:
           self._ptr.pathway_A_discard_if_ends_inside(s,l)

    def pathway_B_discard_if_ends_inside(self, s, l=None):
        if l is None:
           self._ptr.pathway_B_discard_if_ends_inside(s)
        else:
           self._ptr.pathway_B_discard_if_ends_inside(s,l)


cdef object PyTrekker(Trekker *Trekker_ptr):

    cdef T m = T.__new__(T)
    m._ptr = Trekker_ptr
    return m


def initialize(pathToFODimage=None, arg1=None, arg2=None, arg3=None, arg4=None):

    if (isInitialized()==True):
        raise Exception('Trekker is already initialized. To initialize with a new FOD image, first delete your existing Trekker object with \"del <variable_name>\" ')

    cdef Trekker *_ptr = NULL
#
    """Factory function"""
    if (pathToFODimage is None):
        print("Provide FOD image path")
    elif (arg1 is None) and (arg2 is None) and (arg3 is None) and (arg4 is None):
        _ptr = new Trekker(pathToFODimage)
    elif (arg1 is not None) and (arg2 is not None) and (arg3 is None) and (arg4 is None):
        _ptr = new Trekker(pathToFODimage,arg1,arg2)
    elif (arg1 is not None) and (arg2 is not None) and (arg3 is not None) and (arg4 is not None):
        _ptr = new Trekker(pathToFODimage,arg1,arg2,arg3,arg4)
    else:
        raise Exception('Invalid number of arguments used for initialization.')

    cdef T m = PyTrekker(_ptr)
    return m
