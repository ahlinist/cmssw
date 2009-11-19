#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_MET.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_MET::PromptAna_MET(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<double> > ( prefix + "Pt"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "SumEt"  + suffix );
  produces <std::vector<double> > ( prefix + "Px"  + suffix );
  produces <std::vector<double> > ( prefix + "Py"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloMETInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloMETInpHF"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloMETPhiInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloMETPhiInpHF"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloSETInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "CaloSETInpHF"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEtFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "EtFractionHadronic"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEtInEmTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEtInHadTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEtInEB"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEtInEE"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEtInHF"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEtInHB"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEtInHE"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEtInHF"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEtInHO"  + suffix );
}

void PromptAna_MET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  met                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  sumet              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  px                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  py                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloMETInmHF       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloMETInpHF       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloMETPhiInmHF    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloMETPhiInpHF    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloSETInmHF       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  CaloSETInpHF       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  etFractionEm       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  etFractionHad      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEtInEmTowers    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEtInHadTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEtInEB           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEtInEE           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEtInHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEtInHB          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEtInHE          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEtInHF          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEtInHO          ( new std::vector<double>()  ) ;

  //Get the MET Collection
  edm::Handle<reco::CaloMETCollection> metcollection;
  iEvent.getByLabel(inputTag, metcollection);
  
  //Fill the variables
  for(reco::CaloMETCollection::const_iterator it = metcollection->begin(); it != metcollection->end() ; ++it )
    {
      met              -> push_back(it->pt());
      phi              -> push_back(it->phi());
      sumet            -> push_back(it->sumEt());
      px               -> push_back(it->px());
      py               -> push_back(it->py());
      CaloMETInmHF     -> push_back(it->CaloMETInmHF());
      CaloMETInpHF     -> push_back(it->CaloMETInpHF());
      CaloMETPhiInmHF  -> push_back(it->CaloMETPhiInmHF());
      CaloMETPhiInpHF  -> push_back(it->CaloMETPhiInpHF());
      CaloSETInmHF     -> push_back(it->CaloSETInmHF());
      CaloSETInpHF     -> push_back(it->CaloSETInpHF());
      etFractionEm     -> push_back(it->emEtFraction());
      etFractionHad    -> push_back(it->etFractionHadronic());
      maxEtInEmTowers  -> push_back(it->maxEtInEmTowers());
      maxEtInHadTowers -> push_back(it->maxEtInHadTowers());
      emEtInEB         -> push_back(it->emEtInEB());
      emEtInEE         -> push_back(it->emEtInEE());
      emEtInHF         -> push_back(it->emEtInHF());
      hadEtInHB        -> push_back(it->hadEtInHB());
      hadEtInHE        -> push_back(it->hadEtInHE());
      hadEtInHF        -> push_back(it->hadEtInHF());
      hadEtInHO        -> push_back(it->hadEtInHO()); 
    }

  iEvent.put( met               ,  prefix + "Pt" + suffix );
  iEvent.put( phi               ,  prefix + "Phi" + suffix );
  iEvent.put( sumet             ,  prefix + "SumEt" + suffix );
  iEvent.put( px                ,  prefix + "Px" + suffix );
  iEvent.put( py                ,  prefix + "Py" + suffix );
  iEvent.put( CaloMETInmHF      ,  prefix + "CaloMETInmHF"  + suffix );
  iEvent.put( CaloMETInpHF      ,  prefix + "CaloMETInpHF"  + suffix );
  iEvent.put( CaloMETPhiInmHF   ,  prefix + "CaloMETPhiInmHF"  + suffix );
  iEvent.put( CaloMETPhiInpHF   ,  prefix + "CaloMETPhiInpHF"  + suffix );
  iEvent.put( CaloSETInmHF      ,  prefix + "CaloSETInmHF"  + suffix );
  iEvent.put( CaloSETInpHF      ,  prefix + "CaloSETInpHF"  + suffix );
  iEvent.put( etFractionEm      ,  prefix + "EmEtFraction"  + suffix );
  iEvent.put( etFractionHad     ,  prefix + "EtFractionHadronic"  + suffix );
  iEvent.put( maxEtInEmTowers   ,  prefix + "MaxEtInEmTowers"  + suffix );
  iEvent.put( maxEtInHadTowers  ,  prefix + "MaxEtInHadTowers"  + suffix );
  iEvent.put( emEtInEB          ,  prefix + "EmEtInEB"  + suffix );
  iEvent.put( emEtInEE          ,  prefix + "EmEtInEE"  + suffix );
  iEvent.put( emEtInHF          ,  prefix + "EmEtInHF"  + suffix );
  iEvent.put( hadEtInHB         ,  prefix + "HadEtInHB"  + suffix );
  iEvent.put( hadEtInHE         ,  prefix + "HadEtInHE"  + suffix );
  iEvent.put( hadEtInHF         ,  prefix + "HadEtInHF"  + suffix );
  iEvent.put( hadEtInHO         ,  prefix + "HadEtInHO"  + suffix );
}
