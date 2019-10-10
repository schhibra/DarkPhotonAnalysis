#!/bin/bash

workdir="data_v7"
jobDir="/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/jobs_$workdir"

if [ ! -d $jobDir ]; then
    mkdir -p $jobDir;
fi

if [ ! -d $jobDir/histos ]; then
    mkdir -p $jobDir/histos;
fi

if [ ! -d dir_$workdir ]; then
    mkdir -p dir_$workdir;
fi

if [ ! -d dir_$workdir/out ]; then
    mkdir -p dir_$workdir/out;
fi

if [ ! -d dir_$workdir/err ]; then
    mkdir -p dir_$workdir/err;
fi

if [ ! -d dir_$workdir/log ]; then
    mkdir -p dir_$workdir/log;
fi

homedir=$PWD

for datasetpath in `less datasetpaths_${workdir}.txt`;do
    
    echo "datasetpath" $datasetpath
    
    crabdir=`echo $datasetpath | awk -F "crab_" '{print $2}' | awk -F "/" '{print $1}'` #20190513_192510
    #echo "crabdir" $crabdir

    OutDataset=`echo $datasetpath | awk -F "tree_" '{print "trim_"$2".root"}' | sed "s?trim_?trim_${crabdir}_?g"` # trim_20190513_192407_scout_9.root
    #echo "OutDataset" $OutDataset
    
    Dataset=`echo $OutDataset | awk -F "_" '{print $2"_"$3"_"$4"_"$5"_"$6"_"$7}' | awk -F "." '{print $1}'` # 20190513_192407_scout_9
    echo "Dataset" $Dataset

    cat trim_gethisto_data.C | sed "s?DATASET?${datasetpath}*.root?g" | sed "s?OUTPUT?$jobDir/histos/${OutDataset}?g" | sed "s?trim_gethisto_data(?trim_gethisto_data_${Dataset}(?g" > $jobDir/trim_gethisto_data_${Dataset}.C 

    cat run.sh | sed "s?home?$homedir?g" | sed "s?jobs?$jobDir?g" | sed "s?trim_gethisto_data.C?trim_gethisto_data_${Dataset}.C?g" > dir_$workdir/run_${Dataset}.sh
    
    cat condor_job_setup.sub | sed "s?run.sh?run_${Dataset}.sh?g" > dir_$workdir/condor_job_setup_${Dataset}.sub

    cd dir_$workdir
    #condor_submit condor_job_setup_${Dataset}.sub
    cd -

done