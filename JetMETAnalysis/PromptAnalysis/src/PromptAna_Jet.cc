#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Jet.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_Jet::PromptAna_Jet(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , tracksinputTag(iConfig.getParameter<edm::InputTag>("TracksInputTag"))
  , jetCorrectionService  (iConfig.getParameter<std::string>  ("JetCorrectionService"  ))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  // Jet ID helper
  jetIDHelper = reco::helper::JetIDHelper(iConfig.getParameter<edm::ParameterSet>("jetID")  );

  produces <std::vector<int> > ( prefix + "NJets"  + suffix );
  produces <std::vector<int> > ( prefix + "NcleanedJets"  + suffix );
  produces <std::vector<double> > ( prefix + "pT"  + suffix );
  produces <std::vector<double> > ( prefix + "Energy"  + suffix );
  produces <std::vector<int> > ( prefix + "NConstituents"  + suffix );
  produces <std::vector<double> > ( prefix + "Eta"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfHPD"  + suffix );
  produces <std::vector<int> >    ( prefix + "JIDn90Hits"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfRBX"  + suffix );
  //produces <std::vector<double> > ( prefix + "JIDfSubDet1"  + suffix );
  //produces <std::vector<double> > ( prefix + "JIDfSubDet2"  + suffix );
  //produces <std::vector<double> > ( prefix + "JIDfSubDet3"  + suffix );
  //produces <std::vector<double> > ( prefix + "JIDfSubDet4"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDresEMF"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "EnergyFractionHadronic"  + suffix );
  produces <std::vector<double> > ( prefix + "TowersArea"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInEmTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInHadTowers"  + suffix );
  //produces <std::vector<double> > ( prefix + "HadEnergyInHB"  + suffix );
  //produces <std::vector<double> > ( prefix + "HadEnergyInHE"  + suffix );
  //produces <std::vector<double> > ( prefix + "HadEnergyInHO"  + suffix );
  //produces <std::vector<double> > ( prefix + "HadEnergyInHF"  + suffix );
  //produces <std::vector<double> > ( prefix + "EmEnergyInEB"  + suffix );
  //produces <std::vector<double> > ( prefix + "EmEnergyInEE"  + suffix );
  //produces <std::vector<double> > ( prefix + "EmEnergyInHF"  + suffix );
  produces <std::vector<int> > ( prefix + "N60"  + suffix ); 
  produces <std::vector<int> > ( prefix + "N90"  + suffix ); 
  produces <std::vector<double> > ( prefix + "scaleL2L3"  + suffix ); 
  //produces <std::vector<double> > ( prefix + "pTFirst"  + suffix );
  //produces <std::vector<double> > ( prefix + "EnergyFirst"  + suffix );
  //produces <std::vector<double> > ( prefix + "EtaFirst"  + suffix );
  //produces <std::vector<double> > ( prefix + "PhiFirst"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksAll"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrkspx"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrkspy"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrkspz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksLoose"  + suffix );
  produces <std::vector<double> > ( prefix + "LooseAssoTrkspx"  + suffix );
  produces <std::vector<double> > ( prefix + "LooseAssoTrkspy"  + suffix );
  produces <std::vector<double> > ( prefix + "LooseAssoTrkspz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksTight"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrkspx"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrkspy"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrkspz"  + suffix );

}

void PromptAna_Jet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  std::auto_ptr<std::vector<int> >  njets  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >  ncleanedjets  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  pt  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energy  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nconstituents  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  eta  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  fHPD  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  fn90Hits  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  fRBX  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  fSubDet1  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  fSubDet2  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  fSubDet3  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  fSubDet4  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  resEMF  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energyFractionHadronic ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  towersArea   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInEmTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInHadTowers   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  hadEnergyInHB   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  hadEnergyInHE   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  hadEnergyInHO   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  hadEnergyInHF   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  emEnergyInEB   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  emEnergyInEE   ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  emEnergyInHF   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  n60   ( new std::vector<int>()  ); 
  std::auto_ptr<std::vector<int> >  n90   ( new std::vector<int>()  ); 
  std::auto_ptr<std::vector<double> > scaleL2L3   ( new std::vector<double>()  ); 
  //std::auto_ptr<std::vector<double> >  ptfirst  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  energyfirst  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  etafirst  ( new std::vector<double>()  ) ;
  //std::auto_ptr<std::vector<double> >  phifirst  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAll  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrkspx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrkspy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrkspz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksLoose  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  looseAssoTrkspx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  looseAssoTrkspy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  looseAssoTrkspz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksTight  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrkspx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrkspy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrkspz  ( new std::vector<double>()  ) ;

  //Get the Jets Collection
  edm::Handle<reco::CaloJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);
  //
	//Get Tracks collection
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(tracksinputTag, tracks);

  // Get Transient Track Builder
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
  
  /*  edm::Handle<reco::VertexCollection> vertices;   
  iEvent.getByLabel(primaryVertexTag, vertices);
  */
  //

  int clj=0;
  /*  //get the primary vertex 
  const reco::Vertex PrimaryVertex = vertices->front();
  */
  //Fill the variables
  for(reco::CaloJetCollection::const_iterator it = jetcollection->begin(); it != jetcollection->end() ; ++it ){
    
    if(it==jetcollection->begin()){
      njets->push_back(jetcollection->size());
      //ptfirst->push_back(it->pt());
      //energyfirst->push_back(it->energy());
      //etafirst->push_back(it->eta());
      //phifirst->push_back(it->phi());
    }
    jetIDHelper.calculate(iEvent, *it);
    pt->push_back(it->pt());
    energy->push_back(it->energy());
    nconstituents->push_back(it->getCaloConstituents().size());
    eta->push_back(it->eta());
    phi->push_back(it->phi());
    fHPD->push_back(jetIDHelper.fHPD() );
    fRBX->push_back(jetIDHelper.fRBX() );
    fn90Hits->push_back(jetIDHelper.n90Hits() );
    //fSubDet1->push_back(jetIDHelper.fSubDetector1() );
    //fSubDet2->push_back(jetIDHelper.fSubDetector2() );
    //fSubDet3->push_back(jetIDHelper.fSubDetector3() );
    //fSubDet4->push_back(jetIDHelper.fSubDetector4() );
    resEMF->push_back(jetIDHelper.restrictedEMF() );
    //ok for the moment i add this by hand, just to be sure
    if(jetIDHelper.fHPD()<0.98 && jetIDHelper.n90Hits()>=2 && jetIDHelper.restrictedEMF()>0.01) clj++;
    emEnergyFraction->push_back(it->emEnergyFraction());
    energyFractionHadronic->push_back(it->energyFractionHadronic());
    towersArea->push_back(it->towersArea());
    maxEInEmTowers->push_back(it->maxEInEmTowers());
    maxEInHadTowers->push_back(it->maxEInHadTowers());
    //hadEnergyInHB->push_back(it->hadEnergyInHB());
    //hadEnergyInHE->push_back(it->hadEnergyInHE());
    //hadEnergyInHO->push_back(it->hadEnergyInHO());
    //hadEnergyInHF->push_back(it->hadEnergyInHF());
    //emEnergyInEB->push_back(it->emEnergyInEB());
    //emEnergyInEE->push_back(it->emEnergyInEE());
    //emEnergyInHF->push_back(it->emEnergyInHF());
    n60->push_back(it->n60()); 
    n90->push_back(it->n90()); 

    const JetCorrector* corrector = JetCorrector::getJetCorrector (jetCorrectionService,iSetup);
    scaleL2L3->push_back( corrector->correction(it->p4()) );
    //std::cout << corrector->correction(it->p4()) << std::endl;

    //associated tracks
    int    tnAssoTrksAll=0;
    int    tnAssoTrksLoose=0;
    int    tnAssoTrksTight=0;

    double tallAssoTrkspx=0.;
    double tallAssoTrkspy=0.;
    double tallAssoTrkspz=0.;
    double tlooseAssoTrkspx=0.;
    double tlooseAssoTrkspy=0.;
    double tlooseAssoTrkspz=0.;
    double ttightAssoTrkspx=0.;
    double ttightAssoTrkspy=0.;
    double ttightAssoTrkspz=0.;

    if(fabs(it->eta())<2.9){//when the cone of dR=0.5 around the jet is (at least partially) inside the tracker acceptance
    std::vector<const reco::Track*> AssociatedTracks = FindAssociatedTracks(&(*it), tracks.product());
    std::vector<reco::TransientTrack> AssociatedTTracks;
    //    for (reco::TrackRefVector::iterator trk = it->associatedTracks().begin(); trk != it->associatedTracks().end(); ++trk) {
      //(*trk)->pt()>minPttrk && (*trk)->pt()<maxPttrk && fabs((*trk)->dxy(PrimaryVertex.position()))<maxD0trk && (*trk)->chi2()<maxChi2trk){
	  //all tracks
    for(size_t t = 0; t < AssociatedTracks.size(); ++t){
      AssociatedTTracks.push_back(theB->build(AssociatedTracks[t]));
	  tnAssoTrksAll++;
	  tallAssoTrkspx+=AssociatedTracks[t]->px();
	  tallAssoTrkspy+=AssociatedTracks[t]->py();
	  tallAssoTrkspz+=AssociatedTracks[t]->pz();
	  if(AssociatedTracks[t]->quality(reco::Track::loose)) {
	    tnAssoTrksLoose++;
	  tlooseAssoTrkspx+=AssociatedTracks[t]->px();
	  tlooseAssoTrkspy+=AssociatedTracks[t]->py();
	  tlooseAssoTrkspz+=AssociatedTracks[t]->pz();
	  }
	  if(AssociatedTracks[t]->quality(reco::Track::tight)) {
	    tnAssoTrksTight++;
	  ttightAssoTrkspx+=AssociatedTracks[t]->px();
	  ttightAssoTrkspy+=AssociatedTracks[t]->py();
	  ttightAssoTrkspz+=AssociatedTracks[t]->pz();
	  }
	  //	}
    }
    }
    nAssoTrksAll->push_back(tnAssoTrksAll);
    allAssoTrkspx->push_back(tallAssoTrkspx);
    allAssoTrkspy->push_back(tallAssoTrkspy);
    allAssoTrkspz->push_back(tallAssoTrkspz);
    nAssoTrksLoose->push_back(tnAssoTrksLoose);
    looseAssoTrkspx->push_back(tlooseAssoTrkspx);
    looseAssoTrkspy->push_back(tlooseAssoTrkspy);
    looseAssoTrkspz->push_back(tlooseAssoTrkspz);
    nAssoTrksTight->push_back(tnAssoTrksTight);
    tightAssoTrkspx->push_back(ttightAssoTrkspx);
    tightAssoTrkspy->push_back(ttightAssoTrkspy);
    tightAssoTrkspz->push_back(ttightAssoTrkspz);

      //
  }
  ncleanedjets->push_back(clj);

  iEvent.put( njets,  prefix + "NJets"  + suffix );
  iEvent.put( ncleanedjets,  prefix + "NcleanedJets"  + suffix );
  iEvent.put( pt,  prefix + "pT"  + suffix );
  iEvent.put( energy,  prefix + "Energy"  + suffix );
  iEvent.put( nconstituents,  prefix + "NConstituents"  + suffix );
  iEvent.put( eta,  prefix + "Eta"  + suffix );
  iEvent.put( phi,  prefix + "Phi"  + suffix );
  iEvent.put( fHPD,  prefix + "JIDfHPD"    + suffix );
  iEvent.put( fn90Hits,  prefix + "JIDn90Hits"   + suffix );
  iEvent.put( fRBX,  prefix + "JIDfRBX"    + suffix );
  //iEvent.put( fSubDet1,  prefix + "JIDfSubDet1"  + suffix );
  //iEvent.put( fSubDet2,  prefix + "JIDfSubDet2"  + suffix );  
  //iEvent.put( fSubDet3,  prefix + "JIDfSubDet3"  + suffix );
  //iEvent.put( fSubDet4,  prefix + "JIDfSubDet4"  + suffix );
  iEvent.put( resEMF,  prefix + "JIDresEMF"    + suffix );
  iEvent.put( emEnergyFraction,  prefix + "EmEnergyFraction"  + suffix );
  iEvent.put( energyFractionHadronic,  prefix + "EnergyFractionHadronic"  + suffix );
  iEvent.put( towersArea,  prefix + "TowersArea"  + suffix );
  iEvent.put( maxEInEmTowers,  prefix + "MaxEInEmTowers"  + suffix );
  iEvent.put( maxEInHadTowers,  prefix + "MaxEInHadTowers"  + suffix );
  //iEvent.put( hadEnergyInHB,  prefix + "HadEnergyInHB"  + suffix );
  //iEvent.put( hadEnergyInHE,  prefix + "HadEnergyInHE"  + suffix );
  //iEvent.put( hadEnergyInHO,  prefix + "HadEnergyInHO"  + suffix );
  //iEvent.put( hadEnergyInHF,  prefix + "HadEnergyInHF"  + suffix );
  //iEvent.put( emEnergyInEB,  prefix + "EmEnergyInEB"  + suffix );
  //iEvent.put( emEnergyInEE,  prefix + "EmEnergyInEE"  + suffix );
  //iEvent.put( emEnergyInHF,  prefix + "EmEnergyInHF"  + suffix );
  iEvent.put( n60,  prefix + "N60"  + suffix ); 
  iEvent.put( n90,  prefix + "N90"  + suffix );
  iEvent.put( scaleL2L3,  prefix + "scaleL2L3"  + suffix );
  //iEvent.put( ptfirst,  prefix + "pTFirst"  + suffix );
  //iEvent.put( energyfirst,  prefix + "EnergyFirst"  + suffix );
  //iEvent.put( etafirst,  prefix + "EtaFirst"  + suffix );
  //iEvent.put( phifirst,  prefix + "PhiFirst"  + suffix );
  iEvent.put(nAssoTrksAll,  prefix + "NAssoTrksAll"  + suffix );
  iEvent.put(allAssoTrkspx,  prefix + "AllAssoTrkspx"  + suffix );
  iEvent.put(allAssoTrkspy,  prefix + "AllAssoTrkspy"  + suffix );
  iEvent.put(allAssoTrkspz,  prefix + "AllAssoTrkspz"  + suffix );
  iEvent.put(nAssoTrksLoose,  prefix + "NAssoTrksLoose"  + suffix );
  iEvent.put(looseAssoTrkspx,  prefix + "LooseAssoTrkspx"  + suffix );
  iEvent.put(looseAssoTrkspy,  prefix + "LooseAssoTrkspy"  + suffix );
  iEvent.put(looseAssoTrkspz,  prefix + "LooseAssoTrkspz"  + suffix );
  iEvent.put(nAssoTrksTight,  prefix + "NAssoTrksTight"  + suffix );
  iEvent.put(tightAssoTrkspx,  prefix + "TightAssoTrkspx"  + suffix );
  iEvent.put(tightAssoTrkspy,  prefix + "TightAssoTrkspy"  + suffix );
  iEvent.put(tightAssoTrkspz,  prefix + "TightAssoTrkspz"  + suffix );

}

std::vector<const reco::Track*> PromptAna_Jet::FindAssociatedTracks(const reco::CaloJet *jet, const reco::TrackCollection *tracks){
//returns a list of tracks associated to a given jet
	std::vector<const reco::Track*> AssociatedTracks;
	double jeteta=jet->eta();
	double jetphi=jet->phi();
	for(reco::TrackCollection::const_iterator itk = tracks->begin(); itk!=tracks->end(); ++itk){
		if(sqrt(pow(itk->eta()-jeteta,2) + pow(itk->phi()-jetphi,2))<0.5) {
			const reco::Track *t = new reco::Track(*itk);
			AssociatedTracks.push_back(t);
		}
	}
	return AssociatedTracks;
}
