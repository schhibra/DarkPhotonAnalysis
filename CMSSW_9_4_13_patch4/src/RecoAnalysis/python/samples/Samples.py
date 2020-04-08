samples = {}

from RecoAnalysis.TreeMaker.samples.SM          import AddSMSamples
from RecoAnalysis.TreeMaker.samples.DM          import AddDMSamples
from RecoAnalysis.TreeMaker.samples.DY          import AddDYSamples
from RecoAnalysis.TreeMaker.samples.VV          import AddVVSamples
from RecoAnalysis.TreeMaker.samples.WJets       import AddWJetsSamples
from RecoAnalysis.TreeMaker.samples.TT          import AddTTSamples
from RecoAnalysis.TreeMaker.samples.ST          import AddSTSamples
from RecoAnalysis.TreeMaker.samples.DP_Official import AddDP_OfficialSamples

AddSMSamples(samples)
AddDMSamples(samples)
AddDYSamples(samples)
AddVVSamples(samples)
AddWJetsSamples(samples)
AddTTSamples(samples)
AddSTSamples(samples)
AddDP_OfficialSamples(samples)
