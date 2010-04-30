#ifndef VgAnalysis_VgKit_interface_PatKitHelper_h
#define VgAnalysis_VgKit_interface_PatKitHelper_h




//-------------------------------------------------------------------------------------
//
// Original Author:  Salvatore Rappoccio
//         Created:  Mon Jul  7 10:37:27 CDT 2008
// $Id: PatKitHelper.h,v 1.2 2010/04/21 15:55:04 lgray Exp $
//
// Revision History:
//       -  Sal Rappoccio, Mon Jul  7 10:37:27 CDT 2008
//          Creation of object to make SK more inline with Framework advice.
//          This includes removing PatAnalyzerKit as a base class, and anything that
//          needs that functionality should use this class instead of deriving from
//          PatAnalyzerKit.
//-------------------------------------------------------------------------------------
#include "ElectroWeakAnalysis/MultiBosons/interface/PhysicsHistograms.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/View.h"


namespace pat {

  class PatKitHelper {
  public:

    PatKitHelper(edm::ParameterSet const & parameters);
    ~PatKitHelper();

    // Pull out a struct for the axis limits from the config file
    PhysicsHistograms::KinAxisLimits getAxisLimits( std::string name );


    // Book histograms
    void bookHistos( edm::EDProducer * producer );

    // Get handles
    void getHandles( edm::Event  & event,
		     edm::Handle<edm::View<pat::Muon> > &     muonHandle,
		     edm::Handle<edm::View<pat::Electron> > & electronHandle,
		     edm::Handle<edm::View<pat::Tau> > &      tauHandle,
		     edm::Handle<edm::View<pat::Jet> > &      jetHandle,
		     edm::Handle<edm::View<pat::MET> > &      METHandle,
		     edm::Handle<edm::View<pat::Photon> > &   photonHandle,
		     edm::Handle<std::vector<reco::RecoChargedCandidate> > &   trackHandle,
		     edm::Handle<std::vector<reco::GenParticle> > & genParticles,
		     edm::Handle<reco::CandidateView>  &      zmumuHandle

		     );


    // fill histograms
    void fillHistograms( edm::Event & event,
			 edm::Handle<edm::View<pat::Muon> > &     muonHandle,
			 edm::Handle<edm::View<pat::Electron> > & electronHandle,
			 edm::Handle<edm::View<pat::Tau> > &      tauHandle,
			 edm::Handle<edm::View<pat::Jet> > &      jetHandle,
			 edm::Handle<edm::View<pat::MET> > &      METHandle,
			 edm::Handle<edm::View<pat::Photon> > &   photonHandle,
			 edm::Handle<std::vector<reco::RecoChargedCandidate> > &   trackHandle,
			 edm::Handle<std::vector<reco::GenParticle> > & genParticles
			 );


    // Function to add ntuple variables to the EDProducer
    void addNtupleVar ( edm::EDProducer * prod, std::string name, std::string type );

    // Save ntuple variables to event evt
    void saveNtuple (  edm::Event & event,
		       const std::vector<pat::PhysVarHisto*> & ntvars);

    // Helper function template to write objects to event
    template <class T>
      void saveNtupleVar(  edm::Event & event,
			   std::string name, T value);

    // Helper function template to write vectors of objects to event
    template <class T>
      void saveNtupleVec(  edm::Event & event,
			   std::string name, const std::vector<T> & invec);


    // verbose switch
    int verboseLevel_;

    // Keep a version of the parameter set in question
    edm::ParameterSet         parameters_;

    // Here is where the histograms go
    PhysicsHistograms  *      physHistos_;

    // File service for histograms
    edm::Service<TFileService> fs_;

    // List of ntuple variables
    std::vector< pat::PhysVarHisto* > ntVars_ ;


    // run and event numbers
    pat::PhysVarHisto *  h_runNumber_;
    pat::PhysVarHisto *  h_eventNumber_;

  };

}


#endif
