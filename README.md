# COMO: Cooperative Modeller

![COMO Screenshot](https://github.com/moisesjbc/como/blob/master/img/como.png "COMO")


## About

COMO (Cooperative Modeller) is a cooperative 3d modelling sofware. It allows multiple users to connect though the network and work on a same 3D scene in real time.


## Current status

**"Completed"** (refactoring needed).


### A scenes modeller

The cooperative modeller COMO is oriented towards the colaborative creation of scenes instead of the creation of models. This mainly implies:

    Two users can't be working on the same 3D object at the same time. Only the user who selects an object (a mesh, a material, etc) can edit it until he / she until the object is unselected.
    The edges and vertices of 3D meshes can be edited.

The idea is that multiple users create a scene, for example, of the inside of a building, by combining primitives "wall", "chair", "table", etc. COMO wasn't designed for modelling a human character from zero, for example.


## Features

COMO allows users to:

* create a shared scene or connect to a created one.
* load meshes from files and add them to the scene (only OBJ files).
* create cubes, cones, cylinders and spheres.
* create directional lights until the limit defined by the server is reached.
* select and transform multiple entities (meshes, lights and cameras).
	* Allowed transformations are translations, rotations and scales.
	* Incorporates arbitrary and axis related (X, Y or Z) transformations.
	* The pivot point for rotations and scales can be one of three: world origin, selection's centroid or individual centroids.
* remove entities from the scene (excepting the scene's camera).
* edit the parameters of the materials associated to the meshes:
	* Color
	* Ambient reflectivity
	* Diffuse reflectivity
	* Specular reflectivity
	* Specular exponent
* load textures and apply them to walls of cubes, cones, cylinders and spheres. For every wall and its texture, user can set the texture's offset and scale over the wall.
* save and load scenes.


## Technical features

### Operative system

Although COMO has been developed to be cross-platform, it only works on Ubuntu (x86_64) currently. With minor changes in code it should work on others platforms... in theory.

### Libraries

COMO makes use of the following libraries:

* Qt 5.2.1 for the GUI.
* Boost 1.55.0 for network communication, threading and access to the file system.
* OpenGL 4.2 for 3D graphics.
* SDL2 y SDL_image 2.0 for textures loading.
* GLM 0.9.5.1 for vectorial and matrix computation.

COMO also depends on the program "gnome-terminal" for creating a server from the client software.


## Building COMO (Ubuntu 14.04)

### Dependencies
1. Install Qt 5.2 or above. You can dowload and use the online installer [from here](http://qt-project.org/downloads) or you can also install Qt from [this Ubuntu repository](https://launchpad.net/~ubuntu-sdk-team/+archive/ppa) issuing the following commands in a terminal.
   ```
   sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
   sudo apt-get update
   sudo apt-get install qtdeclarative5-dev qtcreator
   ```

2. Use following commands to display your OpenGL version and check that it is 4.2 or greater.
   ```
   sudo apt-get install mesa-utils
   glxinfo | grep OpenGL
   ```

3. Install the rest of dependencies (make sure that the versions are equal or greater than those indicated in "Dependencies".
   ```
   sudo apt-get install gnome-terminal libglm-dev libboost1.55-dev libboost-system1.55.0 libboost-filesystem1.55.0 libboost-thread1.55.0 libboost-system1.55-dev libboost-thread1.55-dev libboost-filesystem1.55-dev gnome-terminal libsdl2-image-dev
   ```

### Building

1. Download the latest version of this repository [from here](https://github.com/moisesjbc/como)

2. Enter in the downloaded directory.

   ```
   cd <directory>
   ```

3. Initialize and update the repository's submodules:

   ```
   git submodule update --init --recursive
  ```

4. Open "project/client/como_project.pro" file with Qt Creator and set the build directory to "../../bin/client".

5. Open "project/server/server.pro" file with Qt Creator and set the build directory to "../../bin/server".

6. Optionally add "-j (n)" to "make additional arguments" in both project's properties. (n) is the desired number of concurrent threads that will be used for building.

7. Build both projects in "release" mode and enjoy!


## Multimedia 

### Documentation

[Check COMO's wiki](https://github.com/moisesjbc/como/wiki) for learning what you can do with COMO and how to do it.

### Videos

On this [Youtube playlist](http://www.youtube.com/playlist?list=PLfi4ueBWh9m5T1Gp72CyoKc2hi_RGa_NG) you can see all the videos I have uploaded about the cooperative modeller COMO.
