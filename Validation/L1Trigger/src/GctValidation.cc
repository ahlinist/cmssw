// -*- C++ -*-
//
// Package:    Validation/L1Trigger
// Class:      GctValidation
// 
/**\class GctValidation GctValidation.cc Validation/L1Trigger/src/GctValidation.cc
 
 Description: Make validation plots for Gct emulator
 
 Implementation:
 DQM module
 */
//
// Original Author:  Jim Brooke
//         Created:  Wed Aug 19 13:32:42 CEST 2009
// $Id: GctValidation.cc,v 1.1 2009/08/19 11:55:33 jbrooke Exp $
// $Log: GctValidation.cc,v $
// Revision 1.1  2009/08/19 11:55:33  jbrooke
// first commit
//

/********************************************************************************
 *                                                                              *
 *    Second Author: Kachanon Nirunpong                                         *
 *    Last Update  : Tuesday 28 October 2009                                    *
 *                                                                              *
 ********************************************************************************/

// system include files
#include <memory>

// framework includes
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// DQM includes
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

// RCT/Gct includes
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

// Include these things to be able to convert rank to physically meaningful et.
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1JetEtScaleRcd.h"
#include "CondFormats/L1TObjects/interface/L1GctJetFinderParams.h"
#include "CondFormats/DataRecord/interface/L1GctJetFinderParamsRcd.h"

// Root
#include "TH2.h"
#include "TFile.h"

#include <iostream>
#include <sstream> 
#include <algorithm>
#include <vector>


template <class A>
class SortByRankFromHighToLow
{
	public:
		bool operator()(A object1, A object2)
		{
			return object1.rank() > object2.rank();
		}
};

template <class B>
class SortByEtFromHighToLow
{
	public:
		bool operator()(B object1, B object2)
		{
			return object1.et > object2.et;
		}
};

struct rctJet
{
	double et;
	double eta;
	double phi;
	bool tauVeto;
};

//
// class decleration
//

class GctValidation : public edm::EDAnalyzer
{
	public:
		explicit GctValidation(const edm::ParameterSet&);
		~GctValidation();
	
	private:
		virtual void beginJob() ;
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob() ;
	
		// DQM stuff
		DQMStore * dbe_;

		bool verbose_;
		std::string outputFile_;

		// RCT and GCT stuff
		edm::InputTag rctEmTag;
		edm::InputTag gctIsoEmTag;
		edm::InputTag gctNonIsoEmTag;
		edm::InputTag rctRegionTag;
		edm::InputTag gctEtSumTag;
		edm::InputTag gctCenJetTag;
		edm::InputTag gctForJetTag;
		edm::InputTag gctTauJetTag;

		// histograms for em
		MonitorElement* h_eta0_isoEm;
		MonitorElement* h_eta1_isoEm;
		MonitorElement* h_eta2_isoEm;
		MonitorElement* h_eta3_isoEm;
		MonitorElement* h_eta0_nonIsoEm;
		MonitorElement* h_eta1_nonIsoEm;
		MonitorElement* h_eta2_nonIsoEm;
		MonitorElement* h_eta3_nonIsoEm;
		MonitorElement* h_phi0_isoEm;
		MonitorElement* h_phi1_isoEm;
		MonitorElement* h_phi2_isoEm;
		MonitorElement* h_phi3_isoEm;
		MonitorElement* h_phi0_nonIsoEm;
		MonitorElement* h_phi1_nonIsoEm;
		MonitorElement* h_phi2_nonIsoEm;
		MonitorElement* h_phi3_nonIsoEm;
		MonitorElement* h_rank0_isoEm;
		MonitorElement* h_rank1_isoEm;
		MonitorElement* h_rank2_isoEm;
		MonitorElement* h_rank3_isoEm;
		MonitorElement* h_rank0_nonIsoEm;
		MonitorElement* h_rank1_nonIsoEm;
		MonitorElement* h_rank2_nonIsoEm;
		MonitorElement* h_rank3_nonIsoEm;	
		std::vector<MonitorElement*> h_eta_isoEm;
		std::vector<MonitorElement*> h_phi_isoEm;
		std::vector<MonitorElement*> h_rank_isoEm;
		std::vector<MonitorElement*> h_eta_nonIsoEm;
		std::vector<MonitorElement*> h_phi_nonIsoEm;
		std::vector<MonitorElement*> h_rank_nonIsoEm;
		
