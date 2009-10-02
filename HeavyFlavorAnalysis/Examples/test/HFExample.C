{
#include <vector>
#define MMUON 0.10566

// -- Open file 
TChain chain("T1");
chain.Add("msel5.root");

Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

// -- Set up for reading
TAna01Event *pEvent = new TAna01Event(0);
TAnaTrack *m1Track, *m2Track;
TAnaCand  *pCand;
TAnaVertex *pVtx;

chain.SetBranchAddress("TAna01Event", &pEvent);
nentries = chain.GetEntries();

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  cout << "event " << iEvent << endl;
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 
  
  pEvent->dumpGenBlock(); 

}

}
