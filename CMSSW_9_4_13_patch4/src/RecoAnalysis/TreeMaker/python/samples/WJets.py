samples = {}

def AddWJetsSamples(samples):

    samples['WJetsToLNu']  = [
        '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        10000
    ]

    samples['WJetsToLNu_ext2']  = [
        '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'FileBased',
        '',
        1
    ]

