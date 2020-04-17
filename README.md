# ShadowVolume
Shadow Volume technique implemented in C++ with OpenGL.

### Thanks to an Italian friend.
### I just polished it with mirror-liked surface.
### And more Models are added in the DEER directory.
### If you want all libraries used in this project
### Please search online and Do It By Youself. Enjoy it!
![ShadowVolume](https://image.ibb.co/maYiqH/Screenshot_2018_03_19_12_08_19.png)

Computer Graphics and 3D.

## Implementation
### Step One
Perform the normal rendering of the scene by establishing two output channels for the fragment shader, one for the ambient component, the other for the specular+diffuse one: both ambient and specular+diffuse must be saved for step 3.
### Step Two
Set the stencil buffer so that it is always successful and keeps track of the decrements (back faces) and increments (front faces), the geometry shader then takes care of the volumes of shadow, necessary to determine the occluded areas.

Depth fail method:
1. Disable writes to the depth and color buffers.
2. Set the stencil operation to increment on depth fail (only count shadows behind the object) if front-face.
3. Set the stencil operation to decrement on depth fail if back-face.
4. Render the shadow volumes.

### Step Three
Execute the blending of the ambient and the specular+diffuse calculated at the first step, excluding from the rendering of the specular+diffuse the occluded pixels (indicated by the stencil buffer).

## Mirror-liked surface added
### Ball
![ShadowVolume](http://47.110.134.247/OpenGL/ball1.png)
### Deer
![ShadowVolume](http://47.110.134.247/OpenGL/deer2.png)
