samples = {}

#double muon datasets
def AddSMSamples(samples):

    samples['SingleMuonRunBv2']  = [
        '/SingleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunC']  = [
        '/SingleMuon/Run2016C-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunD']  = [
        '/SingleMuon/Run2016D-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunE']  = [
        '/SingleMuon/Run2016E-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunF']  = [
        '/SingleMuon/Run2016F-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10','useMediumID2016=True'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunG']  = [
        '/SingleMuon/Run2016G-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]

    samples['SingleMuonRunH']  = [
        '/SingleMuon/Run2016H-17Jul2018-v1/MINIAOD',
        ['applyHLTFilter=True','applyDimuonFilter=True','GlobalTag=94X_dataRun2_v10'],
        'LumiBased',
        '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt',
        100
    ]
