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

     const std::vector < reco::TaggingVariableName > & recoVertex() const;
     const std::vector < reco::TaggingVariableName > & pseudoVertex() const;
     const std::vector < reco::TaggingVariableName > & noVertex() const;

     std::vector < reco::TaggingVariableName > theRecoVars;
     std::vector < reco::TaggingVariableName > thePseudoVars;
     std::vector < reco::TaggingVariableName > theNoVars;
};

#endif
