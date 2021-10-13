from libcpp.string cimport string
from libcpp.vector cimport vector

import numpy as np

cdef extern from "trekker.h":

    bint isInitialized();
    
    cdef cppclass Trekker:
        
        Trekker(string) except +
        Trekker(string,string,bint) except +
        Trekker(string,string,string,string,bint) except +
        
        vector[vector[vector[double]]] run();

        void printParameters();
        void resetParameters();
        
        # Global config
        void numberOfThreads(int);
        void timeLimit(int);
        
        # Tracker config
        void orderOfDirections(string);
        void stepSize(double);
        void minRadiusOfCurvature(double);
        void minFODamp(double);
        void dataSupportExponent(double);
        void minLength(double);
        void maxLength(double);
        void atMaxLength(string);
        void writeInterval(int);
        void directionality(string);
        void maxEstInterval(int)        
        void initMaxEstTrials(int);
        void propMaxEstTrials(int);
        void maxSamplingPerStep(int);
        void useBestAtInit(bint);
        
        void probeLength(double);
        void probeRadius(double);
        void probeCount(int);
        void probeQuality(int);
        void ignoreWeakLinks(double);

        # Seed config
        void seed_image(string);
        void seed_image(string,int);
        void seed_coordinates(vector[vector[double]]);
        void seed_coordinates_with_directions(vector[vector[double]],vector[vector[double]]);
        void seed_count(int);
        void seed_countPerVoxel(int);
        void seed_maxTrials(int);
        
        # Pathway config
        void clearPathwayRules();
        
        void pathway_require_entry(string);
        void pathway_A_require_entry(string);
        void pathway_B_require_entry(string);
        
        void pathway_require_exit(string);
        void pathway_A_require_exit(string);
        void pathway_B_require_exit(string);
        
        void pathway_stop_at_entry(string);
        void pathway_A_stop_at_entry(string);
        void pathway_B_stop_at_entry(string);
        
        void pathway_stop_at_exit(string);
        void pathway_A_stop_at_exit(string);
        void pathway_B_stop_at_exit(string);
        
        void pathway_discard_if_enters(string);
        void pathway_A_discard_if_enters(string);
        void pathway_B_discard_if_enters(string);
        
        void pathway_discard_if_exits(string);
        void pathway_A_discard_if_exits(string);
        void pathway_B_discard_if_exits(string);

        void pathway_discard_if_ends_inside(string);
        void pathway_A_discard_if_ends_inside(string);
        void pathway_B_discard_if_ends_inside(string);

        void pathway_require_entry(string,int);
        void pathway_A_require_entry(string,int);
        void pathway_B_require_entry(string,int);
        
        void pathway_require_exit(string,int);
        void pathway_A_require_exit(string,int);
        void pathway_B_require_exit(string,int);
        
        void pathway_stop_at_entry(string,int);
        void pathway_A_stop_at_entry(string,int);
        void pathway_B_stop_at_entry(string,int);
        
        void pathway_stop_at_exit(string,int);
        void pathway_A_stop_at_exit(string,int);
        void pathway_B_stop_at_exit(string,int);
        
        void pathway_discard_if_enters(string,int);
        void pathway_A_discard_if_enters(string,int);
        void pathway_B_discard_if_enters(string,int);
        
        void pathway_discard_if_exits(string,int);
        void pathway_A_discard_if_exits(string,int);
        void pathway_B_discard_if_exits(string,int);

        void pathway_discard_if_ends_inside(string,int);
        void pathway_A_discard_if_ends_inside(string,int);
        void pathway_B_discard_if_ends_inside(string,int);

