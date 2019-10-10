samples = {}

from ScoutingAnalysis.TreeMaker.samples.SC          import AddSCSamples
from ScoutingAnalysis.TreeMaker.samples.DP_Official import AddDP_OfficialSamples
from ScoutingAnalysis.TreeMaker.samples.DY          import AddDYSamples
from ScoutingAnalysis.TreeMaker.samples.Upsilon     import AddUpsilonSamples

AddSCSamples(samples)
AddDP_OfficialSamples(samples)
AddDYSamples(samples)
AddUpsilonSamples(samples)
