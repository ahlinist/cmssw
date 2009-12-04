#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Vertex.h"
#include "FWCore/Framework/interface/Event.h"

PromptAna_Vertex::PromptAna_Vertex(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<double> > ( prefix + "X"  + suffix );
  produces <std::vector<double> > ( prefix + "Y"  + suffix );
  produces <std::vector<double> > ( prefix + "Z"  + suffix );
  produces <std::vector<double> > ( prefix + "XErr"  + suffix );
  produces <std::vector<double> > ( prefix + "YErr"  + suffix );
  produces <std::vector<double> > ( prefix + "ZErr"  + suffix );
  produces <std::vector<double> > ( prefix + "Chi2"  + suffix );
  produces <std::vector<int> >    ( prefix + "NDF"  + suffix );
  produces <std::vector<int> >    ( prefix + "NTracks"  + suffix );
}

void PromptAna_Vertex::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  x                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  y                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  z                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  xErr             ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  yErr             ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  zErr             ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chi2             ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  ndf                 ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     ntracks      ( new std::vector<int>()   ) ;

  //Get the Vertex Collection
  edm::Handle<std::vector<reco::Vertex> > verticies;  
  iEvent.getByLabel(inputTag, verticies);

  //Fill the variables
  for (std::vector<reco::Vertex>::const_iterator it = verticies->begin(); it != verticies->end(); ++it) 
    {
      x    -> push_back(it->x());
      y    -> push_back(it->y());
      z    -> push_back(it->z());
      xErr -> push_back(it->xError());
      yErr -> push_back(it->yError());
      zErr -> push_back(it->zError());
      chi2 -> push_back(it->chi2());
      ndf  -> push_back(it->ndof());
      ntracks->push_back(int(it->tracksSize()));
    }
  
  iEvent.put( x               ,  prefix + "X" + suffix );
  iEvent.put( y               ,  prefix + "Y" + suffix );
  iEvent.put( z               ,  prefix + "Z" + suffix );
  iEvent.put( xErr            ,  prefix + "XErr" + suffix );
  iEvent.put( yErr            ,  prefix + "YErr" + suffix );
  iEvent.put( zErr            ,  prefix + "ZErr" + suffix );
  iEvent.put( chi2            ,  prefix + "Chi2" + suffix );
  iEvent.put( ndf             ,  prefix + "NDF" + suffix );
  iEvent.put( ntracks         ,  prefix + "NTracks"  + suffix );
}
