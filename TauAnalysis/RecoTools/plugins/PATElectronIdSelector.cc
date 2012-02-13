#include "TauAnalysis/RecoTools/plugins/PATElectronIdSelector.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <typename T>
PATElectronIdSelectorImp<T>::PATElectronIdSelectorImp(const edm::ParameterSet& cfg)
{
    fMVA_ = new ElectronIDMVA();
    fMVA_->Initialize("BDTG method",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet0LowPt_NoIPInfo_BDTG.weights.xml",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet1LowPt_NoIPInfo_BDTG.weights.xml",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet2LowPt_NoIPInfo_BDTG.weights.xml",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet0HighPt_NoIPInfo_BDTG.weights.xml",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet1HighPt_NoIPInfo_BDTG.weights.xml",
                   "/afs/crc.nd.edu/user/j/jkolb/work/data/Subdet2HighPt_NoIPInfo_BDTG.weights.xml",                
                   ElectronIDMVA::kNoIPInfo);

}
template <typename T>
PATElectronIdSelectorImp<T>::~PATElectronIdSelectorImp() 
{
//--- nothing to be done yet...
}
template <typename T>
void PATElectronIdSelectorImp<T>::select(const edm::Handle<collection>& patLeptonCollection,
        edm::Event& evt, const edm::EventSetup& es) 
{
    selected_.clear();
    
    EcalClusterLazyTools myEcalCluster(
            evt,
            es,
            edm::InputTag("reducedEcalRecHitsEB"),
            edm::InputTag("reducedEcalRecHitsEE")
    );

    for ( typename collection::const_iterator patLepton = patLeptonCollection->begin(); 
            patLepton != patLeptonCollection->end(); ++patLepton ) {

        Double_t mva = fMVA_->MVAValue(&(*patLepton), myEcalCluster);

        if( patLepton->pt() < 20 ) {
            if( fabs(patLepton->superCluster()->eta()) < 1.0 ) {
                if( mva > 0.133 ) selected_.push_back(&(*patLepton));
                continue;
            }
            if( fabs(patLepton->superCluster()->eta()) < 1.5 ) {
                if( mva > 0.465 ) selected_.push_back(&(*patLepton));
                continue;
            }
            if( fabs(patLepton->superCluster()->eta()) < 2.5 ) {
                if( mva > 0.518 ) selected_.push_back(&(*patLepton));
                continue;
            }

        } else {
            if( fabs(patLepton->superCluster()->eta()) < 1.0 ) {
                if( mva > 0.942 ) selected_.push_back(&(*patLepton));
                continue;
            }
            if( fabs(patLepton->superCluster()->eta()) < 1.5 ) {
                if( mva > 0.947 ) selected_.push_back(&(*patLepton));
                continue;
            }
            if( fabs(patLepton->superCluster()->eta()) < 2.5 ) {
                if( mva > 0.878 ) selected_.push_back(&(*patLepton));
                continue;
            }

        }
    }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATElectronIdSelectorImp<pat::Electron> > PATElectronIdSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronIdSelector);
