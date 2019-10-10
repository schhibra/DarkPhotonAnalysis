#!/bin/bash

if [ $# -lt 2 ]
then
    echo "You need to provide two arguments : "
    echo "(1) Action option to execute : --submit --status --resubmit --kill --clean"
    echo "(2) Name of the folder in which to look for CRAB jobs"

elif [ $# -gt 2 ]
then
    echo "You need to provide two arguments : "
    echo "(1) Action option to execute : --submit --status --resubmit --kill --clean"
    echo "(2) Name of the folder in which to look for CRAB jobs"

elif [ $1 != "--status" ] && [ $1 != "--submit" ] && [ $1 != "--resubmit" ] && [ $1 != "--kill" ] && [ $1 != "--clean" ]
then
    echo "One of the following options is allowed as first argument : "
    echo "--submit --status --resubmit --kill --clean"

else
    cd $2/;
    
    if [ $1 = --status ]
    then
        echo " " 
        echo ---------------------- $2 -----------------------
        for Y in `ls -d */`
        do
            crab status -d $Y;
        done
    fi
    
    if [ $1 = --submit ]
    then
        cp ../crabConfig.py ./;
        crab submit;
    fi
    
    if [ $1 = --kill ]
    then
        for Y in `ls -d */`
        do
            crab kill -d $Y;
        done
    fi
    
    if [ $1 = --resubmit ]
    then
        for Y in `ls -d */`
        do
            crab resubmit -d $Y;
        done
    fi
    
    if [ $1 = --clean ]
    then
        rm -rf ./*;
    fi
    
    cd ../;

fi
