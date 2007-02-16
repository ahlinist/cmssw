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
         const std::vector < reco::btau::TaggingVariableName > & recovertex,
         const std::vector < reco::btau::TaggingVariableName > & pseudovertex,
         const std::vector < reco::btau::TaggingVariableName > & novertex );

     /**
      *  Retrieve variables of a certain VertexType
      */
     const std::vector < reco::btau::TaggingVariableName > & variables ( 
         reco::btag::Vertices::VertexType ) const;
   
   private:

     const std::vector < reco::btau::TaggingVariableName > & recoVertex() const;
     const std::vector < reco::btau::TaggingVariableName > & pseudoVertex() const;
     const std::vector < reco::btau::TaggingVariableName > & noVertex() const;

     std::vector < reco::btau::TaggingVariableName > theRecoVars;
     std::vector < reco::btau::TaggingVariableName > thePseudoVars;
     std::vector < reco::btau::TaggingVariableName > theNoVars;
};

#endif
