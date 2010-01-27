// K. Hatakeyama, Baylor University
// For PF jets.
//
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_PFJet.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_PFJet::PromptAna_PFJet(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , jetCorrectionService  (iConfig.getParameter<std::string>  ("JetCorrectionService"  ))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{

  produces <std::vector<int> >    ( prefix + "NJets"  + suffix );
  produces <std::vector<double> > ( prefix + "pT"  + suffix );
  produces <std::vector<double> > ( prefix + "Energy"  + suffix );
  produces <std::vector<double> > ( prefix + "Eta"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<int> >    ( prefix + "NConstituents"  + suffix );
  produces <std::vector<double> > ( prefix + "scaleL2L3"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedHadronEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "NeutralHadronEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedEmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "NeutralEmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedMuEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedMultiplicity"  + suffix );
  produces <std::vector<double> > ( prefix + "NeutralMultiplicity"  + suffix );
  produces <std::vector<double> > ( prefix + "MuonMultiplicity"  + suffix );

}

void PromptAna_PFJet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<int> >  njets  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  pt  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energy  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  eta  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nconstituents  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  scaleL2L3  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedHadronEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  neutralHadronEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedEmEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  neutralEmEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedMuEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedMultiplicity  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  neutralMultiplicity  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  muonMultiplicity  ( new std::vector<double>()  ) ;

  //Get the Jets Collection
  edm::Handle<reco::PFJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);
  //

  //Fill the variables
  for(reco::PFJetCollection::const_iterator it = jetcollection->begin(); it != jetcollection->end() ; ++it ){
    
    if(it==jetcollection->begin()){
      njets->push_back(jetcollection->size());
    }
    pt->push_back(it->pt());
    energy->push_back(it->energy());
    nconstituents->push_back(it->getPFConstituents().size());
    eta->push_back(it->eta());
    phi->push_back(it->phi());

    const JetCorrector* corrector = JetCorrector::getJetCorrector (jetCorrectionService,iSetup);
    scaleL2L3->push_back( corrector->correction(it->p4()) );
    //std::cout << corrector->correction(it->p4()) << std::endl;

    chargedHadronEnergyFraction->push_back(it->chargedHadronEnergyFraction());
    neutralHadronEnergyFraction->push_back(it->neutralHadronEnergyFraction());
    chargedEmEnergyFraction->push_back(it->chargedEmEnergyFraction());
    neutralEmEnergyFraction->push_back(it->neutralEmEnergyFraction());
    chargedMuEnergyFraction->push_back(it->chargedMuEnergyFraction());
    chargedMultiplicity->push_back(it->chargedMultiplicity());
    neutralMultiplicity->push_back(it->neutralMultiplicity());
    muonMultiplicity->push_back(it->muonMultiplicity());
    //
  }

  iEvent.put( njets,  prefix + "NJets"  + suffix );
  iEvent.put( pt,  prefix + "pT"  + suffix );
  iEvent.put( energy,  prefix + "Energy"  + suffix );
  iEvent.put( nconstituents,  prefix + "NConstituents"  + suffix );
  iEvent.put( eta,  prefix + "Eta"  + suffix );
  iEvent.put( phi,  prefix + "Phi"  + suffix );
  iEvent.put( scaleL2L3,  prefix + "scaleL2L3"  + suffix );
  iEvent.put( chargedHadronEnergyFraction,  prefix + "ChargedHadronEnergyFraction"  + suffix );
  iEvent.put( neutralHadronEnergyFraction,  prefix + "NeutralHadronEnergyFraction"  + suffix );
  iEvent.put( chargedEmEnergyFraction,  prefix + "ChargedEmEnergyFraction"  + suffix );
  iEvent.put( neutralEmEnergyFraction,  prefix + "NeutralEmEnergyFraction"  + suffix );
  iEvent.put( chargedMuEnergyFraction,  prefix + "ChargedMuEnergyFraction"  + suffix );
  iEvent.put( chargedMultiplicity,  prefix + "ChargedMultiplicity"  + suffix );
  iEvent.put( neutralMultiplicity,  prefix + "NeutralMultiplicity"  + suffix );
  iEvent.put( muonMultiplicity,  prefix + "MuonMultiplicity"  + suffix );
  
}
