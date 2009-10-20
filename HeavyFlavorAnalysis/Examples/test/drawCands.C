{

TChain chain("T1");
chain.Add("test.root");

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

TH1D *h1 = new TH1D("h1", "531130", 60, 4.5, 6.0); 
TH1D *h2 = new TH1D("h2", "531", 60, 4.5, 6.0); 
TH1D *h3 = new TH1D("h3", "1300", 60, 4.5, 6.0); 

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  for (int ic = 0; ic < pEvent->nCands(); ++ic) {
    pCand = pEvent->getCand(ic);

    if (531130 == TMath::Abs(pCand->fType)) {
      h1->Fill(pCand->fMass); 
    }

    if (531 == TMath::Abs(pCand->fType)) {
      h2->Fill(pCand->fMass); 
    }

    if (1300 == TMath::Abs(pCand->fType)) {
      h3->Fill(pCand->fMass); 
    }

    
  }
  
}

zone(2,2); 
h1->Draw();

c0.cd(2);
h2->Draw();

c0.cd(3);
h3->SetMinimum(0); 
h3->Draw();

}
