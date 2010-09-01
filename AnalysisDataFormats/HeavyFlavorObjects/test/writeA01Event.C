{
  
  TFile f("testA01Event.root", "RECREATE");
  
  TTree *pTree = new TTree("T1","simple analysis tree");
  TAna01Event *pEvent = new TAna01Event(0);
  pTree->Branch("TAna01Event", "TAna01Event", &pEvent, 256000/8, 1);
  
  TGenCand  *pGen;

  TAnaTrack *pTrack;
  TAnaMuon  *pMuon;
  TTrgObj   *pTO;

  TAnaCand  *pCand;
  TAnaVertex *pVtx;
  
  int n(0);
  int w1, w2, w3, w4; 
  // -- Generate 100 'events' 
  int NEVENT(100);
  for (int i = 0; i < NEVENT; ++i) {
    pEvent->Clear();
    pEvent->fRunNumber = 123;
    pEvent->fEventNumber = 1000+i;

    // -- Trigger
    w1 = 0; 
    w2 = 0; 
    w3 = 0; 
    w4 = 0; 
    for (int j = 0; j < 32; ++j) {
      pEvent->fL1TNames[j]   = TString(Form("L1TsingleMu%d", j));
      pEvent->fL1TNames[32+j] = TString(Form("L1TeGamma%d", j));
      pEvent->fL1TNames[64+j] = TString(Form("L1TjetMET%d", j));
      pEvent->fL1TNames[96+j] = TString(Form("L1TdoubleMu%d", j));
      if (gRandom->Rndm() > 0.5) w1 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w2 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w3 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w4 |= (0x1<<j); 
    }

    w1 = 0; 
    w2 = 0; 
    w3 = 0; 
    w4 = 0; 
    for (int j = 0; j < 32; ++j) {
      pEvent->fHLTNames[j]   = TString(Form("HLTsingleMu%d", j));
      pEvent->fHLTNames[32+j] = TString(Form("HLTeGamma%d", j));
      pEvent->fHLTNames[64+j] = TString(Form("HLTjetMET%d", j));
      pEvent->fHLTNames[96+j] = TString(Form("HLTdoubleMu%d", j));
      if (gRandom->Rndm() > 0.5) w1 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w2 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w3 |= (0x1<<j); 
      if (gRandom->Rndm() > 0.5) w4 |= (0x1<<j); 
    }
    
    // -- Vertex
    for (int j = 0; j < 2; ++j) {
      pVtx = pEvent->addPV();
      pVtx->setInfo(12., 10, 0.87, 1, 2);
      pVtx->fPoint.SetXYZ(5.235*gRandom->Rndm(), 
			  -5.235*gRandom->Rndm(),
			  gRandom->Rndm()
			  );
    }
   
    // -- Generator cands
    n = static_cast<int>(20*gRandom->Rndm());       
    for (int j = 0; j < n; ++j) {
      pGen = pEvent->addGenCand();
      pGen->fID = 321;
      pGen->fNumber = j;
      pGen->fMom1 = n+2;
      pGen->fMom2 = n-1;
      pGen->fDau1 = n+3;
      pGen->fDau2 = n+4;

      pGen->fP.SetXYZM(10.*gRandom->Gaus(2., 0.10), 
		      10.*gRandom->Gaus(2., 0.12), 
		      10.*gRandom->Gaus(2., 0.20), 
		      10.*gRandom->Gaus(2., 0.20));

      pGen->fV.SetXYZ(10.*gRandom->Gaus(2., 0.10), 
		      10.*gRandom->Gaus(2., 0.12), 
		      10.*gRandom->Gaus(2., 0.20));
    }
    

    // -- RecTracks
    n = static_cast<int>(15*gRandom->Rndm());       
    for (int j = 0; j < n; ++j) {
      pTrack = pEvent->addRecTrack();
      pTrack->fQ = -1;
      pTrack->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
      for (int k = 0; k < 20; ++k) {
	pTrack->fHitPattern[k] = gRandom->Rndm() > 0.5? 1:0;
      }
    }

    int a[2] = {n-1, n-2};
    
    // -- Muons
    n = 2;     
    for (int j = 0; j < n; ++j) {
      pMuon = pEvent->addMuon();
      pMuon->fQ = (j%2 == 0? -1: +1);
      pMuon->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
      pMuon->fIndex = a[j];
      pMuon->fMuonChi2 = gRandom->Gaus(2., 0.10);
      pMuon->fMuonR    = 500. + 50*gRandom->Gaus(2., 0.10);
      pMuon->fMuonZ    = 300. + 20*gRandom->Gaus(2., 0.10);
    }

    // -- trigger objects
    n = 4;       
    for (int j = 0; j < n; ++j) {
      pTO = pEvent->addTrgObj();
      pTO->fID = (j%2 == 0? -13: +13);
      pTO->fP.SetXYZM(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20), 0.106);
      if (j%4 == 0) {
	pTO->fLabel = TString("hltL1MuOpenL1Filtered0");
      } else if (j%4 == 1) {
	pTO->fLabel = TString("hltL1sL1SingleMuOpenL1SingleMu0");
      } else if (j%4 == 2) {
	pTO->fLabel = TString("hltL1sL1DoubleMuOpen");
      } else if (j%4 == 3) {
	pTO->fLabel = TString("hltL1sL1DoubleMuOpen");
      }
    }
    
    // -- Cands
    n = 3;     
    for (int j = 0; j < n; ++j) {
      pCand = pEvent->addCand();
      pCand->fMass = 5.237;
      if (0 == j) pCand->fMass = 5.235 + gRandom->Gaus(0., 0.1);
      if (1 == j) pCand->fMass = 3.097 + gRandom->Gaus(0., 0.05);
      pCand->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
      
      TAnaVertex Vtx; 
      Vtx.setInfo(12., 10, 0.87, 1, 2);
      Vtx.fPoint.SetXYZ(5.235*gRandom->Rndm(), 
			-5.235*gRandom->Rndm(),
			gRandom->Rndm()
			);
      Vtx.addTrack(a[0]);
      Vtx.addTrack(a[1]);

      pCand->fVtx = Vtx;
    }
    
    
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    //    pEvent->dump();  
    
    pTree->Fill();
  }

  pTree->Write();
  f.Close();
}
