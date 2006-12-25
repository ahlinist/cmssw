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
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/BTauReco/interface/CombinedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoTauTag/CombinedTauTag/interface/CombinedTauTagAlg.h"

#include <memory>

using namespace std; 
using namespace edm;
using namespace reco;

class CombinedTauTag : public EDProducer {
   public:
      explicit CombinedTauTag(const ParameterSet& iConfig){
	IsolatedTauTagSrc_         = iConfig.getParameter<string>("IsolatedTauTagSrc");
	PVSrc_                     = iConfig.getParameter<string>("PVSrc");
	theCombinedTauTagAlg=new CombinedTauTagAlg(iConfig);
	string modulname="CombinedTauTag";
	produces<JetTagCollection>().setBranchAlias(modulname);
	string infoBranchName=modulname+"Info";
	produces<CombinedTauTagInfoCollection>().setBranchAlias(infoBranchName);  
      }
      ~CombinedTauTag(){
	delete theCombinedTauTagAlg;
      }
      virtual void produce(Event&, const EventSetup&);
 private:
      CombinedTauTagAlg* theCombinedTauTagAlg;
      string IsolatedTauTagSrc_;
      string PVSrc_;
};
#endif
