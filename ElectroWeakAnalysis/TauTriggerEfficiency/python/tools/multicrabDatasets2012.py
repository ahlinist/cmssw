from HiggsAnalysis.HeavyChHiggsToTauNu.tools.multicrabWorkflowsTools import TaskDef
from HiggsAnalysis.HeavyChHiggsToTauNu.tools.multicrabWorkflowsTriggerEff import addMetLegSkim_44X

skimVersion = "V00_12_03_CMSSW537_v1"
def addMetLegSkim(datasets):
    definitions = {
	"Tau_190456-190738_2012A_Jul13":		TaskDef("/Tau/local-Run2012A_13Jul2012_v1_AOD_190456_190738_analysis_metleg_v53_v1-165ff7ea0b6b7dc6426443c42f2a9122/USER"),
	"Tau_190782-190949_2012A_Aug06":             	TaskDef("/Tau/local-Run2012A_recover_06Aug2012_v1_AOD_190782_190949_analysis_metleg_v53_v1-de59482928f61dc06955dcb204f66afe/USER"),
	"Tau_191043-193621_2012A_Jul13":             	TaskDef("/Tau/local-Run2012A_13Jul2012_v1_AOD_191043_193621_analysis_metleg_v53_v1-43665de791ab95ebca4e518ee6c20319/USER"),
	"Tau_193834-196531_2012B_Jul13":             	TaskDef(""),
	"Tau_198022-198523_2012C_Aug24":             	TaskDef("/Tau/local-Run2012C_24Aug2012_v1_AOD_198022_198523_analysis_metleg_v53_v1-16cf49e1333af46ec078168d0a11d2bc/USER"),
	"Tau_198941-200601_2012C_Prompt":             	TaskDef(""),
	"Tau_200961-202504_2012C_Prompt":             	TaskDef(""),
	"Tau_202792-203742_2012C_Prompt":             	TaskDef(""),
	"Tau_203777-208686_2012D_Prompt":             	TaskDef(""),

	"QCD_Pt30to50_TuneZ2star_Summer12":             TaskDef(""),
	"QCD_Pt50to80_TuneZ2star_Summer12":             TaskDef(""),
	"QCD_Pt80to120_TuneZ2star_Summer12":            TaskDef(""),
	"QCD_Pt120to170_TuneZ2star_Summer12":           TaskDef(""),
	"QCD_Pt170to300_TuneZ2star_Summer12":           TaskDef(""),
	"QCD_Pt170to300_TuneZ2star_v2_Summer12":        TaskDef(""),
	"QCD_Pt300to470_TuneZ2star_Summer12":           TaskDef(""),
	"QCD_Pt300to470_TuneZ2star_v2_Summer12":        TaskDef(""),
	"QCD_Pt300to470_TuneZ2star_v3_Summer12":        TaskDef(""),

	"WW_TuneZ2star_Summer12":             		TaskDef(""),
	"WZ_TuneZ2star_Summer12":             		TaskDef(""),
	"ZZ_TuneZ2star_Summer12":             		TaskDef(""),
	"TTJets_TuneZ2star_Summer12":           	TaskDef("/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"WJets_TuneZ2star_v1_Summer12":         	TaskDef(""),
        "WJets_TuneZ2star_v2_Summer12":         	TaskDef(""),
        "W1Jets_TuneZ2star_Summer12":           	TaskDef(""),
        "W2Jets_TuneZ2star_Summer12":           	TaskDef(""),
        "W3Jets_TuneZ2star_Summer12":           	TaskDef(""),
        "W4Jets_TuneZ2star_Summer12":           	TaskDef(""),
        "DYJetsToLL_M50_TuneZ2star_Summer12":           TaskDef(""),
        "DYJetsToLL_M10to50_TuneZ2star_Summer12":	TaskDef(""),
        "T_t-channel_TuneZ2star_Summer12":             	TaskDef(""),
        "Tbar_t-channel_TuneZ2star_Summer12":           TaskDef(""),
        "T_tW-channel_TuneZ2star_Summer12":             TaskDef(""),
        "Tbar_tW-channel_TuneZ2star_Summer12":          TaskDef(""),
        "T_s-channel_TuneZ2star_Summer12":             	TaskDef(""),
        "Tbar_s-channel_TuneZ2star_Summer12":           TaskDef(""),
    }

    addMetLegSkim_53X(skimVersion, datasets, definitions)
