#ifndef RecoBTag_CombinedSVTaggingVariables
#define RecoBTag_CombinedSVTaggingVariables

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include <vector>

class CombinedSVTaggingVariables {
  /**
   *  Class that defines what variables are used for what vertex category
   *  (i.e. for RecoVertex, PseudoVertex, NoVertex)
   */

   public:
     /**
      *  enum to refer to pre-defined sets of tagging variables.
      *  FullSet: use the whole set of tagging variables.
      *  VertexSet: use only a minimal vertexing-related set (VertexCategory
      *  plus FlightDistance).
      */
     enum TaggingVariableSet { FullSet, VertexSet };

     /**
      *  Constructor that takes an enum to specify
      *  pre-defined sets of tagging variables.
      */
     CombinedSVTaggingVariables ( CombinedSVTaggingVariables::TaggingVariableSet s );

     CombinedSVTaggingVariables (
         const std::vector < reco::TaggingVariableName > & recovertex,
         const std::vector < reco::TaggingVariableName > & pseudovertex,
         const std::vector < reco::TaggingVariableName > & novertex );

     /**
      *  Retrieve variables of a certain VertexType
      */
     const std::vector < reco::TaggingVariableName > & variables ( 
         reco::btag::Vertices::VertexType ) const;
   
   private:
      void pseudoFromNo(); //< copy no vars to pseudo vars
      void recoFromPseudo(); //< copy pseudo vars to reco vars
      void fullSet(); //< the "full" tagging var set
      void vertexSet(); //< the "vertex" tagging var set

     const std::vector < reco::TaggingVariableName > & recoVertex() const;
     const std::vector < reco::TaggingVariableName > & pseudoVertex() const;
     const std::vector < reco::TaggingVariableName > & noVertex() const;

     std::vector < reco::TaggingVariableName > theRecoVars;
     std::vector < reco::TaggingVariableName > thePseudoVars;
     std::vector < reco::TaggingVariableName > theNoVars;
};

#endif
