COMO: Cooperative Modeller
====

## About

COMO (Cooperative Modeller) is an 3D modelling software that allows multiple users to work on the same scene through the network (in real time). 

Among other things, users can create geometric primitives (cubes, cones, cylinders and spheres), import meshes from file (.obj), transform selections of objects (translations, rotations and scales), create multiple directional lights, edit materials or apply textures to certain meshes.

Currently COMO is only available for Ubuntu, but it has been built using cross-platform technologies and tools such as OpenGL, Qt or Boost, so porting the project to other platforms would be easier in the future.

[Check COMO's wiki](https://github.com/moisesjbc/como/wiki) for learning what you can do with COMO and how to do it.


## Dependencies
COMO makes use of the following APIs and libraries:
* Qt v5.2.1
* Boost v1.55 (boost_system, boost_thread, boost_filesystem and boost_asio).
* OpenGL v4.2 and GLSL.
* GLM v0.9.5.1
* SDL2_image

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
