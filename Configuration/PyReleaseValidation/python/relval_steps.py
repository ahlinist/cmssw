
    
class InputInfo(object):
    def __init__(self,dataSet,label='',run=0,files=1000,events=2000000,location='CAF') :
        self.run = run
        self.files = files
        self.events = events
        self.location = location
        self.label = label
        self.dataSet = dataSet

# merge dictionaries, with prioty on the [0] index
def merge(dictlist,TELL=False):
    import copy
    last=len(dictlist)-1
    if TELL: print last,dictlist
    if last==0:
        # ONLY ONE ITEM LEFT
        return copy.copy(dictlist[0])
    else:
        reducedlist=dictlist[0:max(0,last-1)]
        if TELL: print reducedlist
        # make a copy of the last item
        d=copy.copy(dictlist[last])
        # update with the last but one item
        d.update(dictlist[last-1])
        # and recursively do the rest
        reducedlist.append(d)
        return merge(reducedlist,TELL)


# step1 gensim
step1Defaults = {'--relval'      : None, # need to be explicitly set
                 '-s'            : 'GEN,SIM',
                 '-n'            : 10,
                 '--conditions'  : 'auto:mc',
                 '--datatier'    : 'GEN-SIM',
                 '--eventcontent': 'RAWSIM',
                 }

step1 = {}

#### Production test section ####
step1['ProdMinBias']=merge([{'cfg':'MinBias_7TeV_cfi','--relval':'9000,100'},step1Defaults])
step1['ProdTTbar']=merge([{'cfg':'TTbar_Tauola_7TeV_cfi','--relval':'9000,50'},step1Defaults])
step1['ProdQCD_Pt_3000_3500']=merge([{'cfg':'QCD_Pt_3000_3500_7TeV_cfi','--relval':'9000,25'},step1Defaults])

#step1['ProdMinBiasINPUT']={'INPUT':InputInfo(dataSet='/RelValProdMinBias/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='prodmbrv',location='STD')}
#step1['ProdTTbarINPUT']={'INPUT':InputInfo(dataSet='/RelValProdTTbar/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='prodttbrv',location='STD')}
#step1['ProdQCD_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValProdQCD_Pt_3000_3500/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='qcd335',location='STD')}



#### data ####
step1['RunCosmicsA']={'INPUT':InputInfo(dataSet='/Cosmics/Run2010A-v1/RAW',label='cos2010A',run=142089,events=100000)}
step1['MinimumBias2010A']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010A-valskim-v6/RAW-RECO',label='run2010A',location='STD')}
step1['MinimumBias2010B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010B-valskim-v2/RAW-RECO',label='run2010B')}
step1['WZMuSkim2010A']={'INPUT':InputInfo(dataSet='/Mu/Run2010A-WZMu-Nov4Skim_v1/RAW-RECO',label='wzMu2010A')}
step1['WZMuSkim2010B']={'INPUT':InputInfo(dataSet='/Mu/Run2010B-WZMu-Nov4Skim_v1/RAW-RECO',label='wzMu2010B')}
step1['WZEGSkim2010A']={'INPUT':InputInfo(dataSet='/EG/Run2010A-WZEG-Nov4Skim_v1/RAW-RECO',label='wzEG2010A')}
step1['WZEGSkim2010B']={'INPUT':InputInfo(dataSet='/Electron/Run2010B-WZEG-Nov4Skim_v1/RAW-RECO',label='wzEG2010B')}

step1['RunMinBias2010B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010B-v1/RAW',label='mb2010B',run=149011,events=100000)}
step1['RunMu2010B']={'INPUT':InputInfo(dataSet='/Mu/Run2010B-v1/RAW',label='mu2010B',run=149011,events=100000)}
step1['RunElectron2010B']={'INPUT':InputInfo(dataSet='/Electron/Run2010B-v1/RAW',label='electron2010B',run=149011,events=100000)}
step1['RunPhoton2010B']={'INPUT':InputInfo(dataSet='/Photon/Run2010B-v1/RAW',label='photon2010B',run=149011,events=100000)}
step1['RunJet2010B']={'INPUT':InputInfo(dataSet='/Jet/Run2010B-v1/RAW',label='jet2010B',run=149011,events=100000)}

