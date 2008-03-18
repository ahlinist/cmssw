{

TChain chain("T1");
//chain.Add("scratch/ana/bs2mumu_famos.root");
//chain.Add("scratch/ana/bp2jpsikp_famos.root");
chain.Add("scratch/ana/test.root");

// -- Set up for reading
Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

TAna00Event *pEvent = new TAna00Event(0);
TAnaTrack *pTrack;
TGenCand  *pGen, *pDau;
TAnaCand  *pCand;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna00Event", &pEvent);
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
    if (pGen->fDau1 > 0 && pGen->fDau2 > 0) {
      for (int id = pGen->fDau1;  id <= pGen->fDau2; ++id) {
	if ((id > -1) && (id < pEvent->nGenCands())) {
	  pDau = pEvent->getGenCand(id); 
	  if (13 == TMath::Abs(pDau->fID)) {
	    ++muDau;
	  }
	}
      }
    }

    if ((13 == TMath::Abs(pGen->fID)) || (muDau > 0)) {
      pGen->dump();
      if ((13 == TMath::Abs(pGen->fID)) && (pGen->fP.Perp() < 2.5)) {
	cout << "++++++++++++" << endl;
      }
    }

  }

}


}
