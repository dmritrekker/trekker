import Trekker
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

line="-----------------------------------------------------------------"

# Initialize trekker tracker
print(line+"\nDEMO 1: Initialize using demo_FOD.nii:\n")

FOD_path=b"data/FOD.nii"
tracker=Trekker.initialize(FOD_path)

print("\nDEMO 1 RESULT: Initialization was successful\n"+line+"\n")

# Tracking using seed image
print(line+"\nDEMO 2: Seed image based tracking with following parameters:")

seed_path=b"data/seedImage.nii"
tracker.seed_image(seed_path);
tracker.seed_count(100);
tracker.printParameters()
streamlines = tracker.run()

fig = plt.figure()
ax = fig.gca(projection='3d')
for i in range(100):
    ax.plot(streamlines[i][0], streamlines[i][1], streamlines[i][2],'r')

ax.auto_scale_xyz([0, -32], [0, -16], [0, 16])
ax.axis('off')
ax.set_title("Tracking using seed voxel")

print("\nDEMO 2 RESULT: Seed image based tracking was successful\n"+line+"\n")

# Tracking using seed coordinates
print(line+"\nDEMO 3: seed coordinate based tracking with following parameters:")

numberOfStreamlinesToGenerate = 100;
seed_coordinates = np.array([[-8.5,-8.5,2.5]]);
seed_coordinates = np.repeat(seed_coordinates,numberOfStreamlinesToGenerate,axis=0);
tracker.seed_coordinates(seed_coordinates);
tracker.printParameters()
streamlines = tracker.run()

fig = plt.figure()
ax = fig.gca(projection='3d')
for i in range(100):
    ax.plot(streamlines[i][0], streamlines[i][1], streamlines[i][2],'r')

ax.auto_scale_xyz([0, -32], [0, -16], [0, 16])
ax.axis('off')
ax.set_title("Tracking using seed coordinate")

print("\nDEMO 3 RESULT: Seed coordinate based tracking was successful\n"+line+"\n")

print("If you can see the arcs, all is good!\n")

plt.show()

