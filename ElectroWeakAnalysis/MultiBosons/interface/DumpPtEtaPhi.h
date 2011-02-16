#ifndef _ElectroWeakAnalysis_MultiBosons_DumpPtEtaPhi_h_
#define _ElectroWeakAnalysis_MultiBosons_DumpPtEtaPhi_h_

// -*- C++ -*-
//
// Package:    PatAlgos
// Class:      EgammaUserDataProducer
//
/**\class DumpPtEtaPhi DumpPtEtaPhi.h
ElectroWeakAnalysis/MultiBosons/interface/DumpPtEtaPhi.h

Description: Functor for dumping Candidate's pt, eta, phi

Given a candite returns a string of pt eta phi.  The
constructor takes an optional argument of separator.

Author: Jan Veverka (Caltech)

*/

// system includes
#include <iostream>
#include <sstream>
#include <string>

// CMSSW includes
#include "DataFormats/Candidate/interface/CandidateFwd.h"

class DumpPtEtaPhi {
protected:
  std::string separator_;
public:
  DumpPtEtaPhi(std::string separator = " "):
    separator_(separator)
    {}
  virtual std::string operator() (const reco::Candidate & cand) {
    std::ostringstream oss;
    oss << std::setw(7)  << cand.pt() << separator_
        << std::setw(10) << cand.eta() << separator_
        << std::setw(10) << cand.phi();
    return oss.str();
  }
};

#endif