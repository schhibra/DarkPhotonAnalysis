ALL_SUBSYSTEMS+=ScoutingAnalysis
subdirs_src_ScoutingAnalysis = src_ScoutingAnalysis_TreeMaker
ALL_PACKAGES += ScoutingAnalysis/TreeMaker
subdirs_src_ScoutingAnalysis_TreeMaker := src_ScoutingAnalysis_TreeMaker_macros src_ScoutingAnalysis_TreeMaker_python src_ScoutingAnalysis_TreeMaker_plugins src_ScoutingAnalysis_TreeMaker_test src_ScoutingAnalysis_TreeMaker_src
ifeq ($(strip $(PyScoutingAnalysisTreeMaker)),)
PyScoutingAnalysisTreeMaker := self/src/ScoutingAnalysis/TreeMaker/python
src_ScoutingAnalysis_TreeMaker_python_parent := 
ALL_PYTHON_DIRS += $(patsubst src/%,%,src/ScoutingAnalysis/TreeMaker/python)
PyScoutingAnalysisTreeMaker_files := $(patsubst src/ScoutingAnalysis/TreeMaker/python/%,%,$(wildcard $(foreach dir,src/ScoutingAnalysis/TreeMaker/python ,$(foreach ext,$(SRC_FILES_SUFFIXES),$(dir)/*.$(ext)))))
PyScoutingAnalysisTreeMaker_LOC_USE := self  
PyScoutingAnalysisTreeMaker_PACKAGE := self/src/ScoutingAnalysis/TreeMaker/python
ALL_PRODS += PyScoutingAnalysisTreeMaker
PyScoutingAnalysisTreeMaker_INIT_FUNC        += $$(eval $$(call PythonProduct,PyScoutingAnalysisTreeMaker,src/ScoutingAnalysis/TreeMaker/python,src_ScoutingAnalysis_TreeMaker_python,1,1,$(SCRAMSTORENAME_PYTHON),$(SCRAMSTORENAME_LIB),,))
else
$(eval $(call MultipleWarningMsg,PyScoutingAnalysisTreeMaker,src/ScoutingAnalysis/TreeMaker/python))
endif
ALL_COMMONRULES += src_ScoutingAnalysis_TreeMaker_python
src_ScoutingAnalysis_TreeMaker_python_INIT_FUNC += $$(eval $$(call CommonProductRules,src_ScoutingAnalysis_TreeMaker_python,src/ScoutingAnalysis/TreeMaker/python,PYTHON))
ALL_COMMONRULES += src_ScoutingAnalysis_TreeMaker_test
src_ScoutingAnalysis_TreeMaker_test_parent := ScoutingAnalysis/TreeMaker
src_ScoutingAnalysis_TreeMaker_test_INIT_FUNC += $$(eval $$(call CommonProductRules,src_ScoutingAnalysis_TreeMaker_test,src/ScoutingAnalysis/TreeMaker/test,TEST))
