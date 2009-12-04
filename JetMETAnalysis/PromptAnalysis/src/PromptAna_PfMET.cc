#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_PfMET.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_PfMET::PromptAna_PfMET(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<double> > ( prefix + "Pt"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "SumEt"  + suffix );
  produces <std::vector<double> > ( prefix + "Px"  + suffix );
  produces <std::vector<double> > ( prefix + "Py"  + suffix );
}

void PromptAna_PfMET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  met                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  sumet              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  px                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  py                 ( new std::vector<double>()  ) ;

  //Get the MET Collection
  edm::Handle<reco::PFMETCollection> pfmetcoll;
  iEvent.getByLabel(inputTag, pfmetcoll);
  
  //Fill the variables
  for(reco::PFMETCollection::const_iterator it = pfmetcoll->begin(); it != pfmetcoll->end() ; ++it )
    {
      met              -> push_back(it->pt());
      phi              -> push_back(it->phi());
      sumet            -> push_back(it->sumEt());
      px               -> push_back(it->px());
      py               -> push_back(it->py());
    }

  iEvent.put( met               ,  prefix + "Pt" + suffix );
  iEvent.put( phi               ,  prefix + "Phi" + suffix );
  iEvent.put( sumet             ,  prefix + "SumEt" + suffix );
  iEvent.put( px                ,  prefix + "Px" + suffix );
  iEvent.put( py                ,  prefix + "Py" + suffix );
}

