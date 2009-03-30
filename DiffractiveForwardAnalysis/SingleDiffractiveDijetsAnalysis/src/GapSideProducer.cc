// -*- C++ -*-
//
// Package:    GapSideProducer
// Class:      GapSideProducer
//
/**\class GapSideProducer GapSideProducer.cc DiffractiveForwardAnalysis/GapSideProducer/src/GapSideProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Sun Mar  1 22:43:30 BRT 2009
// $Id$
//
//


// system include files
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//JetCollection (CaloTower)
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"


using namespace edm;
using namespace std;
//using namespace math;
using namespace reco;

//
// class decleration
//

class GapSideProducer : public edm::EDProducer
{
public:
    explicit GapSideProducer(const edm::ParameterSet&);
    ~GapSideProducer();

private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    double HFNoise;
    bool RandomGapSide;
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
GapSideProducer::GapSideProducer(const edm::ParameterSet& iConfig) :
        HFNoise( iConfig.getParameter<double>( "HFNoise" ) ),
        RandomGapSide( iConfig.getParameter<bool>( "RandomGapSide" ) )
{
    produces< int >( "GapSide" ).setBranchAlias( "GapSide" );



}


GapSideProducer::~GapSideProducer()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GapSideProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    auto_ptr< int > GapSide( new int );

    if (RandomGapSide == false)
    {
        //Get the CaloTower collection
        Handle<CaloTowerCollection> caloTowers;
        iEvent.getByLabel( "towerMaker", caloTowers );

        double HFEnergyMinus = 0;
        double HFEnergyPlus = 0;

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > HFNoise)
                {
                    if (cal->zside() < 0)
                    {
                        HFEnergyMinus += cal->energy();
                    }
                    if (cal->zside() > 0)
                    {
                        HFEnergyPlus += cal->energy();
                    }
                }
            }
        }

        if (HFEnergyPlus >= HFEnergyMinus)
        {
            *GapSide = -1;
        }
        if (HFEnergyPlus < HFEnergyMinus)
        {
            *GapSide = 1;
        }

    }
    else
    {
        srand ( time(NULL) );
        *GapSide = (int)(pow((-1.),(rand() % 2)));
    }
    //cout << *GapSide << endl;

    iEvent.put( GapSide, "GapSide");



}

// ------------ method called once each job just before starting event loop  ------------
void
GapSideProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
GapSideProducer::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(GapSideProducer);
