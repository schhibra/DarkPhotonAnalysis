samples = {}

def AddTTSamples(samples):

    samples['TTJets_DiLept_ext1']  = [
        '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

    samples['TTJets_DiLept']  = [
        '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

    samples['TTTo2L2Nu']  = [
        '/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

    samples['TTToSemilepton']  = [
        '/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'FileBased',
        '',
        1
    ]

