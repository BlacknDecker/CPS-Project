#!/bin/bash

#############################################
#          AUTOMATE THE EXECUTION           #
# - - - - - - - - - - - - - - - - - - - - - #
# NB The script name has this composition:  #
#     <execFileName>_EXECUTE.sh             #
#############################################

ROOTPATH=$PWD

# Clear the terminal
clear

# Automatically take the name of the file to launch
execFileName=$( echo $0 | cut -d'_' -f 1 )

echo "Name of the executable file: $execFileName"

# Move kilombo settings files
cd "${ROOTPATH}/src/kilombo_settings"
mv -fv *.json ..

# Move in the current directory of the script
cd "${ROOTPATH}/exe"

echo "Running executable: ${execFileName}.exe"
./"${execFileName}.exe"   #Execute the builded file

# Move back kilombo settings files
cd "${ROOTPATH}/src/"
mv -fv *.json ./kilombo_settings

#Going back to start
cd "${ROOTPATH}"

# End
echo "--- Script END ---"
