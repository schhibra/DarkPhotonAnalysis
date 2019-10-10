#!/bin/bash

cd home
eval `scramv1 runtime -sh`

cd jobs
root -l -b -q trim_gethisto_data.C
