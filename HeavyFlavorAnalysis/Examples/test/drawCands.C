{

TChain chain("T1");
chain.Add("/afs/cern.ch/user/u/ursl/scratch0/hfa-ExpressPhysics-123596-v1/*");

// -- Set up for reading
Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

TAna01Event *pEvent = new TAna01Event(0);
TAnaTrack *pTrack;
TGenCand  *pGen, *pDau;
TAnaCand  *pCand;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna01Event", &pEvent);
nentries = chain.GetEntries();

TH1D *h1  = new TH1D("h1", "531130", 60, 4.5, 6.0); 
TH1D *h2  = new TH1D("h2", "531", 60, 4.5, 6.0); 
TH1D *h3a = new TH1D("h3a", "1300", 60, 2.0, 11.0); 
TH1D *h3b = new TH1D("h3b", "1300", 60, 2.7, 3.3); 
TH1D *h4  = new TH1D("h4", "421", 60, 1.6, 1.9); 
TH1D *h5  = new TH1D("h5", "413", 60, 1.9, 2.2); 

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "Event " << iEvent << " ncands = " << pEvent->nCands() << endl;
  if (pEvent->nCands() > 20) continue; 

  for (int ic = 0; ic < pEvent->nCands(); ++ic) {
    pCand = pEvent->getCand(ic);

    cout << "  candidate " << pCand->fType << endl;

    if (100443 == TMath::Abs(pCand->fType)) {
      h1->Fill(pCand->fMass); 
    }

    if (531 == TMath::Abs(pCand->fType)) {
      h2->Fill(pCand->fMass); 
    }

    if (1300 == TMath::Abs(pCand->fType)) {
      h3a->Fill(pCand->fMass); 
      h3b->Fill(pCand->fMass); 
    }

    if (421 == TMath::Abs(pCand->fType)) {
      h4->Fill(pCand->fMass); 
    }

    if (413 == TMath::Abs(pCand->fType)) {
      h5->Fill(pCand->fMass); 
    }

    
  }
  
}

zone(3,2); 
h1->Draw();

c0.cd(2);
h2->Draw();

c0.cd(3);
h3a->SetMinimum(0); 
h3a->Draw();

c0.cd(6);
h3b->SetMinimum(0); 
h3b->Draw();

c0.cd(4);
h4->SetMinimum(0); 
h4->Draw();

c0.cd(5);
h5->SetMinimum(0); 
h5->Draw();

}
