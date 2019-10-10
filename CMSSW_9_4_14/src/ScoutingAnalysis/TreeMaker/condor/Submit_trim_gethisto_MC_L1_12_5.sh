#!/bin/bash

cat > datasetpaths_MC.txt <<EOF
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M10.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M12p5.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M15.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M20.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M25.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M30.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M35.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M40.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M50.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M60.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M70.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DYJetsToLL_M50_ext1_1star.root
/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_UpsilonMuMu.root
EOF

#/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DYJetsToLL_M10to50_ext1.root

workdir="MC"
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

eval `scramv1 runtime -sh`

for datasetpath in `less datasetpaths_${workdir}.txt`;do
    
    echo "datasetpath" $datasetpath
    
    OutDataset=`echo $datasetpath | awk -F "tree_" '{print "trim_L1_12_5_"$2}'`
    #echo "OutDataset" $OutDataset
    
    cat trim_gethisto_MC_L1_12_5.C | sed "s?DATASET?${datasetpath}?g" | sed "s?OUTPUT?$jobDir/histos/${OutDataset}?g" | sed "s?trim_gethisto_MC_L1_12_5(?trim_gethisto_MC_L1_12_5_tmp(?g" > dir_$workdir/trim_gethisto_MC_L1_12_5_tmp.C 

    cd dir_$workdir
    root -l -b -q trim_gethisto_MC_L1_12_5_tmp.C 
    cd -

done