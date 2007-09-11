#ifndef RecoTauTag_CombinedTauTag
#define RecoTauTag_CombinedTauTag

/* class CombinedTauTag
 *  EDProducer of the tagged TauJet with the CombinedTauTagAlgorithm, 
 *  returns two collections: base collection is the JetTag, and extended Collection is the CombinedTauTagInfo,
 *  created: Dec 18 2006,
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

#include "DataFormats/BTauReco/interface/CombinedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "CondFormats/BTauObjects/interface/CalibratedHistogram.h"
#include "CondFormats/BTauObjects/interface/CombinedTauTagCalibration.h"
#include "CondFormats/DataRecord/interface/CombinedTauTagRcd.h"

#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include "CLHEP/Random/RandGauss.h"

#include "RecoTauTag/CombinedTauTag/interface/CombinedTauTagAlg.h"
#include "RecoTauTag/CombinedTauTag/interface/LikelihoodRatio.h"
#include "RecoTauTag/CombinedTauTag/interface/CategoryDataCondDB.h"

#include "RecoBTag/XMLCalibration/interface/CalibrationInterface.h"

#include <memory>

using namespace std; 
using namespace edm;
using namespace edm::eventsetup; 
using namespace reco;

class CombinedTauTag : public EDProducer {
   public:
      explicit CombinedTauTag(const ParameterSet& iConfig){
	IsolatedTauTagSrc_         = iConfig.getParameter<string>("IsolatedTauTagSrc");
	PVSrc_                     = iConfig.getParameter<string>("PVSrc");
	smearedPVsigmaX_           = iConfig.getParameter<double>("smearedPVsigmaX");
	smearedPVsigmaY_           = iConfig.getParameter<double>("smearedPVsigmaY");
	smearedPVsigmaZ_           = iConfig.getParameter<double>("smearedPVsigmaZ");
	theCombinedTauTagAlg=new CombinedTauTagAlg(iConfig);
	string modulname="CombinedTauTag";
	produces<JetTagCollection>().setBranchAlias(modulname);
	string infoBranchName=modulname+"Info";
	produces<CombinedTauTagInfoCollection>().setBranchAlias(infoBranchName);
	theLikelihoodRatio=new LikelihoodRatio(iConfig);  
	m_cacheId=0;
      }
      ~CombinedTauTag(){
	delete theCombinedTauTagAlg;
      }
      virtual void produce(Event&, const EventSetup&);
 private:
      CombinedTauTagAlg* theCombinedTauTagAlg;
      LikelihoodRatio* theLikelihoodRatio;
      unsigned long long m_cacheId;
      string IsolatedTauTagSrc_;
      string PVSrc_;
      double smearedPVsigmaX_;
      double smearedPVsigmaY_;
      double smearedPVsigmaZ_;
};
#endif
