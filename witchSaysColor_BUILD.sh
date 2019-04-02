#!/bin/bash

#############################################
#       AUTOMATE THE BUILD PROCESS          #
# - - - - - - - - - - - - - - - - - - - - - #
# NB The script name has this composition:  #
#     <execFileName>_BUILD.sh               #
#############################################



# Clear the terminal
clear

# Move in the current directory of the script
cd $PWD/src

# Automatically take the name of the file to launch
execFileName=$( echo $0 | cut -d'_' -f 1 )

echo "Name of the executable file: $execFileName"

# Move kilombo settings files
mv -fv ./kilombo_settings/*.json .

# Build
echo "Building project..."
buildStatus=$( make )
echo "Done >> $buildStatus"

# Remove object files
rm *.o

# Check Build status
if [ "$?" -lt "2" ]
then
   # Move executable file in exe folder
   mv -fv "${execFileName}.exe" ../exe 
   echo "Running executable: ${execFileName}.exe"
   ../exe/"${execFileName}.exe"   #Execute the builded file
else
   echo "Build Error!!!"
fi


# Move back kilombo settings files
mv -fv *.json ./kilombo_settings/

#Going back to start
cd $PWD

# End
echo "--- Script END ---"
