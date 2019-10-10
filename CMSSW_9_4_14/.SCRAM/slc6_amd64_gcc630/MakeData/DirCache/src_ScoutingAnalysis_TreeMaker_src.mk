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
