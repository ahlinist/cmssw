## autoSkim 2012 (7E33 HLT menu) --> starting from Run2012B, ... 
autoSkim = {
    'BJetPlusX' : 'LogError+LogErrorMonitor',
    'BTag' : 'LogError+LogErrorMonitor',
    'Commissioning' : 'LogError+LogErrorMonitor+EcalActivity',
    'Cosmics' : 'LogError+LogErrorMonitor+CosmicSP',
    'DoubleElectron' : 'LogError+LogErrorMonitor+ZElectron+DiTau+HZZ',
    'DoubleMu' : 'LogError+LogErrorMonitor+Zmmg+HZZ+EXOHSCP',
    'DoublePhoton' : 'LogError+LogErrorMonitor',
    'DoublePhotonHighPt' : 'LogError+LogErrorMonitor',
    'ElectronHad' : 'LogError+LogErrorMonitor',
    'HTMHT' : 'LogError+LogErrorMonitor+HighMET',
    'HcalNZS' : 'LogError+LogErrorMonitor',
    'JetHT' : 'LogError+LogErrorMonitor+EXOHSCP',
    'JetMon' : 'LogError+LogErrorMonitor',
    'MET' : 'LogError+LogErrorMonitor+ZHbb+HighMET+EXOHSCP',
    'MinimumBias' : 'LogError+LogErrorMonitor+HLTPhysics',
    'MuEG' : 'LogError+LogErrorMonitor+HZZ',
    'MuHad' : 'LogError+LogErrorMonitor',
    'MuOnia' : 'LogError+LogErrorMonitor+ChiB',
    'MultiJet' : 'LogError+LogErrorMonitor+HighMET',
    'NoBPTX' : 'LogError+LogErrorMonitor+EXOHSCP',
    'PhotonHad' : 'LogError+LogErrorMonitor+EXOMonoPhoton',
    'SingleElectron' : 'LogError+LogErrorMonitor+WElectron+HighMET+TOPElePlusJets+DiTau',
    'SingleMu' : 'LogError+LogErrorMonitor+ZMu+HighMET+EXOHSCP+TOPMuPlusJets+MuTau',
    'SinglePhoton' : 'LogError+LogErrorMonitor+EXODisplacedPhoton+HighMET+EXOMonoPhoton',
    'Tau' : 'LogError+LogErrorMonitor',
    'TauPlusX' : 'LogError+LogErrorMonitor+MuTauMET',
    }


## autoSkim 2012 (5E33 HLT menu) --> only Run2012A
"""
autoSkim = {
    'BTag' : 'LogError+LogErrorMonitor',
    'Commissioning' : 'LogError+LogErrorMonitor+EcalActivity',
    'Cosmics' : 'LogError+LogErrorMonitor+CosmicSP',    
    'DoubleElectron' : 'LogError+LogErrorMonitor+ZElectron+DiTau+HZZ',
    'DoubleMu' : 'LogError+LogErrorMonitor+HZZ+Zmmg',
    'ElectronHad' : 'LogError+LogErrorMonitor+EXOHSCP',
    'HcalNZS' : 'LogError+LogErrorMonitor',
    'HT' : 'LogError+LogErrorMonitor+EXOHSCP+HighMET',
    'Jet' : 'LogError+LogErrorMonitor',
    'MET' : 'LogError+LogErrorMonitor+ZHbb+EXOHSCP+HighMET',
    'MinimumBias' : 'LogError+LogErrorMonitor+HLTPhysics',    
    'MuEG' : 'LogError+LogErrorMonitor+HZZ',
    'MuHad' : 'LogError+LogErrorMonitor',
    'MuOnia' : 'LogError+LogErrorMonitor+ChiB',
    'MultiJet' : 'LogError+LogErrorMonitor+HighMET',
    'Photon' : 'LogError+LogErrorMonitor+HighMET+EXOMonoPhoton',
    'PhotonHad' : 'LogError+LogErrorMonitor+EXOMonoPhoton',
    'SingleElectron' : 'LogError+LogErrorMonitor+HighMET+DiTau+WElectron',
    'SingleMu' : 'LogError+LogErrorMonitor+ZMu+HighMET+EXOHSCP+MuTau',
    'Tau' : 'LogError+LogErrorMonitor',
    'TauPlusX' : 'LogError+LogErrorMonitor+MuTauMET',
    }
"""

# IMPORTANT NOTE for Run2012A :
# TOPElePlusJets should go in ElectronHad
# TOPMuPlusJets should go to MuHad
# However, the configurations in the release, used for prompt skimming
# starting from Run2012B, are not good for Run2012A (since trigger names changed).
# So the TOP skims above have been removed from the Run2012A skim matrix
# and cannot be produced at the moment for this run period

## autoSkim 2011
"""
    'MinimumBias':'MuonTrack+BeamBkg+ValSkim+LogError+HSCPSD',
    'ZeroBias':'LogError',
    'Commissioning':'DT+LogError',
    'Cosmics':'CosmicSP+LogError',
    'Mu' : 'WMu+ZMu+HighMET+LogError',    
    'EG':'WElectron+ZElectron+HighMET+LogError',
    'Electron':'WElectron+ZElectron+HighMET+LogError',
    'Photon':'WElectron+ZElectron+HighMET+LogError+DiPhoton+EXOHPTE',
    'JetMETTau':'LogError+Tau',
    'JetMET':'HighMET+LogError',
    'BTau':'LogError+Tau',
    'Jet':'HighMET+LogError',
    'METFwd':'HighMET+LogError',
    'SingleMu' : 'WMu+ZMu+HighMET+LogError+HWW+HZZ+DiTau+EXOHSCP',
    'DoubleMu' : 'WMu+ZMu+HighMET+LogError+HWW+HZZ+EXOHSCP',
    'SingleElectron' : 'WElectron+HighMET+LogError+HWW+HZZ+Tau',
    'DoubleElectron' : 'ZElectron+LogError+HWW+HZZ',
    'MuEG' : 'LogError+HWW+HZZ',
    'METBTag': 'HighMET+LogError+EXOHSCP',
    'BTag': 'LogError+EXOHSCP',
    'MET': 'HighMET+LogError+EXOHSCP',

    'HT': 'HighMET+LogError',

    'Tau': 'LogError',
    'PhotonHad': 'LogError',
    'MuHad': 'LogError',
    'MultiJet': 'LogError',
    'MuOnia': 'LogError',
    'ElectronHad': 'LogError',
    'TauPlusX': 'LogError',
"""

autoSkimPDWG = {
    
    }

autoSkimDPG = {

    }

def mergeMapping(map1,map2):
    merged={}
    for k in list(set(map1.keys()+map2.keys())):
        items=[]
        if k in map1: 
            items.append(map1[k])
        if k in map2:
            items.append(map2[k])
        merged[k]='+'.join(items)
    return merged
    
#autoSkim = mergeMapping(autoSkimPDWG,autoSkimDPG)
#print autoSkim
