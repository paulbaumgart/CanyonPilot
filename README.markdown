Final Project Proposal
======================

by Paul Baumgart and Eric Levine

We intend to build a simple 3d flight simulator game.

The player will control an airplane that flies through an infinite canyon. The objective of the game will be to pilot the plane through the canyon as long as possible without crashing. The player's score will be based on the distance between the start point and the crash site.

The plane will be a textured 3d model, with moving ailerons. It will be controlled with the keyboard arrow keys. The plane will cast a shadow on the ground as it flies, based on a directional light high above the landscape that simulates sunlight during midday. A bounding volume for the plane will be used to determine if it crashes into the canyon walls. The camera will follow the plane. Time permitting, we will include a "Pilot's view" mode in addition to the default 3rd-person view mode.

The canyon will be built on-the-fly from a piecewise cubic Bezier curve. The curve will determine the path of the canyon. In order to ensure a smooth path, the piecewise curve will be C1 continuous. A height map will be generated based on path defined by the curve. The height map will be perturbed using fractal terrain generation techniques in order to create a realistic appearance for the canyon.

Below is a summary of the technical features to be included in this project:
-  Shadow mapping
-  Bounding volumes for collision detection
-  Piecewise Bezier curves as paths
-  C1 continuity
-  Fractal terrain generation
-  Height maps

