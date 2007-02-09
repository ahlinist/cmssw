#ifndef HiggsTo4Leptons_HiggsTo4LeptonsMCBuilder_H
#define HiggsTo4Leptons_HiggsTo4LeptonsMCBuilder_H

/** \class HiggsTo4LeptonsMCBuilder
 *
 * Handles the generator input and ouputs the quantites of intest into
 * a collection of MC objects
 *
 * author: Dominique Fortin - UC Riverside
 */

#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjectsCollection.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <SimDataFormats/HepMCProduct/interface/HepMCProduct.h>

class HiggsTo4LeptonsMCBuilder {

 public:

  explicit HiggsTo4LeptonsMCBuilder( const edm::ParameterSet& ps );
  
  ~HiggsTo4LeptonsMCBuilder();

  void build(const HepMC::GenEvent* evt, HiggsTo4LeptonsMCObjectsCollection& mc);

 private:

  bool debug;

};
#endif
