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

chain.SetBranchAddress("TAna00Event", &pEvent);
nentries = chain.GetEntries();

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  pEvent->dump();
  continue;

  cout << "======================================================================" << endl;

  cout << "Found " << pEvent->nGenCands() << " generator cands in event" << endl;
  for (int ig = 0; ig < pEvent->nGenCands(); ++ig) {
    pGen = pEvent->getGenCand(ig);
    cout << "event " << iEvent 
 	 << " track " << ig 
 	 << " momentum = " << pGen->fP.Rho() << endl;
  }

  cout << "----------------------------------------------------------------------" << endl;
  cout << "Found " << pEvent->nRecTracks() << " rec tracks in event" << endl;
  for (it = 0; it < pEvent->nRecTracks(); ++it) {
    pTrack = pEvent->getRecTrack(it);
    cout << "event " << iEvent 
 	 << " track " << it 
 	 << " momentum = " << pTrack->fPlab.Mag() << endl;
  }

  cout << "----------------------------------------------------------------------" << endl;
  cout << "Found " << pEvent->nSigTracks() << " signal tracks in event" << endl;
  for (it = 0; it < pEvent->nSigTracks(); ++it) {
    pTrack = pEvent->getSigTrack(it);
    cout << "event " << iEvent 
 	 << " track " << it 
 	 << " momentum = " << pTrack->fPlab.Mag() << endl;
  }

  cout << "----------------------------------------------------------------------" << endl;
  cout << "Found " << pEvent->nCands() << " candidates in event" << endl;
  for (it = 0; it < pEvent->nCands(); ++it) {
    pCand = pEvent->getCand(it);
    cout << "event " << iEvent 
 	 << " track " << it 
 	 << " mass = " << pCand->fMass << endl;
    pCand->fVtx.dump();
  }


}

}
