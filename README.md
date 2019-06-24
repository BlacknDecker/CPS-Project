# CPS-Project

## Documentation

The project is documented (more or less) in the [wiki](https://github.com/BlacknDecker/CPS-Project/wiki).
There are some stuffs also on the [drive](https://drive.google.com/open?id=1mAG9sOHKC8oRA2ROmGlMzBnnXhJ0hSxz).

The Kilombo simulator is documented [here](https://github.com/JIC-CSB/kilombo/blob/master/doc/manual.md).

## Project Folders Structure

```bash
CPS-Project
├── README.md
├── src
│   ├── color_module.c
│   ├── communication_module.c
│   ├── game_module.c
│   ├── includes
│   │   ├── color_module.h
│   │   ├── communication_module.h
│   │   ├── game_module.h
│   │   ├── main.h
│   │   ├── movement_module.h
│   │   └── time_module.h
│   ├── kilombo_settings
│   │   ├── endstate.json
│   │   └── kilombo.json
│   ├── main.c
│   ├── Makefile
│   ├── movement_module.c
│   └── time_module.c
├── wiki-images
│   └── ...
├── witchSaysColor_BUILD.sh
├── witchSaysColor_EXECUTE.sh
└── witchSaysColor_UPDATE.sh

```
* **root folder** contains this readme file and some shell script to automate the build process.
* **src** folder contains all the source code, with the headers files in the *includes* folder and the kilombo jsons in the *kilombo_settings* folder.
* **wiki-images** folder contains the images used in the documentation wiki.

## Build and Run the Project

To build the project, move to CPS-Project folder and run `bash witchSaysColor_BUILD`.
This will build the project and run the executable. Don't forget to update the project with `git pull`!

If you just want to run the last executable available, run `bash witchSaysColor_EXECUTE` in the root folder.

