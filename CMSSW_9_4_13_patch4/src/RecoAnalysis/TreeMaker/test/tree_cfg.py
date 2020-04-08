import os
import FWCore.ParameterSet.Config as cms

# Set parameters externally 
from FWCore.ParameterSet.VarParsing import VarParsing
params = VarParsing('analysis')

params.register(
    'isMC', 
    False, 
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether the sample is simulation or data'
    )

params.register(
    'useLHEWeights', 
    False, 
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether or not to use the events weights from a Monte Carlo generator'
    )

params.register(
    'applyHLTFilter', 
    False, 
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether or not to ask the event to fire a trigger used in the analysis'
    )

params.register(
    'applyDimuonFilter', 
    False, 
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether or not to ask the event to fire a trigger used in the analysis'
    )

params.register(
    'useMediumID2016', 
    False, 
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether or not to use the events weights from a Monte Carlo generator'
)

params.register(
    'roccorData', 
    '../data/Rochester/RoccoR2016.txt', 
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Path of the Rochester correction data files'
)

params.register(
    'redoJets', 
    False, #without putting it yes, the cmsRun will not find updatedPatJets
    VarParsing.multiplicity.singleton,VarParsing.varType.bool,
    'Flag to indicate whether or not to remake the jets collection with specified JEC payloads'
)

params.register(
    'trigProcess', 
    'HLT', 
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Process name for the HLT paths'
)

params.register(
    'Dataset',
    '',
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Get input files of a published dataset'
)

params.register(
    'DBSInstance',
    '',
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'DBS instance, e.g. prod/phys03 for USER samples'
)

params.register(
    'GlobalTag',
    '', 
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Global tag'
    )

# Define the process
process = cms.Process("treemaker")

# Parse command line arguments
params.parseArguments()

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# Set the process options -- Display summary at the end, enable unscheduled execution
process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary      = cms.untracked.bool(False) 
)

# How many events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(params.maxEvents) )

# Input EDM files
process.source = cms.Source("PoolSource",
    fileNames  = cms.untracked.vstring(params.inputFiles)
)

if params.Dataset != '':
    dbsinstance = " instance=" + params.DBSInstance if params.DBSInstance != '' else ""
    query = "das_client -query=\"file dataset=" + params.Dataset + dbsinstance + "\""
    fnames = os.popen(query).readlines()
    for fname in fnames:
        process.source.fileNames += [fname.rstrip()]

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

# Load the global tag
process.GlobalTag.globaltag = params.GlobalTag

# Define the services needed for the Rochester corrections and the treemaker
process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("tree_reco.root")
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    mmtree = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName  = cms.untracked.string('TRandom3')
    )
)

# Tree for the generator weights
process.gentree = cms.EDAnalyzer("LHEWeightsTreeMaker",
                                 lheInfo       = cms.InputTag("externalLHEProducer"),
                                 genInfo       = cms.InputTag("generator"),
                                 pileupinfo    = cms.InputTag("slimmedAddPileupInfo"),

                                 useLHEWeights = cms.bool(params.useLHEWeights)
                                 )

# Select good primary vertices
process.goodVertices = cms.EDFilter("VertexSelector",
    src    = cms.InputTag("offlineSlimmedPrimaryVertices"),
    cut    = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True)
)

# Make tree
process.mmtree = cms.EDAnalyzer('TreeMaker',
                                triggerresults    = cms.InputTag("TriggerResults", "", params.trigProcess),
                                triggerobjects    = cms.InputTag("slimmedPatTrigger"),

                                vertices          = cms.InputTag("offlineSlimmedPrimaryVertices"),

                                muons             = cms.InputTag("slimmedMuons"),
                                jets              = cms.InputTag("slimmedJets"),

                                pileupinfo        = cms.InputTag("slimmedAddPileupInfo"),
                                geneventinfo      = cms.InputTag("generator"),
                                gens              = cms.InputTag("prunedGenParticles"),
                                lheInfo           = cms.InputTag("externalLHEProducer"),

                                applyHLTFilter    = cms.bool(params.applyHLTFilter),
                                applyDimuonFilter = cms.bool(params.applyDimuonFilter),
                                isMC              = cms.bool(params.isMC),
                                useLHEWeights     = cms.bool(params.useLHEWeights),                                
                                useMediumID2016   = cms.bool(params.useMediumID2016),

                                RochesterCorr     = cms.bool(True),
                                data              = cms.string(params.roccorData)
)

# Define the Task object
process.edtask = cms.Task()
for key in process.__dict__.keys():
    if(type(getattr(process,key)).__name__=='EDProducer' or type(getattr(process,key)).__name__=='EDFilter') :
        process.edtask.add(getattr(process,key))

process.taskseq = cms.Sequence(process.edtask)

# Analysis path
if params.isMC : 
    process.p = cms.Path(process.taskseq + process.gentree + process.goodVertices + process.mmtree)
else : 
    process.p = cms.Path(process.taskseq +                   process.goodVertices + process.mmtree)

