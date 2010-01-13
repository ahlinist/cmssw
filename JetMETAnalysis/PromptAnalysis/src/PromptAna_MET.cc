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
  produces <std::vector<double> > ( prefix + "METInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "METInpHF"  + suffix );
  produces <std::vector<double> > ( prefix + "METPhiInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "METPhiInpHF"  + suffix );
  produces <std::vector<double> > ( prefix + "SETInmHF"  + suffix );
  produces <std::vector<double> > ( prefix + "SETInpHF"  + suffix );
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
  produces <std::vector<double> > ( prefix + "METSigCornell"  + suffix );
}

void PromptAna_MET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  met                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  sumet              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  px                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  py                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  METInmHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  METInpHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  METPhiInmHF        ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  METPhiInpHF        ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  SETInmHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  SETInpHF           ( new std::vector<double>()  ) ;
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
  std::auto_ptr<std::vector<double> >  METSigCornell      ( new std::vector<double>()  ) ;

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
      METInmHF         -> push_back(it->CaloMETInmHF());
      METInpHF         -> push_back(it->CaloMETInpHF());
      METPhiInmHF      -> push_back(it->CaloMETPhiInmHF());
      METPhiInpHF      -> push_back(it->CaloMETPhiInpHF());
      SETInmHF         -> push_back(it->CaloSETInmHF());
      SETInpHF         -> push_back(it->CaloSETInpHF());
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
      METSigCornell    -> push_back(it->metSignificance()); 
    }

  iEvent.put( met               ,  prefix + "Pt" + suffix );
  iEvent.put( phi               ,  prefix + "Phi" + suffix );
  iEvent.put( sumet             ,  prefix + "SumEt" + suffix );
  iEvent.put( px                ,  prefix + "Px" + suffix );
  iEvent.put( py                ,  prefix + "Py" + suffix );
  iEvent.put( METInmHF          ,  prefix + "METInmHF"  + suffix );
  iEvent.put( METInpHF          ,  prefix + "METInpHF"  + suffix );
  iEvent.put( METPhiInmHF       ,  prefix + "METPhiInmHF"  + suffix );
  iEvent.put( METPhiInpHF       ,  prefix + "METPhiInpHF"  + suffix );
  iEvent.put( SETInmHF          ,  prefix + "SETInmHF"  + suffix );
  iEvent.put( SETInpHF          ,  prefix + "SETInpHF"  + suffix );
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
  iEvent.put( METSigCornell     ,  prefix + "METSigCornell"  + suffix );
}

