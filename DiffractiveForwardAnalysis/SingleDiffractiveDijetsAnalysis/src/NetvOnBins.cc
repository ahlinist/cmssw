// -*- C++ -*-
//
// Package:    NetvOnBins
// Class:      NetvOnBins
//
/**\class NetvOnBins NetvOnBins.cc DiffractiveForwardAnalysis/NetvOnBins/src/NetvOnBins.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Thu Mar  5 22:51:20 BRT 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//ROOT
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>

using namespace edm;
using namespace std;
//using namespace math;
//using namespace reco;
//
// class decleration
//

class NetvOnBins : public edm::EDAnalyzer
{
public:
    explicit NetvOnBins(const edm::ParameterSet&);
    ~NetvOnBins();


private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    string generator;
    TH1F * h_NevtsOnBins;
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
NetvOnBins::NetvOnBins(const edm::ParameterSet& iConfig) :
        generator( iConfig.getParameter<string>( "generator" ) )

{
        //TFileService
    edm::Service<TFileService> fs;

    //now do what ever initialization is needed
    h_NevtsOnBins = fs->make<TH1F>( "h_NevtsOnBins"  , "h_NevtsOnBins", 9,  0., 9. );


}


NetvOnBins::~NetvOnBins()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
NetvOnBins::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    //EventScale
    double geneventscale;
    if (generator == "pomwig")
    {
        Handle<double> ktValue;
        iEvent.getByLabel("genEventKTValue", ktValue);
         geneventscale= *ktValue;
    }
    if (generator == "pythia")
    {
        Handle<double> scale;
        iEvent.getByLabel("genEventScale", scale);
        geneventscale = *scale;
    }



    if (geneventscale > 15. && geneventscale <= 30.)
    {
        h_NevtsOnBins->Fill(0.5);
    }
    if (geneventscale > 30. && geneventscale <= 80.)
    {
        h_NevtsOnBins->Fill(1.5);
    }
    if (geneventscale > 80. && geneventscale <= 170.)
    {
        h_NevtsOnBins->Fill(2.5);
    }
    if (geneventscale > 170. && geneventscale <= 300.)
    {
        h_NevtsOnBins->Fill(3.5);
    }
    if (geneventscale > 300. && geneventscale <= 470.)
    {
        h_NevtsOnBins->Fill(4.5);
    }
    if (geneventscale > 470. && geneventscale <= 800.)
    {
        h_NevtsOnBins->Fill(5.5);
    }
    if (geneventscale > 800. && geneventscale <= 1400.)
    {
        h_NevtsOnBins->Fill(6.5);
    }
    if (geneventscale > 1400. && geneventscale <= 2200.)
    {
        h_NevtsOnBins->Fill(7.5);
    }
    if (geneventscale > 2200.)
    {
        h_NevtsOnBins->Fill(8.5);
    }


}


// ------------ method called once each job just before starting event loop  ------------
void
NetvOnBins::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
NetvOnBins::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(NetvOnBins);
