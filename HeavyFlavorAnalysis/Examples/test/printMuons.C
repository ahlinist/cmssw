{

TChain chain("T1");
chain.Add("scratch/ana/bs2mumu_famos.root");

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

  cout << "======================================================================" << endl;

  cout << "Found " << pEvent->nGenCands() << " generator cands in event " << iEvent << endl;
  for (int ig = 0; ig < pEvent->nGenCands(); ++ig) {
    pGen = pEvent->getGenCand(ig);
    if (531 == TMath::Abs(pGen->fID) || 13 == TMath::Abs(pGen->fID)) {
      pGen->dump();
      if ((13 == TMath::Abs(pGen->fID)) && (pGen->fP.Perp() < 2.5)) {
	cout << "++++++++++++" << endl;
      }
    }

  }

}


}
