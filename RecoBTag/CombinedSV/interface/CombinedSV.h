#ifndef RecoBTag_CombinedSV
#define RecoBTag_CombinedSV

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVAlgorithm.h"

class CombinedSV : public edm::EDProducer {
  /**
   *  Class that glues the combined btagging algorithm to the framework
   */
   public:
      explicit CombinedSV( const edm::ParameterSet & );
      ~CombinedSV();

      virtual void produce( edm::Event &, const edm::EventSetup &);

   private:
      /// instantiate btagging algorithm
      void instantiateAlgorithm( const edm::ParameterSet & );

      reco::Vertex getPrimaryVertex ( const edm::Event &, 
                                      const edm::EventSetup & ) const;

   private:
      /// the b-tagging algorithm
      combsv::CombinedSVAlgorithm * algorithm_;
      std::string vertexcoll_;
      std::string trackcoll_;
      std::string associatorID_;
};

#endif
