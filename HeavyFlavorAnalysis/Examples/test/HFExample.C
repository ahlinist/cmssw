{

// -- Open file 
TChain chain("T1");
chain.Add("scratch/ana/hfexample.root");

Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

// -- Set up for reading
TAna00Event *pEvent = new TAna00Event(0);
TAnaTrack *m1Track, *m2Track;
TGenCand  *g1, *g2;
TAnaCand  *pCand;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna00Event", &pEvent);
nentries = chain.GetEntries();

TH1D *m100 = new TH1D("m100", "reco mass", 40, 4.0, 6.0); 
TH1D *m101 = new TH1D("m101", "reco mass", 40, 4.0, 6.0); m101->SetLineColor(kBlue);

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  cout << "event " << iEvent << endl;
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  for (int it = 0; it < pEvent->nCands(); ++it) {
    pCand = pEvent->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    m100->Fill(pCand->fMass); 
    double sig = pCand->fVtx.fD3d/pCand->fVtx.fD3dE; 
    if (sig > 10) {
      m101->Fill(pCand->fMass); 
    }
  }
  
}

m100->Draw("");
m101->Draw("same");

}
