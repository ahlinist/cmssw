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
  // gen-reco match:
  matchedjetsOne1_    = iConfig.getParameter<edm::InputTag> ("matchMapOne1");
  matchedjetsOne2_    = iConfig.getParameter<edm::InputTag> ("matchMapOne2");  

  //  cout << "switchSignalDefinition = " << switchSignalDefinition << endl;

 // define Observables
   myJetRejLRObs   = new JetRejLRObservables();

  //produces<  vector< JetRejObs > >(); 
  produces<JetRejObsMap>();  
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
  iEvent.getByLabel("offlinePrimaryVerticesFromCTFTracks", primvertex); 

  Handle<vector<reco::GenJet> > genjets;
  //  if (switchSignalDefinition == 2) iEvent.getByLabel( selgenjetsrc_, genjets); 
  if (switchSignalDefinition > 1) iEvent.getByLabel( selgenjetsrc_, genjets); 
  
  Handle<vector<reco::CaloJet> > jets;
  iEvent.getByLabel( selcalojetsrc_, jets); 
  
  Handle<vector<reco::JetTag> > jettags;	
  iEvent.getByLabel (seljettagsrc_, jettags);

  // reco-gen matching (see PhysicsTools/JetMCAlgos/test/matchOneToOne.cc, V00-05-00):
  edm::Handle<CandMatchMap>        matchedjetsOne1;
  edm::Handle<CandMatchMap>        matchedjetsOne2;
  try {
    iEvent.getByLabel (matchedjetsOne1_, matchedjetsOne1 );
    iEvent.getByLabel (matchedjetsOne2_, matchedjetsOne2 );
  } catch(std::exception& ce) {
    cerr << "[matchOneToOne] caught std::exception " << ce.what() << endl;
    return;
  }
  
  // ---------------Generator Level-----------------
  std::vector<const reco::Candidate*> leptons, quarks;
  leptons.clear();
  quarks.clear();

  // create the vectors
  //auto_ptr< vector< JetRejObs > > Obs( new  vector<JetRejObs > ); 
  //vector<  JetRejObs > *myObs = new vector<JetRejObs >;
  //------------------------
  double DeltaRjp;
  int nogluonradiation=0;
  vector<reco::CaloJet> calojet = (*jets);  
  vector<unsigned int> vctidxJet;

  for( CandMatchMap::const_iterator f  = matchedjetsOne1->begin();
       f != matchedjetsOne1->end();
       f++) {
    const Candidate * genRef = &*(f->key);
    const Candidate * recoRef  = &*(f->val);
  
    DeltaRjp =  ROOT::Math::VectorUtil::DeltaR( genRef->p4(), recoRef->p4() );
    if(DeltaRjp < DeltaRcut_) nogluonradiation++;  

  // find matched CaloJet:
       for (unsigned int inr=0; inr < calojet.size(); inr++ ){
	   double pt1 = calojet[inr].pt();
	   double eta1 = calojet[inr].eta();
	   double phi1 = calojet[inr].phi();
	   double pt2 = (*recoRef).pt();
	   double eta2 = (*recoRef).eta();
	   double phi2 = (*recoRef).phi();
	   if (pt1==pt2 && eta1==eta2 && phi1==phi2) {
	     if(DeltaRjp < DeltaRcut_) vctidxJet.push_back( inr ) ;
	     continue;
	   }
	 }
 
 } 
  //-----------------------

 
 vector<pair<int,double> > vectDeltaR;
  double wantedJet; 

  // create map passing the handle to the matched collection
   std::auto_ptr<JetRejObsMap> PJetRejObs(new JetRejObsMap);
   JetRejObsMap::Filler filler(*PJetRejObs);
   {
     std::vector<JetRejObs> obs(calojet.size());
     for(size_t inr = 0; inr != calojet.size(); ++inr) {
       //-----------------------------------------
       wantedJet = 0.;
       for(unsigned int ij =0; ij < vctidxJet.size(); ij++){
	 if (inr == vctidxJet[ij]) wantedJet = 1.;
       }
       double JetEta = calojet[inr].eta(); 
       double JetPhi = calojet[inr].phi(); 
       bool trk_Jet_Ass = false;
       for(unsigned int sjt=0; sjt< jettags->size(); sjt++){
	 edm::RefVector< reco::TrackCollection > tracks;
	 tracks = (*jettags)[sjt].tracks();

	 //  reco::Jet JetTrAss;
	 edm::RefToBase<Jet> JetTrAss;
	 JetTrAss = (*jettags)[sjt].jet();

	 if((*JetTrAss).eta() != JetEta && (*JetTrAss).phi() != JetPhi) continue;    
	 //	for(unsigned int sti=0; sti<tracks.size(); sti++){
	 if(tracks.size() > 0)  trk_Jet_Ass = true;
	   //	}
       }   
       //observable class: JetRejLRObservables
       //if(trk_Jet_Ass){
       JetRejObs jetobs = (*myJetRejLRObs) (calojet[inr], primvertex, jettags, wantedJet);
       //}
       //-----------------------------------------
       obs[inr] = jetobs ;
     }
     filler.insert(jets, obs.begin(), obs.end());
   }
   
   // really fill the association map
   filler.fill();
   // put into the event 
   iEvent.put(PJetRejObs);   
  
}
 //define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
 DEFINE_FWK_MODULE(JetRejObsProducer);
