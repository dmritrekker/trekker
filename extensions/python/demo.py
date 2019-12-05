import Trekker
import numpy as np

FOD_path=b"/home/baran/Work/code/dev/baranaydogan/test_fod.nii";
tracker=Trekker.tracker(FOD_path);

numberOfStreamlinesToGenerate = 1000;
seed_coordinates = np.array([[-8.49,-8.39,2.5]]);
seed_coordinates = np.repeat(seed_coordinates,numberOfStreamlinesToGenerate,axis=0);

tracker.set_numberOfThreads(20);
tracker.set_seed_maxTrials(1);
tracker.set_stepSize(0.1);
tracker.set_minFODamp(0.04);
tracker.set_probeQuality(3);

tracker.set_seeds(seed_coordinates);
streamlines = tracker.run();

