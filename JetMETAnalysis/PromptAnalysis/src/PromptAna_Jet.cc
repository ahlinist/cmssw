#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Jet.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_Jet::PromptAna_Jet(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , tracksinputTag(iConfig.getParameter<edm::InputTag>("TracksInputTag"))
  , jetCorrectionService  (iConfig.getParameter<std::string>  ("JetCorrectionService"  ))
  , jetTracksAtCaloinputTag  (iConfig.getParameter<edm::InputTag>("JetTracksAtCalo"))
  , jetTracksAtVertexinputTag  (iConfig.getParameter<edm::InputTag>("JetTracksAtVertex"))
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
  produces <std::vector<double> > ( prefix + "SigmaEta"  + suffix );
  produces <std::vector<double> > ( prefix + "SigmaPhi"  + suffix );
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
  //using the jet-tracks association at the calorimeter face
  produces <std::vector<int> > ( prefix + "NAssoTrksAtCaloAll"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtCalopx"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtCalopy"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtCalopz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksAtCaloHighPurity"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtCalopx"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtCalopy"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtCalopz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksAtCaloTight"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtCalopx"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtCalopy"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtCalopz"  + suffix );
  //using the jet-tracks association at the vertex
  produces <std::vector<int> > ( prefix + "NAssoTrksAtVtxAll"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtVtxpx"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtVtxpy"  + suffix );
  produces <std::vector<double> > ( prefix + "AllAssoTrksAtVtxpz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksAtVtxHighPurity"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtVtxpx"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtVtxpy"  + suffix );
  produces <std::vector<double> > ( prefix + "HighPurityAssoTrksAtVtxpz"  + suffix );
  produces <std::vector<int> > ( prefix + "NAssoTrksAtVtxTight"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtVtxpx"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtVtxpy"  + suffix );
  produces <std::vector<double> > ( prefix + "TightAssoTrksAtVtxpz"  + suffix );

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
  std::auto_ptr<std::vector<double> >  sigmaEta  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  sigmaPhi  ( new std::vector<double>()  ) ;
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
  std::auto_ptr<std::vector<int> >  nAssoTrksAtCaloAll  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtCalopx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtCalopy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtCalopz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAtCaloHighPurity  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtCalopx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtCalopy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtCalopz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAtCaloTight  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtCalopx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtCalopy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtCalopz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAtVtxAll  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtVtxpx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtVtxpy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  allAssoTrksAtVtxpz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAtVtxHighPurity  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtVtxpx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtVtxpy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  highPurityAssoTrksAtVtxpz  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nAssoTrksAtVtxTight  ( new std::vector<int>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtVtxpx  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtVtxpy  ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  tightAssoTrksAtVtxpz  ( new std::vector<double>()  ) ;

  //Get the Jets Collection
  edm::Handle<reco::CaloJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);
  //
	//Get Tracks collection
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(tracksinputTag, tracks);
  
  edm::Handle<reco::JetTracksAssociation::Container> jetTracksAtCalo;
  iEvent.getByLabel(jetTracksAtCaloinputTag,jetTracksAtCalo);

  edm::Handle<reco::JetTracksAssociation::Container> jetTracksAtVertex;
  iEvent.getByLabel(jetTracksAtVertexinputTag,jetTracksAtVertex);

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
    sigmaEta->push_back(sqrt(it->etaetaMoment()));
    sigmaPhi->push_back(sqrt(it->phiphiMoment()));
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
    int    tnAssoTrksAtCaloAll=0;
    int    tnAssoTrksAtCaloHighPurity=0;
    int    tnAssoTrksAtCaloTight=0;

    double tallAssoTrksAtCalopx=0.;
    double tallAssoTrksAtCalopy=0.;
    double tallAssoTrksAtCalopz=0.;
    double thighPurityAssoTrksAtCalopx=0.;
    double thighPurityAssoTrksAtCalopy=0.;
    double thighPurityAssoTrksAtCalopz=0.;
    double ttightAssoTrksAtCalopx=0.;
    double ttightAssoTrksAtCalopy=0.;
    double ttightAssoTrksAtCalopz=0.;

    //associated tracks
    int    tnAssoTrksAtVtxAll=0;
    int    tnAssoTrksAtVtxHighPurity=0;
    int    tnAssoTrksAtVtxTight=0;

    double tallAssoTrksAtVtxpx=0.;
    double tallAssoTrksAtVtxpy=0.;
    double tallAssoTrksAtVtxpz=0.;
    double thighPurityAssoTrksAtVtxpx=0.;
    double thighPurityAssoTrksAtVtxpy=0.;
    double thighPurityAssoTrksAtVtxpz=0.;
    double ttightAssoTrksAtVtxpx=0.;
    double ttightAssoTrksAtVtxpy=0.;
    double ttightAssoTrksAtVtxpz=0.;

    //use the jet-tracks-assciator at calorimeter face
    reco::TrackRefVector AssociatedTracksAtCalo = reco::JetTracksAssociation::getValue (*jetTracksAtCalo, (*it));
    //all tracks
    for(size_t t = 0; t < AssociatedTracksAtCalo.size(); ++t){
      //AssociatedTTracksAtCalo.push_back(theB->build(AssociatedTracksAtCalo[t]));
      tnAssoTrksAtCaloAll++;
      tallAssoTrksAtCalopx+=AssociatedTracksAtCalo[t]->px();
      tallAssoTrksAtCalopy+=AssociatedTracksAtCalo[t]->py();
      tallAssoTrksAtCalopz+=AssociatedTracksAtCalo[t]->pz();
      if(AssociatedTracksAtCalo[t]->quality(reco::Track::highPurity)) {
	tnAssoTrksAtCaloHighPurity++;
	thighPurityAssoTrksAtCalopx+=AssociatedTracksAtCalo[t]->px();
	thighPurityAssoTrksAtCalopy+=AssociatedTracksAtCalo[t]->py();
	thighPurityAssoTrksAtCalopz+=AssociatedTracksAtCalo[t]->pz();
      }
      if(AssociatedTracksAtCalo[t]->quality(reco::Track::tight)) {
	tnAssoTrksAtCaloTight++;
	ttightAssoTrksAtCalopx+=AssociatedTracksAtCalo[t]->px();
	ttightAssoTrksAtCalopy+=AssociatedTracksAtCalo[t]->py();
	ttightAssoTrksAtCalopz+=AssociatedTracksAtCalo[t]->pz();
      }
      
    }
    //use the jet-tracks associator at the vertex    
    reco::TrackRefVector AssociatedTracksAtVtx = reco::JetTracksAssociation::getValue (*jetTracksAtVertex, (*it));
      //all tracks
    for(size_t t = 0; t < AssociatedTracksAtVtx.size(); ++t){
      tnAssoTrksAtVtxAll++;
      tallAssoTrksAtVtxpx+=AssociatedTracksAtVtx[t]->px();
      tallAssoTrksAtVtxpy+=AssociatedTracksAtVtx[t]->py();
      tallAssoTrksAtVtxpz+=AssociatedTracksAtVtx[t]->pz();
      if(AssociatedTracksAtVtx[t]->quality(reco::Track::highPurity)) {
	tnAssoTrksAtVtxHighPurity++;
	thighPurityAssoTrksAtVtxpx+=AssociatedTracksAtVtx[t]->px();
	thighPurityAssoTrksAtVtxpy+=AssociatedTracksAtVtx[t]->py();
	thighPurityAssoTrksAtVtxpz+=AssociatedTracksAtVtx[t]->pz();
      }
      if(AssociatedTracksAtVtx[t]->quality(reco::Track::tight)) {
	tnAssoTrksAtVtxTight++;
	ttightAssoTrksAtVtxpx+=AssociatedTracksAtVtx[t]->px();
	  ttightAssoTrksAtVtxpy+=AssociatedTracksAtVtx[t]->py();
	  ttightAssoTrksAtVtxpz+=AssociatedTracksAtVtx[t]->pz();
      }
    }
    nAssoTrksAtCaloAll->push_back(tnAssoTrksAtCaloAll);
    allAssoTrksAtCalopx->push_back(tallAssoTrksAtCalopx);
    allAssoTrksAtCalopy->push_back(tallAssoTrksAtCalopy);
    allAssoTrksAtCalopz->push_back(tallAssoTrksAtCalopz);
    nAssoTrksAtCaloHighPurity->push_back(tnAssoTrksAtCaloHighPurity);
    highPurityAssoTrksAtCalopx->push_back(thighPurityAssoTrksAtCalopx);
    highPurityAssoTrksAtCalopy->push_back(thighPurityAssoTrksAtCalopy);
    highPurityAssoTrksAtCalopz->push_back(thighPurityAssoTrksAtCalopz);
    nAssoTrksAtCaloTight->push_back(tnAssoTrksAtCaloTight);
    tightAssoTrksAtCalopx->push_back(ttightAssoTrksAtCalopx);
    tightAssoTrksAtCalopy->push_back(ttightAssoTrksAtCalopy);
    tightAssoTrksAtCalopz->push_back(ttightAssoTrksAtCalopz);

    nAssoTrksAtVtxAll->push_back(tnAssoTrksAtVtxAll);
    allAssoTrksAtVtxpx->push_back(tallAssoTrksAtVtxpx);
    allAssoTrksAtVtxpy->push_back(tallAssoTrksAtVtxpy);
    allAssoTrksAtVtxpz->push_back(tallAssoTrksAtVtxpz);
    nAssoTrksAtVtxHighPurity->push_back(tnAssoTrksAtVtxHighPurity);
    highPurityAssoTrksAtVtxpx->push_back(thighPurityAssoTrksAtVtxpx);
    highPurityAssoTrksAtVtxpy->push_back(thighPurityAssoTrksAtVtxpy);
    highPurityAssoTrksAtVtxpz->push_back(thighPurityAssoTrksAtVtxpz);
    nAssoTrksAtVtxTight->push_back(tnAssoTrksAtVtxTight);
    tightAssoTrksAtVtxpx->push_back(ttightAssoTrksAtVtxpx);
    tightAssoTrksAtVtxpy->push_back(ttightAssoTrksAtVtxpy);
    tightAssoTrksAtVtxpz->push_back(ttightAssoTrksAtVtxpz);

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
  iEvent.put(sigmaEta,  prefix + "SigmaEta"    + suffix );
  iEvent.put(sigmaPhi,  prefix + "SigmaPhi"    + suffix );
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
  iEvent.put(nAssoTrksAtCaloAll,  prefix + "NAssoTrksAtCaloAll"  + suffix );
  iEvent.put(allAssoTrksAtCalopx,  prefix + "AllAssoTrksAtCalopx"  + suffix );
  iEvent.put(allAssoTrksAtCalopy,  prefix + "AllAssoTrksAtCalopy"  + suffix );
  iEvent.put(allAssoTrksAtCalopz,  prefix + "AllAssoTrksAtCalopz"  + suffix );
  iEvent.put(nAssoTrksAtCaloHighPurity,  prefix + "NAssoTrksAtCaloHighPurity"  + suffix );
  iEvent.put(highPurityAssoTrksAtCalopx,  prefix + "HighPurityAssoTrksAtCalopx"  + suffix );
  iEvent.put(highPurityAssoTrksAtCalopy,  prefix + "HighPurityAssoTrksAtCalopy"  + suffix );
  iEvent.put(highPurityAssoTrksAtCalopz,  prefix + "HighPurityAssoTrksAtCalopz"  + suffix );
  iEvent.put(nAssoTrksAtCaloTight,  prefix + "NAssoTrksAtCaloTight"  + suffix );
  iEvent.put(tightAssoTrksAtCalopx,  prefix + "TightAssoTrksAtCalopx"  + suffix );
  iEvent.put(tightAssoTrksAtCalopy,  prefix + "TightAssoTrksAtCalopy"  + suffix );
  iEvent.put(tightAssoTrksAtCalopz,  prefix + "TightAssoTrksAtCalopz"  + suffix );

  iEvent.put(nAssoTrksAtVtxAll,  prefix + "NAssoTrksAtVtxAll"  + suffix );
  iEvent.put(allAssoTrksAtVtxpx,  prefix + "AllAssoTrksAtVtxpx"  + suffix );
  iEvent.put(allAssoTrksAtVtxpy,  prefix + "AllAssoTrksAtVtxpy"  + suffix );
  iEvent.put(allAssoTrksAtVtxpz,  prefix + "AllAssoTrksAtVtxpz"  + suffix );
  iEvent.put(nAssoTrksAtVtxHighPurity,  prefix + "NAssoTrksAtVtxHighPurity"  + suffix );
  iEvent.put(highPurityAssoTrksAtVtxpx,  prefix + "HighPurityAssoTrksAtVtxpx"  + suffix );
  iEvent.put(highPurityAssoTrksAtVtxpy,  prefix + "HighPurityAssoTrksAtVtxpy"  + suffix );
  iEvent.put(highPurityAssoTrksAtVtxpz,  prefix + "HighPurityAssoTrksAtVtxpz"  + suffix );
  iEvent.put(nAssoTrksAtVtxTight,  prefix + "NAssoTrksAtVtxTight"  + suffix );
  iEvent.put(tightAssoTrksAtVtxpx,  prefix + "TightAssoTrksAtVtxpx"  + suffix );
  iEvent.put(tightAssoTrksAtVtxpy,  prefix + "TightAssoTrksAtVtxpy"  + suffix );
  iEvent.put(tightAssoTrksAtVtxpz,  prefix + "TightAssoTrksAtVtxpz"  + suffix );
}

