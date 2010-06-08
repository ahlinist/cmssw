def Main(fileName):

    f = ROOT.TFile.Open(fileName,"READ")
    tree  = f.Get("VgAnalyzerKit/EventTree")
        
    print "\n\n==============\n", fileName, tree

    # output file for histos
    if "rfio" in fileName:
        fileName = fileName.split("/")[-1]


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
    Leg1Pt      = {}
    Leg2Pt      = {}
    LegPt       = {}
    MET         = {}
    GammaEt     = {}
    dRLeg1Gamma = {}
    dRLeg2Gamma = {}
    dRLegGamma  = {}
    ZMass       = {}
    WtMass      = {}
    
    ptHistoNBINS = 30
    ptHistoMin   = 0
    ptHistoMax   = 150
    
    for channel in Zchannels:
        Leg1Pt[channel]      =   ROOT.TH1F("%s_ZLeg1Pt"    %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        Leg2Pt[channel]      =   ROOT.TH1F("%s_ZLeg2Pt"    %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        GammaEt[channel]     =   ROOT.TH1F("%s_GammaEt"    %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        dRLeg1Gamma[channel] =   ROOT.TH1F("%s_dRLeg1Gamma"%channel,"", 60          , 0         , 6         )
        dRLeg2Gamma[channel] =   ROOT.TH1F("%s_dRLeg2Gamma"%channel,"", 60          , 0         , 6         )
        ZMass[channel]       =   ROOT.TH1F("%s_ZMass"      %channel,"", 12          ,50         ,130        )

        Leg1Pt[channel].Sumw2()
        Leg2Pt[channel].Sumw2()
        GammaEt[channel].Sumw2()
        dRLeg1Gamma[channel].Sumw2()
        dRLeg2Gamma[channel].Sumw2()
        ZMass[channel].Sumw2()

    for channel in Wchannels:
        LegPt[channel]       =   ROOT.TH1F("%s_WLegPt"    %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        MET[channel]         =   ROOT.TH1F("%s_MET"       %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        GammaEt[channel]     =   ROOT.TH1F("%s_GammaEt"   %channel,"", ptHistoNBINS, ptHistoMin, ptHistoMax)
        dRLegGamma[channel]  =   ROOT.TH1F("%s_dRLegGamma"%channel,"", 60          , 0         , 6         )
        WtMass[channel]      =   ROOT.TH1F("%s_WtMass"    %channel,"", 12          ,50         ,130        )

        LegPt[channel].Sumw2()
        MET[channel].Sumw2()
        GammaEt[channel].Sumw2()
        dRLegGamma[channel].Sumw2()
        WtMass[channel].Sumw2()
        
    # ===============
    # CODE =============================
    # ===============
    
    for entry in range (0, Nentries):
        
        if entry % 10000 == 0 : print entry # coffee counter
        if (entry > 15000):     break       # analyse only fraction of events
        
        tree.GetEntry(entry)
        
        # =================================
        # PHOTONS
        # =================================

        goodPhoInd=[]
        dRcut = 0.7 # **********  used later in seperation of gamma from a lepton ************

        for iPho in range (0, tree.nPho): # identify good photons
            passPhoEtCut       = tree.phoEt[iPho]               > 20
            passPhoEcalIso     = tree.phoEcalIsoDR04[iPho]      < 4.2 + 0.004 * tree.phoEt[iPho]
            passPhoHcalIso     = tree.phoHcalIsoDR04[iPho]      < 2.2 + 0.001 * tree.phoEt[iPho]
            passPhoHoverE      = tree.phoHoverE[iPho]           < 0.05                           # isEM
            passPhoTrkIso      = tree.phoTrkIsoHollowDR04[iPho] < 2.  + 0.001 * tree.phoEt[iPho] # isLoose
            passPhoSigIEIE     = tree.phoSigmaIEtaIEta[iPho]    < 0.013                          # isTight # DOESN'T THIS CUT OUT ENDCAP???
            requireNoPixelSeed = tree.phohasPixelSeed[iPho]     == 0
            requireNoOverlapEl = tree.phoOverlap[iPho]          == 0
            if passPhoEtCut and passPhoEcalIso and passPhoHcalIso and passPhoHoverE and passPhoTrkIso and requireNoPixelSeed and requireNoOverlapEl:
                goodPhoInd.append(iPho)
                
        if goodPhoInd.__len__() == 0: continue # if no good photon found skip event


        

        # =================================
        # ZEE+Gamma
        # =================================

        # according to https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZeeBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID
        for iZee in range ( 0, tree.nZee):
            leg1Ind = int (tree.ZeeLeg1Index[iZee])
            leg2Ind = int (tree.ZeeLeg2Index[iZee])
            
            # CHECK IT THIS IS CORRECT
            passTrg1 = ( tree.ttbit40==1 or tree.ttbit41==1) and tree.ttbitBH==0
            passTrg2 = 1.-(tree.eleETop[leg1Ind]+tree.eleEBottom[leg1Ind]+tree.eleERight[leg1Ind]+tree.eleELeft[leg1Ind])/tree.eleEMax[leg1Ind] < 0.95
            passTrg3 = 1.-(tree.eleETop[leg2Ind]+tree.eleEBottom[leg2Ind]+tree.eleERight[leg2Ind]+tree.eleELeft[leg2Ind])/tree.eleEMax[leg2Ind] < 0.95

            if passTrg1 and passTrg2 and passTrg3:

                # Also should require one of electrons to be matched to trigger object
                passElPtCut          = tree.elePt[leg1Ind] > 20. and tree.elePt[leg2Ind]     > 20. # again we need ele SC ET cut here
                passElMassWindow     = tree.ZeeMass[iZee]  > 60. and tree.ZeeMass[iZee]      < 120.
                passEtaCut1          = math.fabs(tree.eleSCEta[leg1Ind]) < 2.5 and not ( math.fabs(tree.eleSCEta[leg1Ind])>1.4442 and  math.fabs(tree.eleSCEta[leg1Ind])<1.56 )
                passEtaCut2          = math.fabs(tree.eleSCEta[leg2Ind]) < 2.5 and not ( math.fabs(tree.eleSCEta[leg2Ind])>1.4442 and  math.fabs(tree.eleSCEta[leg2Ind])<1.56 )
                passEtaCut           = passEtaCut1 and passEtaCut2
                # requiring electrons to be "simpleEleId95relIso"
                passElID          = tree.eleID[(leg1Ind+1)*12-1] == 7 and tree.eleID[(leg2Ind+1)*12-1] == 7
                
                if passElPtCut and passElMassWindow and passEtaCut and passElID:
                    for iPho in goodPhoInd:
                        # require gamma to be seperated from Z leptons
                        dR1           = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.eleEta[leg1Ind], tree.elePhi[leg1Ind])
                        dR2           = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.eleEta[leg2Ind], tree.elePhi[leg2Ind])
                        passEle1PhoDR = dR1 > dRcut
                        passEle2PhoDR = dR2 > dRcut
                        
                        if passEle1PhoDR and passEle2PhoDR:
                            Leg1Pt["ZeeGamma"].Fill (tree.elePt[leg1Ind])
                            Leg2Pt["ZeeGamma"].Fill (tree.elePt[leg2Ind])
                            GammaEt["ZeeGamma"].Fill(tree.phoEt[iPho])
                            dRLeg1Gamma["ZeeGamma"].Fill(dR1)
                            dRLeg2Gamma["ZeeGamma"].Fill(dR2)
                            ZMass["ZeeGamma"].Fill(tree.ZeeMass[iZee])
                        




        # =================================
        # ZMuMu+Gamma
        # =================================

        # https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZMuMuBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID
        for iZmumu in range ( 0, tree.nZmumu):
            leg1Ind = int (tree.ZmumuLeg1Index[iZmumu])
            leg2Ind = int (tree.ZmumuLeg2Index[iZmumu])
            
            # NEED TO ADD HLT
            # ID
            passMuD0             = tree.muD0[leg1Ind]                     < 0.2 and  tree.muD0[leg2Ind]                     < 0.2
            passChi2             = tree.muChi2NDF[leg1Ind]                < 10. and  tree.muChi2NDF[leg2Ind]                < 10.
            passPixelHits        = tree.muNumberOfValidPixelHits[leg1Ind] > 0.  and  tree.muNumberOfValidPixelHits[leg2Ind] > 0.
            passStripHits        = tree.muNumberOfValidTrkHits[leg1Ind]   > 10. and  tree.muNumberOfValidTrkHits[leg2Ind]   > 10.
            passMuonHits         = tree.muNumberOfValidMuonHits[leg1Ind]  > 0.  and  tree.muNumberOfValidMuonHits[leg2Ind]  > 0.
            # Iso
            passMuIso            = tree.muIsoTrk[leg1Ind]                 < 3.  and  tree.muIsoTrk[leg2Ind]                 < 3.
            # kinematic
            passMuOppositeCharge = tree.muCharge[leg1Ind]                  *         tree.muCharge[leg2Ind]                 < 0
            passMuPt             = tree.muPt[leg1Ind]                     > 20. and  tree.muPt[leg2Ind]                     > 20.
            passMuEta1           = math.fabs(tree.muEta[leg1Ind])         < 2.1 and  math.fabs(tree.muEta[leg2Ind])         < 2.4
            passMuEta2           = math.fabs(tree.muEta[leg1Ind])         < 2.4 and  math.fabs(tree.muEta[leg2Ind])         < 2.1
            passMuEta            = passMuEta1                                   or   passMuEta2
            passMuMassWindow     = tree.ZmumuMass[iZmumu]                 > 60. and  tree.ZmumuMass[iZmumu]                 < 120.

            if passMuD0 and passChi2 and passPixelHits and passStripHits and passMuonHits:
                if passMuIso and passMuOppositeCharge and passMuPt and passMuEta and passMuMassWindow:
                    
                    for iPho in goodPhoInd:
                        # require gamma to be seperated from Z leptons
                        dR1           = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leg1Ind], tree.muPhi[leg1Ind])
                        dR2           = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leg2Ind], tree.muPhi[leg2Ind])
                        passMu1PhoDR  = dR1 > dRcut
                        passMu2PhoDR  = dR2 > dRcut
                
                        if passMu1PhoDR and passMu2PhoDR:
                            Leg1Pt["ZMuMuGamma"].Fill (tree.muPt[leg1Ind])
                            Leg2Pt["ZMuMuGamma"].Fill (tree.muPt[leg2Ind])
                            GammaEt["ZMuMuGamma"].Fill(tree.phoEt[iPho])
                            dRLeg1Gamma["ZMuMuGamma"].Fill(dR1)
                            dRLeg2Gamma["ZMuMuGamma"].Fill(dR2)
                            ZMass["ZMuMuGamma"].Fill(tree.ZmumuMass[iZmumu])
                            



        # =================================
        # WMuNu+Gamma
        # =================================

        # https://twiki.cern.ch/twiki/bin/view/CMS/VbtfZMuMuBaselineSelection
        # https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID

        for iWmunu in range ( 0, tree.nWmunu):
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
                        # ATTENTION!!!
                        # from VBTF twiki - We are rejecting events with two global muons satisfying: ptmu1>20 GeV, ptmu2>10 GeV, where ptmu1 is the highest muon pt and ptmu2 is the second highest muon pt in the event.
                        # looks like in nTupliser we are require muons to be Global + Tracker... needs to be fixed to be coherent with VBTF...
                
                # now we should have Wmunu candidate
                if passPt and passIso and passMet and passAcop and passRejectDY:
                    
                    # check that good photon is seperated from W muon
                    for iPho in goodPhoInd:
                        dR             = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.muEta[leptonInd], tree.muPhi[leptonInd])
                        passWmuonPhoDR = dR > dRcut
                        
                        if passWmuonPhoDR:
                            LegPt["WmunuGamma"].Fill(tree.muPt[leptonInd])
                            MET["WmunuGamma"].Fill(tree.pfMET)
                            GammaEt["WmunuGamma"].Fill(tree.phoEt[iPho])
                            dRLegGamma["WmunuGamma"].Fill(dR)
                            WtMass["WmunuGamma"].Fill(tree.WmunuMassTPfMET[iWmunu])

        for iWenu in range ( 0, tree.nWenu):
            leptonInd = int (tree.WenuEleIndex[iWenu])
            

            passTrg1 = ( tree.ttbit40==1 or tree.ttbit41==1) and tree.ttbitBH==0
            passTrg2 = 1.-(tree.eleETop[leptonInd]+tree.eleEBottom[leptonInd]+tree.eleERight[leptonInd]+tree.eleELeft[leptonInd])/tree.eleEMax[leptonInd] < 0.95

            if passTrg1 and passTrg2:
                # according https://twiki.cern.ch/twiki/bin/view/CMS/VbtfWenuBaselineSelection
                # ELECTRON SHOULD BE TRIGGER MATCHED
                passPtCut          = tree.elePt[leptonInd]          >    25.  # this is suppose to be Et cut on Electron SuperCluser!!!! please add to ntuple vars
                passEtaCut         = math.fabs(tree.eleSCEta[leptonInd]) < 2.5 and not ( math.fabs(tree.eleSCEta[leptonInd])>1.4442 and  math.fabs(tree.eleSCEta[leptonInd])<1.56 )
                passID             = tree.eleID[(leptonInd+1)*12-4] ==   7     # 80% eff. from egamma, note that it uses relative iso
                passRejectDY = True
                for jEle in range( 0, tree.nEle):
                    # we reject events with a second electron passing the electron WP95 selection and having ET > 20.0 GeV 
                    if jEle!=leptonInd and tree.eleID[(jEle+1)*12-1] == 7 and tree.elePt[jEle] > 20. :
                        passRejectDY = False
                        
                # now we should have Wenu candidate
                if passPtCut and passEtaCut and passID and passRejectDY:
                    # check that good photon is seperated from W electron
                    for iPho in goodPhoInd:
                        dR            = delRcalculator( tree.phoEta[iPho], tree.phoPhi[iPho], tree.eleEta[leptonInd], tree.elePhi[leptonInd])
                        passWelePhoDR = dR > dRcut
                        
                        if passWelePhoDR:
                            LegPt["WenuGamma"].Fill(tree.elePt[leptonInd])
                            MET["WenuGamma"].Fill(tree.pfMET)
                            GammaEt["WenuGamma"].Fill(tree.phoEt[iPho])
                            dRLegGamma["WenuGamma"].Fill(dR)
                            WtMass["WenuGamma"].Fill(tree.WenuMassTPfMET[iWenu])
                            
    # END OF CODE PART
    
    # ===================
    # once running loops is finished collect plots and put them in output file
    # ===================
    
    for channel in Zchannels:
        # stylise histos
        beautify( Leg1Pt[channel]      , ROOT.kRed    , "", "Z lepton pT", "events / 5 GeV")
        beautify( Leg2Pt[channel]      , ROOT.kBlue   , "", "Z lepton pT", "events / 5 GeV")
        beautify( GammaEt[channel]     , ROOT.kGreen  , "", "#gamma Et"  , "events / 5 GeV")
        beautify( ZMass[channel]       , ROOT.kCyan   , "", "Z mass"     , "events / 5 GeV")
        beautify( dRLeg1Gamma[channel] , ROOT.kBlack  , "", "dR leg1 gam", ""              )
        beautify( dRLeg2Gamma[channel] , ROOT.kBlack  , "", "dR leg2 gam", ""              )

        f_out.mkdir(channel)
        f_out.cd(channel)
        GammaEt[channel].Write()
        Leg1Pt[channel].Write()
        Leg2Pt[channel].Write()
        dRLeg1Gamma[channel].Write()
        dRLeg2Gamma[channel].Write()
        ZMass[channel].Write()

    for channel in Wchannels:
        # stylise histos
        beautify( LegPt[channel]      , ROOT.kRed    , "", "W lepton pT", "events / 5 GeV")
        beautify( MET[channel]        , ROOT.kBlue   , "", "pf MET"     , "events / 5 GeV")
        beautify( GammaEt[channel]    , ROOT.kGreen  , "", "#gamma Et"  , "events / 5 GeV")
        beautify( WtMass[channel]     , ROOT.kCyan   , "", "Wt mass"    , "events / 5 GeV")
        beautify( dRLegGamma[channel] , ROOT.kBlack  , "", "dR leg gam" , ""              )
        
        f_out.mkdir(channel)
        f_out.cd(channel)
        GammaEt[channel].Write()
        LegPt[channel].Write()
        MET[channel].Write()
        dRLegGamma[channel].Write()
        WtMass[channel].Write()
        
    f_out.Close()
    f.Close()
    print "\n=======================\nFile %s done\n================"%f_out.GetName()






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
