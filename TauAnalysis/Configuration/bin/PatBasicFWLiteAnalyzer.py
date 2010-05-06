#! /usr/bin/env python

import ROOT
import sys
from DataFormats.FWLite import Events, Handle

#files = ["rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMinBiasMuonSkim_C/patTupleMuSkim_99_0.root"]
#files = ["rfio:/castor/cern.ch/user/l/lusito/MCMerged1.root"]
#files = ["rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMinBiasMuonSkim_D/patTupleMuSkim_2_1.root"]
#files = ["file:/tmp/lusito/Data/patTupleMuSkim_100_0.root"]
#files = ["file:/tmp/lusito/Data/patTupleMuSkim_101_0.root"]


##files = ["file:/tmp/lusito/Data/patTupleMuSkim_100_0.root"]
##files = ["file:/tmp/lusito/Data/patTupleMuSkim_101_0.root"]
##files = ["file:/tmp/lusito/Data/patTupleMuSkim_103_0.root"]
##files = ["file:/tmp/lusito/Data/patTupleMuSkim_104_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_105_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_106_3.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_107_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_108_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_109_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_10_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_110_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_111_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_112_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_113_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_114_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_115_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_116_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_118_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_119_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_11_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_120_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_121_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_122_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_123_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_124_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_125_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_126_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_127_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_129_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_12_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_130_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_133_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_134_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_135_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_136_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_137_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_138_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_139_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_140_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_141_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_142_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_143_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_144_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_145_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_147_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_148_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_14_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_150_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_151_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_153_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_154_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_155_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_156_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_157_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_158_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_159_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_160_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_161_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_162_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_163_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_164_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_165_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_166_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_167_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_168_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_169_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_170_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_171_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_172_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_173_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_174_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_175_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_176_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_177_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_178_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_17_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_181_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_182_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_183_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_185_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_186_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_187_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_188_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_18_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_1_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_23_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_24_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_25_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_26_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_27_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_28_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_29_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_30_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_31_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_32_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_34_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_35_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_36_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_37_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_39_1.root"]
## files = ["ile:/tmp/lusito/Data/patTupleMuSkim_40_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_41_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_43_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_44_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_46_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_47_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_49_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_4_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_50_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_51_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_53_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_54_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_55_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_56_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_57_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_58_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_59_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_61_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_62_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_63_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_64_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_67_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_6_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_70_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_72_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_75_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_76_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_77_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_79_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_7_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_82_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_83_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_84_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_85_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_87_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_88_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_89_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_8_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_91_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_92_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_93_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_94_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_95_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_96_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_97_1.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_98_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_99_0.root"]
## files = ["file:/tmp/lusito/Data/patTupleMuSkim_9_0.root"]



#files = ["reco_7TeV_firstdata_356_pat.root"]
#events = Events (files)
events = 2300
handleMuon  = Handle ("std::vector<pat::Muon>")
handleTau  = Handle ("std::vector<pat::Tau>")
handleMET = Handle ("std::vector<pat::METs>")
handleJet = Handle ("std::vector<pat::Jet>")
handleVertex = Handle ("std::vector<reco::Vertex>")

# for now, label is just a tuple of strings that is initialized just
# like and edm::InputTag
labelMuon = ("patMuons")
labelTau = ("patTaus")
labelMET = ("patMETs")
labelJet = ("patJets")
labelVertex = ("offlinePrimaryVertex")


f = ROOT.TFile("analyzerPython.root", "RECREATE")
f.cd()

muonPt  = ROOT.TH1F("muonPt", "pt",    100,  0.,300.)
muonEta = ROOT.TH1F("muonEta","eta",   100, -3.,  3.)
muonPhi = ROOT.TH1F("muonPhi","phi",   100, -5.,  5.)



tauPt  = ROOT.TH1F("tauPt", "pt",    100,  0.,300.)
tauEta = ROOT.TH1F("tauEta","eta",   100, -3.,  3.)
tauPhi = ROOT.TH1F("tauPhi","phi",   100, -5.,  5.) 
metPt  = ROOT.TH1F("metPt", "pt",    100,  0.,300.)
metEta = ROOT.TH1F("metEta","eta",   100, -3.,  3.)
metPhi = ROOT.TH1F("metPhi","phi",   100, -5.,  5.) 
jetPt  = ROOT.TH1F("jetPt", "pt",    100,  0.,300.)
jetEta = ROOT.TH1F("jetEta","eta",   100, -3.,  3.)
jetPhi = ROOT.TH1F("jetPhi","phi",   100, -5.,  5.) 

# loop over events
i = 0
for event in events:
    i = i + 1
    print  i
    # use getByLabel, just like in cmsRun
    event.getByLabel (labelMuon, handleMuon)
    event.getByLabel (labelTau, handleTau)
    event.getByLabel (labelMET, handleMET)
    event.getByLabel (labelJet, handleJet)
    event.getByLabel (labelVertex, handleVertex)
    # get the product
muons = handleMuon.product()
taus = handleTau.product()
met = handleMET.product()
jets = handleJet.product()

for muon in muons :
        muonPt.Fill( muon.pt() )
        muonEta.Fill( muon.eta() )
        muonPhi.Fill( muon.phi() )

for tau in taus :
        tauPt.Fill( tau.pt() )
        tauEta.Fill( tau.eta() )
        tauPhi.Fill( tau.phi() )

for met in met :
        metPt.Fill( met.pt() )
        metEta.Fill( met.eta() )
        metPhi.Fill( met.phi() )


for jet in jets :
        jetPt.Fill( jet.pt() )
        jetEta.Fill( jet.eta() )
        jetPhi.Fill( jet.phi() )




f.cd()

muonPt.Write()
muonEta.Write()
muonPhi.Write()

tauPt.Write()
tauEta.Write()
tauPhi.Write()

metPt.Write()
metEta.Write()
metPhi.Write()

jetPt.Write()
jetEta.Write()
jetPhi.Write()


f.Close()
