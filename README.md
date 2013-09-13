COMO: Cooperative Modeller
====

## About
COMO is a project which aims to create a Cooperative Modeller, where users could share and work on a 3D scene at the same time. Two examples where this can be useful are presented:
* An online modelling course. Teacher can chat with his/her students and teach them how to create the model while doing it. When finished, students can try to make their own models while the teacher supervises the process.
* A team working on a map for a videogame. Main designer can sketch the world while the others see it. When finished, the main designer assignes each part of the map to a junior designer, so all can work in parallel while seeing how the whole map envolves.

## Roadmap
The proyect will be developed through prototypes.

### Current status
By now, COMO is only a local modeller of cubes :). The user can:
- [X] Create cubes.
- [X] Delete cubes.
- [X] View the scene through 4 viewports, whose cameras can be set to six predefined views: left, right, top, bottom, front and back. 
- [X] Transform cubes (translations, rotations and scales). The rotations and scales can be made relative to three different pivot points: selection centroid, individual origins and world origin.

### Next steps
Currently, I'm designing the online component: thinking how the scenes will be shared, how the synchronization between clients will be done, etc.

## Instalation
1. Clone this repository in local:

	git clone git@github.com:Neodivert/como - directory -

2.  Enter in the previous local directory.

	cd - directory -

3. Initialize and update the repository's submodules:

	git submodules init
	git submodules update
