#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Jet.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_Jet::PromptAna_Jet(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<double> > ( prefix + "EmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "EnergyFractionHadronic"  + suffix );
  produces <std::vector<double> > ( prefix + "TowersArea"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInEmTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInHadTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEnergyInHB"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEnergyInHE"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEnergyInHO"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEnergyInHF"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEnergyInEB"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEnergyInEE"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEnergyInHF"  + suffix );
  produces <std::vector<int> > ( prefix + "N60"  + suffix ); 
  produces <std::vector<int> > ( prefix + "N90"  + suffix ); 
}

void PromptAna_Jet::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  emEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energyFractionHadronic ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  towersArea   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInEmTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInHadTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEnergyInHB   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEnergyInHE   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEnergyInHO   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEnergyInHF   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEnergyInEB   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEnergyInEE   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEnergyInHF   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  n60   ( new std::vector<int>()  ); 
  std::auto_ptr<std::vector<int> >  n90   ( new std::vector<int>()  ); 

  
  //Get the Jets Collection
  edm::Handle<reco::CaloJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);
  
  //Fill the variables
  for(reco::CaloJetCollection::const_iterator it = jetcollection->begin(); it != jetcollection->end() ; ++it ){
    emEnergyFraction->push_back(it->emEnergyFraction());
    energyFractionHadronic->push_back(it->energyFractionHadronic());
    towersArea->push_back(it->towersArea());
    maxEInEmTowers->push_back(it->maxEInEmTowers());
    maxEInHadTowers->push_back(it->maxEInHadTowers());
    hadEnergyInHB->push_back(it->hadEnergyInHB());
    hadEnergyInHE->push_back(it->hadEnergyInHE());
    hadEnergyInHO->push_back(it->hadEnergyInHO());
    hadEnergyInHF->push_back(it->hadEnergyInHF());
    emEnergyInEB->push_back(it->emEnergyInEB());
    emEnergyInEE->push_back(it->emEnergyInEE());
    emEnergyInHF->push_back(it->emEnergyInHF());
    n60->push_back(it->n60()); 
    n90->push_back(it->n90()); 
  }
  
  iEvent.put( emEnergyFraction,  prefix + "EmEnergyFraction"  + suffix );
  iEvent.put( energyFractionHadronic,  prefix + "EnergyFractionHadronic"  + suffix );
  iEvent.put( towersArea,  prefix + "TowersArea"  + suffix );
  iEvent.put( maxEInEmTowers,  prefix + "MaxEInEmTowers"  + suffix );
  iEvent.put( maxEInHadTowers,  prefix + "MaxEInHadTowers"  + suffix );
  iEvent.put( hadEnergyInHB,  prefix + "HadEnergyInHB"  + suffix );
  iEvent.put( hadEnergyInHE,  prefix + "HadEnergyInHE"  + suffix );
  iEvent.put( hadEnergyInHO,  prefix + "HadEnergyInHO"  + suffix );
  iEvent.put( hadEnergyInHF,  prefix + "HadEnergyInHF"  + suffix );
  iEvent.put( emEnergyInEB,  prefix + "EmEnergyInEB"  + suffix );
  iEvent.put( emEnergyInEE,  prefix + "EmEnergyInEE"  + suffix );
  iEvent.put( emEnergyInHF,  prefix + "EmEnergyInHF"  + suffix );
  iEvent.put( n60,  prefix + "N60"  + suffix ); 
  iEvent.put( n90,  prefix + "N90"  + suffix ); 
}
