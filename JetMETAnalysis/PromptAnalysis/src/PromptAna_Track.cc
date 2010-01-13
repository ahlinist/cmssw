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
  produces <std::vector<double> > ( prefix + "DXY"  + suffix );
  produces <std::vector<double> > ( prefix + "DXYError"  + suffix );
  produces <std::vector<int> >    ( prefix + "NumberOfValidHits"  + suffix );
  produces <std::vector<int> >    ( prefix + "NumberOfValidPixelHits"  + suffix );
  produces <std::vector<int> >    ( prefix + "NumberOfValidStripHits"  + suffix );
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
  std::auto_ptr<std::vector<double> >  dxy          ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<double> >  dxyerror     ( new std::vector<double>()) ;
  std::auto_ptr<std::vector<int> >     nValidhits   ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     nValidpixelhits   ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     nValidstriphits   ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     quality      ( new std::vector<int>()   ) ;
  std::auto_ptr<std::vector<int> >     algorithm    ( new std::vector<int>()   ) ;

  //Get the Tracks Collection
  edm::Handle<reco::TrackCollection>  tracks;    
  iEvent.getByLabel(inputTag, tracks);

  edm::Handle<reco::VertexCollection> vertices;   
  iEvent.getByLabel(primaryVertexTag, vertices);

  for(reco::TrackCollection::const_iterator it = tracks->begin(); it != tracks->end() ; ++it )
    {
      pt        -> push_back(it->pt()); //minPt=0.0
      pterror   -> push_back(it->ptError());
      eta       -> push_back(it->eta());
      etaerror  -> push_back(it->etaError());
      phi       -> push_back(it->phi());
      phierror  -> push_back(it->phiError()); 
      chi2      -> push_back(it->chi2()); //maxNormalizedChi2 = cms.double(5.0),
      ndof      -> push_back(it->ndof()); 
      nValidhits->push_back(int(it->numberOfValidHits()));//minSiliconHits = cms.int32(7)
      nValidpixelhits->push_back(int(it->hitPattern().numberOfValidPixelHits()));//minPixelHits > 2
      nValidstriphits->push_back(int(it->hitPattern().numberOfValidStripHits()));
      //       quality   ->push_back(int(it->qualityMask()));
      algorithm ->push_back(int(it->algo()));

      //-- track quality flags
      int qualityFlag = 0;
      /* --> quality flag for tracks is assigned bitwise
         bit 0: undefQuality
         bit 1: loose
         bit 2: tight
         bit 3: highPurity
         bit 4: confirmed
         bit 5: goodIterative
         bit 6: qualitySize

	 --> to extract info while analyzing the ntuples
	 "tracksQuality" is the variable stored in the ntuple for each track
	 for (int itrack=0; itrack<tracksPt->size(); itrack++){
	   int trackFlags = tracksQuality->at(itrack);
	   cout << "######" << endl;
	   cout << trackFlags << endl;
	   int bit2 = 2; //looking at tight flag
	   if( ( trackFlags & 1 << bit2) > 0)
	     cout << "flag corresponding to bit " << bit2 << " is set TRUE for track " << itrack << endl;
         }
      */

      if(it->quality(reco::Track::undefQuality))
	qualityFlag = qualityFlag | 1 << 0; 
      if(it->quality(reco::Track::loose))
	qualityFlag = qualityFlag | 1 << 1; 
      if(it->quality(reco::Track::tight))
	qualityFlag = qualityFlag | 1 << 2; 
      if(it->quality(reco::Track::highPurity))
	qualityFlag = qualityFlag | 1 << 3; 
      if(it->quality(reco::Track::confirmed))
	qualityFlag = qualityFlag | 1 << 4; 
      if(it->quality(reco::Track::goodIterative))
	qualityFlag = qualityFlag | 1 << 5; 
      if(it->quality(reco::Track::qualitySize))
	qualityFlag = qualityFlag | 1 << 6; 
      
      quality->push_back(qualityFlag);
      //--

      dxy       ->push_back(it->dxy());     
      dxyerror  ->push_back(it->dxyError());//maxD0Significance = cms.double(5.0)
    }
  
  iEvent.put( pt,         prefix + "Pt"  + suffix );
  iEvent.put( pterror,    prefix + "PtError"  + suffix );
  iEvent.put( eta,        prefix + "Eta"  + suffix );
  iEvent.put( etaerror,   prefix + "EtaError"  + suffix );
  iEvent.put( phi,        prefix + "Phi"  + suffix );
  iEvent.put( phierror,   prefix + "PhiError"  + suffix );
  iEvent.put( chi2,       prefix + "Chi2"  + suffix );
  iEvent.put( ndof,       prefix + "NDOF"  + suffix );
  iEvent.put( dxy,        prefix + "DXY"  + suffix );
  iEvent.put( dxyerror,   prefix + "DXYError"  + suffix );
  iEvent.put( nValidhits, prefix + "NumberOfValidHits"  + suffix );
  iEvent.put( nValidpixelhits, prefix + "NumberOfValidPixelHits"  + suffix );
  iEvent.put( nValidstriphits, prefix + "NumberOfValidStripHits"  + suffix );
  iEvent.put( quality   , prefix + "Quality"  + suffix );
  iEvent.put( algorithm , prefix + "Algorithm"  + suffix );
}
// Just for reference: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/DataFormats/TrackReco/interface/TrackBase.h
// enum TrackAlgorithm { undefAlgorithm=0, ctf=1, rs=2, cosmics=3, iter0=4, 
// 		      iter1=5, iter2=6, iter3=7, iter4=8, iter5=9, iter6=10, iter7=11, iter8=12, iter9=13,iter10=14,
// 		      outInEcalSeededConv=15, inOutEcalSeededConv=16, 
// 		      nuclInter=17,
// 		      standAloneMuon=18,globalMuon=19,cosmicStandAloneMuon=20,cosmicGlobalMuon=21,
// 		      iter1LargeD0=22,iter2LargeD0=23,iter3LargeD0=24,iter4LargeD0=25,iter5LargeD0=26,
// 		      bTagGhostTracks=27,
// 		      beamhalo=28, 
// 		      gsf=29,
// 		      algoSize=30 };
