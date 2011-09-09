#ifndef TauAnalysis_Core_HistManagerBase_h
#define TauAnalysis_Core_HistManagerBase_h

/** \class PUreweightHistogramProducer
 *
 * Produce histogram for simultaneous reweighting of Monte Carlo simulated events
 * to match in-time and out-of-time pile-up in Data
 * ( cf. Alexei Raspereza's presentation in Higgs --> tau tau meeting on May 26th 2011:
 *      https://indico.cern.ch/getFile.py/access?contribId=46&sessionId=0&resId=0&materialId=slides&confId=140191 )
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PUreweightHistogramProducer.h,v 1.1 2011/08/13 07:47:49 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

class PUreweightHistogramProducer : public edm::EDAnalyzer
{
 public:
  // constructor 
  explicit PUreweightHistogramProducer(const edm::ParameterSet&);
  
  // destructor
  virtual ~PUreweightHistogramProducer() {}

  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob() {}

 private:

  std::string moduleLabel_;

  edm::InputTag srcVertices_;
  edm::InputTag srcPFNeutralRho_;

  double rhoMax_;

  edm::InputTag srcWeight_;

  MonitorElement* meVtxMultiplicity_;
  MonitorElement* mePFNeutralRho_;

  MonitorElement* mePFNeutralRhoVsVtxMultiplicity_;
};

#endif  

