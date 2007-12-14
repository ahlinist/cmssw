{

// -- Open file 
TChain chain("T1");
chain.Add("test.root");

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

TH1D *h0 = new TH1D("h0", "generator mass", 100, 0., 10.); 
TH1D *h1 = new TH1D("h1", "reco mass", 100, 0., 10.); 

cout << "Found " << nentries << " entries in the chain" << endl;

double mmu(0.105658);

for (iEvent = 0; iEvent < nentries; iEvent++) {
  cout << "event " << iEvent << endl;
  pEvent->Clear();
  nb += chain.GetEvent(iEvent); 

  // -- generator muons from Bs
  for (int i = 0; i < pEvent->nGenCands(); ++i) {
    g1 = pEvent->getGenCand(i);
    if (13 == TMath::Abs(g1->fID) && (531 == TMath::Abs(pEvent->getGenCand(g1->fMom1)->fID))) {
      for (int j = i+1; j < pEvent->nGenCands(); ++j) {
	g2 = pEvent->getGenCand(j);
	if (13 == TMath::Abs(g2->fID) && (531 == TMath::Abs(pEvent->getGenCand(g2->fMom1)->fID))) {
	  TLorentzVector gmm = g1->fP + g2->fP;
	  h0->Fill(gmm.M());
	  break;
	}
      }
      break;
    } 
  }

  // -- reco tracks
  for (int i = 0; i < pEvent->nRecTracks(); ++i) {
    m1Track = pEvent->getRecTrack(i); 
    if (TMath::Abs(m1Track->fMCID) == 13) {
      cout << " muon " << i << " pT = " << m1Track->fPlab.Perp() 
	   << " phi = " << m1Track->fPlab.Phi()
	   << " eta = " << m1Track->fPlab.PseudoRapidity()
	   << endl;
      g1 = pEvent->getGenCand(m1Track->fGenIndex);
      cout << "   gen " <<  g1->fNumber 
	   << " pT = "  << g1->fP.Perp() 
	   << " phi = " << g1->fP.Phi()
	   << " eta = " << g1->fP.PseudoRapidity()
	   << endl;
      for (int j = i+1; j < pEvent->nRecTracks(); ++j) {
	m2Track = pEvent->getRecTrack(j); 
	if (TMath::Abs(m2Track->fMCID) == 13) {
	  TLorentzVector mm = TLorentzVector(m1Track->fPlab, TMath::Sqrt(m1Track->fPlab.Mag2() + mmu*mmu)) 
	    + TLorentzVector(m2Track->fPlab, TMath::Sqrt(m2Track->fPlab.Mag2() + mmu*mmu));

	  cout << " muon " << j << " pT = " << m2Track->fPlab.Perp() 
	       << " phi = " << m2Track->fPlab.Phi()
	       << " eta = " << m2Track->fPlab.PseudoRapidity()
	       << endl;
	  g2 = pEvent->getGenCand(m2Track->fGenIndex);
	  cout << "   gen " <<  g2->fNumber 
	       << " pT = "  << g2->fP.Perp() 
	       << " phi = " << g2->fP.Phi()
	       << " eta = " << g2->fP.PseudoRapidity()
	       << endl;
	  cout << " MM = " << mm.M() << endl;
	  h1->Fill(mm.M());
	  break;
	}
      }
    }
  }

}

//h0->Draw();
h1->Draw("");

}
