//
// Original Author:  Fedor Ratnikov Dec 27, 2006
// $Id: ChainedJetCorrector.h,v 1.1 2008/02/29 23:00:36 fedor Exp $
//
// Correction which chains other corrections
//
#ifndef ChainedJetCorrector_h
#define ChainedJetCorrector_h

#include "boost/shared_ptr.hpp"
#include <vector>

#include "JetMETCorrections/Objects/interface/JetCorrector.h"


class ChainedJetCorrector : public JetCorrector
{
public:
  ChainedJetCorrector (){}
  virtual ~ChainedJetCorrector () {}
  
  virtual double correction (const JetCorrector::LorentzVector& fJet) const;
  virtual double correction (const reco::Jet& fJet) const;
  virtual double correction (const reco::Jet& fJet,
			     const edm::RefToBase<reco::Jet>& fJetRef,
			     const edm::Event& fEvent,
			     const edm::EventSetup& fSetup) const;
  
  virtual bool eventRequired () const;
  
  void push_back (const JetCorrector* fCorrector) {mCorrectors.push_back (fCorrector);}
  void clear () {mCorrectors.clear ();}
  
private:
  std::vector <const JetCorrector*> mCorrectors;
};

#endif
