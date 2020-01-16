import Trekker
import numpy as np

FOD_path=b"test_fod.nii";
SEED_path=b"test_seed.nii";

tracker=Trekker.tracker(FOD_path);
tracker.numberOfThreads(8);
tracker.seed_maxTrials(1);
tracker.stepSize(0.01);
tracker.minRadiusOfCurvature(0.25);
tracker.minFODamp(0.04);
tracker.probeQuality(3);

numberOfStreamlinesToGenerate = 100;
seed_coordinates = np.array([[-8.49,-8.39,2.5]]);
seed_coordinates = np.repeat(seed_coordinates,numberOfStreamlinesToGenerate,axis=0);
tracker.seed_coordinates(seed_coordinates);

tracker.seed_image_using_label(SEED_path,1);
tracker.seed_count(10);

streamlines = tracker.run();

tracker.printParameters()
