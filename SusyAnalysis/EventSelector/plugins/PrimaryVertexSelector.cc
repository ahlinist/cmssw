#include "SusyAnalysis/EventSelector/interface/PrimaryVertexEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//________________________________________________________________________________________
PrimaryVertexEventSelector::PrimaryVertexEventSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), vertexTag_(pset.getParameter<edm::InputTag> ("vertexTag")) {

   defineVariable("numberOfVertices");

}

//________________________________________________________________________________________
bool PrimaryVertexEventSelector::select(const edm::Event& event) const {
   // reset cached variables
   resetVariables();
   // Get the vertices
   edm::Handle<edm::View<reco::Vertex> > vertexHandle;
   event.getByLabel(vertexTag_, vertexHandle);
   if (!vertexHandle.isValid()) {
      edm::LogWarning("PrimaryVertexEventSelector") << "No Vertex results for InputTag " << vertexTag_;
      return false;
   }

   int noVertices = 0;

   for (edm::View<reco::Vertex>::const_iterator iv = (*vertexHandle).begin(); iv != (*vertexHandle).end(); ++iv) {

     if(!iv->isFake() && iv->ndof() > 4 && fabs(iv->z()) <= 24 && iv->position().rho() < 2) noVertices ++;
   }

   setVariable("numberOfVertices", noVertices);
   //   setVariable("numberOfVertices", (*vertexHandle).size());

   //   return (*vertexHandle).size() >= 1;
   return noVertices >= 1;

}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, PrimaryVertexEventSelector, "PrimaryVertexEventSelector");
