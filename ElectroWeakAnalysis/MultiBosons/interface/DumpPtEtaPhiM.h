#ifndef _ElectroWeakAnalysis_MultiBosons_DumpPtEtaPhiM_h_
#define _ElectroWeakAnalysis_MultiBosons_DumpPtEtaPhiM_h_

// -*- C++ -*-
//
// Package:    PatAlgos
// Class:      EgammaUserDataProducer
//
/**\class DumpPtEtaPhiM DumpPtEtaPhiM.h
ElectroWeakAnalysis/MultiBosons/interface/DumpPtEtaPhiM.h

Description: Functor for dumping Candidate's pt, eta, phi and mass

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
#include "ElectroWeakAnalysis/MultiBosons/interface/DumpPtEtaPhi.h"

class DumpPtEtaPhiM : DumpPtEtaPhi {
public:
  DumpPtEtaPhiM(std::string separator = " ") : DumpPtEtaPhi(separator) {}
  virtual std::string operator() (const reco::Candidate & cand) {
    std::ostringstream oss;
    oss << DumpPtEtaPhi::operator()(cand) << separator_
        << std::setw(7) << cand.mass();
    return oss.str();
  }
};

#endif