import Trekker

import numpy as np

FOD_path=b"fod.nii"

tracker=Trekker.initialize(FOD_path)
tracker.numberOfThreads(8)
tracker.stepSize(0.01)
tracker.minRadiusOfCurvature(1)
tracker.minFODamp(0.05)
tracker.printParameters()

# Make a numpy array of coordinates
numberOfStreamlinesToGenerate = 1000;
seed_coordinates = np.array([[-8.49,-8.39,2.5]]);
seed_coordinates = np.repeat(seed_coordinates,numberOfStreamlinesToGenerate,axis=0);

tracker.seed_coordinates(seed_coordinates);

streamlines = tracker.run();

