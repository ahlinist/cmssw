#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Jet.h"
#include "FWCore/Framework/interface/Event.h"


PromptAna_Jet::PromptAna_Jet(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  //Get Input Tags
  //  inputTag       = iConfig.getParameter<edm::InputTag>("InputTag");
  //prefix         = iConfig.getParameter<std::string>  ("Prefix"  );
  //suffix         = iConfig.getParameter<std::string>  ("Suffix"  );
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
  produces <std::vector<double> > ( prefix + "JIDfSubDet1"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfSubDet2"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfSubDet3"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfSubDet4"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDresEMF"  + suffix );
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
  produces <std::vector<double> > ( prefix + "pTFirst"  + suffix );
  produces <std::vector<double> > ( prefix + "EnergyFirst"  + suffix );
  produces <std::vector<double> > ( prefix + "EtaFirst"  + suffix );
  produces <std::vector<double> > ( prefix + "PhiFirst"  + suffix );

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
  std::auto_ptr<std::vector<double> >  fSubDet1  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  fSubDet2  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  fSubDet3  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  fSubDet4  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  resEMF  ( new std::vector<double>()  ) ;
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
  std::auto_ptr<std::vector<double> >  ptfirst  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energyfirst  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  etafirst  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phifirst  ( new std::vector<double>()  ) ;
  
  //Get the Jets Collection
  edm::Handle<reco::CaloJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);


  int clj=0;

  //Fill the variables
  for(reco::CaloJetCollection::const_iterator it = jetcollection->begin(); it != jetcollection->end() ; ++it ){
    
    if(it==jetcollection->begin()){
      njets->push_back(jetcollection->size());
      ptfirst->push_back(it->pt());
      energyfirst->push_back(it->energy());
      etafirst->push_back(it->eta());
      phifirst->push_back(it->phi());
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
    fSubDet1->push_back(jetIDHelper.fSubDetector1() );
    fSubDet2->push_back(jetIDHelper.fSubDetector2() );
    fSubDet3->push_back(jetIDHelper.fSubDetector3() );
    fSubDet4->push_back(jetIDHelper.fSubDetector4() );
    resEMF->push_back(jetIDHelper.restrictedEMF() );
    //ok for the moment i add this by hand, just to be sure
    if(jetIDHelper.fHPD()<0.98 && jetIDHelper.n90Hits()>=2 && jetIDHelper.restrictedEMF()>0.01) clj++;
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
  iEvent.put( fSubDet1,  prefix + "JIDfSubDet1"  + suffix );
  iEvent.put( fSubDet2,  prefix + "JIDfSubDet2"  + suffix );  
  iEvent.put( fSubDet3,  prefix + "JIDfSubDet3"  + suffix );
  iEvent.put( fSubDet4,  prefix + "JIDfSubDet4"  + suffix );
  iEvent.put( resEMF,  prefix + "JIDresEMF"    + suffix );
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
  iEvent.put( ptfirst,  prefix + "pTFirst"  + suffix );
  iEvent.put( energyfirst,  prefix + "EnergyFirst"  + suffix );
  iEvent.put( etafirst,  prefix + "EtaFirst"  + suffix );
  iEvent.put( phifirst,  prefix + "PhiFirst"  + suffix );
}