#### Standard release validation samples ####

stCond={'--conditions':'auto:startup'}
K9by25={'--relval':'9000,25'}
K9by50={'--relval':'9000,50'}
K9by100={'--relval':'9000,100'}
K9by250={'--relval':'9000,250'}
K25by250={'--relval':'25000,250'}

def gen(fragment,howMuch):
    global step1Defaults
    return merge([{'cfg':fragment},howMuch,step1Defaults])

step1['MinBias']=gen('MinBias_7TeV_cfi',K9by100)
step1['QCD_Pt_3000_3500']=gen('QCD_Pt_3000_3500_7TeV_cfi',K9by25)
step1['QCD_Pt_80_120']=gen('QCD_Pt_80_120_7TeV_cfi',K9by50)
step1['SingleElectronPt10']=gen('SingleElectronPt10_cfi',K9by250)
step1['SingleElectronPt35']=gen('SingleElectronPt35_cfi',K9by250)
step1['SingleGammaPt10']=gen('SingleGammaPt10_cfi',K9by100)
step1['SingleGammaPt35']=gen('SingleGammaPt35_cfi',K9by100)
step1['SingleMuPt10']=gen('SingleMuPt10_cfi',K25by250)
step1['SingleMuPt100']=gen('SingleMuPt100_cfi',K9by250)
step1['SingleMuPt1000']=gen('SingleMuPt1000_cfi',K9by100)
step1['TTbar']=gen('TTbar_Tauola_7TeV_cfi',K9by50)
step1['ZEE']=gen('ZEE_7TeV_cfi',K9by100)
step1['Wjet_Pt_80_120']=gen('Wjet_Pt_80_120_7TeV_cfi',K9by100)
step1['Wjet_Pt_3000_3500']=gen('Wjet_Pt_3000_3500_7TeV_cfi',K9by100)
step1['LM1_sfts']=gen('LM1_sfts_7TeV_cfi',K9by100)
step1['QCD_FlatPt_15_3000']=gen('QCDForPF_7TeV_cfi',K9by100)

