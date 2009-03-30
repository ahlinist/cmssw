// -*- C++ -*-
//
// Package:    SdDijetsPlotter
// Class:      SdDijetsPlotter
//
/**\class SdDijetsPlotter SdDijetsPlotter.cc DiffractiveForwardAnalysis/SdDijetsPlotter/src/SdDijetsPlotter.cc

 Description: <one line class summary>
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Araújo (CMS)
//         Created:  Fri Feb 27 21:07:40 BRT 2009
// $Id$
//
//


// system include files
#include <memory>
#include <vector>

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

//CASTOR Tower
#include "DataFormats/CastorReco/interface/CastorJet.h"

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

class SdDijetsPlotter : public edm::EDAnalyzer
{
public:
    explicit SdDijetsPlotter(const edm::ParameterSet&);
    ~SdDijetsPlotter();


private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    //read gap side info and return correspondent multiplicity vector
    template <class T>
    pair< T , T >
    Gap2Mult(int GapSide, T Minus, T Plus)
    {
        pair< T , T > gap2mult;
        if (GapSide == -1)
        {
            gap2mult.first = Minus;
            gap2mult.second= Plus;
        }
        else
        {
            gap2mult.first = Plus;
            gap2mult.second= Minus;
        }
        return (gap2mult);
    }

//    pair< vector<int> , vector<int> > Gap2Mult(int GapSide, vector<int> Minus, vector<int> Plus);

    // ----------member data ---------------------------

    string GapSideSource;
    double HFNoise;
    double CASTORNoise;
    int ntrackCut;


    //gap side
    TH2D * h_Mult_HFlow_HFhigh;
    TH2D * h_Mult_HF_CASTOR;
    TH2D * h_Mult_HF_GENCASTOR;
    TH2D * h_Mult_GENCASTOR_CASTOR;

    //opposite to gap side
    TH2D * h_Mult_HFlow_HFhigh_Opp;
    TH2D * h_Mult_HF_CASTOR_Opp;
    TH2D * h_Mult_HF_GENCASTOR_Opp;
    TH2D * h_Mult_GENCASTOR_CASTOR_Opp;

    //plus side
    TH2D * h_Mult_HFlow_HFhigh_Plus;
    TH2D * h_Mult_HF_CASTOR_Plus;
    TH2D * h_Mult_HF_GENCASTOR_Plus;
    TH2D * h_Mult_GENCASTOR_CASTOR_Plus;


    //minus side
    TH2D * h_Mult_HFlow_HFhigh_Minus;
    TH2D * h_Mult_HF_CASTOR_Minus;
    TH2D * h_Mult_HF_GENCASTOR_Minus;
    TH2D * h_Mult_GENCASTOR_CASTOR_Minus;

    //gap side
    TH1D * h_GapSide;


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
SdDijetsPlotter::SdDijetsPlotter(const edm::ParameterSet& iConfig) :
        GapSideSource( iConfig.getParameter<string>( "GapSideSource" ) ),
        HFNoise( iConfig.getParameter<double>( "HFNoise" ) ),
        CASTORNoise( iConfig.getParameter<double>( "CASTORNoise" ) ),
        ntrackCut( iConfig.getParameter<int>( "ntrackCut" ) )

{
    //now do what ever initialization is needed

    edm::Service<TFileService> fs;

    //gap side
    h_Mult_HFlow_HFhigh = fs->make<TH2D>( "h_Mult_HFlow_HFhigh"  , "h_Mult_HFlow_HFhigh ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR = fs->make<TH2D>( "h_Mult_HF_CASTOR"  , "h_Mult_HF_CASTOR ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_GENCASTOR = fs->make<TH2D>( "h_Mult_HF_GENCASTOR"  , "h_Mult_HF_GENCASTOR ; n HF towers; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_GENCASTOR_CASTOR = fs->make<TH2D>( "h_Mult_GENCASTOR_CASTOR"  , "h_Mult_GENCASTOR_CASTOR ; n CASTOR; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );



    //opposite to gap side
    h_Mult_HFlow_HFhigh_Opp = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Opp"  , "h_Mult_HFlow_HFhigh ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Opp = fs->make<TH2D>( "h_Mult_HF_CASTOR_Opp"  , "h_Mult_HF_CASTOR ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_GENCASTOR_Opp = fs->make<TH2D>( "h_Mult_HF_GENCASTOR_Opp"  , "h_Mult_HF_GENCASTOR ; n HF towers; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_GENCASTOR_CASTOR_Opp = fs->make<TH2D>( "h_Mult_GENCASTOR_CASTOR_Opp"  , "h_Mult_GENCASTOR_CASTOR_Opp ; n CASTOR; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );


    //plus side
    h_Mult_HFlow_HFhigh_Plus = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Plus"  , "h_Mult_HFlow_HFhigh_Plus ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Plus = fs->make<TH2D>( "h_Mult_HF_CASTOR_Plus"  , "h_Mult_HF_CASTOR_Plus ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_GENCASTOR_Plus = fs->make<TH2D>( "h_Mult_HF_GENCASTOR_Plus"  , "h_Mult_HF_GENCASTOR_Plus ; n HF towers; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_GENCASTOR_CASTOR_Plus = fs->make<TH2D>( "h_Mult_GENCASTOR_CASTOR_Plus"  , "h_Mult_GENCASTOR_CASTOR_Plus ; n CASTOR; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );


    //minus side
    h_Mult_HFlow_HFhigh_Minus = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Minus"  , "h_Mult_HFlow_HFhigh_Minus ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Minus = fs->make<TH2D>( "h_Mult_HF_CASTOR_Minus"  , "h_Mult_HF_CASTOR_Minus ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_GENCASTOR_Minus = fs->make<TH2D>( "h_Mult_HF_GENCASTOR_Minus"  , "h_Mult_HF_GENCASTOR_Minus ; n HF towers; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_GENCASTOR_CASTOR_Minus = fs->make<TH2D>( "h_Mult_GENCASTOR_CASTOR_Minus"  , "h_Mult_GENCASTOR_CASTOR_Minus ; n CASTOR; n CASTOR (Gen Level) ; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );

    //gap side
    h_GapSide = fs->make<TH1D>( "h_GapSide"  , "h_GapSide ; Gap Side; N events ( 10 pb^{-1} )", 2,  -1, 1);


}


SdDijetsPlotter::~SdDijetsPlotter()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SdDijetsPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    // Get the weight
    Handle< double > weight;
    iEvent.getByLabel("WeightProducer","Weight",weight);


    // Get the gap side
    Handle< int > gapside;
    iEvent.getByLabel(GapSideSource,"GapSide",gapside);
    //cout << GapSide << endl;
    int GapSide = *gapside;

//    cout << GapSide << endl;














    //Get the CaloTower collection
    Handle<CaloTowerCollection> caloTowers;
    iEvent.getByLabel( "towerMaker", caloTowers );
//    for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
//    {
//        cout << cal->energy() << endl;
//        h_calo_energy->Fill( cal->energy() );
//    }


    //Get the CastorTower collection
    Handle<CastorTowerCollection> castorTowers;
    iEvent.getByLabel( "CastorTowerReco", castorTowers );
//    for ( CastorTowerCollection::const_iterator cas = castorTowers->begin(); cas != castorTowers->end(); ++ cas )
//    {
//        cout << "CASTOR TOWER: " << cas->energy() << endl;
//        a++;
//        //h_calo_energy->Fill( cal->energy() );
//    }
//
    //Get the GenParticle collection
    Handle<GenParticleCollection> genparticles;
    iEvent.getByLabel( "genParticles", genparticles );
//    for ( GenParticleCollection::const_iterator part = genparticles->begin(); part != genparticles->end(); ++ part )
//    {
//        cout << "GEN PARTICLE: " << part->eta() << endl;
//        //a++;
//        //h_calo_energy->Fill( cal->energy() );
//    }



    //Castor - Gen
    double CastorGenSlicePlus[16];
    double CastorGenSliceMinus[16];
    for (int j = 0; j < 16; j++)
    {
        CastorGenSlicePlus[j] = 0.;
        CastorGenSliceMinus[j] = 0.;
    }

    for ( GenParticleCollection::const_iterator part = genparticles->begin(); part != genparticles->end(); ++ part )
    {
        //CastorGen
        if (part->status() == 1 && fabs(part->eta()) > 5.2 && fabs(part->eta()) < 6.6)
        {
            // set sector
            int sector = -1;
            for (int j = 0; j < 16; j++)
            {
                double a = -M_PI + j*0.3927;
                double b = -M_PI + (j+1)*0.3927;
                if ( (part->phi() > a) && (part->phi() <= b))
                {
                    sector = j;
                }
            }

            if (part->eta() > 0. && part->energy() >= 10.)
            {
                CastorGenSlicePlus[sector] += part->energy();
            }
            if (part->eta() < 0. && part->energy() >= 10.)
            {
                CastorGenSliceMinus[sector] += part->energy();
            }
        }
    }


    //Fill: CastorGen
    int nCastorSlicesPlus = 0;
    for (int j = 0; j < 16; j++)
    {
        if (CastorGenSlicePlus[j] >= 10.)
        {
            nCastorSlicesPlus++;
        }
    }
    int castorGenPlus = nCastorSlicesPlus;
//    cout << nCastorSlicesPlus << endl;

    int nCastorSlicesMinus = 0;
    for (int j = 0; j < 16; j++)
    {
        if (CastorGenSliceMinus[j] >= 10.)
        {
            nCastorSlicesMinus++;
        }
    }
    int castorGenMinus = nCastorSlicesMinus;
    //cout << nCastorSlicesMinus << endl;





    //hf multiplicities
    int nHFPlus = 0;
    int nHFMinus = 0;
    int nLowHFPlus = 0;
    int nLowHFMinus = 0;
    int nHighHFPlus = 0;
    int nHighHFMinus = 0;

    for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
    {
        double hfNoiseThreshold = HFNoise;
        //nHF
        if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
        {
            if (cal->energy() > hfNoiseThreshold)
            {
                if (cal->zside() < 0)
                {
                    nHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nHFPlus++;
                }
            }
        }

        //nHFSlices - Low
        if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) <= 4.0)
        {
            if (cal->energy() > hfNoiseThreshold)
            {
                if (cal->zside() < 0)
                {
                    nLowHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nLowHFPlus++;
                }

            }
        }

        //nHFSlices - High
        if (fabs(cal->eta()) > 4.0 && fabs(cal->eta()) < 5.2)
        {
            if (cal->energy() > hfNoiseThreshold)
            {
                if (cal->zside() < 0)
                {
                    nHighHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nHighHFPlus++;
                }

            }
        }
    }

    int hfMinus = nHFMinus;
    int hfPlus = nHFPlus;
    int lowetaMinus = nLowHFMinus;
    int lowetaPlus = nLowHFPlus;
    int highetaMinus = nHighHFMinus;
    int highetaPlus = nHighHFPlus;



    //castor multiplicities
    int nCASTORPlus=0;
    int nCASTORMinus=0;

    for ( CastorTowerCollection::const_iterator cas = castorTowers->begin(); cas != castorTowers->end(); ++ cas )
    {
        double castorNoiseThreshold = CASTORNoise;
        if (cas->energy() > castorNoiseThreshold)
        {
            if (cas->eta() < 0)
            {
//                    cout << "oi PLus" << endl;
                nCASTORMinus++;
            }
            if (cas->eta() > 0)
            {
//                    cout << "oi MInus" << endl;
                nCASTORPlus++;
            }
        }
    }
    int castorMinus = nCASTORMinus;
    int castorPlus = nCASTORPlus;




    pair < int , int > castor = Gap2Mult < int > (GapSide, castorMinus, castorPlus);
    pair < int , int > hf = Gap2Mult < int > (GapSide, hfMinus, hfPlus);
    pair < int , int > loweta = Gap2Mult < int > (GapSide, lowetaMinus, lowetaPlus);
    pair < int , int > highteta = Gap2Mult < int > (GapSide, highetaMinus, highetaPlus);
    pair < int , int > castorgen = Gap2Mult < int > (GapSide, castorGenMinus, castorGenPlus);


    if (GapSide > 0) h_GapSide->Fill(0.9,*weight);
    if (GapSide < 0) h_GapSide->Fill(-0.9,*weight);


    // Get the nTracks
    Handle< int > ntracks;
    iEvent.getByLabel("TrackMultiplicity","trackMult",ntracks);

    if (*ntracks <= ntrackCut)
    {
        //gap side
        h_Mult_HFlow_HFhigh->Fill(loweta.first,highteta.first,*weight);
        h_Mult_GENCASTOR_CASTOR->Fill(castorgen.first,castor.first,*weight);

        if (GapSide < 0)
        {
            h_Mult_HF_CASTOR->Fill(hf.first,castor.first,*weight);
            h_Mult_HF_GENCASTOR->Fill(hf.first,castorgen.first,*weight);
        }

        //opposite to gap side
        h_Mult_HFlow_HFhigh_Opp->Fill(loweta.second,highteta.second,*weight);
        h_Mult_GENCASTOR_CASTOR_Opp->Fill(castorgen.second,castor.second,*weight);
        if (GapSide < 0)
        {
            h_Mult_HF_CASTOR_Opp->Fill(hf.second,castor.second,*weight);
            h_Mult_HF_GENCASTOR_Opp->Fill(hf.second,castorgen.second,*weight);
        }

        //plus side
        h_Mult_HFlow_HFhigh_Plus->Fill(lowetaPlus,highetaPlus,*weight);
        h_Mult_HF_CASTOR_Plus->Fill(hfPlus,castorPlus,*weight);
        h_Mult_HF_GENCASTOR_Plus->Fill(hfPlus,castorGenPlus,*weight);
        h_Mult_GENCASTOR_CASTOR_Plus->Fill(castorGenPlus,castorPlus,*weight);


        //minus side
        h_Mult_HFlow_HFhigh_Minus->Fill(lowetaMinus,highetaMinus,*weight);
        h_Mult_HF_CASTOR_Minus->Fill(hfMinus,castorMinus,*weight);
        h_Mult_HF_GENCASTOR_Minus->Fill(hfMinus,castorGenMinus,*weight);
        h_Mult_GENCASTOR_CASTOR_Minus->Fill(castorGenMinus,castorMinus,*weight);

    }
}


// ------------ method called once each job just before starting event loop  ------------
void
SdDijetsPlotter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
SdDijetsPlotter::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(SdDijetsPlotter);
