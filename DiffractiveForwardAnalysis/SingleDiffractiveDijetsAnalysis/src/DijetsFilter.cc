// -*- C++ -*-
//
// Package:    DijetsFilter
// Class:      DijetsFilter
//
/**\class DijetsFilter DijetsFilter.cc DiffractiveForwardAnalysis/DijetsFilter/src/DijetsFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Mon Mar  2 00:45:29 BRT 2009
// $Id: DijetsFilter.cc,v 1.2 2009/05/22 03:29:29 fsilva Exp $
//
//


// system include files
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//deltaR
#include "DataFormats/Math/interface/deltaR.h"

//ROOT
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

//
// class declaration
//

class DijetsFilter : public edm::EDFilter
{
public:
    explicit DijetsFilter(const edm::ParameterSet&);
    ~DijetsFilter();

private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------

    string GapSideSource;

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
DijetsFilter::DijetsFilter(const edm::ParameterSet& iConfig) :
        GapSideSource( iConfig.getParameter<string>( "GapSideSource" ) )
{
    //now do what ever initialization is needed

}


DijetsFilter::~DijetsFilter()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
DijetsFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    bool FilterResult = false;
    // Get the jet collection
    Handle<CaloJetCollection> jets;
    iEvent.getByLabel("L2L3CorJetSC5Calo",jets);

    //#jets above et_cut
    //int goodJetsCount = 0;
    double jet1Et = 0.;
    double jet2Et = 0.;
    double jet1Eta = 0.;
    double jet1Phi = 0.;
    double jet2Eta = 0.;
    double jet2Phi = 0.;
    //for (CaloJetCollection::const_iterator i_jet = jets->begin(); i_jet != jets->end(); i_jet++)
    for (CaloJetCollection::const_iterator i_jet = jets->begin(); i_jet != (jets->begin()+2); i_jet++)
    {
        if (i_jet == jets->begin())
        {
            jet1Et = i_jet->et();
            jet1Eta = i_jet->eta();
            jet1Phi = i_jet->phi();
        }
        if (i_jet == jets->begin()+1)
        {
            jet2Et = i_jet->et();
            jet2Eta = i_jet->eta();
            jet2Phi = i_jet->phi();
        }
    }

    // Get the gap side
    Handle< int > gapside;
    iEvent.getByLabel(GapSideSource,"GapSide",gapside);

    //cout << GapSide << endl;
    int GapSide_ = *gapside;


    //filter
     ////usual Jets_eta cut
    if (jet1Et >= 55. && jet2Et >= 55. && fabs(jet1Eta - jet2Eta) < 3)
    {
        if (GapSide_ > 0)
        {
            if ( (jet1Eta < 1. && jet1Eta > -4.) && (jet2Eta < 1. && jet2Eta > -4.) )
            {
                FilterResult = true;
            }
        }
        if (GapSide_ < 0)
        {
            if ( (jet1Eta > -1. && jet1Eta < 4.) && (jet2Eta > -1. && jet2Eta < 4.) )
            {
                FilterResult = true;
            }
        }
    }
//    // stricker Jets_eta cut
//    if (jet1Et >= 55. && jet2Et >= 55. && fabs(jet1Eta - jet2Eta) < 3)
//    {
//        if (GapSide_ > 0)
//        {
//            if ( (jet1Eta < -1. && jet1Eta > -4.) && (jet2Eta < -1. && jet2Eta > -4.) )
//            {
//                FilterResult = true;
//            }
//        }
//        if (GapSide_ < 0)
//        {
//            if ( (jet1Eta > 1. && jet1Eta < 4.) && (jet2Eta > 1. && jet2Eta < 4.) )
//            {
//                FilterResult = true;
//            }
//        }
//    }


    return FilterResult;
}

// ------------ method called once each job just before starting event loop  ------------
void
DijetsFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
DijetsFilter::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(DijetsFilter);