		// histogram for jet
		MonitorElement* h_eta0_cenJet;
		MonitorElement* h_eta1_cenJet;
		MonitorElement* h_eta2_cenJet;
		MonitorElement* h_eta3_cenJet;
		MonitorElement* h_eta0_forJet;
		MonitorElement* h_eta1_forJet;
		MonitorElement* h_eta2_forJet;
		MonitorElement* h_eta3_forJet;
		MonitorElement* h_eta0_tauJet;
		MonitorElement* h_eta1_tauJet;
		MonitorElement* h_eta2_tauJet;
		MonitorElement* h_eta3_tauJet;
		MonitorElement* h_phi0_cenJet;
		MonitorElement* h_phi1_cenJet;
		MonitorElement* h_phi2_cenJet;
		MonitorElement* h_phi3_cenJet;
		MonitorElement* h_phi0_forJet;
		MonitorElement* h_phi1_forJet;
		MonitorElement* h_phi2_forJet;
		MonitorElement* h_phi3_forJet;
		MonitorElement* h_phi0_tauJet;
		MonitorElement* h_phi1_tauJet;
		MonitorElement* h_phi2_tauJet;
		MonitorElement* h_phi3_tauJet;
		MonitorElement* h_et0_cenJet;
		MonitorElement* h_et1_cenJet;
		MonitorElement* h_et2_cenJet;
		MonitorElement* h_et3_cenJet;
		MonitorElement* h_et0_forJet;
		MonitorElement* h_et1_forJet;
		MonitorElement* h_et2_forJet;
		MonitorElement* h_et3_forJet;
		MonitorElement* h_et0_tauJet;
		MonitorElement* h_et1_tauJet;
		MonitorElement* h_et2_tauJet;
		MonitorElement* h_et3_tauJet;
		std::vector<MonitorElement*> h_eta_cenJet;
		std::vector<MonitorElement*> h_phi_cenJet;
		std::vector<MonitorElement*> h_et_cenJet;
		std::vector<MonitorElement*> h_eta_forJet;
		std::vector<MonitorElement*> h_phi_forJet;
		std::vector<MonitorElement*> h_et_forJet;
		std::vector<MonitorElement*> h_eta_tauJet;
		std::vector<MonitorElement*> h_phi_tauJet;
		std::vector<MonitorElement*> h_et_tauJet;
	
