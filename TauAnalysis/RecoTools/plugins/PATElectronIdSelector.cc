#include "TauAnalysis/RecoTools/plugins/PATElectronIdSelector.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <TMath.h>

#include <string>

template <typename T>
PATElectronIdSelectorImp<T>::PATElectronIdSelectorImp(const edm::ParameterSet& cfg)
{
  srcVertex_ = cfg.getParameter<edm::InputTag>("srcVertex");

  std::vector<std::string> mvaWeightFiles;
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml");
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml");
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml");
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml");
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml");
  mvaWeightFiles.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml");
  
  std::vector<std::string> mvaWeightFiles_full;
  for ( std::vector<std::string>::const_iterator mvaWeightFile = mvaWeightFiles.begin();
	mvaWeightFile != mvaWeightFiles.end(); ++mvaWeightFile ) {
    edm::FileInPath mvaWeightFile_full(*mvaWeightFile);
    if ( !mvaWeightFile_full.isLocal()) throw cms::Exception("PATElectronIdSelector::PATElectronIdSelector")
      << " Failed to find File = " << (*mvaWeightFile) << " !!\n";
    mvaWeightFiles_full.push_back(mvaWeightFile_full.fullPath());
  }

  fMVA_ = new EGammaMvaEleEstimator();
  fMVA_->initialize("BDT",
		    EGammaMvaEleEstimator::kNonTrig,
		    true, 
		    mvaWeightFiles_full);

  std::string cut_string = cfg.getParameter<std::string>("cut");
  if      ( cut_string == "loose" ) cut_ = kLoose;
  else if ( cut_string == "tight" ) cut_ = kTight;
  else throw cms::Exception("PATElectronIdSelector::PATElectronIdSelector")
    << " Invalid Configuration parameter 'cut' = " << cut_string << " !!\n";

  verbosity_ = ( cfg.exists("verbosity") ) ?
    cfg.getParameter<int>("verbosity") : 0;
}

template <typename T>
PATElectronIdSelectorImp<T>::~PATElectronIdSelectorImp() 
{
//--- nothing to be done yet...
}

template <typename T>
void PATElectronIdSelectorImp<T>::select(const edm::Handle<collection>& patLeptonCollection, edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();
    
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertex_, vertices);
  if ( !(vertices->size() >= 1) ) return;
  const reco::Vertex& theEventVertex = vertices->front();

  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  const TransientTrackBuilder* trackBuilder = trackBuilderHandle.product();
  if ( !trackBuilder ) 
    throw cms::Exception("PATElectronIdSelector::select()")
      << " Failed to access TransientTrackBuilder !!\n";
  
  EcalClusterLazyTools myEcalCluster(evt, es, edm::InputTag("reducedEcalRecHitsEB"), edm::InputTag("reducedEcalRecHitsEE"));

  for ( typename collection::const_iterator patLepton = patLeptonCollection->begin(); 
	patLepton != patLeptonCollection->end(); ++patLepton ) {

    double mva = fMVA_->mvaValue(*patLepton, theEventVertex, *trackBuilder, myEcalCluster, verbosity_ > 0);

    double pt = patLepton->pt();
    double absEta = TMath::Abs(patLepton->superCluster()->eta());

    double mvaCut = 1.e+3;
    if ( pt < 20 ) { 
      // loose cut values used for e+mu channel of Htautau analysis (cf. https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#Object_ID)
      // (e+tau channel does not select electrons of Pt < 20 GeV)
      if      ( absEta < 0.8   ) mvaCut = 0.925;
      else if ( absEta < 1.479 ) mvaCut = 0.915;
      else                       mvaCut = 0.965;
    } else {   
      if ( cut_ == kLoose ) { 
	// loose cut values used for e+mu channel of Htautau analysis (cf. https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#Object_ID)
	if      ( absEta < 0.8   ) mvaCut = 0.905;
	else if ( absEta < 1.479 ) mvaCut = 0.955;
	else                       mvaCut = 0.975;
      } else if ( cut_ == kTight ) { 
	// tight cut values used for e+tau channel of Htautau analysis (cf. https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#Object_ID)
	if      ( absEta < 0.8   ) mvaCut = 0.925;
	else if ( absEta < 1.479 ) mvaCut = 0.975;
	else                       mvaCut = 0.985;
      } else assert(0);
    }

    if ( mva > mvaCut ) selected_.push_back(&(*patLepton));
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATElectronIdSelectorImp<pat::Electron> > PATElectronIdSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronIdSelector);
