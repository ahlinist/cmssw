/** \class MuonSelectorExample
 *  Example selector of muons.
 *
 *  $Date: 2009/09/25 10:13:44 $
 *  $Revision: 1.1 $
 *  \author G. Petrucciani (SNS Pisa)
 */

// Base Class Headers
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

class MuonSelectorExample : public edm::EDProducer {

    public:
        /// constructor
        MuonSelectorExample(const edm::ParameterSet &iConfig) ;
        /// destructor
        ~MuonSelectorExample() ;
    
        /// method to be called at each event
        virtual void produce(edm::Event &iEvent, const edm::EventSetup &iSetup) ;

    private:
        //--- put here your data members ---

        /// Input collection of muons
        edm::InputTag src_;

}; // C++ note: you need a ';' at the end of the class declaration.


/// Constructor: read the configuration, initialize data members, declare what to produce
MuonSelectorExample::MuonSelectorExample(const edm::ParameterSet &iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src"))
{
    // declare what we produce: a vector of references to Muon-like objects (can be reco::Muon or pat::Muons)
    // subsequent modules should read this with View<reco::Muon>
    produces<edm::RefToBaseVector<reco::Muon> >();    
}

/// Destructor: here there is usually nothing to do
MuonSelectorExample::~MuonSelectorExample() 
{
}

/// Produce: the method where we do something
void 
MuonSelectorExample::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
    using namespace edm; // avoid the prefix 'edm::' in front of Handle, View, Ref..., 

    // read the input from the event.
    // View<reco::Muon> can read any collection of reco::Muon or pat::Muon, or any collection of references to them
    Handle<View<reco::Muon> > src;
    iEvent.getByLabel(src_, src);
    
    // if you need to read other stuff, e.g. other collections, associations or condition data, do it here.

    /// prepare the vector for the output
    std::auto_ptr<RefToBaseVector<reco::Muon> > out(new RefToBaseVector<reco::Muon>());

    /// Now loop
    for (size_t i = 0, n = src->size(); i < n; ++i) {
        // read the edm reference to the muon
        RefToBase<reco::Muon> muonRef = src->refAt(i);

        // get also the muon, to make the code more readable
        const reco::Muon & mu = *muonRef;

        /// now we perform some example selection...
        if (mu.isGlobalMuon() &&                                                // a method of the reco::Muon object
            (mu.isIsolationValid() && mu.isolationR03().sumPt/mu.pt() < 0.1) && // data within the reco::Muon object
            muon::isGoodMuon(mu, muon::TMLastStationTight) &&                   // a standard MuonPOG selector
            (mu.track().isNonnull() && mu.track()->numberOfValidHits() >= 11))  // a cut on the track of the muon
                                                                                //   always check that the track is actually there!
        {
            // save the muon reference in the output vector
            out->push_back(muonRef);
        }
        
    }

    // Write the output to the event
    iEvent.put(out);
}

/// Register this as a CMSSW Plugin
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MuonSelectorExample);
