#!/bin/sh

path=$(pwd)/${1} 
cd $path../cmake

while true; do
    read -p "WARNING! delete all file in: $(pwd) [Y/N] ?" yn
    case $yn in
        [Yy]* ) make install; break;;
        [Nn]* ) exit;;
        * ) echo "Please input Y or N.";;
    esac
done

more
rm -rf *
cmake ..
make
