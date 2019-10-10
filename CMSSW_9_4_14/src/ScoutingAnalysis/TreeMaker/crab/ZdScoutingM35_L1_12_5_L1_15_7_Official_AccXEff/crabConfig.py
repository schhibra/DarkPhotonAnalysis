from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

from ScoutingAnalysis.TreeMaker.samples.Samples import samples

import os
dset = os.getcwd().replace(os.path.dirname(os.getcwd())+'/', '')

print 'Submitting jobs for dataset ' + samples[dset][0]

params = samples[dset][1]
params+= ['roccorData=Rochester/RoccoR2017.txt']
print 'Config parameters for sample',
print dset + ' :',
print params

config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../../test/tree_scout_cfg.py'

config.JobType.pyCfgParams = params

config.JobType.scriptExe   = '../exec.sh'
config.JobType.inputFiles  = ['../Rochester.tar.gz']

config.Data.inputDataset   = samples[dset][0]
config.Data.splitting      = samples[dset][2]
config.Data.unitsPerJob    = samples[dset][4]
if samples[dset][3] != '' :
    config.Data.lumiMask   = samples[dset][3]

config.Data.outLFNDirBase  = '/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017'
config.Data.publication    = False
config.Site.storageSite    = 'T2_CH_CERN'

#config.Data.outLFNDirBase  = '/store/user/%s/' % (getUsernameFromSiteDB())
#config.Data.publication    = False
#config.Site.storageSite    = 'T2_IT_Bari'

