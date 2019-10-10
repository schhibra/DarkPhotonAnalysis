ifeq ($(strip $(ScoutingAnalysisTreeMakerAuto)),)
ScoutingAnalysisTreeMakerAuto := self/src/ScoutingAnalysis/TreeMaker/plugins
PLUGINS:=yes
ScoutingAnalysisTreeMakerAuto_files := $(patsubst src/ScoutingAnalysis/TreeMaker/plugins/%,%,$(wildcard $(foreach dir,src/ScoutingAnalysis/TreeMaker/plugins ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
ScoutingAnalysisTreeMakerAuto_BuildFile    := $(WORKINGDIR)/cache/bf/src/ScoutingAnalysis/TreeMaker/plugins/BuildFile
ScoutingAnalysisTreeMakerAuto_LOC_USE := self  FWCore/Framework FWCore/PluginManager FWCore/ParameterSet FWCore/ServiceRegistry FWCore/Utilities DataFormats/Scouting DataFormats/Math SimDataFormats/PileupSummaryInfo SimDataFormats/GeneratorProducts CondFormats/DataRecord CommonTools/UtilAlgos HLTrigger/HLTcore DataFormats/HLTReco HLTrigger/Muon DataFormats/VertexReco L1Trigger/L1TGlobal
ScoutingAnalysisTreeMakerAuto_PRE_INIT_FUNC += $$(eval $$(call edmPlugin,ScoutingAnalysisTreeMakerAuto,ScoutingAnalysisTreeMakerAuto,$(SCRAMSTORENAME_LIB),src/ScoutingAnalysis/TreeMaker/plugins))
ScoutingAnalysisTreeMakerAuto_PACKAGE := self/src/ScoutingAnalysis/TreeMaker/plugins
ALL_PRODS += ScoutingAnalysisTreeMakerAuto
ScoutingAnalysis/TreeMaker_forbigobj+=ScoutingAnalysisTreeMakerAuto
ScoutingAnalysisTreeMakerAuto_INIT_FUNC        += $$(eval $$(call Library,ScoutingAnalysisTreeMakerAuto,src/ScoutingAnalysis/TreeMaker/plugins,src_ScoutingAnalysis_TreeMaker_plugins,$(SCRAMSTORENAME_BIN),,$(SCRAMSTORENAME_LIB),$(SCRAMSTORENAME_LOGS)))
ScoutingAnalysisTreeMakerAuto_CLASS := LIBRARY
else
$(eval $(call MultipleWarningMsg,ScoutingAnalysisTreeMakerAuto,src/ScoutingAnalysis/TreeMaker/plugins))
endif
ALL_COMMONRULES += src_ScoutingAnalysis_TreeMaker_plugins
src_ScoutingAnalysis_TreeMaker_plugins_parent := ScoutingAnalysis/TreeMaker
src_ScoutingAnalysis_TreeMaker_plugins_INIT_FUNC += $$(eval $$(call CommonProductRules,src_ScoutingAnalysis_TreeMaker_plugins,src/ScoutingAnalysis/TreeMaker/plugins,PLUGINS))
ifeq ($(strip $(ScoutingAnalysis/TreeMaker)),)
ALL_COMMONRULES += src_ScoutingAnalysis_TreeMaker_src
src_ScoutingAnalysis_TreeMaker_src_parent := ScoutingAnalysis/TreeMaker
src_ScoutingAnalysis_TreeMaker_src_INIT_FUNC := $$(eval $$(call CommonProductRules,src_ScoutingAnalysis_TreeMaker_src,src/ScoutingAnalysis/TreeMaker/src,LIBRARY))
ScoutingAnalysisTreeMaker := self/ScoutingAnalysis/TreeMaker
ScoutingAnalysis/TreeMaker := ScoutingAnalysisTreeMaker
ScoutingAnalysisTreeMaker_files := $(patsubst src/ScoutingAnalysis/TreeMaker/src/%,%,$(wildcard $(foreach dir,src/ScoutingAnalysis/TreeMaker/src ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
ScoutingAnalysisTreeMaker_LOC_USE := self  
ScoutingAnalysisTreeMaker_PACKAGE := self/src/ScoutingAnalysis/TreeMaker/src
ALL_PRODS += ScoutingAnalysisTreeMaker
ScoutingAnalysisTreeMaker_CLASS := LIBRARY
ScoutingAnalysis/TreeMaker_forbigobj+=ScoutingAnalysisTreeMaker
ScoutingAnalysisTreeMaker_INIT_FUNC        += $$(eval $$(call Library,ScoutingAnalysisTreeMaker,src/ScoutingAnalysis/TreeMaker/src,src_ScoutingAnalysis_TreeMaker_src,$(SCRAMSTORENAME_BIN),,$(SCRAMSTORENAME_LIB),$(SCRAMSTORENAME_LOGS)))
endif
