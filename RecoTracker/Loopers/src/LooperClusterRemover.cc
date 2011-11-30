#include "RecoTracker/Loopers/interface/LooperClusterRemover.h"
#include "RecoTracker/Loopers/interface/Methods.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"

//
// constructors and destructor
//
LooperClusterRemover::LooperClusterRemover(const edm::ParameterSet& iConfig)
{
   //register your products

   //produce the mask of looper clusters
   produces<edmNew::DetSetVector<SiPixelClusterRefNew> >();
   produces<edmNew::DetSetVector<SiStripRecHit1D::ClusterRef> >();

   std::string whichOne = iConfig.getParameter<std::string>("algo");
   if (whichOne=="EveryNMethod")
        method_ = new LooperClusterRemoverMethod::EveryNMethod(iConfig);
   else if (whichOne=="FractionOfTruth")
       method_ = new LooperClusterRemoverMethod::FractionOfTruth(iConfig);
   else if (whichOne=="ReadFileIn")
     method_ = new LooperClusterRemoverMethod::ReadFileIn(iConfig);
   else if (whichOne=="LooperMethod")
     method_ = new LooperClusterRemoverMethod::LooperMethod(iConfig);
   else if (whichOne=="PerJet")
     method_ = new LooperClusterRemoverMethod::PerJet(iConfig);
   else {
     std::cout<<whichOne<<" is not recognized";
     method_=0;//and crash
   }

   // side produces the track candidate collection to be used for track fitting.
   produces<TrackCandidateCollection>();

}


LooperClusterRemover::~LooperClusterRemover()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
LooperClusterRemover::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //final products
  prod_.removedPixelClsuterRefs.reset(new edmNew::DetSetVector<SiPixelClusterRefNew>());
  prod_.removedStripClsuterRefs.reset(new edmNew::DetSetVector<SiStripRecHit1D::ClusterRef>());
  prod_.tcOut.reset(new TrackCandidateCollection());
  method_->run(iEvent,iSetup,
	       prod_);
  
  edm::LogError("LooperClusterRemover")<<" rejecting pixels on "<<prod_.removedPixelClsuterRefs->size()<<" modules\n"
				       <<" rejecting strip on "<<prod_.removedStripClsuterRefs->size()<<" modules\n"
				       <<" making "<<prod_.tcOut->size()<<" track candidated";
  
  //put the product in the event
  iEvent.put( prod_.removedPixelClsuterRefs );
  iEvent.put( prod_.removedStripClsuterRefs );
  iEvent.put( prod_.tcOut );
}

// ------------ method called once each job just before starting event loop  ------------
void 
LooperClusterRemover::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LooperClusterRemover::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
LooperClusterRemover::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
LooperClusterRemover::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
LooperClusterRemover::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
LooperClusterRemover::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LooperClusterRemover::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LooperClusterRemover);
