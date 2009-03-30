// -*- C++ -*-
//
// Package:    KinPlotter
// Class:      KinPlotter
//
/**\class KinPlotter KinPlotter.cc DiffractiveForwardAnalysis/KinPlotter/src/KinPlotter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Sun Mar  1 22:49:46 BRT 2009
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

//GenParticle
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

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
// class decleration
//

class KinPlotter : public edm::EDAnalyzer
{
public:
    explicit KinPlotter(const edm::ParameterSet&);
    ~KinPlotter();


private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------

    //Gen Particles
    TH1F * h_EnergyWeightedEta;

    TH1F * h_GenXiProtonMinus;
    TH1F * h_GenXiProtonPlus;
    TH1F * h_GenXiSumMinus;
    TH1F * h_GenXiSumPlus;

    //Jets
    TH1F * h_EtaJet1;
    TH1F * h_EtaJet2;
    TH1F * h_DeltaEtaJets;

    TH1F * h_EtJet1;
    TH1F * h_EtJet2;
    TH1F * h_AveEtJets;
    TH1F * h_DeltaEtJets;

    TH1F * h_PhiJet1;
    TH1F * h_PhiJet2;
    TH1F * h_DeltaPhiJets;

    TH1F * h_DeltaRJets;

    //tracks
    TH1F * h_MultTracks;



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
KinPlotter::KinPlotter(const edm::ParameterSet& iConfig)

{
    //now do what ever initialization is needed
    //TFileService
    edm::Service<TFileService> fs;

    //Gen Particles
    h_EnergyWeightedEta = fs->make<TH1F>( "h_EnergyWeightedEta"  , "h_EnergyWeightedEta ; #eta (Stable Particles); ", 100,  -15., 15. );

    h_GenXiProtonMinus = fs->make<TH1F>( "h_GenXiProtonMinus"  , "h_GenXiProtonMinus ; Generated #Xi;", 100,  0., 0.2 );
    h_GenXiProtonPlus = fs->make<TH1F>( "h_GenXiProtonPlus"  , "h_GenXiProtonPlus ; Generated #Xi;", 100,  0., 0.2 );
    h_GenXiSumMinus = fs->make<TH1F>( "h_GenXiSumMinus"  , "h_GenXiSumMinus ; Generated #Xi (Sum);", 100,  0., 0.2 );
    h_GenXiSumPlus = fs->make<TH1F>( "h_GenXiSumPlus"  , "h_GenXiSumPlus ; Generated #Xi (Sum);", 100,  0., 0.2 );



    //Jets
    h_EtaJet1 = fs->make<TH1F>( "h_EtaJet1"  , "h_EtaJet1 ; #eta Jet1;", 100,  -5., 5. );
    h_EtaJet2 = fs->make<TH1F>( "h_EtaJet2"  , "h_EtaJet2 ; #eta Jet2;", 100,  -5., 5. );
    h_DeltaEtaJets = fs->make<TH1F>( "h_DeltaEtaJets"  , "h_DeltaEtaJets ; #Delta#eta Leading-Jets;", 100,  0., 15. );

    h_EtJet1 = fs->make<TH1F>( "h_EtJet1"  , "h_EtJet1 ; Et Jet1 (GeV);", 100,  0., 300. );
    h_EtJet2 = fs->make<TH1F>( "h_EtJet2"  , "h_EtJet2 ; Et Jet2 (GeV);", 100,  0., 300. );
    h_AveEtJets = fs->make<TH1F>( "h_AveEtJets"  , "h_AveEtJets ; Average Et (GeV) Leading-Jets;", 100,  0., 300. );
    h_DeltaEtJets = fs->make<TH1F>( "h_DeltaEtJets"  , "h_DeltaEtJets ; #DeltaEt (GeV) Leading-Jets;", 100,  0., 300. );

    h_PhiJet1 = fs->make<TH1F>( "h_PhiJet1"  , "h_PhiJet1 ; #phi Jet1;", 100,  -3.2, 3.2 );
    h_PhiJet2 = fs->make<TH1F>( "h_PhiJet2"  , "h_PhiJet2 ; #phi Jet2;", 100,  -3.2, 3.2 );
    h_DeltaPhiJets = fs->make<TH1F>( "h_DeltaPhiJets"  , "h_DeltaPhiJets ; #Delta#phi Leading-Jets;", 100,  0., 6. );

    h_DeltaRJets = fs->make<TH1F>( "h_DeltaRJets"  , "h_DeltaRJets ; #DeltaR Leading-Jets;", 100,  0., 10. );

    //tracks
    h_MultTracks = fs->make<TH1F>( "h_MultTracks"  , "h_MultTracks ; N_{tracks};", 40,  0., 40. );


}


KinPlotter::~KinPlotter()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
KinPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    // Get the weight
    Handle< double > weight;
    iEvent.getByLabel("WeightProducer","Weight",weight);

    //cout << *weight << endl;

    //Get the GenParticle collection
    Handle<GenParticleCollection> genparticles;
    iEvent.getByLabel( "genParticles", genparticles );

    //xi - proton
    double xiProtonMinusSide = 999.;
    double xiProtonPlusSide = 999.;
    //xi - Sum
    double xiSumMinusSide = 0.;
    double xiSumPlusSide = 0.;
    for ( GenParticleCollection::const_iterator part = genparticles->begin(); part != genparticles->end(); ++ part )
    {
        //xiProton
        if (part->status() == 1 && part->pdgId() == 2212 && fabs(part->pz()) > 3800.)
        {
            if (part->eta() < 0.) xiProtonMinusSide = (1.0-(part->p()/5000));
            if (part->eta() > 0.) xiProtonPlusSide = (1.0-(part->p()/5000));
        }

        //xiSum
        if (part->status() == 1)
        {
            if (part->pdgId() == 2212 && fabs(part->pz()) < 3800.)
            {
                xiSumMinusSide += (part->et()*exp(-part->eta()))/10000.;
                xiSumPlusSide += (part->et()*exp(part->eta()))/10000.;
            }
            if (part->pdgId() != 2212)
            {
                xiSumMinusSide += (part->et()*exp(-part->eta()))/10000.;
                xiSumPlusSide += (part->et()*exp(part->eta()))/10000.;
            }
        }
        //energy weighted eta distribution
        if (part->status() == 1)
        {
            h_EnergyWeightedEta->Fill(part->eta(),(part->energy())*(*weight));
        }
    }
    h_GenXiProtonMinus->Fill(xiProtonMinusSide,*weight);
    h_GenXiProtonPlus->Fill(xiProtonPlusSide,*weight);
    h_GenXiSumMinus->Fill(xiSumMinusSide,*weight);
    h_GenXiSumPlus->Fill(xiSumPlusSide,*weight);




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
    h_EtaJet1->Fill(jet1Eta,*weight);
    h_EtaJet2->Fill(jet2Eta,*weight);
    h_DeltaEtaJets->Fill(fabs(jet1Eta-jet2Eta),*weight);

    h_EtJet1->Fill(jet1Et,*weight);
    h_EtJet2->Fill(jet2Et,*weight);
    h_AveEtJets->Fill((jet1Et+jet2Et)/2.,*weight);
    h_DeltaEtJets->Fill(fabs(jet1Et-jet2Et),*weight);

    h_PhiJet1->Fill(jet1Phi,*weight);
    h_PhiJet2->Fill(jet2Phi,*weight);
    h_DeltaPhiJets->Fill(deltaPhi(jet1Phi,jet2Phi),*weight);

    h_DeltaRJets->Fill(deltaR(jet1Eta,jet1Phi,jet2Eta,jet2Phi),*weight);


    // Get the nTracks
    Handle< int > ntracks;
    iEvent.getByLabel("TrackMultiplicity","trackMult",ntracks);

    h_MultTracks->Fill(*ntracks,*weight);


}


// ------------ method called once each job just before starting event loop  ------------
void
KinPlotter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
KinPlotter::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(KinPlotter);
