# Python script for analyzing CMS EDM data using FWLite.
# Do not use cmsRun. Simply:
# python HcalPFGRecHitAndJetAna_ExpressData_FWLite.py
# Loops over Hcal RecHits and Jets, fills basic histograms, and writes a root file.


# Author: Jim Hirschauer

import sys

from ROOT import TFile, TH1F, TH2F
from DataFormats.FWLite import Events, Handle


runnum = 163286
tempfiles = [
'/store/express/Run2011A/ExpressPhysics/FEVT/Express-v2/000/163/286/020B6A60-D96C-E011-9823-0030487A18A4.root',
'/store/express/Run2011A/ExpressPhysics/FEVT/Express-v2/000/163/286/00B76A60-D96C-E011-BCE5-0030487A17B8.root',
'/store/express/Run2011A/ExpressPhysics/FEVT/Express-v2/000/163/286/0094F111-D36C-E011-A090-003048F01E88.root',
'/store/express/Run2011A/ExpressPhysics/FEVT/Express-v2/000/163/286/0074B30A-C56C-E011-AA79-003048F11C5C.root',
'/store/express/Run2011A/ExpressPhysics/FEVT/Express-v2/000/163/286/004E7188-C86C-E011-BD75-0030487CD184.root'
]

cernRecoString = 'rfio:'
cernExpString = 'rfio://castorcms/?svcClass=t0express&path=/castor/cern.ch/cms'
fnalString = 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11'
prefix = cernExpString
files = []
for tempfile in tempfiles:
    files.append(prefix+tempfile)
    

h_bx           = TH1F('h_bx'      , 'h_bx'        , 3601  , -0.5, 3600.5)
h_nvtx         = TH1F('h_nvtx'    , 'h_nvtx'      , 30    , -0.5, 29.5)
h_nvtx_jet     = TH1F('h_nvtx_jet', 'h_nvtx_jet'  , 30    , -0.5, 29.5)

h_rh_hfp       = TH1F('h_rh_hfp'   , 'h_rh_hfp'   , 1000  , -5., 100.)
h_rh_hfm       = TH1F('h_rh_hfm'   , 'h_rh_hfm'   , 1000  , -5., 100.)

h_rh_hbp       = TH1F('h_rh_hbp'   , 'h_rh_hbp'   , 1000  , -5., 100.)
h_rh_hbm       = TH1F('h_rh_hbm'   , 'h_rh_hbm'   , 1000  , -5., 100.)

h_rh_hep       = TH1F('h_rh_hep'   , 'h_rh_hep'   , 1000  , -5., 100.)
h_rh_hem       = TH1F('h_rh_hem'   , 'h_rh_hem'   , 1000  , -5., 100.)

h_jet_eta      = TH1F('h_jet_eta'     , 'h_jet_eta'     , 100, -5., 5.)
h_jet_pt       = TH1F('h_jet_pt'      , 'h_jet_pt'      , 200, -5., 200.)
h_jet_hf_pt    = TH1F('h_jet_hf_pt'   , 'h_jet_hf_pt'   , 200, -5., 200.)
h_jet_he_pt    = TH1F('h_jet_he_pt'   , 'h_jet_he_pt'   , 200, -5., 200.)
h_jet_trans_pt = TH1F('h_jet_trans_pt', 'h_jet_trans_pt', 200, -5., 200.)
h_jet_hb_pt    = TH1F('h_jet_hb_pt'   , 'h_jet_hb_pt'   , 200, -5., 200.)

events = Events (files)

trigHandle = Handle("edm::TriggerResults")
trigLabel  = ("TriggerResults")

l1trigHandle = Handle("L1GlobalTriggerReadoutRecord")
l1trigLabel = ("gtDigis")

jetHandle = Handle("vector<reco::CaloJet>")
jetLabel = ("ak5CaloJets")

hfrhHandle = Handle("edm::SortedCollection<HFRecHit,edm::StrictWeakOrdering<HFRecHit>>")
hfrhLabel = ("hfreco")

