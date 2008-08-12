{
#include <vector>
#define MMUON 0.10566

// -- Open file 
TChain chain("T1");
//chain.Add("scratch/ana/hfexample.root");
chain.Add("../python/hfexample.root");
//chain.Add("../python/hfexample10000.root");

Int_t nentries(0), nb(0);
Int_t iEvent(0), it(0);

// -- Set up for reading
TAna00Event *pEvent = new TAna00Event(0);
TAnaTrack *m1Track, *m2Track;
TGenCand  *g1, *g2, *mother, *grandmother, *daughter;
TAnaCand  *pCand;
TAnaVertex *pVtx;
TAnaTrack *pRecTrack;

chain.SetBranchAddress("TAna00Event", &pEvent);
nentries = chain.GetEntries();

TH1D *m100 = new TH1D("m100", "reco mass", 40, 4.0, 6.0); 
TH1D *m101 = new TH1D("m101", "reco mass", 40, 4.0, 6.0); m101->SetLineColor(kBlue);
TH1D *mJpsi = new TH1D("mJpsi", "reco J/psi mass", 40, 2.5, 3.5); 

cout << "Found " << nentries << " entries in the chain" << endl;

for (iEvent = 0; iEvent < nentries; iEvent++) {
  cout << "event " << iEvent << endl;
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  cout << "nCands = " << pEvent->nCands() << endl;

  for (int it = 0; it < pEvent->nCands(); ++it) {
    pCand = pEvent->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    m100->Fill(pCand->fMass); 
    double sig = pCand->fVtx.fD3d/pCand->fVtx.fD3dE; 
    if (sig > 10) {
      m101->Fill(pCand->fMass); 
      cout << "mass" << pCand->fMass << endl;
      
    }
  }

  std::vector<int> recMuonTracks;
  for (int it = 0; it < pEvent->nRecTracks(); ++it)
    {
      pRecTrack = pEvent->getRecTrack(it);
      if (TMath::Abs(pRecTrack->fMCID) == 13)
	{
	  //	  cout << "Found a muon!!!" << endl;

	  g1 = pEvent->getGenCand(pRecTrack->fGenIndex);
	  mother = pEvent->getGenCand(g1->fMom1);
	  //	  cout << "mother ID = " << mother->fID << endl;
	  if (mother->fID == 443) 
	    {
	      recMuonTracks->push_back(it);
	      grandmother = pEvent->getGenCand(mother->fMom1);
	      //   cout << "grandmother ID = " << grandmother->fID << endl;
	      daughter = pEvent->getGenCand(grandmother->fDau2);
	      //   cout << "daughter ID = " << daughter->fID << endl;
	    }      
	}
    }
  //  cout << "Number of muons from J/psi = " << recMuonTracks->size() << endl;

  if (recMuonTracks->size() == 2)
    {
    TLorentzVector m1, m2, k, psi, b; 
    m1.SetVectM(pEvent->getRecTrack(recMuonTracks->at(0))->fPlab,MMUON); 
    m2.SetVectM(pEvent->getRecTrack(recMuonTracks->at(1))->fPlab,MMUON); 
    psi = m1+m2;
    //    cout << "J/psi mass = " << psi.M() << endl;
    mJpsi->Fill(psi.M());

    }  
  for (int it = 0; it < pEvent->nRecTracks(); ++it)
    {
      pRecTrack = pEvent->getRecTrack(it);
      if (TMath::Abs(pRecTrack->fMCID) == 321)
	{
	  //	  cout << "ID = " << pRecTrack->fMCID << endl;

	  g2 = pEvent->getGenCand(pRecTrack->fGenIndex);
	  mother = pEvent->getGenCand(g2->fMom1);
	  if (recMuonTracks->size() == 2) cout << "mother ID = " << mother->fID << endl;
	  //	  if (mother->fID == 443) recMuonTracks->push_back(it);
	}      
    }
  


}

 m100->Draw("");
// m101->Draw("same");
//mJpsi->Draw();



}
