#ifndef RecoBTag_CombinedSVLearning
#define RecoBTag_CombinedSVLearning

#include "FWCore/Framework/interface/Frameworkfwd.h"
// #include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "RecoBTag/CombinedSVLearning/interface/NCategoriesTeacher.h"
#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"                                                    

class CombinedSVLearning : public /* edm::EDProducer */ edm::EDAnalyzer {
  /**
   *  Class that glues the combined btagging algorithm to the framework
   */
   public:
      explicit CombinedSVLearning( const edm::ParameterSet & );
      ~CombinedSVLearning();

      // virtual void produce( edm::Event &, const edm::EventSetup &);
      virtual void analyze( const edm::Event &, const edm::EventSetup &);

   private:
      NCategoriesTeacher * theTeacher;
      JetFlavourIdentifier theIdentifier;
};

#endif
