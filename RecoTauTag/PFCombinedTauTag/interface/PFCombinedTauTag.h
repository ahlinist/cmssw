#ifndef RecoTauTag_PFCombinedTauTag
#define RecoTauTag_PFCombinedTauTag

/* class PFCombinedTauTag
 *  EDProducer of the tagged TauJet with the PFCombinedTauTagAlgorithm, 
 *  returns a PFCombinedTauTagInfo collection,
 *  created: Apr 23 2007,
 *  revised: 
 *  author: Ludovic Houchu.
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/PFCombinedTauTagInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "CondFormats/BTauObjects/interface/CalibratedHistogram.h"
#include "CondFormats/BTauObjects/interface/CombinedTauTagCalibration.h"
#include "CondFormats/DataRecord/interface/CombinedTauTagRcd.h"

#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include "CLHEP/Random/RandGauss.h"

#include "RecoTauTag/PFCombinedTauTag/interface/PFCombinedTauTagAlg.h"
#include "RecoTauTag/PFCombinedTauTag/interface/LikelihoodRatio.h"
#include "RecoTauTag/PFCombinedTauTag/interface/CategoryDataCondDB.h"

#include "RecoBTag/XMLCalibration/interface/CalibrationInterface.h"

#include <memory>

using namespace std; 
using namespace edm;
using namespace edm::eventsetup; 
using namespace reco;

class PFCombinedTauTag : public EDProducer {
   public:
      explicit PFCombinedTauTag(const ParameterSet& iConfig){
	PFIsolatedTauTagSrc_         = iConfig.getParameter<string>("PFIsolatedTauTagSrc");
	PVSrc_                     = iConfig.getParameter<string>("PVSrc");
	smearedPVsigmaX_           = iConfig.getParameter<double>("smearedPVsigmaX");
	smearedPVsigmaY_           = iConfig.getParameter<double>("smearedPVsigmaY");
	smearedPVsigmaZ_           = iConfig.getParameter<double>("smearedPVsigmaZ");
	thePFCombinedTauTagAlg=new PFCombinedTauTagAlg(iConfig);
	string modulname="PFCombinedTauTag";
	produces<JetTagCollection>().setBranchAlias(modulname);
	string infoBranchName=modulname+"Info";
	produces<PFCombinedTauTagInfoCollection>().setBranchAlias(infoBranchName);
	theLikelihoodRatio=new LikelihoodRatio(iConfig);  
	m_cacheId=0;
      }
      ~PFCombinedTauTag(){
	delete thePFCombinedTauTagAlg;
      }
      virtual void produce(Event&, const EventSetup&);
 private:
      PFCombinedTauTagAlg* thePFCombinedTauTagAlg;
      LikelihoodRatio* theLikelihoodRatio;
      unsigned long long m_cacheId;
      string PFIsolatedTauTagSrc_;
      string PVSrc_;
      double smearedPVsigmaX_;
      double smearedPVsigmaY_;
      double smearedPVsigmaZ_;
};
#endif
