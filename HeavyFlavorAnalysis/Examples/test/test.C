{

TChain chain("T1");
chain.Add("test.root");

// -- Set up for reading
Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

TAna00Event *pEvent = new TAna00Event(0);
TAnaTrack *pTrack;
TGenCand  *pGen;
TAnaCand  *pCand;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna00Event", &pEvent);
nentries = chain.GetEntries();

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

  cout << "Found " << pEvent->nGenCands() << " generator cands in event" << endl;
  int bsIdx(-1); 
  for (int ig = 0; ig < pEvent->nGenCands(); ++ig) {
    pGen = pEvent->getGenCand(ig);
    if (531 == TMath::Abs(pGen->fID)) {
      bsIdx = ig;
      cout << "event " << iEvent 
	   << " track " << ig 
	   << " id = " << pGen->fID
	   << " dau = " << pGen->fDau1 << ", " << pGen->fDau2
	   << " momentum = " << pGen->fP.Rho() << endl;
    }
    
    if (bsIdx == pGen->fMom1) {
      cout << "event " << iEvent 
	   << " track " << ig 
	   << " id = " << pGen->fID
	   << " mom = " << pGen->fMom1 
	   << " momentum = " << pGen->fP.Rho() << endl;
    }      
  }

}

}
