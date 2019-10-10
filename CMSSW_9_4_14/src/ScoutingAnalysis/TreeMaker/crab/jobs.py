import sys
import os
from glob import glob

samples   = []

#samples  += ["ScoutingRun*"]
#samples  += ["ZdScoutingM*"]
samples  += ["DYJetsToLL_M-10to50*"]
#samples  += ["UpsilonMuMu*"]

xsamples  = [] 

folders   = []
xfolders  = []

for sample in samples :
    folders += glob(sample+"/")

for xsample in xsamples :
    xfolders += glob(xsample+"/")

craboptions  = []
craboptions += ['--status']
craboptions += ['--submit']
craboptions += ['--resubmit']
craboptions += ['--kill']
craboptions += ['--clean']

if len(sys.argv) == 1 :
    print "Provide at least one of these arguments : "
    print craboptions

elif len(sys.argv) > 2 :
    print "Only one argument allowed"

elif not sys.argv[1] in craboptions : 
    print "One of the following arguments is allowed  : "
    print craboptions
    print "You have provided argument : " + sys.argv[1]        

else :
    for folder in folders :
        if not folder in xfolders :
            command = "./jobs.sh " + sys.argv[1] + " " + folder
            os.system(command)

