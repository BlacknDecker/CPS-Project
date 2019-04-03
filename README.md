# CPS-Project

## Documentation

The project is documented (more or less) in the [wiki](https://github.com/BlacknDecker/CPS-Project/wiki).

The Kilombo simulator is documented [here](https://github.com/JIC-CSB/kilombo/blob/master/doc/manual.md).

## Project Folders Structure

CPS-Project
|_ src                  <br />  
|	|_ includes         <br />
|	|_ kilombo_settings <br />
|                       <br />
|_ wiki-images          <br />


* **root folder** contains this readme file and some shell script to automate the build process.
* **src** folder contains all the source code, with the headers files in the *includes* folder and the kilombo jsons in the *kilombo_settings* folder.
* **wiki-images** folder contains the images used in the documentation wiki.

## Build and Run the Project

To build the project, move to CPS-Project folder and run `bash witchSaysColor_BUILD`.
This will build the project and run the executable. Don't forget to update the project with `git pull`!

If you just want to run the last executable available, run `bash witchSaysColor_EXECUTE` in the root folder.

