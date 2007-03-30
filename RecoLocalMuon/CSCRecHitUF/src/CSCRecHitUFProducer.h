#ifndef CSCRecHitUF_CSCRecHitUFProducer_h
#define CSCRecHitUF_CSCRecHitUFProducer_h

/** \class CSCRecHitUFProducer 
 *
 * An algorithm for fast and efficient track (hits and) segment
 * reconstruction in Cathode Strip Chambers. Designed to be
 * CPU-efficient, the algorithm is specifically targeted for the High
 * Level Trigger purposes. The segment finding efficiency and the
 * spatial resolution attainable with the proposed algorithm as well
 * as the required CPU time are benchmarked using the MTCC data and
 * found to surpass the HLT requirements.
 *
 * \author Alexey.Drozdetskiy@cern.ch (UF), Andrey Korytov (UF), Yuriy Pakhotin (UF)
 *
 */

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCRecHitUFBuilder; 

class CSCRecHitUFProducer : public edm::EDProducer {

public:
  explicit CSCRecHitUFProducer( const edm::ParameterSet& ps);
  ~CSCRecHitUFProducer();

  virtual void produce( edm::Event&, const edm::EventSetup& );

 private:

  // Counting events processed
  int iev; 
  bool useCalib;
  bool isData;
  bool debug;

  std::string stripDigiProducer_;
  std::string wireDigiProducer_;

  CSCRecHitUFBuilder* recHitBuilder_;
};

#endif
