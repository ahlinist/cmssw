#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Track.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_Track::PromptAna_Track(const edm::ParameterSet& iConfig) 
{
  //Get Input Tags
  inputTag         = iConfig.getParameter<edm::InputTag>("InputTag");
  primaryVertexTag = iConfig.getParameter<edm::InputTag>("PrimaryVertexTag");
  prefix           = iConfig.getParameter<std::string>  ("Prefix");
  suffix           = iConfig.getParameter<std::string>  ("Suffix");
  minPT            = iConfig.getParameter<double>       ("MinPT");
      
  produces <std::vector<double> > ( prefix + "Pt"  + suffix );
  produces <std::vector<double> > ( prefix + "PtError"  + suffix );
  produces <std::vector<double> > ( prefix + "Eta"  + suffix );
  produces <std::vector<double> > ( prefix + "EtaError"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "PhiError"  + suffix );
  produces <std::vector<double> > ( prefix + "Chi2"  + suffix );
  produces <std::vector<double> > ( prefix + "NDOF"  + suffix );
  produces <std::vector<int> >    ( prefix + "NumberOfValidHits"  + suffix );
  produces <std::vector<int> >    ( prefix + "Quality"  + suffix );
  produces <std::vector<int> >    ( prefix + "Algorithm"  + suffix );
}

void PromptAna_Track::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  pt           ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  pterror      ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  eta          ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  etaerror     ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  phi          ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  phierror     ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  chi2         ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  ndof         ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<int> >     nValidhits   ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     quality      ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     algorithm    ( new std::vector<int>()   ) ;

  //Get the Tracks Collection
  edm::Handle<reco::TrackCollection>  tracks;    
  iEvent.getByLabel(inputTag, tracks);

  edm::Handle<reco::VertexCollection> vertices;   
  iEvent.getByLabel(primaryVertexTag, vertices);

  for(reco::TrackCollection::const_iterator it = tracks->begin(); it != tracks->end() ; ++it )
    {
      pt        -> push_back(it->pt());
      pterror   -> push_back(it->ptError());
      eta       -> push_back(it->eta());
      etaerror  -> push_back(it->etaError());
      phi       -> push_back(it->phi());
      phierror  -> push_back(it->phiError()); 
      chi2      -> push_back(it->chi2()); 
      ndof      -> push_back(it->ndof()); 
      nValidhits->push_back(int(it->numberOfValidHits()));
      quality   ->push_back(int(it->qualityMask()));
      algorithm ->push_back(int(it->algo()));
    }

  iEvent.put( pt,         prefix + "Pt"  + suffix );
  iEvent.put( pterror,    prefix + "PtError"  + suffix );
  iEvent.put( eta,        prefix + "Eta"  + suffix );
  iEvent.put( etaerror,   prefix + "EtaError"  + suffix );
  iEvent.put( phi,        prefix + "Phi"  + suffix );
  iEvent.put( phierror,   prefix + "PhiError"  + suffix );
  iEvent.put( chi2,       prefix + "Chi2"  + suffix );
  iEvent.put( ndof,       prefix + "NDOF"  + suffix );
  iEvent.put( nValidhits, prefix + "NumberOfValidHits"  + suffix );
  iEvent.put( quality   , prefix + "Quality"  + suffix );
  iEvent.put( algorithm , prefix + "Algorithm"  + suffix );
}