step1['MinBiasINPUT']={'INPUT':InputInfo(dataSet='/RelValMinBias/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',location='STD')}
step1['QCD_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_3000_3500/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='',location='STD')}
step1['QCD_Pt_80_120INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_80_120/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='QCD_Pt_80_120rv',location='STD')}
step1['SingleElectronPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleElectronPt10/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleElectronPt10rv',location='STD')}
step1['SingleElectronPt35INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleElectronPt35/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleElectronPt35rv',location='STD')}
step1['SingleGammaPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleGammaPt10/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleGammaPt10rv',location='STD')}
step1['SingleGammaPt35INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleGammaPt35/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleGammaPt35rv',location='STD')}
step1['SingleMuPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt10/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleMuPt10rv',location='STD')}
step1['SingleMuPt100INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt100/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleMuPt100rv',location='STD')}
step1['SingleMuPt1000INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt1000/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='SingleMuPt1000rv',location='STD')}
step1['TTbarINPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='ttbarrv',location='STD')}
step1['ZEEINPUT']={'INPUT':InputInfo(dataSet='/RelValZEE/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='ZEErv',location='STD')}
step1['Wjet_Pt_80_120INPUT']={'INPUT':InputInfo(dataSet='/RelValWjet_Pt_80_120/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='Wjet_Pt_80_120rv',location='STD')}
step1['Wjet_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValWjet_Pt_3000_3500/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='Wjet_Pt_3000_3500rv',location='STD')}
step1['LM1_sftsINPUT']={'INPUT':InputInfo(dataSet='/RelValLM1_sfts/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='LM1_sftsrv',location='STD')}
step1['QCD_FlatPt_15_3000INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_FlatPt_15_3000/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='QCD_FlatPt_15_3000rv',location='STD')}

## high stat step1
K700by280={'--relval': '70000,280'}
K250by100={'--relval': '25000,100'}
K3250000by1300000={'--relval': '325000000,1300000'}
K250by250={'--relval': '25000,250'}
K110000by45000={'--relval': '11000000,45000'}
K562by225={'--relval': '56250,225'}

ecalHcal={
    '-s':'GEN,SIM,DIGI,DIGI2RAW,RAW2DIGI,L1Reco,RECO',
    '--datatier':'GEN-SIM-DIGI-RAW-RECO',
    '--geometry':'ECALHCAL',
    '--eventcontent':'FEVTDEBUG',
    '--customise':'Validation/Configuration/ECALHCAL.customise',
    '--beamspot':'NoSmear'}
step1['SingleElectronE120EHCAL']=merge([{'cfg':'SingleElectronE120EHCAL_cfi'},ecalHcal,K25by250,step1Defaults])
step1['SinglePiE50HCAL']=merge([{'cfg':'SinglePiE50HCAL_cfi'},ecalHcal,K25by250,step1Defaults])

step1['MinBiasHS']=gen('MinBias_7TeV_cfi',K25by250)
step1['InclusiveppMuX']=gen('InclusiveppMuX_7TeV_cfi',K110000by45000)
step1['SingleElectronFlatPt5To100']=gen('SingleElectronFlatPt5To100_cfi',K250by250)
step1['SinglePiPt1']=gen('SinglePiPt1_cfi',K250by250)
step1['SingleMuPt1']=gen('SingleMuPt1_cfi',K250by250)
step1['ZPrime5000Dijet']=gen('ZPrime5000JJ_7TeV_cfi',K250by100)
step1['SinglePi0E10']=gen('SinglePi0E10_cfi',K250by100)
step1['SinglePiPt10']=gen('SinglePiPt10_cfi',K250by250)
step1['SingleGammaFlatPt10To100']=gen('SingleGammaFlatPt10To100_cfi',K250by250)
step1['SingleTauPt50Pythia']=gen('SingleTaupt_50_cfi',K250by100)
step1['SinglePiPt100']=gen('SinglePiPt100_cfi',K250by250)


def genS(fragment,howMuch):
    global step1Defaults,stCond
    return merge([{'cfg':fragment},stCond,howMuch,step1Defaults])

step1['MinBias2']=genS('MinBias_7TeV_cfi',K9by100)
step1['Higgs200ChargedTaus']=genS('H200ChargedTaus_Tauola_7TeV_cfi',K9by100)
step1['QCD_Pt_3000_3500_2']=genS('QCD_Pt_3000_3500_7TeV_cfi',K9by25)
step1['QCD_Pt_80_120_2']=genS('QCD_Pt_80_120_7TeV_cfi',K9by50)
step1['JpsiMM']=genS('JpsiMM_7TeV_cfi',{'--relval':'65250,725'})
step1['TTbar2']=genS('TTbar_Tauola_7TeV_cfi',K9by50)
step1['WE']=genS('WE_7TeV_cfi',K9by100)
step1['WM']=genS('WM_7TeV_cfi',K9by100)
step1['ZEE2']=genS('ZEE_7TeV_cfi',K9by100)
step1['ZMM']=genS('ZMM_7TeV_cfi',{'--relval':'18000,200'})
step1['ZTT']=genS('ZTT_Tauola_All_hadronic_7TeV_cfi',K9by100)
step1['H130GGgluonfusion']=genS('H130GGgluonfusion_7TeV_cfi',K9by100)
step1['PhotonJets_Pt_10']=genS('PhotonJet_Pt_10_7TeV_cfi',K9by100)
step1['QQH1352T_Tauola']=genS('QQH1352T_Tauola_7TeV_cfi',K9by100)

step1['MinBias2INPUT']={'INPUT':InputInfo(dataSet='/RelValMinBias/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='MinBiasrv',location='STD')}
step1['Higgs200ChargedTausINPUT']={'INPUT':InputInfo(dataSet='/RelValHiggs200ChargedTaus/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='Higgs200ChargedTausrv',location='STD')}
step1['QCD_Pt_3000_3500_2INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_3000_3500_2/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='QCD_Pt_3000_3500rv',location='STD')}
step1['QCD_Pt_80_120_2INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_80_120/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='QCD_Pt_80_120rv',location='STD')}
step1['JpsiMMINPUT']={'INPUT':InputInfo(dataSet='/RelValJpsiMM/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='JpsiMMrv',location='STD')}
step1['TTbar2INPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='ttbarrv',location='STD')}
step1['WEINPUT']={'INPUT':InputInfo(dataSet='/RelValWE/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='WErv',location='STD')}
step1['WMINPUT']={'INPUT':InputInfo(dataSet='/RelValWM/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='WMrv',location='STD')}
step1['ZEE2INPUT']={'INPUT':InputInfo(dataSet='/RelValZEE/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='ZEErv',location='STD')}
step1['ZMMINPUT']={'INPUT':InputInfo(dataSet='/RelValZMM/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='ZMMrv',location='STD')}
step1['ZTTINPUT']={'INPUT':InputInfo(dataSet='/RelValZTT/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='ZTTrv',location='STD')}
step1['H130GGgluonfusionINPUT']={'INPUT':InputInfo(dataSet='/RelValH130GGgluonfusion/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='H130GGgluonfusionrv',location='STD')}
step1['PhotonJets_Pt_10INPUT']={'INPUT':InputInfo(dataSet='/RelValPhotonJets_Pt_10/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='PhotonJets_Pt_10rv',location='STD')}
step1['QQH1352T_TauolaINPUT']={'INPUT':InputInfo(dataSet='/RelValQQH1352T_Tauola/CMSSW_4_3_0_pre2-START42_V9-v1/GEN-SIM',label='QQH1352T_Tauolarv',location='STD')}

step1['Cosmics']=merge([{'cfg':'UndergroundCosmicMu_cfi.py','--relval':'666000,7400','--scenario':'cosmics'},step1Defaults])
step1['BeamHalo']=merge([{'cfg':'BeamHalo_cfi.py','--scenario':'cosmics'},K9by100,step1Defaults])

step1['CosmicsINPUT']={'INPUT':InputInfo(dataSet='/RelValCosmics/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='Cosmicrv',location='STD')}
step1['BeamHaloINPUT']={'INPUT':InputInfo(dataSet='/RelValBeamHalo/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='BeamHalorv',location='STD')}

step1['QCD_Pt_50_80']=genS('QCD_Pt_50_80_7TeV_cfi',K250by100)
step1['QCD_Pt_15_20']=genS('QCD_Pt_15_20_7TeV_cfi',K250by100)
step1['ZTTHS']=merge([K250by100,step1['ZTT']])
step1['QQH120Inv']=genS('QQH120Inv_7TeV_cfi',K250by100)
step1['TTbar2HS']=merge([K250by100,step1['TTbar2']])
step1['JpsiMM_Pt_20_inf']=genS('JpsiMM_Pt_20_inf_7TeV_cfi',K700by280)
step1['QCD_Pt_120_170']=genS('QCD_Pt_120_170_7TeV_cfi',K250by100)
step1['H165WW2L']=genS('H165WW2L_Tauola_7TeV_cfi',K250by100)
step1['UpsMM']=genS('UpsMM_7TeV_cfi',K562by225)
step1['RSGrav']=genS('RS750_quarks_and_leptons_7TeV_cff',K250by100)
step1['QCD_Pt_80_120_2HS']=merge([K250by100,step1['QCD_Pt_80_120_2']])
step1['bJpsiX']=genS('bJpsiX_7TeV_cfi',K3250000by1300000)
step1['QCD_Pt_30_50']=genS('QCD_Pt_30_50_7TeV_cfi',K250by100)
step1['H200ZZ4L']=genS('H200ZZ4L_Tauola_7TeV_cfi',K250by100)
step1['LM9p']=genS('LM9p_7TeV_cff',K250by100)
step1['QCD_Pt_20_30']=genS('QCD_Pt_20_30_7TeV_cfi',K250by100)
step1['QCD_Pt_170_230']=genS('QCD_Pt_170_230_7TeV_cfi',K250by100)




def changeRefRelease(step1s,listOfPairs):
    for s in step1s:
        if ('INPUT' in step1s[s]):
            oldD=step1[s]['INPUT'].dataSet
            for ref,newRef in listOfPairs:
                if  ref in oldD:
                    step1[s]['INPUT'].dataSet=oldD.replace(ref,newRef)
                                        
def addForAll(steps,d):
    for s in steps:
        steps[s].update(d)


#changeRefRelease(step1,[('CMSSW_4_3_0_pre2-START42_V9-v1','CMSSW_4_3_0_pre2-START42_V9-v1'),
#                        ('CMSSW_4_3_0_pre2-MC_42_V9-v1','CMSSW_4_3_0_pre2-MC_42_V9-v1')
#                        ])
                        

#### fastsim section ####
##no forseen to do things in two steps GEN-SIM then FASTIM->end: maybe later
step1FastDefaults =merge([{'-s':'GEN,FASTSIM,HLT:GRun,VALIDATION',
                           '--eventcontent':'FEVTDEBUGHLT',
                           '--datatier':'GEN-SIM-DIGI-RECO',
                           '--relval':'27000,1000'},
                          step1Defaults])
K100byK1={'--relval':'100000,1000'}
step1['TTbarFS1']=merge([{'cfg':'TTbar_Tauola_7TeV_cfi'},K100byK1,step1FastDefaults])
step1['TTbarFS2']=merge([{'cfg':'TTbar_Tauola_7TeV_cfi'},K100byK1,stCond,step1FastDefaults])
step1['SingleMuPt1FS']=merge([{'cfg':'SingleMuPt1_cfi'},step1FastDefaults])
step1['SingleMuPt10FS']=merge([{'cfg':'SingleMuPt10_cfi'},step1FastDefaults])
step1['SingleMuPt100FS']=merge([{'cfg':'SingleMuPt100_cfi'},step1FastDefaults])
step1['SinglePiPt1FS']=merge([{'cfg':'SinglePiPt1_cfi'},step1FastDefaults])
step1['SinglePiPt10FS']=merge([{'cfg':'SinglePiPt10_cfi'},step1FastDefaults])
step1['SinglePiPt100FS']=merge([{'cfg':'SinglePiPt100_cfi'},step1FastDefaults])
step1['ZEEFS1']=merge([{'cfg':'ZEE_7TeV_cfi'},K100byK1,step1FastDefaults])
step1['ZEEFS2']=merge([{'cfg':'ZEE_7TeV_cfi'},K100byK1,stCond,step1FastDefaults])
step1['ZTTFS1']=merge([{'cfg':'ZTT_Tauola_OneLepton_OtherHadrons_7TeV_cfi'},K100byK1,step1FastDefaults])
step1['ZTTFS2']=merge([{'cfg':'ZTT_Tauola_OneLepton_OtherHadrons_7TeV_cfi'},K100byK1,stCond,step1FastDefaults])
step1['QCDFlatPt153000FS']=merge([{'cfg':'QCDForPF_7TeV_cfi'},step1FastDefaults])
step1['QCD_Pt_80_120FS']=merge([{'cfg':'QCD_Pt_80_120_7TeV_cfi'},K100byK1,stCond,step1FastDefaults])
step1['QCD_Pt_3000_3500FS']=merge([{'cfg':'QCD_Pt_3000_3500_7TeV_cfi'},K100byK1,stCond,step1FastDefaults])
step1['H130GGgluonfusionFS']=merge([{'cfg':'H130GGgluonfusion_7TeV_cfi'},step1FastDefaults])
step1['SingleGammaFlatPt10To10FS']=merge([{'cfg':'SingleGammaFlatPt10To100_cfi'},K100byK1,step1FastDefaults])

#### generator test section ####
step1GenDefaults=merge([{'-s':'GEN,VALIDATION:genvalid',
                         '--relval':'1000000,20000',
                         '--eventcontent':'RAWSIM',
                         '--datatier':'GEN'},
                        step1Defaults])
def genvalid(fragment,d,suffix='',fi=''):
    import copy
    c=copy.copy(d)
    if suffix:
        c['-s']=c['-s'].replace('genvalid','genvalid_'+suffix)
    if fi:
        c['--filein']='mcdb:%d'%(fi,)
    c['cfg']=fragment
    return c
    
step1['QCD_Pt-30_7TeV_herwigpp']=genvalid('QCD_Pt_30_7TeV_herwigpp_cff',step1GenDefaults,'qcd')
step1['DYToLL_M-50_TuneZ2_7TeV_pythia6-tauola']=genvalid('DYToLL_M_50_TuneZ2_7TeV_pythia6_tauola_cff',step1GenDefaults,'dy')
step1['QCD_Pt-30_TuneZ2_7TeV_pythia6']=genvalid('QCD_Pt_30_TuneZ2_7TeV_pythia6_cff',step1GenDefaults,'qcd')
step1['QCD_Pt-30_7TeV_pythia8']=genvalid('QCD_Pt_30_7TeV_pythia8_cff',step1GenDefaults,'qcd')
step1['GluGluTo2Jets_M-100_7TeV_exhume']=genvalid('GluGluTo2Jets_M_100_7TeV_exhume_cff',step1GenDefaults,'qcd')
step1['TT_TuneZ2_7TeV_pythia6-evtgen']=genvalid('TT_TuneZ2_7TeV_pythia6_evtgen_cff',step1GenDefaults)
step1['MinBias_TuneZ2_7TeV_pythia6']=genvalid('MinBias_TuneZ2_7TeV_pythia6_cff',step1GenDefaults,'qcd')
step1['WToLNu_TuneZ2_7TeV_pythia6-tauola']=genvalid('WToLNu_TuneZ2_7TeV_pythia6_tauola_cff',step1GenDefaults,'w')
step1['QCD_Pt-30_7TeV_herwig6']=genvalid('QCD_Pt_30_7TeV_herwig6_cff',step1GenDefaults,'qcd')
step1['MinBias_7TeV_pythia8']=genvalid('MinBias_7TeV_pythia8_cff',step1GenDefaults,'qcd')

step1['DYToMuMu_M-20_7TeV_mcatnlo']=genvalid('DYToMuMu_M_20_7TeV_mcatnlo_cff',step1GenDefaults,'dy',664)
step1['TT_7TeV_mcatnlo']=genvalid('TT_7TeV_mcatnlo_cff',step1GenDefaults,'',346)
step1['WminusToENu_7TeV_mcatnlo']=genvalid('WminusToENu_7TeV_mcatnlo_cff',step1GenDefaults,'w',666)
step1['WminusToMuNu_7TeV_mcatnlo']=genvalid('WminusToMuNu_7TeV_mcatnlo_cff',step1GenDefaults,'w',668)
step1['WplusToENu_7TeV_mcatnlo']=genvalid('WplusToENu_7TeV_mcatnlo_cff',step1GenDefaults,'w',665)
step1['WplusToMuNu_7TeV_mcatnlo']=genvalid('WplusToMuNu_7TeV_mcatnlo_cff',step1GenDefaults,'w',667)
step1['QCD_Ht-100To250_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',546)
step1['QCD_Ht-250To500_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',592)
step1['QCD_Ht-500To1000_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',594)
step1['TTJets_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'',846)
step1['WJetsLNu_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'w',882)
step1['ZJetsLNu_TuneD6T_7TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneD6T_7TeV_madgraph_tauola_cff',step1GenDefaults,'dy',851)
step1['QCD2Jets_Pt-40To120_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'qcd',785)
step1['QCD3Jets_Pt-40To120_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'qcd',786)
step1['QCD4Jets_Pt-40To120_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'qcd',787)
step1['QCD5Jets_Pt-40To120_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'qcd',832)
step1['TT0Jets_Et-40_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et48ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'',472)
step1['TT1Jets_Et-40_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et48ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'',475)
step1['TT2Jets_Et-40_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et48ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'',478)
step1['TT3Jets_Et-40_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et48ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'',481)
step1['W0Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'w',397)
step1['W1Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'w',398)
step1['W2Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'w',399)
step1['W3Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'w',400)
step1['Z0Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'dy',440)
step1['Z1Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'dy',441)
step1['Z2Jets_Pt-0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'dy',442)
step1['Z3Jets-Pt_0To100_TuneZ2_7TeV_alpgen_tauola']=genvalid('Hadronizer_Et20ExclTuneZ2_7TeV_alpgen_tauola_cff',step1GenDefaults,'dy',443)

PU1={'--pileup':'E7TeV_FlatDist10_2011EarlyData_inTimeOnly'}
step1['ZmumuJets_Pt_20_300PU1']=merge([gen('ZmumuJets_Pt_20_300_GEN_7TeV_cfg',K250by100),PU1])
step1['TTbarPU2']=merge([step1['TTbar2'],PU1])

##########################



# step2 
step2Defaults = { 'cfg'           : 'step2',
                  '-s'            : 'DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco',
                  '--datatier'    : 'GEN-SIM-DIGI-RAW-HLTDEBUG',
                  '--eventcontent': 'FEVTDEBUGHLT',
                  '--conditions'  : 'auto:mc',
                  }

step2 = {}

step2['DIGIPROD1']=merge([{'--eventcontent':'RAWSIM','--datatier':'GEN-SIM-RAW'},step2Defaults])
step2['DIGI1']=merge([step2Defaults])
step2['DIGI2']=merge([stCond,step2Defaults])
step2['DIGICOS']=merge([{'--scenario':'cosmics','--eventcontent':'FEVTDEBUG','--datatier':'GEN-SIM-DIGI-RAW'},stCond,step2Defaults])

step2['DIGIPU1']=merge([step2['DIGI1'],PU1])
    
#add this line when testing from an input file that is not strictly GEN-SIM
#addForAll(step2,{'--process':'DIGI'})

dataReco={'--conditions':'auto:com10',
          '-s':'RAW2DIGI,L1Reco,RECO,ALCA:SiStripCalZeroBias+SiStripCalMinBias+TkAlMinBias,DQM',
          '--datatier':'RECO,DQM',
          '--eventcontent':'RECO,DQM',
          '--data':'',
          '--magField':'AutoFromDBCurrent',
          '--customise':'Configuration/DataProcessing/RecoTLR.customisePPData',
          '--inputCommands':'"keep *","drop *_*_*_RECO"',
          '--process':'reRECO',
          }
step2['RECOD']=merge([{'--scenario':'pp',},dataReco])
step2['RECOVALSKIM']=merge([{'--scenario':'pp','--customise':'Configuration/DataProcessing/RecoTLR.customiseVALSKIM','-s':'RAW2DIGI,L1Reco,RECO,DQM'},step2['RECOD']])
step2['RECOVALSKIMALCA']=merge([{'--scenario':'pp','--customise':'Configuration/DataProcessing/RecoTLR.customiseVALSKIM'},step2['RECOD']])


step2['RECOCOSD']=merge([{'--scenario':'cosmics',
                          '-s':'RAW2DIGI,L1Reco,RECO,L1HwVal,DQM,ALCA:MuAlCalIsolatedMu+DtCalib',
                          '--customise':'Configuration/DataProcessing/RecoTLR.customiseCosmicData'
                          },dataReco])



# step3 
step3Defaults = { 'cfg'           : 'step3',
                  '-s'            : 'RAW2DIGI,L1Reco,RECO,VALIDATION,DQM',
                  '--filein'      : 'file:reco.root',
                  '--conditions'  : 'auto:mc',
                  '--no_exec'     : '',
                  '--datatier'    : 'GEN-SIM-RECO,DQM',
                  '--eventcontent': 'RECOSIM,DQM'
                  }

step3 = {}

step3['RECO1']=merge([step3Defaults])
step3['RECO2']=merge([stCond,step3Defaults])
step3['RECOPROD1']=merge([{ '-s' : 'RAW2DIGI,L1Reco,RECO', '--datatier' : 'GEN-SIM-RECO,AODSIM', '--eventcontent' : 'RECOSIM,AODSIM'},step3Defaults])
step3['RECOMU']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu+DtCalib','--datatier':'GEN-SIM-RECO','--eventcontent':'RECOSIM'},stCond,step3Defaults])
step3['RECOCOS']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu','--datatier':'GEN-SIM-RECO','--eventcontent':'RECOSIM','--scenario':'cosmics'},stCond,step3Defaults])
step3['RECOMIN']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:SiStripCalZeroBias+SiStripCalMinBias+EcalCalPhiSym+EcalCalPi0Calib+EcalCalEtaCalib,VALIDATION,DQM'},stCond,step3Defaults])
step3['RECOQCD']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu+DtCalib+EcalCalPi0Calib+EcalCalEtaCalib,VALIDATION,DQM'},stCond,step3Defaults])

step3['RECOPU1']=merge([step3['RECO1'],PU1])

#add this line when testing from an input file that is not strictly GEN-SIM
#addForAll(step3,{'--hltProcess':'DIGI'})

step3['ALCACOSD']={'--conditions':'auto:com10',
                   '--datatier':'ALCARECO',
                   '--eventcontent':'ALCARECO',
                   '--scenario':'cosmics',
                   '-s':'ALCA:TkAlCosmics0T+MuAlGlobalCosmics+HcalCalHOCosmics+DQM'
                   }
step3['ALCAPROMPT']={'-s':'ALCA:PromptCalibProd',
                     '--filein':'file:TkAlMinBias.root',
                     '--conditions':'auto:com10',
                     '--datatier':'ALCARECO',
                     '--eventcontent':'ALCARECO'}
   
# step4
step4Defaults = { 'cfg'           : 'step4',
                  '-s'            : 'ALCA:TkAlMuonIsolated+TkAlMinBias+EcalCalElectron+HcalCalIsoTrk+MuAlOverlaps',
                  '-n'            : 1000,
                  '--filein'      : 'file:reco.root',
                  '--conditions'  : 'auto:mc',
                  '--datatier'    : 'ALCARECO',
                  '--eventcontent': 'ALCARECO',
                  }
step4 = {}

step4['ALCATT1']=merge([step4Defaults])
step4['ALCATT2']=merge([stCond,step4Defaults])
step4['ALCAMIN']=merge([{'-s':'ALCA:TkAlMinBias'},stCond,step4Defaults])
step4['ALCAQCD']=merge([{'-s':'ALCA:HcalCalIsoTrk+HcalCalDijets+HcalCalHO'},stCond,step4Defaults])
step4['ALCAMU']=merge([{'-s':'ALCA:MuAlOverlaps+TkAlMuonIsolated+TkAlZMuMu'},stCond,step4Defaults])
step4['ALCACOS']=merge([{'-s':'ALCA:TkAlCosmics0T+MuAlGlobalCosmics+HcalCalHOCosmics'},stCond,step4Defaults])
step4['ALCABH']=merge([{'-s':'ALCA:TkAlBeamHalo+MuAlBeamHaloOverlaps+MuAlBeamHalo'},stCond,step4Defaults])

step4['ALCAHARVD']={'-s':'ALCAHARVEST:BeamSpotByRun+BeamSpotByLumi',
                    '--conditions':'auto:com10',
                    '--scenario':'pp',
                    '--data':'',
                    '--filein':'file:PromptCalibProd.root'}

step2['ALCANZS']=merge([{'-s':'ALCA:HcalCalMinBias','cfg':'step2'},step4Defaults])
step2['HARVGEN']={'-s':'HARVESTING:genHarvesting',
                  '--harvesting':'AtJobEnd',
                  '--conditions':'auto:mc',
                  '--mc':'',
                  '--filein':'file:raw.root'}
                 

#### for special wfs ###
step1['TTbar_REDIGI_RERECO']=merge([{'cfg':'TTbar_Tauola_7TeV_cfi',
                                     '-s':'GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu+DtCalib,VALIDATION,DQM',
                                     '--datatier':'GEN-SIM-DIGI-RAW-HLTDEBUG-RECO,DQM',
                                     '--eventcontent':'FEVTDEBUGHLT,DQM'},
                                    K9by50,stCond,step1Defaults])
step2['REDIGI2RECO']=merge([{'-s':'DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO,VALIDATION,DQM',
                             '--customise':'Configuration/StandardSequences/DigiToRecoNoPU.customise',
                             '--filtername':'REDIGItoRECO',
                             '--process':'REDIGI'},
                            stCond,step3Defaults])
step3['RECOFROMRECO']=merge([{'-s':'RECO,ALCA:MuAlCalIsolatedMu+DtCalib',
                              '--filtername':'RECOfromRECO',
                              '--datatier':'GEN-SIM-RECO',
                              '--eventcontent':'RECOSIM'},
                             stCond,step3Defaults])



# to handle things easier in other places, make a list of all the steps:
stepList = [step1, step2, step3, step4]


