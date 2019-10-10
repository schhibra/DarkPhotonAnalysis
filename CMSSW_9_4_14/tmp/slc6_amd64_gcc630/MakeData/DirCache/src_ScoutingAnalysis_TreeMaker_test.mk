ALL_COMMONRULES += src_ScoutingAnalysis_TreeMaker_test
src_ScoutingAnalysis_TreeMaker_test_parent := ScoutingAnalysis/TreeMaker
src_ScoutingAnalysis_TreeMaker_test_INIT_FUNC += $$(eval $$(call CommonProductRules,src_ScoutingAnalysis_TreeMaker_test,src/ScoutingAnalysis/TreeMaker/test,TEST))
