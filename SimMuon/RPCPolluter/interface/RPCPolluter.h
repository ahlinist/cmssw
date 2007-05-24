#ifndef SimMuon_RPCPolluter_h
#define SimMuon_RPCPolluter_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


/*  class RPCPolluter
 *
 *  Creates neutron hits in a given RPCRoll
 *
 *  Authors : C. Avila, B. Gomez, J.C. Sanabria
 *
 *            Universidad de los Andes, Bogota
 */


class RPCPolluter : public edm::EDProducer
{

 public :

  explicit RPCPolluter(const edm::ParameterSet& iConfig);

  ~RPCPolluter();

 private:

  int N_hits;

  int nbxing;

  double rate;

  double gate;

  virtual void produce(edm::Event&, const edm::EventSetup&);


};


#endif
