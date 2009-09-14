{

TChain chain("T1");
chain.Add("testA01Event.root");

// -- Set up for reading
Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

TAna01Event *pEvent = new TAna01Event(0);
TAnaTrack *pTrack;
TGenCand  *pGen;
TAnaCand  *pCand;
TAnaMuon  *pMuon;
TTrgObj   *pTrgObj;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna01Event", &pEvent);
nentries = chain.GetEntries();

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  pEvent->dump();
  continue;
}

}
