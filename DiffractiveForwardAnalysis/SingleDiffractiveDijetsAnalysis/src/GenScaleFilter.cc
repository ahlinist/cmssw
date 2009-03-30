// -*- C++ -*-
//
// Package:    GenScaleFilter
// Class:      GenScaleFilter
//
/**\class GenScaleFilter GenScaleFilter.cc DiffractiveForwardAnalysis/GenScaleFilter/src/GenScaleFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Fri Mar  6 00:58:16 BRT 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"


using namespace edm;
using namespace std;
using namespace reco;

//
// class declaration
//

class GenScaleFilter : public edm::EDFilter
{
public:
    explicit GenScaleFilter(const edm::ParameterSet&);
    ~GenScaleFilter();

private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    string generator;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenScaleFilter::GenScaleFilter(const edm::ParameterSet& iConfig) :
        generator( iConfig.getParameter<string>( "generator" ) )

{
    //now do what ever initialization is needed

}


GenScaleFilter::~GenScaleFilter()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
GenScaleFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    bool FilterResult = true;

//Skim_v38
////EventScale
    if (generator == "pomwig")
    {
        Handle<double> ktValue;
        iEvent.getByLabel("genEventKTValue", ktValue);
        double geneventscale = *ktValue;

        if (geneventscale <= 30.)
        {
            FilterResult = false;
        }
    }
    if (generator == "pythia")
    {
        Handle<double> scale;
        iEvent.getByLabel("genEventScale", scale);
        double geneventscale = *scale;

        if (geneventscale <= 15.)
        {
            FilterResult = false;
        }
    }

    // Get the jet collection
    Handle<CaloJetCollection> jets;
    iEvent.getByLabel("L2L3CorJetSC5Calo",jets);

    if (jets->size() < 2)
    {
        FilterResult = false;
    }


    return FilterResult;
}

// ------------ method called once each job just before starting event loop  ------------
void
GenScaleFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
GenScaleFilter::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenScaleFilter);
