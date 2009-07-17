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

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "======================================================================" << endl;
  cout << "Found " << pEvent->nGenCands() << " generator cands in event " << iEvent << endl;

  int muDau(0);
  for (int ig = 0; ig < pEvent->nGenCands(); ++ig) {
    pGen = pEvent->getGenCand(ig);
    muDau = 0; 
    if (13 == TMath::Abs(pGen->fID)) {
      pGen->dump(); 
    }
    if ((521 == TMath::Abs(pGen->fID)) ||
	(531 == TMath::Abs(pGen->fID)) ||
	(511 == TMath::Abs(pGen->fID)) ||
	(5122== TMath::Abs(pGen->fID))) {
      pGen->dump();
      if (pGen->fDau1 > 0 && pGen->fDau2 > 0) {
	for (int id = pGen->fDau1;  id <= pGen->fDau2; ++id) {
	  if ((id > -1) && (id < pEvent->nGenCands())) {
	    pDau = pEvent->getGenCand(id); 
	    pDau->dump();
	    if (13 == TMath::Abs(pDau->fID)) {
	      ++muDau;
	    }
	  }
	}
      }
    }
    
  }
  
}


}
