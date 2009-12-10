Canyon Pilot
======================

by Eric Levine and Paul Baumgart

Storyline: High over enemy territory, your jet is attacked by an enemy fighter. You dive down into a canyon to avoid the fighter, and manage to shake him off your tail. But the enemy's laser cannons surround the canyon- if you fly too high, they'll mercilessly blow you apart. How long can you keep your cover without crashing into the canyon walls? How long can you survive?

The game begins with an into sequence: an enemy fighter engaging your plane. You dodge their lasers and dive into the canyon. The two airplanes follow separate piecewise Bezier curves for this animation.

The player controls an airplane that flies through an infinite canyon, using the keyboard arrow keys. The objective of the game is to pilot the plane through the canyon as long as possible without crashing.

The airplane is a 3d model of an F-22, with a material that gives the appearance of radar-absorbing paint. The camera follows the plane.

Per-quad collision detection is implemented for three points: the wing tips and the nose of the plane. If any of these points collide with the ground, the plane explodes.

The canyon is constructed on-the-fly, in a separate thread, from a piecewise cubic Bezier curve. The curve determines the path of the canyon. In order to ensure a smooth path, the piecewise curve is C1 continuous. A height map is generated based on path defined by the curve. The height map is perturbed using Gaussian-distributed random noise and fractal terrain generation techniques in order to create a realistic appearance for the canyon. The canyon is textured, and this texture is modulated with colors depending on the height level in order to create geological layers.

A sky box forms the backdrop for the scene.

The lasers are drawn on the screen using OpenGL's line drawing functions.

The player's score is calculated based on the amount of time before the plane crashes. A high score is maintained in a file.

Below is a summary of the technical features to be included in this project:

- Per-quad collision detection
- Piecewise Bezier curves as paths
- C1 continuity
- Sky box
- Fractal terrain generation
- Height maps

Controls:
- Keyboard arrow keys/,aoe: up/left/down/right
- Space bar: run game at double speed
- p: pause the game

