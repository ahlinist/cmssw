// -*- C++ -*-
//
// Package:    PATLeptonSVAssociatorSelector
// Class:      PATLeptonSVAssociatorSelector
// 
/**\class PATLeptonSVAssociatorSelector PATLeptonSVAssociatorSelector.cc MyCode/PATLeptonSVAssociatorSelector/src/PATLeptonSVAssociatorSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonSVAssociatorSelector.cc,v 1.1 2008/10/16 11:58:03 gurrola Exp $
//
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonSVAssociatorSelector.h"

PATLeptonSVAssociatorSelector::PATLeptonSVAssociatorSelector(const edm::ParameterSet& iConfig)
{
  src_ = iConfig.getParameter<edm::InputTag>("src");
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  TrackProd_ = iConfig.getParameter<std::string>("TrackProducer");
  VertexProd_ = iConfig.getParameter<std::string>("VertexProducer");
  vfPSet = iConfig.getParameter<edm::ParameterSet>("VFParameters");
  VertexMathingDistance_ = iConfig.getParameter<double>("VertexMathingDistance");
  VertexMuonMaxDistance_ = iConfig.getParameter<double>("VertexMuonMaxDistance");
  MinAssociatedSVCut_ = iConfig.getParameter<double>("MinAssociatedSVCut");
  MaxAssociatedSVCut_ = iConfig.getParameter<double>("MaxAssociatedSVCut");
  if(LeptonType_ == "Muon") {
    produces<PATMuonCollection>();
  } else {
    produces<PATElectronCollection>();
  }
}


PATLeptonSVAssociatorSelector::~PATLeptonSVAssociatorSelector() {}

void PATLeptonSVAssociatorSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  if(LeptonType_ == "Muon") {
    Handle<View<pat::Muon> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Muon> leptons = *leptonHandle;

    // Tracks
    Handle<reco::TrackCollection> TrackHandle;
    iEvent.getByLabel(TrackProd_, TrackHandle);

    // Transient Tracks
    ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    std::vector<reco::TransientTrack> t_tks = (*theB).build(TrackHandle);

    // Standard primary vertex collection
    Handle<reco::VertexCollection> theVertexCollection;
    iEvent.getByLabel(VertexProd_, theVertexCollection);

    std::auto_ptr<PATMuonCollection> pOut(new PATMuonCollection);

    // Define the vertex vectors to be used and their iterators
    std::vector<math::XYZVector> PriVcontainer;
    std::vector<math::XYZVector> SecVcontainer;
    std::vector<math::XYZVector> SecVcandidate;
    std::vector<math::XYZVector>::iterator secVit;
    std::vector<math::XYZVector>::iterator priVit;

    // Use the Configurable Reconstructor to determine the list of vertices
    SecVcontainer.clear();
    if(t_tks.size() > 1) {
      ConfigurableVertexReconstructor vf(vfPSet);
      std::vector<TransientVertex> tv = vf.vertices(t_tks);
      for(unsigned int j = 0; j < tv.size(); j++) {
        if(tv.at(j).isValid() ) {
          double vertexX = tv.at(j).position().x();
          double vertexY = tv.at(j).position().y();
          double vertexZ = tv.at(j).position().z();
          math::XYZVector Allvert(vertexX,vertexY,vertexZ);
          SecVcontainer.push_back(Allvert);
        }
      }
    }

    // Obtain the 3D position of the primary vertices and store the 3D information in to a vector
    PriVcontainer.clear();
    if(theVertexCollection->size() > 0) {
      for(reco::VertexCollection::const_iterator vitt = theVertexCollection->begin(); vitt != theVertexCollection->end();++vitt) {
        math::XYZVector vert(vitt->x(),vitt->y(),vitt->z());
        PriVcontainer.push_back(vert);
      }
    }

    // Seperate SV's from PV's
    SecVcandidate.clear();
    for(secVit = SecVcontainer.begin(); secVit != SecVcontainer.end(); secVit++) {
      bool SecondaryVertexFlag = true;
      for(priVit = PriVcontainer.begin(); priVit != PriVcontainer.end(); priVit++) {
        double deltaX2 = ((*secVit).x() - (*priVit).x()) * ((*secVit).x() - (*priVit).x());
        double deltaY2 = ((*secVit).y() - (*priVit).y()) * ((*secVit).y() - (*priVit).y());
        double deltaZ2 = ((*secVit).z() - (*priVit).z()) * ((*secVit).z() - (*priVit).z());
        if(sqrt(deltaX2 + deltaY2 + deltaZ2) < VertexMathingDistance_) {SecondaryVertexFlag = false;}
      }
      if(SecondaryVertexFlag) {SecVcandidate.push_back((*secVit));}
    }

    for(View<pat::Muon>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      int associatedSecV = 0;
      // First loop over transient tracks in order to find the transient track that is also the muon track
      std::vector<math::XYZVector>::iterator secVitt;
      for(std::vector<reco::TransientTrack>::const_iterator itT = t_tks.begin(); itT != t_tks.end() ; itT++ ) {
        if( ( ((*itT).track().pt()) == (trk->track()->pt())) &&
            ( ((*itT).track().eta()) == (trk->track()->eta())) &&
            ( ((*itT).track().phi()) == (trk->track()->phi())) ) {
          if(SecVcandidate.size() > 0) {
            for(secVitt = SecVcandidate.begin(); secVitt != SecVcandidate.end(); secVitt++) {
              GlobalPoint theVertexPoint((*secVitt).x(),(*secVitt).y(),(*secVitt).z());
              math::XYZVector vert(secVitt->x(),secVitt->y(),secVitt->z());
              FreeTrajectoryState theMuonNearVertex = (*itT).trajectoryStateClosestToPoint(theVertexPoint).theState();
              math::XYZVector pcb(theMuonNearVertex.position().x(),theMuonNearVertex.position().y(),theMuonNearVertex.position().z());
              math::XYZVector dd = pcb - vert;
              double magd = sqrt((dd.x() * dd.x()) + (dd.y() * dd.y()) + (dd.z() * dd.z()));
              if(magd < VertexMuonMaxDistance_) {associatedSecV++;}
            }
          }
        }
      }
      if ( ( associatedSecV >= MinAssociatedSVCut_ ) && ( associatedSecV <= MaxAssociatedSVCut_ ) ) {pOut->push_back( * trk );}
      std::cout << "Vertex Matching Distance      :  " << VertexMathingDistance_ << std::endl;
      std::cout << "Vertex to Muon Max Distance   :  " << VertexMuonMaxDistance_ << std::endl;
      std::cout << "Minimum # of associated SV's  :  " << MinAssociatedSVCut_ << std::endl;
      std::cout << "Maximum # of associated SV's  :  " << MaxAssociatedSVCut_ << std::endl;
      std::cout << "Number of Secondary Vertices  :  " << SecVcandidate.size() << std::endl;
      std::cout << "Associated Secondary Vertices :  " << associatedSecV << std::endl;
    }

    iEvent.put(pOut);

  } else if(LeptonType_ == "Electron") {
    Handle<View<pat::Electron> > leptonHandle;
    iEvent.getByLabel(src_,leptonHandle);
    View<pat::Electron> leptons = *leptonHandle;

    // Tracks
    Handle<reco::TrackCollection> TrackHandle;
    iEvent.getByLabel(TrackProd_, TrackHandle);

    // Transient Tracks
    ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    std::vector<reco::TransientTrack> t_tks = (*theB).build(TrackHandle);

    // Standard primary vertex collection
    Handle<reco::VertexCollection> theVertexCollection;
    iEvent.getByLabel(VertexProd_, theVertexCollection);

    std::auto_ptr<PATElectronCollection> pOut(new PATElectronCollection);

    // Define the vertex vectors to be used and their iterators
    std::vector<math::XYZVector> PriVcontainer;
    std::vector<math::XYZVector> SecVcontainer;
    std::vector<math::XYZVector> SecVcandidate;
    std::vector<math::XYZVector>::iterator secVit;
    std::vector<math::XYZVector>::iterator priVit;

    // Use the Configurable Reconstructor to determine the list of vertices
    SecVcontainer.clear();
    if(t_tks.size() > 1) {
      ConfigurableVertexReconstructor vf(vfPSet);
      std::vector<TransientVertex> tv = vf.vertices(t_tks);
      for(unsigned int j = 0; j < tv.size(); j++) {
        if(tv.at(j).isValid() ) {
          double vertexX = tv.at(j).position().x();
          double vertexY = tv.at(j).position().y();
          double vertexZ = tv.at(j).position().z();
          math::XYZVector Allvert(vertexX,vertexY,vertexZ);
          SecVcontainer.push_back(Allvert);
        }
      }
    }

    // Obtain the 3D position of the primary vertices and store the 3D information in to a vector
    PriVcontainer.clear();
    if(theVertexCollection->size() > 0) {
      for(reco::VertexCollection::const_iterator vitt = theVertexCollection->begin(); vitt != theVertexCollection->end();++vitt) {
        math::XYZVector vert(vitt->x(),vitt->y(),vitt->z());
        PriVcontainer.push_back(vert);
      }
    }

    // Seperate SV's from PV's
    SecVcandidate.clear();
    for(secVit = SecVcontainer.begin(); secVit != SecVcontainer.end(); secVit++) {
      bool SecondaryVertexFlag = true;
      for(priVit = PriVcontainer.begin(); priVit != PriVcontainer.end(); priVit++) {
        double deltaX2 = ((*secVit).x() - (*priVit).x()) * ((*secVit).x() - (*priVit).x());
        double deltaY2 = ((*secVit).y() - (*priVit).y()) * ((*secVit).y() - (*priVit).y());
        double deltaZ2 = ((*secVit).z() - (*priVit).z()) * ((*secVit).z() - (*priVit).z());
        if(sqrt(deltaX2 + deltaY2 + deltaZ2) < VertexMathingDistance_) {SecondaryVertexFlag = false;}
      }
      if(SecondaryVertexFlag) {SecVcandidate.push_back((*secVit));}
    }

    for(View<pat::Electron>::const_iterator trk = leptons.begin();trk != leptons.end(); ++trk) {
      int associatedSecV = 0;
      // First loop over transient tracks in order to find the transient track the is also the muon track
      std::vector<math::XYZVector>::iterator secVitt;
      for(std::vector<reco::TransientTrack>::const_iterator itT = t_tks.begin(); itT != t_tks.end() ; itT++ ) {
        if( ( ((*itT).track().pt()) == (trk->track()->pt())) &&
            ( ((*itT).track().eta()) == (trk->track()->eta())) &&
            ( ((*itT).track().phi()) == (trk->track()->phi())) ) {
          if(SecVcandidate.size() > 0) {
            for(secVitt = SecVcandidate.begin(); secVitt != SecVcandidate.end(); secVitt++) {
              GlobalPoint theVertexPoint((*secVitt).x(),(*secVitt).y(),(*secVitt).z());
              math::XYZVector vert(secVitt->x(),secVitt->y(),secVitt->z());
              FreeTrajectoryState theMuonNearVertex = (*itT).trajectoryStateClosestToPoint(theVertexPoint).theState();
              math::XYZVector pcb(theMuonNearVertex.position().x(),theMuonNearVertex.position().y(),theMuonNearVertex.position().z());
              math::XYZVector dd = pcb - vert;
              double magd = sqrt((dd.x() * dd.x()) + (dd.y() * dd.y()) + (dd.z() * dd.z()));
              if(magd < VertexMuonMaxDistance_) {associatedSecV++;}
            }
          }
        }
      }
      if ( ( associatedSecV >= MinAssociatedSVCut_ ) && ( associatedSecV <= MaxAssociatedSVCut_ ) ) {pOut->push_back( * trk );}
      std::cout << "Vertex Matching Distance      :  " << VertexMathingDistance_ << std::endl;
      std::cout << "Vertex to Muon Max Distance   :  " << VertexMuonMaxDistance_ << std::endl;
      std::cout << "Minimum # of associated SV's  :  " << MinAssociatedSVCut_ << std::endl;
      std::cout << "Maximum # of associated SV's  :  " << MaxAssociatedSVCut_ << std::endl;
      std::cout << "Number of Secondary Vertices  :  " << SecVcandidate.size() << std::endl;
      std::cout << "Associated Secondary Vertices :  " << associatedSecV << std::endl;
    }

    iEvent.put(pOut);

  } else {
    std::cerr << "Incorrect Lepton Type!!! " << std::endl;
    std::cerr << "Please re-run and set the LeptonType to 'Muon' or 'Electron' " << std::endl;
    return;
  }
 
}

// ------------ method called once each job just before starting event loop  ------------
void PATLeptonSVAssociatorSelector::beginJob(const edm::EventSetup&) {}

// ------------ method called once each job just after ending the event loop  ------------
void PATLeptonSVAssociatorSelector::endJob() {}
