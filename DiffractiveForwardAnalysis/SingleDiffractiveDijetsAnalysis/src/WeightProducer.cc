// -*- C++ -*-
//
// Package:    WeightProducer
// Class:      WeightProducer
//
/**\class WeightProducer WeightProducer.cc DiffractiveForwardAnalysis/WeightProducer/src/WeightProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Thu Mar  5 22:43:36 BRT 2009
// $Id: WeightProducer.cc,v 1.1 2009/03/30 04:04:26 fsilva Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace std;
//using namespace math;
//using namespace reco;

//
// class decleration
//

class WeightProducer : public edm::EDProducer
{
public:
    explicit WeightProducer(const edm::ParameterSet&);
    ~WeightProducer();

private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    string generator;
    double Lumi;
    double RGSP;
    double Eff;
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
WeightProducer::WeightProducer(const edm::ParameterSet& iConfig) :
        generator( iConfig.getParameter<string>( "generator" ) ),
        Lumi( iConfig.getParameter<double>( "Lumi" ) ),
        RGSP( iConfig.getParameter<double>( "RGSP" ) ),
        Eff( iConfig.getParameter<double>( "Eff" ) )

{
    produces< double >( "Weight" ).setBranchAlias( "Weight" );


}


WeightProducer::~WeightProducer()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
WeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    auto_ptr< double > Weight( new double );


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


    //provisory
    //double CS = 1.;

    // skim_v38
    if (generator == "pomwig")
    {
        if (geneventscale > 30. && geneventscale <= 80.)
        {
            *Weight = (300000.*RGSP*Lumi*Eff)/(5600914.);
        }
        if (geneventscale > 80.)
        {
            *Weight = (4000.*RGSP*Lumi*Eff)/(1231451.);
        }
    }

    if (generator == "pythia")
    {
        if (geneventscale > 15. && geneventscale <= 30.)
        {
            *Weight = (1457159248.*Lumi*Eff)/(21184096.);
        }
        if (geneventscale > 30. && geneventscale <= 80.)
        {
            *Weight = (109057220.4*Lumi*Eff)/(11443625.);
        }
        if (geneventscale > 80. && geneventscale <= 170.)
        {
            *Weight = (1934639.567*Lumi*Eff)/(9217463.);
        }
        if (geneventscale > 170. && geneventscale <= 300.)
        {
            *Weight = (62562.87713*Lumi*Eff)/(9206001.);
        }
        if (geneventscale > 300. && geneventscale <= 470.)
        {
            *Weight = (3664.608301*Lumi*Eff)/(9210406.);
        }
        if (geneventscale > 470. && geneventscale <= 800.)
        {
            *Weight = (315.5131272*Lumi*Eff)/(9980402.);
        }
        if (geneventscale > 800. && geneventscale <= 1400.)
        {
            *Weight = (11.9419745*Lumi*Eff)/(9440071.);
        }
        if (geneventscale > 1400. && geneventscale <= 2200.)
        {
            *Weight = (0.1720187189*Lumi*Eff)/(4694722.);
        }
        if (geneventscale > 2200.)
        {
            *Weight = (0.0014207778*Lumi*Eff)/(5035140.);
        }
    }





    iEvent.put( Weight, "Weight");


}

// ------------ method called once each job just before starting event loop  ------------
void
WeightProducer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
WeightProducer::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(WeightProducer);
