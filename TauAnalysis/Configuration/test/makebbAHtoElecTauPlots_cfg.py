import FWCore.ParameterSet.Config as cms

process = cms.Process('makebbAHtoElecTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

from TauAnalysis.Configuration.tools.tauAnalysisMaker import *
from TauAnalysis.Configuration.cutsbbAHtoElecTau_cff import cuts,options
from TauAnalysis.Configuration.plotbbAHtoElecTau_processes_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

plotDirectoryName = cms.string("/localscratch/")

maker = TauAnalysisMaker(cuts,options,process)

add_processes = {
#  'qcdSum':[
#    'QCD_BCtoE_Pt20to30',
#    'QCD_BCtoE_Pt30to80',
#    'QCD_BCtoE_Pt80to170',
#    'QCD_EMenriched_Pt20to30',
#    'QCD_EMenriched_Pt30to80',
#    'QCD_EMenriched_Pt80to170'
#  ],
#  'gammaPlusJetsSum':[
#    'gammaPlusJets_Pt15to20',
#    'gammaPlusJets_Pt20to25',
#    'gammaPlusJets_Pt25to30',
#    'gammaPlusJets_Pt30to35',
#    'gammaPlusJets_PtGt35'
#  ]
}
direct_processes = [
  #'AH115_tautau',
  #'AH160_tautau',
  'AH115bb_tautau',
  'AH160bb_tautau',
  'Ztautau',
  'Zee',
  #'ZeePlusJets',
  #'ZtautauPlusJets',
  'ZplusJets',
  'WplusJets',
  'TTplusJets'
]
dump_processes = direct_processes+add_processes.keys()
load_processes = copy.deepcopy(direct_processes)
for a in add_processes:
  load_processes.extend(add_processes[a])


process.makebbAHtoElecTauPlots = cms.Sequence(
  maker.makeDQMFileLoad(load_processes,globals())
 +maker.makeDQMFileAdd(add_processes)
 +maker.makeDQMSave()
 +maker.makeDQMDump(dump_processes)
 +maker.makeDQMPlotter(direct_processes,add_processes,globals(),stack=False,normalize=True)
)
     
process.p = cms.Path(process.makebbAHtoElecTauPlots)

