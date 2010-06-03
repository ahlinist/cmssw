def Main(fileName):
    f = ROOT.TFile(fileName,"READ")
    tree  = f.Get("VgAnalyzerKit/EventTree")
    print "\n\n==============\n", fileToRun, tree
    
    # output file for histos
    f_out = ROOT.TFile("histos_%s"%fileName,"RECREATE")
    f_out.cd()
    
    Nentries = tree.GetEntries()
    print "\nTOTAL NUMBER OF ENTRIES:\n%s\n\n"%Nentries
    
    # HISTOGRAMS
    Zchannels = []
    Zchannels.append("ZeeGamma")
    Zchannels.append("ZMuMuGamma")
    Wchannels = []
    Wchannels.append("WenuGamma")
    Wchannels.append("WmunuGamma")
    Leg1Pt  = {}
    Leg2Pt  = {}
    LegPt   = {}
    MET     = {}
    GammaEt = {}

    ptHistoNBINS = 30
    ptHistoMin   = 0
    ptHistoMax   = 150
    
    for channel in Zchannels:
        Leg1Pt[channel]  =   ROOT.TH1F("%s_ZLeg1Pt"%channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)
        Leg2Pt[channel]  =   ROOT.TH1F("%s_ZLeg2Pt"%channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)
        GammaEt[channel] =   ROOT.TH1F("%s_GammaEt"%channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)

    for channel in Wchannels:
        LegPt[channel]   =   ROOT.TH1F("%s_WLegPt" %channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)
        MET[channel]     =   ROOT.TH1F("%s_METt"   %channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)
        GammaEt[channel] =   ROOT.TH1F("%s_GammaEt"%channel,"",ptHistoNBINS, ptHistoMin, ptHistoMax)
        
    # ===============
    # CODE =============================
    # ===============
    
    for entry in range (0, Nentries):
        
        if entry % 10000 == 0 : print entry # coffee counter
        #if (entry > 1000): break # analyse only fraction of events
        
        tree.GetEntry(entry) # if there are many files for the single dataset use TChain to process them in one go
        
        # =================================
        # PHOTONS
        # =================================
        goodPhoInd=[]
        dRcut = 0.7 # used later in seperation of g from a lepton
        for iPho in range (0, tree.nPho): # identify good photons
            passPhoEtCut   = tree.phoEt[iPho]               > 20
            passPhoEcalIso = tree.phoEcalIsoDR04[iPho]      < 4.2 + 0.004 * tree.phoEt[iPho]
            passPhoHcalIso = tree.phoHcalIsoDR04[iPho]      < 2.2 + 0.001 * tree.phoEt[iPho]
            passPhoHoverE  = tree.phoHoverE[iPho]           < 0.05                           # isEM
            passPhoTrkIso  = tree.phoTrkIsoHollowDR04[iPho] < 2.  + 0.001 * tree.phoEt[iPho] # isLoose
            passPhoSigIEIE = tree.phoSigmaEtaEta[iPho]      < 0.013                          # isTight
            
            #if passPhoEtCut and passPhoEcalIso and passPhoHcalIso and passPhoHoverE and passPhoTrkIso and passPhoSigIEIE:
            if passPhoEtCut and passPhoEcalIso and passPhoHcalIso and passPhoHoverE and passPhoTrkIso:
                goodPhoInd.append(iPho)
                
        if goodPhoInd.__len__() == 0: continue

        # =================================
        # assuming Zee will be similar to https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZMuMuBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID
        # ZEE+Gamma
        # =================================
        for iZee in range ( 0, tree.nZee):
            leg1Ind = int (tree.ZeeLeg1Index[iZee])
            leg2Ind = int (tree.ZeeLeg2Index[iZee])
            
            # NEED TO ADD HLT REQUIREMENT
            passElOppositeCharge = tree.eleCharge[leg1Ind]   *   tree.eleCharge[leg2Ind] < 0
            passElPtCut          = tree.elePt[leg1Ind] > 20. and tree.elePt[leg2Ind]     > 20.
            passElMassWindow     = tree.ZeeMass[iZee]  > 60. and tree.ZeeMass[iZee]      < 120.
            # NOTE ===============
            # if lets say el indices are 0 and 1 and for each electron there are 12 value maps for electron ID with its own indices [0,11], then requiring egamma eID with rel iso corresponding to 95% eff (Wenu) would mean requiring eleID[11] > 2 and eleID[23] > 2, here 2 is flag, flag 3 means electron is passing Isolation (and ID)
            # print (leg1Ind+1)*12-1, (leg2Ind+1)*12-1
            passElID          = tree.eleID[(leg1Ind+1)*12-1] > 2 and tree.eleID[(leg2Ind+1)*12-1] > 2
            
            if passElOppositeCharge and passElPtCut and passElMassWindow and passElID:
                
                for iPho in goodPhoInd:
                    passEle1PhoDR = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.eleEta[leg1Ind], tree.elePhi[leg1Ind]) > dRcut
                    passEle2PhoDR = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.eleEta[leg2Ind], tree.elePhi[leg2Ind]) > dRcut
                
                    if passEle1PhoDR and passEle2PhoDR:
                        Leg1Pt["ZeeGamma"].Fill (tree.elePt[leg1Ind])
                        Leg2Pt["ZeeGamma"].Fill (tree.elePt[leg2Ind])
                        GammaEt["ZeeGamma"].Fill(tree.phoEt[iPho])
                        

        # =================================
        # https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZMuMuBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID
        # ZMuMu+Gamma
        # =================================
        for iZmumu in range ( 0, tree.nZmumu):
            leg1Ind = int (tree.ZmumuLeg1Index[iZmumu])
            leg2Ind = int (tree.ZmumuLeg2Index[iZmumu])
            
            # NEED TO ADD HLT

            # ID
            passMuD0             = tree.muD0[leg1Ind]                     < 0.2 and  tree.muD0[leg2Ind]                     < 0.2
            passChi2             = tree.muChi2NDF[leg1Ind]                < 10. and  tree.muChi2NDF[leg2Ind]                < 10.
            passPixelHits        = tree.muNumberOfValidPixelHits[leg1Ind] > 0.  and  tree.muNumberOfValidPixelHits[leg2Ind] > 0.
            passStripHits        = tree.muNumberOfValidTrkHits[leg1Ind]   > 10. and  tree.muNumberOfValidTrkHits[leg2Ind]   > 0.
            passMuonHits         = tree.muNumberOfValidMuonHits[leg1Ind]  > 0.  and  tree.muNumberOfValidMuonHits[leg2Ind]  > 0.
            
            # Iso
            passMuIso            = tree.muIsoTrk[leg1Ind]                 < 3.  and  tree.muIsoTrk[leg2Ind]                 < 3.
            
            # kinematic
            passMuOppositeCharge = tree.muCharge[leg1Ind]                  *   tree.muCharge[leg2Ind]             < 0
            passMuPt             = tree.muPt[leg1Ind]                > 20. and tree.muPt[leg2Ind]                > 20.
            passMuEta1           = math.fabs(tree.muEta[leg1Ind])    < 2.1 and math.fabs(tree.muEta[leg2Ind])    < 2.4
            passMuEta2           = math.fabs(tree.muEta[leg1Ind])    < 2.4 and math.fabs(tree.muEta[leg2Ind])    < 2.1
            passMuEta            = passMuEta1 or passMuEta2
            passMuMassWindow     = tree.ZmumuMass[iZmumu]            > 60. and tree.ZmumuMass[iZmumu]            < 120.

            if passMuD0 and passChi2 and passPixelHits and passStripHits and passMuonHits:
                if passMuIso and passMuOppositeCharge and passMuPt and passMuEta and passMuMassWindow:
                    
                    for iPho in goodPhoInd:
                        passMu1PhoDR = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leg1Ind], tree.muPhi[leg1Ind]) > dRcut
                        passMu2PhoDR = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leg2Ind], tree.muPhi[leg2Ind]) > dRcut
                
                        if passMu1PhoDR and passMu2PhoDR:
                            Leg1Pt["ZMuMuGamma"].Fill (tree.muPt[leg1Ind])
                            Leg2Pt["ZMuMuGamma"].Fill (tree.muPt[leg2Ind])
                            GammaEt["ZMuMuGamma"].Fill(tree.phoEt[iPho])
                            




        # =================================
        # https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZMuMuBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID
        # WMuNu+Gamma
        # =================================

        for iWmunu in range ( 0, tree.nWmunu):
            #print tree.WmunuMuIndex.__len__(), tree.nWmunu
            if tree.WmunuMuIndex.__len__() < tree.nWmunu: continue # there must be bug somewhere, for some entries I get 0 length for WmunuMuIndex while having none zero Wmunu candidates
            leptonInd = int (tree.WmunuMuIndex[iWmunu])
            
            # Wmunu selection from https://twiki.cern.ch/twiki/bin/view/CMS/VbtfWmunuBaselineSelection
            # MUON ID 
            # need to add requirement for HLT_Mu9 
            #  The muon must be identified as both global muon and tracker muon ------- THIS SELECTION IS NOT POSSIBLE IN SCRIPT -> SHOULD BE ADDED TO NTUPLISER
            passTrkHits  = tree.muNumberOfValidTrkHits[leptonInd]      >    10.
            passD0       = tree.muD0[leptonInd]                        <    0.2 # please check if this corresponds to 2 mm
            passChi2     = tree.muChi2NDF[leptonInd]                   <    10
            passMuonHits = tree.muNumberOfValidMuonHits[leptonInd]     >    0.
            passEta      = math.fabs(tree.muEta[leptonInd])            <    2.1
            
            if passTrkHits and passD0 and passChi2 and passMuonHits and passEta:
                # Wmunu candidate
                passPt       = tree.muPt[leptonInd]                        >    25.
                passIso      = ( tree.muIsoTrk[leptonInd]+tree.muIsoEcal[leptonInd]+tree.muIsoHcal[leptonInd] )/ tree.muPt[leptonInd] < 0.15
                # NOTE FOR ISOLATION FROM TWIKI
                # * Relative combined isolation = (sumPt + emEt + hcalEt)/ptmu < 0.15 in a deltaR < 0.3 cone
                #      o Default PFlow isolation in a deltaR < 0.3 cone is also an equally valid option, provided that there is perfect matching between muons selected with the muonId criteria described in this page and muons identified by the PFlow muon algorithms (which seems to be the case today).
                passMet      = tree.pfMET                                  >    50.
                passAcop     = tree.WmunuACopPfMET[iWmunu]                 <    2.
                passRejectDY = True
                if tree.nMu >= 2:
                    if tree.muPt[0] > 20. and tree.muPt[1] > 10.:
                        passRejectDY = False
                        # NOTE from VBTF twiki - We are rejecting events with two global muons satisfying: ptmu1>20 GeV, ptmu2>10 GeV, where ptmu1 is the highest muon pt and ptmu2 is the second highest muon pt in the event.
                        # looks like in nTupliser we are require muons to be Global + Tracker... needs to be fixed to be coherent with VBTF...
                
                # now we should have Wmunu candidate
                if passPt and passIso and passMet and passAcop and passRejectDY:
                    
                    # check that good photon is seperated from W muon
                    for iPho in goodPhoInd:
                        passWmuonPhoDR = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leptonInd], tree.muPhi[leptonInd]) > dRcut
                        
                        if passWmuonPhoDR:
                            LegPt["WmunuGamma"].Fill(tree.muPt[leptonInd])
                            MET["WmunuGamma"].Fill(tree.pfMET)
                            GammaEt["WmunuGamma"].Fill(tree.phoEt[iPho])
                            
    # END OF CODE PART
    
    # ===================
    # once running loops is finished collect plots and put them in output file
    # ===================
    
    for channel in Zchannels:
        # stylise histos
        beautify( Leg1Pt[channel]  , ROOT.kRed    , "", "Z lepton pT", "events / 5 GeV")
        beautify( Leg2Pt[channel]  , ROOT.kBlue   , "", "Z lepton pT", "events / 5 GeV")
        beautify( GammaEt[channel] , ROOT.kGreen  , "", "#gamma Et"  , "events / 5 GeV")
        
        f_out.mkdir(channel)
        f_out.cd(channel)
        GammaEt[channel].Write()
        Leg1Pt[channel].Write()
        Leg2Pt[channel].Write()

    for channel in Wchannels:
        # stylise histos
        beautify( LegPt[channel]   , ROOT.kRed    , "", "W lepton pT", "events / 5 GeV")
        beautify( MET[channel]     , ROOT.kBlue   , "", "pf MET"     , "events / 5 GeV")
        beautify( GammaEt[channel] , ROOT.kGreen  , "", "#gamma Et"  , "events / 5 GeV")
        
        f_out.mkdir(channel)
        f_out.cd(channel)
        GammaEt[channel].Write()
        LegPt[channel].Write()
        MET[channel].Write()
        
    f_out.Close()
    f.Close()
    






# =======================================================
# IGNORE
# TAB COMPLETION
try:
    import readline
except ImportError:
    print "Module readline not available."
else:
    import rlcompleter
    readline.parse_and_bind("tab: complete")
# =======================================================
