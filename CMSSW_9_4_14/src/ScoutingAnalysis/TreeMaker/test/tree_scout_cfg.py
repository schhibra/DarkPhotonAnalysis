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
    'trigProcess', 
    'HLT', 
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Process name for the HLT paths'
    )

params.register(
    'roccorData', 
    '../data/Rochester/RoccoR2017.txt', 
    VarParsing.multiplicity.singleton,VarParsing.varType.string,
    'Path of the Rochester correction data files'
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

# Define the services needed for the treemaker
process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("tree_scout.root")
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
   mmtree = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName  = cms.untracked.string('TRandom3')
    )
)

##--- l1 stage2 digis ---
process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )
process.load('PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff')

# Tree for the generator weights
process.gentree = cms.EDAnalyzer("LHEWeightsTreeMakerForScout",
                                 lheInfo       = cms.InputTag("externalLHEProducer"),
                                 genInfo       = cms.InputTag("generator"),
                                 pileupinfo    = cms.InputTag("addPileupInfo"),

                                 useLHEWeights = cms.bool(params.useLHEWeights)
                                 )

from ScoutingAnalysis.TreeMaker.TriggerPaths_cfi import getL1Conf

# Make tree
process.mmtree = cms.EDAnalyzer('TreeMaker',
                                triggerresults    = cms.InputTag("TriggerResults", "", params.trigProcess),
                                vertices          = cms.InputTag("hltScoutingMuonPackerCalo","displacedVtx","HLT"),
                                muons             = cms.InputTag("hltScoutingMuonPackerCalo"),
                                pileupinfo        = cms.InputTag("addPileupInfo"),
                                geneventinfo      = cms.InputTag("generator"),
                                gens              = cms.InputTag("genParticles", "", "HLT"),

                                lheInfo           = cms.InputTag("externalLHEProducer"),

                                recoMuons         = cms.InputTag("muons"),

                                isMC              = cms.bool(params.isMC),
                                useLHEWeights     = cms.bool(params.useLHEWeights),
                                applyHLTFilter    = cms.bool(params.applyHLTFilter),
                                applyDimuonFilter = cms.bool(params.applyDimuonFilter),
                                
                                doL1              = cms.bool(True),
                                
                                RochesterCorr     = cms.bool(True),
                                data              = cms.string(params.roccorData),                                

                                AlgInputTag           = cms.InputTag("gtStage2Digis"),
                                l1tAlgBlkInputTag     = cms.InputTag("gtStage2Digis"),
                                l1tExtBlkInputTag     = cms.InputTag("gtStage2Digis"),
                                ReadPrescalesFromFile = cms.bool( False ),
                                
                                l1Seeds           = cms.vstring(getL1Conf())
                                )

# Analysis path
if params.isMC : 
    process.p = cms.Path(process.gentree       + process.mmtree)
else : 
    process.p = cms.Path(process.gtStage2Digis + process.mmtree)
