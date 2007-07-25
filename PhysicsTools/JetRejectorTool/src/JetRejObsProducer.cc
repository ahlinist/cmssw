#include "PhysicsTools/JetRejectorTool/interface/JetRejObsProducer.h"


//
// constructor
//

JetRejObsProducer::JetRejObsProducer(const edm::ParameterSet& iConfig){
  selgenjetsrc_ = iConfig.getParameter<edm::InputTag>( "selgenjetsrc" );  
  selcalojetsrc_ = iConfig.getParameter<edm::InputTag>( "selcalojetsrc" );  
  seljettagsrc_= iConfig.getParameter<edm::InputTag>( "seljettagsrc" ) ; 
  jetNumNoRad_   = iConfig.getParameter<double > ("jetNumNoRad");
  leptonIsolation_  = iConfig.getParameter< double > ("leptonIsolation");
  DeltaRcut_   = iConfig.getParameter< double > ("DeltaRcut");
  switchSignalDefinition = iConfig.getParameter< unsigned int > ("signalDefinition");

  //  cout << "switchSignalDefinition = " << switchSignalDefinition << endl;

 // define Observables
   myJetRejLRObs   = new JetRejLRObservables();

  produces<  vector< JetRejObs > >();  
}

// destructor
JetRejObsProducer::~JetRejObsProducer()
{
  delete myJetRejLRObs;
}



void JetRejObsProducer::produce( edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  Handle<CandidateCollection > genEvt; //class containing a vector of generated Monte-Carlo particles
  iEvent.getByLabel ("genParticleCandidates", genEvt);

  Handle<vector<reco::Vertex> > primvertex;
  iEvent.getByLabel("pixelVertices", primvertex); 

  Handle<vector<reco::GenJet> > genjets;
  if (switchSignalDefinition == 2) iEvent.getByLabel( selgenjetsrc_, genjets); // for the moment switch=2 doesn't work because GenJets are not selected!
  
  Handle<vector<reco::CaloJet> > jets;
  iEvent.getByLabel( selcalojetsrc_, jets); 
  
  Handle<vector<reco::JetTag> > jettags;	
  iEvent.getByLabel (seljettagsrc_, jettags);
   
  // ---------------Generator Level-----------------
  std::vector<const reco::Candidate*> leptons, quarks;
  leptons.clear();
  quarks.clear();

  //semileptonic decays with a muon and etaGen of partons less 2.4

  for(size_t igp = 0; igp < genEvt->size(); ++igp){
    const reco::Candidate & cand = (*genEvt)[igp];
    int st = reco::status(cand);
    if(st==3){
      if (switchSignalDefinition == 1) {
	//	cout<< "@@@@ ===> switch =1!!!"<< endl;
	if(abs((*genEvt)[igp].pdgId())<6 && (*genEvt)[igp].mother()->pdgId() != 2212 &&(*genEvt)[igp].pdgId()!=(*genEvt)[igp].mother()->pdgId()){// without isr
 // if(abs((*genEvt)[igp].pdgId())<6 &&(*genEvt)[igp].pdgId()!=(*genEvt)[igp].mother()->pdgId()){ //with isr
	  quarks.push_back(& cand);
	}
      }
      if(abs((*genEvt)[igp].pdgId()==11) || abs((*genEvt)[igp].pdgId()==13)) leptons.push_back(&(*genEvt)[igp]);
    }
  }

  if (switchSignalDefinition > 1) { // in this case, the "quarks" are GenJets or PartonJets
    vector<reco::GenJet> genjet;
    if (switchSignalDefinition == 2) {
      // cout<< "@@@@ ===> switch =2!!!"<< endl;
      genjet= (*genjets);  
      //    } else if (switchSignalDefinition == 3) {  // to be uncommented when available (CMSSW > 1_5_0)
      //      genjet= (*partonjets);  
    } else {
      cout << " !!ERROR!! no such option!" << endl;
      return;
    }
    for(size_t i=0; i<genjet.size(); i++) {
      const reco::Candidate & cand = genjet[i];
      quarks.push_back(& cand);
    }
  }

  // create the vectors
  auto_ptr< vector< JetRejObs > > Obs( new  vector<JetRejObs > ); 
  vector<  JetRejObs > *myObs = new vector<JetRejObs >;
  //------------------------

  vector<reco::CaloJet> calojet = (*jets);  

  JetPartonMatching matching = JetPartonMatching(quarks, calojet,1);
  vector<pair<unsigned int,unsigned int> > bestMatch = matching.getMatching(); 

  double DeltaRjp; 
  double DeltaRTot=0;
  int nogluonradiation=0; 
  unsigned int idxPart;
  unsigned int idxJet;
  vector<int> vctidxJet;

  for(unsigned int icbm=0; icbm < bestMatch.size(); icbm++ ){
  
    idxPart = bestMatch[icbm].first;
    idxJet  = bestMatch[icbm].second;
    DeltaRjp =  ROOT::Math::VectorUtil::DeltaR( calojet[idxJet].p4(), quarks[idxPart]->p4() );
    DeltaRTot += DeltaRjp;
    vctidxJet.push_back( bestMatch[icbm].second );
    if(DeltaRjp < DeltaRcut_) nogluonradiation++;
  } 
  
  vector<pair<int,double> > vectDeltaR;
  double wantedJet; 

  //  if(nogluonradiation >= jetNumNoRad_){  ///only the events with four recojets matching the four primary quarks
  if(nogluonradiation >= 4){  ///only the events with four recojets matching the four primary quarks
    
    //// Beginning Loop on jets!!!!
    for(unsigned int inr=0; inr < calojet.size(); inr++ ){
      wantedJet = 0.;
      for(unsigned int ij =0; ij < vctidxJet.size(); ij++){
	if (inr == vctidxJet[ij]) wantedJet = 1.; 
      }

      //observable class: JetRejLRObservables
      JetRejObs obs = (*myJetRejLRObs) (calojet[inr], primvertex, jettags, wantedJet);
        
        myObs->push_back( obs );
      
    }//close the loop on jets!!!!
  }//close nogluonradiation
  

  
  ////-----------------------
  std::auto_ptr<std::vector< JetRejObs > > pOut(myObs);
  iEvent.put( pOut );
  ///-----------------------
  
}
