import Trekker
import numpy as np

FOD_path=b"test_fod.nii";
tracker=Trekker.tracker(FOD_path);

numberOfStreamlinesToGenerate = 1;
seed_coordinates = np.array([[-8.49,-8.39,2.5]]);
seed_coordinates = np.repeat(seed_coordinates,numberOfStreamlinesToGenerate,axis=0);

tracker.set_seeds(seed_coordinates);
streamlines = tracker.run();
print(streamlines)
