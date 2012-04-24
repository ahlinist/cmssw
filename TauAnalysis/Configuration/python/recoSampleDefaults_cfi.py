import FWCore.ParameterSet.Config as cms

SAMPLE_DEFAULTS = {
    'dbs_url'                                    : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    'conditions'                                 : 'START52_V9::All',
    'hlt_paths'                                  : [],
    'lumi_mask'                                  : '',
    'runselection'                               : '',
    'SE_white_list'                              : '',
    'SE_black_list'                              : '',
    'disableDuplicateCheck'                      : False,
    'skim_eff'                                   : 1.,
    'genPhaseSpaceCut'                           : '',
    'genFinalStateFilter'                        : False, 
    'applyElectronTriggerEfficiencyCorrection'   : False,
    'applyElectronTriggerEfficiencyCorrection'   : False,
    'applyElectronIsolationEfficiencyCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection'       : False,
    'applyTauMetTriggerEfficiencyCorrection'     : False,
    'applyVertexMultiplicityReweighting'         : 'Summer12',
    'applyRhoNeutralReweighting'                 : False,
    'enableSysUncertainties'                     : False,
    'applyZrecoilCorrection'                     : False,
    'saveNtuple'                                 : False,
    'noRunLumiEventSave'                         : False
}