hbherhHandle = Handle("edm::SortedCollection<HBHERecHit,edm::StrictWeakOrdering<HBHERecHit>>")
hbherhLabel = ("hbhereco")

vtxHandle = Handle('vector<reco::Vertex>')
vtxLabel = ("offlinePrimaryVertices")

nEvent = 0
for event in events:
    nEvent += 1
    if nEvent%1000==1:
        print "record:",nEvent,"Run:",event.object().id().run(),\
              "event:",event.object().id().event()

    event.getByLabel(l1trigLabel,l1trigHandle)
    l1trig = l1trigHandle.product()

    passedJet16or36or52 = False
    dword = l1trig.decisionWord()
    if dword.at(15) or dword.at(16) or dword.at(17): passedJet16or36or52 = True

    
    bx = event.object().bunchCrossing()
    h_bx.Fill(bx)
    
    event.getByLabel(vtxLabel,vtxHandle)    
    vtxes = vtxHandle.product()
    nvtx = vtxes.size()
    h_nvtx.Fill(nvtx)

    if passedJet16or36or52:
        h_nvtx_jet.Fill(nvtx)
        event.getByLabel(jetLabel,jetHandle)
        jets = jetHandle.product()
        if jets.size()>0:
            leadjet_eta = jets.at(0).eta()
            h_jet_eta.Fill(leadjet_eta)
            h_jet_pt.Fill(jets.at(0).pt())
            if abs(leadjet_eta)<1.1: h_jet_hb_pt.Fill(jets.at(0).pt())
            elif abs(leadjet_eta)<1.5: h_jet_trans_pt.Fill(jets.at(0).pt())
            elif abs(leadjet_eta)<3.0: h_jet_he_pt.Fill(jets.at(0).pt())
            else: h_jet_hf_pt.Fill(jets.at(0).pt())

    event.getByLabel(hfrhLabel,hfrhHandle)
    hfrechits = hfrhHandle.product()
    for hfrechit in hfrechits:
        hfeta = hfrechit.id().ieta()
        hfene = hfrechit.energy()
        if hfeta > 0 :
            h_rh_hfp.Fill(hfene)
        else:
            h_rh_hfm.Fill(hfene)

    event.getByLabel(hbherhLabel,hbherhHandle)
    hbherechits = hbherhHandle.product()
    for hbherechit in hbherechits:
        hbheeta = hbherechit.id().ieta()
        hbhedep = hbherechit.id().depth()
        hbheene = hbherechit.energy()
        
        if hbheeta > 0 and hbheeta <=15:
            h_rh_hbp.Fill(hbheene)
        elif hbheeta < 0 and hbheeta >=-15:
            h_rh_hbm.Fill(hbheene)
        elif hbheeta == 16 and (hbhedep == 1 or hbhedep == 2):
            h_rh_hbp.Fill(hbheene)
        elif hbheeta == -16 and (hbhedep == 1 or hbhedep == 2):
            h_rh_hbm.Fill(hbheene)
        elif hbheeta == 16 and hbhedep == 3:
            h_rh_hep.Fill(hbheene)
        elif hbheeta == -16 and hbhedep == 3:
            h_rh_hem.Fill(hbheene)
        elif hbheeta > 16 :
            h_rh_hep.Fill(hbheene)
        elif hbheeta < -16 :
            h_rh_hem.Fill(hbheene)
        else:
            print "Unaccounted for HBHE rechit eta, dep =",hbheeta, hbhedep
            
    if nEvent >= 25000: break

print nEvent

toutfile = TFile("hists_hfzstest_zs_express_"+str(runnum)+".root","recreate")
h_jet_eta  .Write()
h_jet_pt   .Write()
h_jet_hf_pt.Write()
h_jet_hb_pt.Write()
h_jet_he_pt.Write()
h_jet_trans_pt.Write()

h_rh_hfp.Write()
h_rh_hfm.Write()
h_rh_hbp.Write()
h_rh_hbm.Write()
h_rh_hep.Write()
h_rh_hem.Write()

h_bx.Write()
h_nvtx.Write()
h_nvtx_jet.Write()

toutfile.Close()
        
        



