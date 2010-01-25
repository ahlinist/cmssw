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
  produces <std::vector<double> > ( prefix + "SumPt" + suffix );//SumPt of all tracks
  produces <std::vector<double> > ( prefix + "SumPtW5" + suffix );//SumPt of tracks with weight > 0.5
  produces <std::vector<double> > ( prefix + "NDF"  + suffix );
  produces <std::vector<int> >    ( prefix + "NTracks"  + suffix );
  produces <std::vector<int> >    ( prefix + "NTracksW5"  + suffix );
  produces <std::vector<bool> >   ( prefix + "isValid"  + suffix );
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
  std::auto_ptr<std::vector<double> >  sumpt            ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  sumptw5          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  ndf              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  ntracks             ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >  ntracksw5           ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<bool> >  isvalid            ( new std::vector<bool>() ) ;

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
      isvalid->push_back(int(it->isValid()));

      float _SumPt  = 0.0;
      float _SumPtw5  = 0.0;
      int _ntracksw5 = 0;
      if(it->tracksSize() > 0)
	{
	  std::vector<TrackBaseRef>::const_iterator trackIt;
	  for( trackIt = it->tracks_begin(); trackIt != it->tracks_end(); trackIt++)
	    {
	      if(fabs((**trackIt).charge()) <= 1.)
		{
		  _SumPt += (**trackIt).pt();
		  
		  if (it->trackWeight(*trackIt) >= 0.5 )
		    {_SumPtw5 += (**trackIt).pt(); _ntracksw5++;}
		}
	    }
	}
      sumpt->push_back(_SumPt);
      sumptw5->push_back(_SumPtw5);
      ntracksw5->push_back(_ntracksw5);
    }
  
  iEvent.put( x               ,  prefix + "X" + suffix );
  iEvent.put( y               ,  prefix + "Y" + suffix );
  iEvent.put( z               ,  prefix + "Z" + suffix );
  iEvent.put( xErr            ,  prefix + "XErr" + suffix );
  iEvent.put( yErr            ,  prefix + "YErr" + suffix );
  iEvent.put( zErr            ,  prefix + "ZErr" + suffix );
  iEvent.put( chi2            ,  prefix + "Chi2" + suffix );
  iEvent.put( sumpt           ,  prefix + "SumPt" + suffix );
  iEvent.put( sumptw5         ,  prefix + "SumPtW5" + suffix );
  iEvent.put( ndf             ,  prefix + "NDF" + suffix );
  iEvent.put( ntracks         ,  prefix + "NTracks"  + suffix );
  iEvent.put( ntracksw5       ,  prefix + "NTracksW5"  + suffix );
  iEvent.put( isvalid         ,  prefix + "isValid"  + suffix );
}
