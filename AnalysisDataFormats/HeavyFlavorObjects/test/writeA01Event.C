{
  
  TFile f("testA01Event.root", "RECREATE");
  
  TTree *pTree = new TTree("T1","simple analysis tree");
  TAna01Event *pEvent = new TAna01Event(0);
  pTree->Branch("TAna00Event", "TAna01Event", &pEvent, 256000/8, 1);
  
  TGenCand  *pGen;

  TAnaTrack *pTrack;
  TAnaMuon  *pMuon;
  TTrgObj   *pTO;

  TAnaCand  *pCand;
  TAnaVertex *pVtx;
  
  int n(0);
  // -- Generate 100 'events' 
  int NEVENT(100);
  for (int i = 0; i < NEVENT; ++i) {
    pEvent->Clear();
    pEvent->fRunNumber = 123;
    pEvent->fEventNumber = 1000+i;

    pVtx = new TAnaVertex();
    pVtx->setInfo(12., 10, 0.87, 1, 2);
    pVtx->fPoint.SetXYZ(5.235*gRandom->Rndm(), 
			-5.235*gRandom->Rndm(),
			gRandom->Rndm()
			);
    
    pEvent->fPrimaryVertex = *pVtx;

   
    n = static_cast<int>(20*gRandom->Rndm());       // each event has a random number of generator candidates
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
    
    
    n = static_cast<int>(15*gRandom->Rndm());       // each event has a random number of tracks
    for (int j = 0; j < n; ++j) {
      pTrack = pEvent->addRecTrack();
      pTrack->fQ = -1;
      pTrack->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
    }

    int a[2] = {n-1, n-2};
    
    n = 2;       // each event has 2 muons
    for (int j = 0; j < n; ++j) {
      pMuon = pEvent->addMuon();
      pMuon->fQ = (j%2 == 0? -1: +1);
      pMuon->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
      pMuon->fIndex = a[j];
      pMuon->fMuonChi2 = gRandom->Gaus(2., 0.10);
      pMuon->fMuonR    = 500. + 50*gRandom->Gaus(2., 0.10);
      pMuon->fMuonZ    = 300. + 20*gRandom->Gaus(2., 0.10);
    }

    n = 4;       // each event has 4 trigger objects
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
    
    n = 1;       // each event has 1 B Candidate
    for (int j = 0; j < n; ++j) {
      pCand = pEvent->addCand();
      pCand->fMass = 5.235*gRandom->Rndm();
      pCand->fPlab.SetXYZ(10.*gRandom->Gaus(2., 0.10), 10.*gRandom->Gaus(2., 0.12), 10.*gRandom->Gaus(2., 0.20));
      
      pVtx = new TAnaVertex();
      pVtx->setInfo(12., 10, 0.87, 1, 2);
      pVtx->fPoint.SetXYZ(5.235*gRandom->Rndm(), 
			  -5.235*gRandom->Rndm(),
			  gRandom->Rndm()
			  );
      pVtx->addTrack(a[j]);
      
      pCand->fVtx = *pVtx;
      
    }
    
    
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    pEvent->dump();  
    
    pTree->Fill();
  }

  pTree->Write();
  f.Close();
}
