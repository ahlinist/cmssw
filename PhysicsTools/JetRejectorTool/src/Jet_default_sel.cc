#include "PhysicsTools/JetRejectorTool/interface/Jet_default_sel.h"


//
// constructor
//

Jet_default_sel::Jet_default_sel(const edm::ParameterSet& iConfig){
  genjetsrc_ = iConfig.getParameter<edm::InputTag>( "genjetsrc" );  
  calojetsrc_ = iConfig.getParameter<edm::InputTag>( "calojetsrc" );  
  jettagsrc_= iConfig.getParameter<edm::InputTag>( "jettagsrc" ) ; 
  jetEnergycut_   = iConfig.getParameter< double > ("jetEnergycut");
  jetNConstMin_  = iConfig.getParameter< double > ("jetNConstMin");
  produces< vector<reco::JetTag> >();
  produces< vector<reco::CaloJet> >();  
  produces< vector<reco::GenJet> >();  

  // outputRootFile = new TFile("JetRejObsProducer.root","recreate");
}


Jet_default_sel::~Jet_default_sel()
{
}



void Jet_default_sel::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  Handle<vector<reco::GenJet> > Gjets;	
  iEvent.getByLabel ( genjetsrc_ ,Gjets);

  Handle<vector<reco::CaloJet> > Cjets;	
  iEvent.getByLabel ( calojetsrc_ ,Cjets);
  
  Handle<vector<reco::JetTag> > ICjettags;	
  iEvent.getByLabel ( jettagsrc_ , ICjettags);

  // create the vectors

  //auto_ptr<std::vector<reco::CaloJet> > selCaloJet( new std::vector<reco::CaloJet> );
  //auto_ptr< vector<reco::JetTag> > selJetTag ( new vector<reco::JetTag>  );
  std::vector<reco::GenJet> *selGenJet = new std::vector<reco::GenJet>();
  std::vector<reco::CaloJet> *selCaloJet = new std::vector<reco::CaloJet>();
  std::vector<reco::JetTag>  *selJetTag = new std::vector<reco::JetTag>();

  //Jet Selection
  PhysicsJet seljet ; 
  PhysicsJet selgenjet ; 
  vector<reco::JetTag> seljet_trk = seljet.selection(ICjettags, jetEnergycut_, jetNConstMin_);
  vector<reco::CaloJet> seljets = seljet.selection(Cjets, jetEnergycut_, jetNConstMin_);
  vector<reco::GenJet> selgenjets = seljet.selection(Gjets, jetEnergycut_, jetNConstMin_);
  
  unsigned int sizecalojet = seljets.size();
  unsigned int sizegenjet = selgenjets.size();
  unsigned int sizejettag = seljet_trk.size();
  //selCaloJet->reserve( sizecalojet );
  //selJetTag->reserve( sizejettag );

  for(unsigned int caloj =0; caloj< sizecalojet; caloj++){
    selCaloJet->push_back( seljets[caloj] );
  }

  for(unsigned int genj =0; genj< sizegenjet; genj++){
    selGenJet->push_back( selgenjets[genj] );
  }

  for(unsigned int tagj =0; tagj < sizejettag; tagj++ ){
    selJetTag->push_back( seljet_trk[tagj] );
  }
  //auto_ptr<std::vector<reco::CaloJet> > selCaloJet( new std::vector<reco::CaloJet> );
  //auto_ptr< vector<reco::JetTag> > selJetTag ( new vector<reco::JetTag>  );
  
  std::auto_ptr<std::vector<reco::CaloJet> > pOut1(selCaloJet);
  std::auto_ptr<std::vector<reco::JetTag> > pOut2(selJetTag);
  std::auto_ptr<std::vector<reco::GenJet> > pOut3(selGenJet);
  iEvent.put( pOut1 );
  iEvent.put( pOut2 );
  iEvent.put( pOut3 );

}
