COMO: Cooperative Modeller
====

## About
COMO is a project which aims to create a Cooperative Modeller, where users could share and work on a 3D scene at the same time. Two examples where this can be useful are presented:
* An online modelling course. Teacher can chat with his/her students and teach them how to create the model while doing it. When finished, students can try to make their own models while the teacher supervises the process.
* A team working on a map for a videogame. Main designer can sketch the world while the others see it. When finished, the main designer assignes each part of the map to a junior designer, so all can work in parallel while seeing how the whole map envolves.

## Roadmap
The proyect will be developed through prototypes.

### Current status
By now, COMO allows users to have fun with cubes :). Each user can do the following things and the others will see it:
* Creating cubes.
* Deleting cubes.
* Selecting multiple cubes.
* Translating, rotating and scalling selections of cubes. The rotations and scales can be made relative to three different pivot points: selection centroid, individual origins and world origin.


## Instalation

### Dependencies
COMO makes use of the following APIs and libraries:
* Qt 5.2.
* Boost 1.55 (boost_system and boost_thread).
* OpenGL 4.2 and GLSL.
* GLM.


### Steps

* Clone this repository in local:

```
	git clone git@github.com:Neodivert/como <directory>
```

* Enter in the previous local directory.

```
	cd <directory>
```

* Initialize and update the repository's submodules:

```
	git submodule init  
	git submodule update  
```

* For compiling the client: enter into "project/client" subdirectory and compile project "como_project.pro" (Qt creator). The binary will be placed in "bin/client" directory.

* For compiling the server: enter into "project/server" subdirectory and compile project "server.pro" (Qt creator). The binary will appear in "bin/server" directory.
