#!/bin/sh
if ![ -d "$1" ] 
then
    echo "Directory $1 exists" 
else
    echo "Directory $1 does not exist"
fi
