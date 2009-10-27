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
 *    Last Update  : Tuesday 29 September 2009                                  *
 *                                                                              *
 ********************************************************************************/

template <class A>
class SortByRankFromHighToLow
{
	public:
		bool operator()(A object1, A object2)
		{
			return object1.rank() > object2.rank();
		}
};


// system include files
#include <memory>

// framework includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// DQM includes
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

// RCT/Gct includes
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

// Root
#include "TH2.h"
#include "TFile.h"

#include <iostream>
#include <sstream> 
#include <algorithm>
#include <vector>

//
// class decleration
//

class GctValidation : public edm::EDAnalyzer
{
	public:
		explicit GctValidation(const edm::ParameterSet&);
		~GctValidation();

		std::vector<L1GctEmCand>  SelectGctEm(edm::Handle<std::vector<L1GctEmCand> > tempGctEm);
		std::vector<L1CaloEmCand> SelectRctEm(edm::Handle<std::vector<L1CaloEmCand> > tempRctEm);
	
	private:
		virtual void beginJob() ;
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob() ;
	
		// DQM stuff
		DQMStore * dbe_;

		bool verbose_;
		std::string outputFile_;

		// Gct stuff
		edm::InputTag rctEmTag;
		edm::InputTag gctIsoEmTag;
		edm::InputTag gctNonIsoEmTag;
	
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

GctValidation::GctValidation(const edm::ParameterSet& iConfig)
{
	verbose_        = iConfig.getUntrackedParameter<bool>("verbose", false);
	outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile", "");
	rctEmTag        = iConfig.getParameter<edm::InputTag>("rctEmTag"); 
	gctIsoEmTag     = iConfig.getParameter<edm::InputTag>("gctIsoEmTag");
	gctNonIsoEmTag  = iConfig.getParameter<edm::InputTag>("gctNonIsoEmTag");


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
	
	edm::Handle<std::vector<L1GctEmCand> > tempGctIsoEm;
	iEvent.getByLabel(gctIsoEmTag, tempGctIsoEm);
	
	edm::Handle<std::vector<L1GctEmCand> > tempGctNonIsoEm;
	iEvent.getByLabel(gctNonIsoEmTag, tempGctNonIsoEm);
	
	edm::Handle<std::vector<L1CaloEmCand> > tempRctEm;
	iEvent.getByLabel(rctEmTag, tempRctEm);

	std::vector<L1GctEmCand>  gctIsoEm    = SelectGctEm(tempGctIsoEm);
	std::vector<L1GctEmCand>  gctNonIsoEm = SelectGctEm(tempGctNonIsoEm);

	std::vector<L1CaloEmCand> rctEm       = SelectRctEm(tempRctEm);
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

	for(int i = 0; i <= 3; ++i)
	{
		h_eta_isoEm.at(i)     -> Fill(rctIsoEm.at(i).regionId().ieta(),    gctIsoEm.at(i).regionId().ieta());
		h_phi_isoEm.at(i)     -> Fill(rctIsoEm.at(i).regionId().iphi(),    gctIsoEm.at(i).regionId().iphi());
		h_rank_isoEm.at(i)    -> Fill(rctIsoEm.at(i).rank(),               gctIsoEm.at(i).rank());
		h_eta_nonIsoEm.at(i)  -> Fill(rctNonIsoEm.at(i).regionId().ieta(), gctNonIsoEm.at(i).regionId().ieta());
		h_phi_nonIsoEm.at(i)  -> Fill(rctNonIsoEm.at(i).regionId().iphi(), gctNonIsoEm.at(i).regionId().iphi());
		h_rank_nonIsoEm.at(i) -> Fill(rctNonIsoEm.at(i).rank(),            gctNonIsoEm.at(i).rank());
	}

}

void GctValidation::beginJob()
{
	h_eta0_isoEm      = dbe_->book2D("h_eta0_isoem",     "eta0  of isolated e/gammar",      15, 0, 15, 15, 0, 15);
	h_eta1_isoEm      = dbe_->book2D("h_eta1_isoem",     "eta1  of isolated e/gammar",      15, 0, 15, 15, 0, 15);
	h_eta2_isoEm      = dbe_->book2D("h_eta2_isoem",     "eta2  of isolated e/gammar",      15, 0, 15, 15, 0, 15);
	h_eta3_isoEm      = dbe_->book2D("h_eta3_isoem",     "eta3  of isolated e/gammar",      15, 0, 15, 15, 0, 15);
	h_eta0_nonIsoEm   = dbe_->book2D("h_eta0_nonIsoem",  "eta0  of non-isolated e/gammar",  15, 0, 15, 15, 0, 15);
	h_eta1_nonIsoEm   = dbe_->book2D("h_eta1_nonIsoem",  "eta1  of non-isolated e/gammar",  15, 0, 15, 15, 0, 15);
	h_eta2_nonIsoEm   = dbe_->book2D("h_eta2_nonIsoem",  "eta2  of non-isolated e/gammar",  15, 0, 15, 15, 0, 15);
	h_eta3_nonIsoEm   = dbe_->book2D("h_eta3_nonIsoem",  "eta3  of non-isolated e/gammar",  15, 0, 15, 15, 0, 15);
	h_phi0_isoEm      = dbe_->book2D("h_phi0_isoem",     "phi0  of isolated e/gammar",      19, 0, 19, 19, 0, 19);
	h_phi1_isoEm      = dbe_->book2D("h_phi1_isoem",    "phi1  of isolated e/gammar",       19, 0, 19, 19, 0, 19);
	h_phi2_isoEm      = dbe_->book2D("h_phi2_isoem",     "phi2  of isolated e/gammar",      19, 0, 19, 19, 0, 19);
	h_phi3_isoEm      = dbe_->book2D("h_phi3_isoem",     "phi3  of isolated e/gammar",      19, 0, 19, 19, 0, 19);
	h_phi0_nonIsoEm   = dbe_->book2D("h_phi0_nonIsoem",  "phi0  of non-isolated e/gammar",  19, 0, 19, 19, 0, 19);
	h_phi1_nonIsoEm   = dbe_->book2D("h_phi1_nonIsoem",  "phi1  of non-isolated e/gammar",  19, 0, 19, 19, 0, 19);
	h_phi2_nonIsoEm   = dbe_->book2D("h_phi2_nonIsoem",  "phi2  of non-isolated e/gammar",  19, 0, 19, 19, 0, 19);
	h_phi3_nonIsoEm   = dbe_->book2D("h_phi3_nonIsoem",  "phi3  of non-isolated e/gammar",  19, 0, 19, 19, 0, 19);
	h_rank0_isoEm     = dbe_->book2D("h_rank0_isoem",    "rank0  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank1_isoEm     = dbe_->book2D("h_rank1_isoem",    "rank1  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank2_isoEm     = dbe_->book2D("h_rank2_isoem",    "rank2  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank3_isoEm     = dbe_->book2D("h_rank3_isoem",    "rank3  of isolated e/gammar",     100, 0, 100, 100, 0, 100);
	h_rank0_nonIsoEm  = dbe_->book2D("h_rank0_nonIsoEm", "rank0  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank1_nonIsoEm  = dbe_->book2D("h_rank1_nonIsoEm", "rank1  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank2_nonIsoEm  = dbe_->book2D("h_rank2_nonIsoEm", "rank2  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100);
	h_rank3_nonIsoEm  = dbe_->book2D("h_rank3_nonIsoEm", "rank3  of non-isolated e/gammar", 100, 0, 100, 100, 0, 100); 

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
}

void GctValidation::endJob()
{
	if(outputFile_.size() != 0 && dbe_)
	{
		dbe_->save(outputFile_);
	}
	
	return;
}

std::vector<L1GctEmCand> GctValidation::SelectGctEm(edm::Handle<std::vector<L1GctEmCand> > tempGctEm)
{
	std::vector<L1GctEmCand> gctEm;
	for(std::vector<L1GctEmCand>::const_iterator i = tempGctEm->begin(); i != tempGctEm->end(); ++i)
	{
		if(i->bx() == 0)
		{
			gctEm.push_back(*i);
		}
	}
	std::sort(gctEm.begin(), gctEm.end(), SortByRankFromHighToLow<L1GctEmCand>());
	return gctEm;
}

std::vector<L1CaloEmCand> GctValidation::SelectRctEm(edm::Handle<std::vector<L1CaloEmCand> > tempRctEm)
{
	std::vector<L1CaloEmCand> rctEm;
	for(std::vector<L1CaloEmCand>::const_iterator i = tempRctEm->begin(); i != tempRctEm->end(); ++i)
	{
		if(i->bx() == 0)
		{
			rctEm.push_back(*i);
		}
	}
	std::sort(rctEm.begin(), rctEm.end(), SortByRankFromHighToLow<L1CaloEmCand>());
	return rctEm;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GctValidation);