		// histogram for et sum
		MonitorElement* h_etSum;
	
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

template <typename L1Type>
std::vector<L1Type> SelectL1Object(edm::Handle<std::vector<L1Type> > InputL1ObjectVec);

GctValidation::GctValidation(const edm::ParameterSet& iConfig)
{
	verbose_        = iConfig.getUntrackedParameter<bool>("verbose", false);
	outputFile_     = iConfig.getUntrackedParameter<std::string>("outputFile", "");

	rctEmTag        = iConfig.getParameter<edm::InputTag>("rctEmTag"); 
	gctIsoEmTag     = iConfig.getParameter<edm::InputTag>("gctIsoEmTag");
	gctNonIsoEmTag  = iConfig.getParameter<edm::InputTag>("gctNonIsoEmTag");

	rctRegionTag    = iConfig.getParameter<edm::InputTag>("rctRegionTag");
	gctEtSumTag     = iConfig.getParameter<edm::InputTag>("gctEtSumTag");
	
	gctCenJetTag    = iConfig.getParameter<edm::InputTag>("gctCenJetTag");
	gctForJetTag    = iConfig.getParameter<edm::InputTag>("gctForJetTag");
	gctTauJetTag    = iConfig.getParameter<edm::InputTag>("gctTauJetTag");
	
	if(outputFile_.size() != 0)
	{
		edm::LogInfo("OutputInfo") << " GCT validation histograms will be saved to " << outputFile_.c_str();
	}
	else
	{
		edm::LogInfo("OutputInfo") << " GCT validation histograms will NOT be saved";
	}
	
	// get hold of back-end interface
	dbe_ = edm::Service<DQMStore>().operator->();           
	if(dbe_)
	{
		if(verbose_)
		{ 
			dbe_->setVerbose(1); 
			dbe_->showDirStructure();
		} 
		else
		{
			dbe_->setVerbose(0); 
		}
	}
	
	dbe_->setCurrentFolder("L1Trigger/GCTVal");

}

GctValidation::~GctValidation()
{
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void GctValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	/************************************************** EM **************************************************/

	// Get a collection of GCT isolated em
	edm::Handle<std::vector<L1GctEmCand> > tempGctIsoEm;
	iEvent.getByLabel(gctIsoEmTag, tempGctIsoEm);

	// Get a collection of GCT non-isolated em
	edm::Handle<std::vector<L1GctEmCand> > tempGctNonIsoEm;
	iEvent.getByLabel(gctNonIsoEmTag, tempGctNonIsoEm);

	//Get a collection of RCT em
	edm::Handle<std::vector<L1CaloEmCand> > tempRctEm;
	iEvent.getByLabel(rctEmTag, tempRctEm);

	// Create vectors of GCT isolated em and GCT non-isolated em that have bunch crossing index of 0.
	// The elements of the vectors are sorted descendingly by rank. Both of them has 4 elements.
	std::vector<L1GctEmCand> gctIsoEm    = SelectL1Object<L1GctEmCand>(tempGctIsoEm);
	std::vector<L1GctEmCand> gctNonIsoEm = SelectL1Object<L1GctEmCand>(tempGctNonIsoEm);

	// Create a vector of RCT em that has bunch crossing index of 0.
	// The elements of the vector are sorted descendingly by rank. The vector has 18 elements.
	std::vector<L1CaloEmCand> rctEm = SelectL1Object<L1CaloEmCand>(tempRctEm);

	// Create 4-element vectors of RCT isolated em and RCT non-isolated em out of rctEm.
	// Each vector's elements are (automatically) sorted by rank descendingly.
	std::vector<L1CaloEmCand> rctIsoEm;
	std::vector<L1CaloEmCand> rctNonIsoEm;
	for(std::vector<L1CaloEmCand>::const_iterator i = rctEm.begin(); i != rctEm.end(); ++i)
	{
		if(i->isolated() && rctIsoEm.size() < 4)
		{
			rctIsoEm.push_back(*i);
		}
		if(!(i->isolated()) && rctNonIsoEm.size() < 4)
		{
			rctNonIsoEm.push_back(*i);
		}
	}

	/********************************************************************************************************/

	/************************************************ ET sum ************************************************/

	// Create a vector of RCT region.
	// RCT region vector has 18 x 22 = 396 elements.
	// Every element has bunch crossing index of 0.
	edm::Handle<std::vector<L1CaloRegion> > tempRctRegion;
	iEvent.getByLabel(rctRegionTag, tempRctRegion);
	
	// Create a vector of GCT ET Sum.
	// GCT ET sum vector has 3 elements, one with bx = -1, one with bx = 0, and one with bx = 1
	edm::Handle<std::vector<L1GctEtTotal> > tempGctEtSum;
	iEvent.getByLabel(gctEtSumTag, tempGctEtSum);
	
	// Create a vector of GCT ET Sum that has bx = 0
	// and calculate RCT ET sum.
	double rctEtSum = 0.0;
	std::vector<L1CaloRegion> rctRegion;
	for(std::vector<L1CaloRegion>::const_iterator i = tempRctRegion->begin(); i != tempRctRegion->end(); ++i)
	{
		if(i->bx() == 0)
		{
			rctRegion.push_back(*i);
			rctEtSum = rctEtSum + i->et();
		}
	}
	
	// Calculate GCT ET sum.
	double gctEtSum = 0.0;
	for(std::vector<L1GctEtTotal>::const_iterator i = tempGctEtSum->begin(); i != tempGctEtSum->end(); ++i)
	{
		if(i->bx() == 0)
		{
			gctEtSum = i->et();
		}
	}

	/********************************************************************************************************/

	/************************************************* Jets *************************************************/
	
	// I am going to run a simple jet finder algorithm over the RCT regions.
	// It will look for all possible 3x3 RCT regions, if the central region has highest et among 9 regions, call that 3x3 RCT regions a jet.
	// Eta and phi of the jet is eta and phi of the central region, and et of the jet is summation of et over all 9 regions. 
	
	// Declare and initialise 3 arrays for storing et, eta, and phi of 3x3 RCT regions.
	double rctSubRegionEt[3][3];
	double rctSubRegionEta[3][3];
	double rctSubRegionPhi[3][3];
	for(int i=0; i<=2; ++i)
	{
		for(int j=0; j<=2; ++j)
		{
			rctSubRegionEt[i][j]  = -1;
			rctSubRegionEta[i][j] = -1;
			rctSubRegionPhi[i][j] = -1;
		}
	}
	
	std::vector<rctJet> rctJets;  // a vector to store rctJet

	// Loop over rctRegion(created in ET Sum part) and look for 3x3 regions around the considering region. 
	// After all iteration, I will get a vector of rctJet. (rctJet is defined using struct at the top of the page after #include things)
	for(std::vector<L1CaloRegion>::const_iterator rr1 = rctRegion.begin(); rr1 != rctRegion.end(); ++rr1)
	{
		int eta = rr1->id().ieta();  // eta of the considering region
		int phi = rr1->id().iphi();  // phi of the considering region
		
		double rctJetsEt_  = 0.0;
		
		// The region that has eta equals 0 or 21 has only 6 regions around it (inclunding itself). The others have 9 regions around them (including themself). 
		// Before I loop over regions around the considering region(the region that rr1 point to), I need a correct initial and final eta to loop over.

		int i_min;
		int i_max;
		if(eta==0)			// The region at eta = 0 has no region below it.
		{
			i_min = 1;
			i_max = 2;
		}
		else if(eta==21)	// The region at eta = 21 has no region above it.
		{
			i_min = 0;
			i_max = 1;
		}
		else				// The region at another eta has regions below and above it.
		{
			i_min = 0;
			i_max = 2;
		}
		
		// Now, I am going to loop over eta(i) and phi(j) around the considering region.
		// Eta of the regions around the considering region (I will call it subEta) is depended on eta of the considering region and i.
		// Phi of the regions around the considering region (I will call it subPhi) is depended on phi of the considering region and i.
		for(int i=i_min; i<=i_max; ++i)
		{
			for(int j=0; j<=2; ++j)
			{
				int subEta = eta+i-1;  // subEta is eta of a region around the considering region.
				int subPhi = phi+j-1;  // subPhi is eta of a region around the considering region.
				// Phi has periodic boundary.
				if(subPhi == -1)
				{
					subPhi = 17;
				}
				if(subPhi == 18)
				{
					subPhi = 0;
				}

				// Loop over rctRegion and choose the RCT region that has (eta,phi) matched with (subEta,subPhi).
				std::vector<L1CaloRegion>::const_iterator RR;
				for(std::vector<L1CaloRegion>::const_iterator rr2 = rctRegion.begin(); rr2 != rctRegion.end(); ++rr2)
				{

					if(rr2->id().ieta() == (unsigned)subEta && rr2->id().iphi() == (unsigned)subPhi)
					{
						RR = rr2;
					}
				}

				rctSubRegionEta[i][j] = RR->id().ieta();
				rctSubRegionPhi[i][j] = RR->id().iphi();
				rctSubRegionEt[i][j]  = RR->et();

				// Calculate jets' et
				rctJetsEt_ = rctJetsEt_ + rctSubRegionEt[i][j];
			}
		}
		

		// Check if the middle region has the highest et. If yes, call it a jet.
		bool isJet = true;
		for(int j=0; j<=2; ++j)
		{
			for(int i=i_min; i<=i_max; ++i)
			{
				if(rctSubRegionEt[1][1] < rctSubRegionEt[i][j])
				{
					isJet = false;
				}
			}
		}
		double rctJetsEta_ = 0.0;
		double rctJetsPhi_ = 0.0;

		// If it is a jet, create rctJet object and push it back to a vector.
		// The rctJet will have eta and phi of the considering region. rctJets' et is the one calclated above.
		// Also check if the considering region is tau.
		if(isJet)
		{
			rctJetsEta_ = rr1->id().ieta();
			rctJetsPhi_ = rr1->id().iphi();
			
			rctJet jet;
			jet.eta = rr1->id().ieta();
			jet.phi = rr1->id().iphi();
			jet.et  = rctJetsEt_;
			jet.tauVeto = rr1->tauVeto();
			
			rctJets.push_back(jet);
		}
		
		// Sort the RCT jets vector by et.
		std::sort(rctJets.begin(), rctJets.end(), SortByEtFromHighToLow<rctJet>());

		/*** Code testing ***/
		/*
		if(isJet)
		{
			std::cout << "isJet = " << isJet << std::endl;
			std::cout << "RctRegion Coordinate: (" << rr1->id().ieta() << ", " << rr1->id().iphi() << ")" << std::endl;
			for(int j=0; j<=2; ++j)
			{
				for(int i=i_min; i<=i_max; ++i)
				{
					std::cout << "(" << rctSubRegionEta[i][j] << "," << rctSubRegionPhi[i][j] << ")   ";
					if(i%2 == 0 && i!=0)
					{
						std::cout << std:: endl;
					}
				}
			}
			std::cout << std::endl;
			
			for(int j=0; j<=2; ++j)
			{
				for(int i=i_min; i<=i_max; ++i)
				{
					std::cout << rctSubRegionEt[i][j] << "   ";
					if(i%2 == 0 && i!=0)
					{
						std::cout << std:: endl;
					}
				}
			}
			
			std::cout << "Jet's (eta, phi) = " << "(" << rctJetsEta_ << ", " << rctJetsPhi_ << ")" << std::endl;
			std::cout << "Jet's et         = " << rctJetsEt_ << std::endl;
			std::cout << std::endl << std::endl << std::endl << std::endl;
		}
		*/
		/********************/

	}

	// Separate RCT jets into 3 categories; forward jets, central jets, and tau jets.
	std::vector<rctJet> rctCenJets;
	std::vector<rctJet> rctForJets;
	std::vector<rctJet> rctTauJets;
	for(std::vector<rctJet>::const_iterator i = rctJets.begin(); i != rctJets.end(); ++i)
	{
		if((i->eta < 4 || i->eta > 17) && rctForJets.size() < 4)
		{
			rctForJets.push_back(*i);
		}
		if((i->eta >= 4 || i->eta <= 17) && i->tauVeto && rctCenJets.size() < 4)
		{
			rctCenJets.push_back(*i);
		}
		if((i->eta >= 4 || i->eta <= 17) && !(i->tauVeto) && rctTauJets.size() < 4)
		{
			rctTauJets.push_back(*i);
		}
	}

	// Convert rank to et
    edm::ESHandle< L1CaloEtScale > etScale ;
    iSetup.get< L1JetEtScaleRcd >().get( etScale ) ;

	// A constant used to get RCT region's et in GeV
	edm::ESHandle< L1GctJetFinderParams > jfPars ;
	iSetup.get< L1GctJetFinderParamsRcd >().get( jfPars ) ;
	double lsbForEt = jfPars.product()->getRgnEtLsbGeV();
	
	/*** Code testing ***/
	/*
	for(std::vector<rctJet>::const_iterator i = rctCenJets.begin(); i != rctCenJets.end(); ++i)
	{
		std::cout << "rct central jet coordinate = " << "(" << i->eta << "," << i->phi << ")   " ;
		std::cout << "rct central jet et         = " << i->et << std::endl;
	}
	for(std::vector<rctJet>::const_iterator i = rctTauJets.begin(); i != rctTauJets.end(); ++i)
	{
		std::cout << "rct tau jet coordinate     = " << "(" << i->eta << "," << i->phi << ")   " ;
		std::cout << "rct tau jet et             = " << i->et << std::endl;
	}
	for(std::vector<rctJet>::const_iterator i = rctForJets.begin(); i != rctForJets.end(); ++i)
	{
		std::cout << "rct forward jet coordinate = " << "(" << i->eta << "," << i->phi << ")   " ;
		std::cout << "rct forward jet et         = " << i->et << std::endl;
	}
	*/
	/********************/

	// Get collections of L1GctJetCand type. There are 3 collections; central jets, forward jets, and tau jets.
	edm::Handle<std::vector<L1GctJetCand> > tempGctCenJet;
	iEvent.getByLabel(gctCenJetTag, tempGctCenJet);
	edm::Handle<std::vector<L1GctJetCand> > tempGctForJet;
	iEvent.getByLabel(gctForJetTag, tempGctForJet);
	edm::Handle<std::vector<L1GctJetCand> > tempGctTauJet;
	iEvent.getByLabel(gctTauJetTag, tempGctTauJet);

	// Create vectors of GCT central jets, forward jets, and tau jets that have bunch crossing index of 0.
	// The elements of the vectors are sorted descendingly by rank. All of them has 4 elements.
	std::vector<L1GctJetCand> gctCenJet  = SelectL1Object<L1GctJetCand>(tempGctCenJet);
	std::vector<L1GctJetCand> gctForJet  = SelectL1Object<L1GctJetCand>(tempGctForJet);
	std::vector<L1GctJetCand> gctTauJet  = SelectL1Object<L1GctJetCand>(tempGctTauJet);

	/********************************************************************************************************/

	// Fill histograms.
	for(unsigned int i = 0; i <= 3; ++i)
	{
		if(rctIsoEm.at(i).rank() != 0 && gctIsoEm.at(i).rank() != 0)
		{
			h_eta_isoEm.at(i) -> Fill(rctIsoEm.at(i).regionId().ieta(), gctIsoEm.at(i).regionId().ieta());
			h_phi_isoEm.at(i) -> Fill(rctIsoEm.at(i).regionId().iphi(), gctIsoEm.at(i).regionId().iphi());
		}
		if(rctNonIsoEm.at(i).rank() != 0 && gctNonIsoEm.at(i).rank() != 0)
		{
			h_eta_nonIsoEm.at(i) -> Fill(rctNonIsoEm.at(i).regionId().ieta(), gctNonIsoEm.at(i).regionId().ieta());
			h_phi_nonIsoEm.at(i) -> Fill(rctNonIsoEm.at(i).regionId().iphi(), gctNonIsoEm.at(i).regionId().iphi());
		}
		h_rank_isoEm.at(i)    -> Fill(rctIsoEm.at(i).rank(),    gctIsoEm.at(i).rank());
		h_rank_nonIsoEm.at(i) -> Fill(rctNonIsoEm.at(i).rank(), gctNonIsoEm.at(i).rank());

		if(i < rctCenJets.size())  // In some events, there are less than 4 central jets.
		{
			if(rctCenJets.at(i).et != 0 &&  etScale->et(gctCenJet.at(i).rank()) != 0)
			{
				h_eta_cenJet.at(i)    -> Fill(rctCenJets.at(i).eta, gctCenJet.at(i).regionId().ieta());
				h_phi_cenJet.at(i)    -> Fill(rctCenJets.at(i).phi, gctCenJet.at(i).regionId().iphi());
			}
			h_et_cenJet.at(i)     -> Fill(lsbForEt*rctCenJets.at(i).et,  etScale->et(gctCenJet.at(i).rank()));
		}
		if(i < rctForJets.size())  // In some events, there are less than 4 forward jets.
		{
			if(rctForJets.at(i).et != 0 &&  etScale->et(gctForJet.at(i).rank()) != 0)
			{
				h_eta_forJet.at(i)    -> Fill(rctForJets.at(i).eta, gctForJet.at(i).regionId().ieta());
				h_phi_forJet.at(i)    -> Fill(rctForJets.at(i).phi, gctForJet.at(i).regionId().iphi());
			}
			h_et_forJet.at(i)     -> Fill(lsbForEt*rctForJets.at(i).et,  etScale->et(gctForJet.at(i).rank()));
		}
		if(i < rctTauJets.size())  // In some events, there are less than 4 tau jets.
		{
			if(rctTauJets.at(i).et != 0 &&  etScale->et(gctTauJet.at(i).rank()) != 0)
			{
				h_eta_tauJet.at(i)    -> Fill(rctTauJets.at(i).eta, gctTauJet.at(i).regionId().ieta());
				h_phi_tauJet.at(i)    -> Fill(rctTauJets.at(i).phi, gctTauJet.at(i).regionId().iphi());
			}
			h_et_tauJet.at(i)     -> Fill(lsbForEt*rctTauJets.at(i).et,  etScale->et(gctTauJet.at(i).rank()));
		}
		
	}
	h_etSum -> Fill(rctEtSum, gctEtSum);

}

void GctValidation::beginJob()
{
	h_eta0_isoEm      = dbe_->book2D("h_eta0_isoEm",     "eta0  of isolated e/gammar",      22, 0, 22, 22, 0, 22);
	h_eta1_isoEm      = dbe_->book2D("h_eta1_isoEm",     "eta1  of isolated e/gammar",      22, 0, 22, 22, 0, 22);
	h_eta2_isoEm      = dbe_->book2D("h_eta2_isoEm",     "eta2  of isolated e/gammar",      22, 0, 22, 22, 0, 22);
	h_eta3_isoEm      = dbe_->book2D("h_eta3_isoEm",     "eta3  of isolated e/gammar",      22, 0, 22, 22, 0, 22);
	h_eta0_nonIsoEm   = dbe_->book2D("h_eta0_nonIsoEm",  "eta0  of non-isolated e/gammar",  22, 0, 22, 22, 0, 22);
	h_eta1_nonIsoEm   = dbe_->book2D("h_eta1_nonIsoEm",  "eta1  of non-isolated e/gammar",  22, 0, 22, 22, 0, 22);
	h_eta2_nonIsoEm   = dbe_->book2D("h_eta2_nonIsoEm",  "eta2  of non-isolated e/gammar",  22, 0, 22, 22, 0, 22);
	h_eta3_nonIsoEm   = dbe_->book2D("h_eta3_nonIsoEm",  "eta3  of non-isolated e/gammar",  22, 0, 22, 22, 0, 22);
	h_phi0_isoEm      = dbe_->book2D("h_phi0_isoEm",     "phi0  of isolated e/gammar",      22, 0, 22, 22, 0, 22);
	h_phi1_isoEm      = dbe_->book2D("h_phi1_isoEm",     "phi1  of isolated e/gammar",      18, 0, 18, 18, 0, 18);
	h_phi2_isoEm      = dbe_->book2D("h_phi2_isoEm",     "phi2  of isolated e/gammar",      18, 0, 18, 18, 0, 18);
	h_phi3_isoEm      = dbe_->book2D("h_phi3_isoEm",     "phi3  of isolated e/gammar",      18, 0, 18, 18, 0, 18);
	h_phi0_nonIsoEm   = dbe_->book2D("h_phi0_nonIsoEm",  "phi0  of non-isolated e/gammar",  18, 0, 18, 18, 0, 18);
	h_phi1_nonIsoEm   = dbe_->book2D("h_phi1_nonIsoEm",  "phi1  of non-isolated e/gammar",  18, 0, 18, 18, 0, 18);
	h_phi2_nonIsoEm   = dbe_->book2D("h_phi2_nonIsoEm",  "phi2  of non-isolated e/gammar",  18, 0, 18, 18, 0, 18);
	h_phi3_nonIsoEm   = dbe_->book2D("h_phi3_nonIsoEm",  "phi3  of non-isolated e/gammar",  18, 0, 18, 18, 0, 18);
	h_rank0_isoEm     = dbe_->book2D("h_rank0_isoEm",    "rank0  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank1_isoEm     = dbe_->book2D("h_rank1_isoEm",    "rank1  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank2_isoEm     = dbe_->book2D("h_rank2_isoEm",    "rank2  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank3_isoEm     = dbe_->book2D("h_rank3_isoEm",    "rank3  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank0_nonIsoEm  = dbe_->book2D("h_rank0_nonIsoEm", "rank0  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank1_nonIsoEm  = dbe_->book2D("h_rank1_nonIsoEm", "rank1  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank2_nonIsoEm  = dbe_->book2D("h_rank2_nonIsoEm", "rank2  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank3_nonIsoEm  = dbe_->book2D("h_rank3_nonIsoEm", "rank3  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100); 
	
	h_eta0_cenJet     = dbe_->book2D("h_eta0_cenJet", "eta0 of central jet", 22, 0, 22, 22, 0, 22);
	h_eta1_cenJet     = dbe_->book2D("h_eta1_cenJet", "eta1 of central jet", 22, 0, 22, 22, 0, 22);
	h_eta2_cenJet     = dbe_->book2D("h_eta2_cenJet", "eta2 of central jet", 22, 0, 22, 22, 0, 22);
	h_eta3_cenJet     = dbe_->book2D("h_eta3_cenJet", "eta3 of central jet", 22, 0, 22, 22, 0, 22);
	h_eta0_forJet     = dbe_->book2D("h_eta0_forJet", "eta0 of forward jet", 22, 0, 22, 22, 0, 22);
	h_eta1_forJet     = dbe_->book2D("h_eta1_forJet", "eta1 of forward jet", 22, 0, 22, 22, 0, 22);
	h_eta2_forJet     = dbe_->book2D("h_eta2_forJet", "eta2 of forward jet", 22, 0, 22, 22, 0, 22);
	h_eta3_forJet     = dbe_->book2D("h_eta3_forJet", "eta3 of forward jet", 22, 0, 22, 22, 0, 22);
	h_eta0_tauJet     = dbe_->book2D("h_eta0_tauJet", "eta0 of tau jet",     22, 0, 22, 22, 0, 22);
	h_eta1_tauJet     = dbe_->book2D("h_eta1_tauJet", "eta1 of tau jet",     22, 0, 22, 22, 0, 22);
	h_eta2_tauJet     = dbe_->book2D("h_eta2_tauJet", "eta2 of tau jet",     22, 0, 22, 22, 0, 22);
	h_eta3_tauJet     = dbe_->book2D("h_eta3_tauJet", "eta3 of tau jet",     22, 0, 22, 22, 0, 22);
	h_phi0_cenJet     = dbe_->book2D("h_phi0_cenJet", "phi0 of central jet", 18, 0, 18, 18, 0, 18);
	h_phi1_cenJet     = dbe_->book2D("h_phi1_cenJet", "phi1 of central jet", 18, 0, 18, 18, 0, 18);
	h_phi2_cenJet     = dbe_->book2D("h_phi2_cenJet", "phi2 of central jet", 18, 0, 18, 18, 0, 18);
	h_phi3_cenJet     = dbe_->book2D("h_phi3_cenJet", "phi3 of central jet", 18, 0, 18, 18, 0, 18);
	h_phi0_forJet     = dbe_->book2D("h_phi0_forJet", "phi0 of forward jet", 18, 0, 18, 18, 0, 18);
	h_phi1_forJet     = dbe_->book2D("h_phi1_forJet", "phi1 of forward jet", 18, 0, 18, 18, 0, 18);
	h_phi2_forJet     = dbe_->book2D("h_phi2_forJet", "phi2 of forward jet", 18, 0, 18, 18, 0, 18);
	h_phi3_forJet     = dbe_->book2D("h_phi3_forJet", "phi3 of forward jet", 18, 0, 18, 18, 0, 18);
	h_phi0_tauJet     = dbe_->book2D("h_phi0_tauJet", "phi0 of tau jet",     18, 0, 18, 18, 0, 18);
	h_phi1_tauJet     = dbe_->book2D("h_phi1_tauJet", "phi1 of tau jet",     18, 0, 18, 18, 0, 18);
	h_phi2_tauJet     = dbe_->book2D("h_phi2_tauJet", "phi2 of tau jet",     18, 0, 18, 18, 0, 18);
	h_phi3_tauJet     = dbe_->book2D("h_phi3_tauJet", "phi3 of tau jet",     18, 0, 18, 18, 0, 18);
	h_et0_cenJet     = dbe_->book2D("h_et0_cenJet", "et0 of central jet",    100, 0, 100, 100, 0, 100);
	h_et1_cenJet     = dbe_->book2D("h_et1_cenJet", "et1 of central jet",    100, 0, 100, 100, 0, 100);
	h_et2_cenJet     = dbe_->book2D("h_et2_cenJet", "et2 of central jet",    100, 0, 100, 100, 0, 100);
	h_et3_cenJet     = dbe_->book2D("h_et3_cenJet", "et3 of central jet",    100, 0, 100, 100, 0, 100);
	h_et0_forJet     = dbe_->book2D("h_et0_forJet", "et0 of forward jet",    100, 0, 100, 100, 0, 100);
	h_et1_forJet     = dbe_->book2D("h_et1_forJet", "et1 of forward jet",    100, 0, 100, 100, 0, 100);
	h_et2_forJet     = dbe_->book2D("h_et2_forJet", "et2 of forward jet",    100, 0, 100, 100, 0, 100);
	h_et3_forJet     = dbe_->book2D("h_et3_forJet", "et3 of forward jet",    100, 0, 100, 100, 0, 100);
	h_et0_tauJet     = dbe_->book2D("h_et0_tauJet", "et0 of tau jet",        100, 0, 100, 100, 0, 100);
	h_et1_tauJet     = dbe_->book2D("h_et1_tauJet", "et1 of tau jet",        100, 0, 100, 100, 0, 100);
	h_et2_tauJet     = dbe_->book2D("h_et2_tauJet", "et2 of tau jet",        100, 0, 100, 100, 0, 100);
	h_et3_tauJet     = dbe_->book2D("h_et3_tauJet", "et3 of tau jet",        100, 0, 100, 100, 0, 100);
	
	h_etSum           = dbe_->book2D("h_etSum", "ET sum", 1000, 0, 1000, 1000, 0, 1000);

	h_eta_isoEm.push_back(h_eta0_isoEm);
	h_eta_isoEm.push_back(h_eta1_isoEm);
	h_eta_isoEm.push_back(h_eta2_isoEm);
	h_eta_isoEm.push_back(h_eta3_isoEm);
	h_phi_isoEm.push_back(h_phi0_isoEm);
	h_phi_isoEm.push_back(h_phi1_isoEm);
	h_phi_isoEm.push_back(h_phi2_isoEm);
	h_phi_isoEm.push_back(h_phi3_isoEm);
	h_rank_isoEm.push_back(h_rank0_isoEm);
	h_rank_isoEm.push_back(h_rank1_isoEm);
	h_rank_isoEm.push_back(h_rank2_isoEm);
	h_rank_isoEm.push_back(h_rank3_isoEm);
	h_eta_nonIsoEm.push_back(h_eta0_nonIsoEm);
	h_eta_nonIsoEm.push_back(h_eta1_nonIsoEm);
	h_eta_nonIsoEm.push_back(h_eta2_nonIsoEm);
	h_eta_nonIsoEm.push_back(h_eta3_nonIsoEm);
	h_phi_nonIsoEm.push_back(h_phi0_nonIsoEm);
	h_phi_nonIsoEm.push_back(h_phi1_nonIsoEm);
	h_phi_nonIsoEm.push_back(h_phi2_nonIsoEm);
	h_phi_nonIsoEm.push_back(h_phi3_nonIsoEm);
	h_rank_nonIsoEm.push_back(h_rank0_nonIsoEm);
	h_rank_nonIsoEm.push_back(h_rank1_nonIsoEm);
	h_rank_nonIsoEm.push_back(h_rank2_nonIsoEm);
	h_rank_nonIsoEm.push_back(h_rank3_nonIsoEm);

	h_eta_cenJet.push_back(h_eta0_cenJet);
	h_eta_cenJet.push_back(h_eta1_cenJet);
	h_eta_cenJet.push_back(h_eta2_cenJet);
	h_eta_cenJet.push_back(h_eta3_cenJet);
	h_phi_cenJet.push_back(h_phi0_cenJet);
	h_phi_cenJet.push_back(h_phi1_cenJet);
	h_phi_cenJet.push_back(h_phi2_cenJet);
	h_phi_cenJet.push_back(h_phi3_cenJet);
	h_et_cenJet.push_back(h_et0_cenJet);
	h_et_cenJet.push_back(h_et1_cenJet);
	h_et_cenJet.push_back(h_et2_cenJet);
	h_et_cenJet.push_back(h_et3_cenJet);
	h_eta_forJet.push_back(h_eta0_forJet);
	h_eta_forJet.push_back(h_eta1_forJet);
	h_eta_forJet.push_back(h_eta2_forJet);
	h_eta_forJet.push_back(h_eta3_forJet);
	h_phi_forJet.push_back(h_phi0_forJet);
	h_phi_forJet.push_back(h_phi1_forJet);
	h_phi_forJet.push_back(h_phi2_forJet);
	h_phi_forJet.push_back(h_phi3_forJet);
	h_et_forJet.push_back(h_et0_forJet);
	h_et_forJet.push_back(h_et1_forJet);
	h_et_forJet.push_back(h_et2_forJet);
	h_et_forJet.push_back(h_et3_forJet);
	h_eta_tauJet.push_back(h_eta0_tauJet);
	h_eta_tauJet.push_back(h_eta1_tauJet);
	h_eta_tauJet.push_back(h_eta2_tauJet);
	h_eta_tauJet.push_back(h_eta3_tauJet);
	h_phi_tauJet.push_back(h_phi0_tauJet);
	h_phi_tauJet.push_back(h_phi1_tauJet);
	h_phi_tauJet.push_back(h_phi2_tauJet);
	h_phi_tauJet.push_back(h_phi3_tauJet);
	h_et_tauJet.push_back(h_et0_tauJet);
	h_et_tauJet.push_back(h_et1_tauJet);
	h_et_tauJet.push_back(h_et2_tauJet);
	h_et_tauJet.push_back(h_et3_tauJet);

}

void GctValidation::endJob()
{
	if(outputFile_.size() != 0 && dbe_)
	{
		dbe_->save(outputFile_);
	}
	
	return;
}


// A template function that take a vecor of L1 GCT object,
// select ones that has bunch crossing index = 0,
// push_back them to a vector,
// sort the vector descendingly by rank,
// and return that vector.
template <typename L1Type>
std::vector<L1Type> SelectL1Object(edm::Handle<std::vector<L1Type> > InputL1ObjectVec)
{
	typedef std::vector<L1Type> l1Collection;
	l1Collection L1ObjectVec;
	for (typename l1Collection::const_iterator i = InputL1ObjectVec->begin(); i != InputL1ObjectVec->end(); ++i)
	{
		if(i->bx() == 0)
		{
			L1ObjectVec.push_back(*i);
		}
	}
	std::sort(L1ObjectVec.begin(), L1ObjectVec.end(), SortByRankFromHighToLow<L1Type>());
	
	return L1ObjectVec;

}

//define this as a plug-in
DEFINE_FWK_MODULE(GctValidation);
