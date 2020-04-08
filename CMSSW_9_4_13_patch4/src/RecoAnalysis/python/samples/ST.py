samples = {}

def AddSTSamples(samples):

    samples['ST_tW_top_5f']  = [ # ext1 is not processed
        '/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

    samples['ST_tW_antitop_5f']  = [ # ext1 is not processed
        '/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

    samples['ST_t-channel_top_4f']  = [ # ext1 is not processed
        '/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'FileBased',
        '',
        1
    ]

    samples['ST_t-channel_antitop_4f']  = [ # ext1 is not processed
        '/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM',
        ['isMC=True','useLHEWeights=True','applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_mcRun2_asymptotic_v3'],
        'EventAwareLumiBased',
        '',
        50000
    ]

