#!/bin/bash

cd /afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor
eval `scramv1 runtime -sh`

cd /eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/jobs_data_v5
root -l -b -q trim_gethisto_data_20190513_192407_scout_9.C
