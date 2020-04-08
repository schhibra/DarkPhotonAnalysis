samples = {}

#double muon datasets
def AddDMSamples(samples):

    samples['DoubleMuonRunBv2']  = [
        '/DoubleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunC']  = [
        '/DoubleMuon/Run2016C-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunD']  = [
        '/DoubleMuon/Run2016D-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunE']  = [
        '/DoubleMuon/Run2016E-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunF']  = [
        '/DoubleMuon/Run2016F-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunG']  = [
        '/DoubleMuon/Run2016G-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['DoubleMuonRunH']  = [
        '/DoubleMuon/Run2016H-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]
