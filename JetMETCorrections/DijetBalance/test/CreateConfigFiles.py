#! /usr/bin/env python
import os

TYPE_LIST   = ['Data','QCDDiJet']
ALGO_LIST   = ['ak5calo','ak5calol2l3','ak5pf','ak5pfl2l3','ak5jpt','ak5jptl2l3']
PTBINS_LIST = ['1,18,31,40,70,120,150,300','1,37,60,75,120,150,300','1,37,60,75,120,150,300','1,40,63,80,120,150,300','1,43,68,85,120,150,300','1,43,68,85,120,150,300','1,38,60,75,120,150,300','1,39,62,75,120,150,300','1,39,62,75,120,150,300']
NPT_LIST    = ['7','6','6','6','6','6','6','6','6']
HLT_LIST    = ['0,0,1,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1','0,0,1,1,1,1']
MCHLT_LIST  = ['0,1,2,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2','0,1,2,2,2,2']
ETABINS     = '0.0,0.3,0.6,0.9,1.1,1.3,1.5,1.8,2.1,2.4,2.7,3.0,3.3,3.6,3.9,4.2,4.5,5.0'
PATH        = '/uscms_data/d2/kkousour/7TeV/DijetBalance/'
SCRIPT_PATH = '/uscms/home/kkousour/work/dataAnalysis/7TeV/CMSSW_3_6_1/src/JetMETCorrections/DijetBalance/test/scripts/'
ROOT_PATH   = '/uscms/home/kkousour/work/dataAnalysis/7TeV/CMSSW_3_6_1/src/JetMETCorrections/DijetBalance/test/rootfiles/'
TXT_PATH    = '/uscms/home/kkousour/work/dataAnalysis/7TeV/CMSSW_3_6_1/src/JetMETCorrections/DijetBalance/test/txtfiles/'
SYMMETRY    = 'true'
PREFIX      = ''
NETA        = '34'
#######################################################################################################
if (SYMMETRY == 'true'):
  PREFIX = 'sym'
  NETA   = '17';

for tt in TYPE_LIST:
  counter = 0
  if (tt == 'QCDDiJet'):
    PATH    = '/uscms_data/d2/kkousour/7TeV/Spring10/'
  for aa in ALGO_LIST:
    print 'HistoSettings_'+tt+'_'+aa+'.conf'
    file = open(SCRIPT_PATH+'HistoSettings_'+tt+'_'+aa+'.conf','w')
    file.write('TreeFilename      = '+PATH+'DiJetTree_'+tt+'.root \n')
    file.write('HistoFilename     = '+ROOT_PATH+PREFIX+'DiJetHisto_'+tt+'_'+aa+'.root \n')
    file.write('JetAlgos          = '+aa+ '\n')
    file.write('DPHI_CUT          = 2.7 \n')
    file.write('RATIO_CUT         = 0.2 \n')
    file.write('PTJET3_CUT        = 3 \n')
    file.write('ETA_SYMMETRY      = '+SYMMETRY+' \n')
    if (tt == 'QCDDiJet'):
      file.write('HLTbit            = HLT_L1Jet6U,HLT_DiJetAve15U_8E29,HLT_DiJetAve30U_8E29 \n')
      file.write('IsMCarlo          = true \n')
      file.write('ApplyWeights      = true \n')
      file.write('PtHatBoundaries   = 0,15,20,30,50,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,2600,3000,3500 \n')
      file.write('PtHatXsec         = 4.844e+10,5.794e+8,2.361e+8,5.311e+7,6.358e+6,7.849e+5,1.151e+5,2.014e+4,4.094e+3,9.346e+2,2.338e+2,7.021e+1,1.557e+1,1.843,3.318e-1,1.086e-2,3.499e-4,7.549e-6,6.465e-8,6.295e-11 \n')
      file.write('PtHatEvents       = 2197029,2256430,1034680,1161768,111289,606771,58888,51680,52894,64265,52207,20380,22448,26000,23956,20575,33970,22580,20644,23460\n')
    else:
      file.write('HLTbit            = HLT_DiJetAve15U_8E29,HLT_DiJetAve30U_8E29 \n')
      file.write('IsMCarlo          = false \n')
      file.write('ApplyWeights      = false \n')
    file.write('DijetPtBoundaries = '+PTBINS_LIST[counter]+'\n')
    file.write('EtaBoundaries     = '+ETABINS+'\n')
    file.close()

    print 'CombineSettings_'+tt+'_'+aa+'.conf'
    file = open(SCRIPT_PATH+'CombineSettings_'+tt+'_'+aa+'.conf','w')
    file.write('InputFileName  = '+ROOT_PATH+PREFIX+'DiJetHisto_'+tt+'_'+aa+'.root \n')
    file.write('OutputFileName = '+ROOT_PATH+PREFIX+'DiJetCombined_'+tt+'_'+aa+'.root \n')
    file.write('JetAlgos       = '+aa+ '\n')
    if (tt == 'QCDDiJet'):
      file.write('HLTIndex       = '+MCHLT_LIST[counter]+' \n')
    else:
      file.write('HLTIndex       = '+HLT_LIST[counter]+' \n')
    file.write('NPT            = '+NPT_LIST[counter]+' \n')
    file.write('NETA           = '+NETA+' \n')
    file.close()

    print 'FitterSettings_'+tt+'_'+aa+'.conf'
    file = open(SCRIPT_PATH+'FitterSettings_'+tt+'_'+aa+'.conf','w')
    file.write('HistoFilename     = '+ROOT_PATH+PREFIX+'DiJetCombined_'+tt+'_'+aa+'.root \n')
    file.write('FitterFilename    = '+ROOT_PATH+PREFIX+'DiJetFitter_'+tt+'_'+aa+'.root \n')
    file.write('ETA_SYMMETRY      = '+SYMMETRY+' \n')
    file.write('JetAlgos          = '+aa+ '\n')
    file.write('DijetPtBoundaries = '+PTBINS_LIST[counter]+'\n')
    file.write('EtaBoundaries     = '+ETABINS+'\n')
    file.close()  

    print 'L2Settings_'+tt+'_'+aa+'.conf'
    file = open(SCRIPT_PATH+'L2Settings_'+tt+'_'+aa+'.conf','w') 
    file.write('FitterFilename    = '+ROOT_PATH+PREFIX+'DiJetFitter_'+tt+'_'+aa+'.root \n')
    file.write('L2ROOTFilename    = '+ROOT_PATH+PREFIX+'L2Graphs_'+tt+'_'+aa+'.root \n')
    file.write('L2TXTFilename     = '+TXT_PATH+PREFIX+'L2_'+tt+'_'+aa+'.txt \n')
    file.write('JetAlgos          = '+aa+ '\n')
    file.write('ETA_SYMMETRY      = '+SYMMETRY+' \n')
    file.write('EtaBoundaries     = '+ETABINS+'\n')
    file.write('NPT               = '+NPT_LIST[counter]+' \n')
    if (tt == 'Data' and (aa == 'ak5calol2l3' or aa == 'ak5pfl2l3' or aa == 'ak5jptl2l3')):
      file.write('BiasROOTFilename = '+ROOT_PATH+PREFIX+'DiJetFitter_QCDDiJet_'+aa+'.root \n')
    file.close()


    counter+=1

