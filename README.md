## Overview
This project is a demo game, showcasing mechanics that will be implemented in a larger 3D OpenGL game with the same theme. The demo is a single-level platformer that follows the player's character - an incarnation of Indiana Jones - who attempts to cross pits filled with spikes. The character is able to perform short jumps, and can grapple with his whip when needing to jump over larger gaps.
<br>The game implements simple controls, player states that manage game mechanics (e.g. swinging across gaps or death by falling into spikes), as well as using .svg graphics for the shapes of the objects (converted from XML data into OpenGL-compatible polygons).


## Controls
Move left/right - A, D<br>
Jump - space<br>
Use whip - left click. The whip can help you grab onto a pole in order to allow you to jump farther.


## Implementation of triangle polygon models
The objects in the game use an unusual design of being composed by triangles rather than being simply overlain with a 2D texture. The models were designed in Inkscape and structured as .svg image files, which have an underlying XML data structure that records coordinates. A parser for the data structure was written in Python and used to convert the models into objects with OpenGL compatibility. Thus, while the Rectangle class provides the solid hitboxes of the objects, all triangles that were designed in the svg file act as the "texture" component of the objects.


## Explanation of jumping animation algorithm
- Our rendering is done by actually imposing translation transformations on the terrain instead of the player, keeping the player's character in the center at all times. It's the kinematic principle of the frame of reference - which, in our system, is the player, and all world coordinates are calculated with respect to the player. This is therefore not an efficient approach, but it works for our demonstration purposes.

- This is done by making the objects move as if the camera was moving instead; all objects (except the player) have the following translation applied:
	trans = glm::translate(glm::mat4(1.0f), glm::vec3(camera_pos_x, camera_pos_y, 0.0));
where camera_pos_x, camera_pos_y are given by player input and animations.
camera_pos_y is affected by by the jumping animation.

- Firstly, the jumping animation is initiated when the space key is pressed. This alters the global boolean "jumping = 1;", and captures the moment when the jump was started with "firstJumpFrame = glfwGetTime();".

- The jump animation is configured to last for a certain period of time:
	jumpDuration = 1.0f;
as well as to displace the player over a certain height:
	jumpHeight = 0.2f;

- Until the jumpDuration period has passed, the state of "jumping == 1" will remain active. This will perform a change of the player's position (aka a movement, aka an animation) with respect to deltaJumpTime, denoting how much time has passed since the animation began:
	float currentJumpFrame = glfwGetTime();
	deltaJumpTime = currentJumpFrame - firstJumpFrame;

- Our movement will be given by a cosine; however, simply applying:
	cos(deltaJumpTime)
	will give a cosine that increases up to a point, but does not descend; the player will appear to rise in the air, and remain afloat. In order to create an effect of a cosine that rises and then falls, we will need to account for the values over (-1, 1) which a cosine over (0, pi) will take.

- We will therefore have to treat our deltaJumpTime's progress percentage over the (0, jumpDuration) timespan, as the cosine's progress over (0, pi). This will be called "normalizedTime":
	normalizedTime = (deltaJumpTime / jumpDuration*2) * 3.1456f;

- Then, the cosine can be employed, along with the "oscillation radius", i.e. the range over which the cosine's oscillation happens, which in our case, is the jump height:
	const float radius = jumpHeight;
	float cosinus = cos(normalizedTime);

- The "cos(normalizedTime)" will output values over (-1, 1), so to bring it to (0, jumpHeight), we will need to add an offset and account for jumpHeight.
	camera_pos_y = (-1.0f + cosinus) * radius;


- Finally, we add a check for when the duration of the animation expires, which will remove the "jumping" state:
	if (deltaJumpTime > jumpDuration)
	jumping = 0;
