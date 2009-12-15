#ifndef RecoTauTag_TauAnalysisTools_HandleManager_h
#define RecoTauTag_TauAnalysisTools_HandleManager_h

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

class HandleManager {
   public:
      HandleManager(){};
      virtual ~HandleManager(){}
      virtual void fill(const edm::Event& evt) = 0;
};

template<class ToGet>
class HandleManagerImpl : public HandleManager {
   public: 
      HandleManagerImpl(const edm::InputTag &src):src_(src){}
      // Get a pointer to this handle
      const edm::Handle<ToGet>* get() const {return &handle_;}
      // Fill the Handle with data from event
      void fill(const edm::Event &evt);
   private:
      edm::InputTag src_;
      edm::Handle<ToGet> handle_;
};

template<class ToGet>
void
HandleManagerImpl<ToGet>::fill(const edm::Event &evt)
{
   evt.getByLabel(src_, handle_);
}

// some common implementations

#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
typedef HandleManagerImpl<reco::PFTauDiscriminator> PFTauDiscriminatorHandleManager;
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
typedef HandleManagerImpl<reco::CaloTauDiscriminator> CaloTauDiscriminatorHandleManager;
#include "DataFormats/Candidate/interface/CandMatchMap.h"
typedef HandleManagerImpl<reco::CandViewMatchMap> CandViewMatchMapHandler;
#include "DataFormats/TauReco/interface/PFTauDecayModeAssociation.h"
typedef HandleManagerImpl<reco::PFTauDecayModeAssociation> PFTauDecayModeAssociationHandleManager;


#endif
