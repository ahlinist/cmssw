#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesis_h

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Candidate/interface/Candidate.h" 

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TH1.h>

#include <string>

class NSVfitEventHypothesis
{
 public:
  NSVfitEventHypothesis() {}
  NSVfitEventHypothesis(const edm::Ptr<reco::Candidate>& met) 
    : met_(met),
      p4MEt_(met->p4())
  {}
  NSVfitEventHypothesis(const NSVfitEventHypothesis&);
  virtual ~NSVfitEventHypothesis() {}

  virtual NSVfitEventHypothesis* clone() const { return new NSVfitEventHypothesis(*this); }
  
  virtual NSVfitEventHypothesis& operator=(const NSVfitEventHypothesis&);

  const std::string& name() const { return name_; }
  int barcode() const { return barcode_; }

  /// pointer to MET object from which this hypothesis was made
  virtual const edm::Ptr<reco::Candidate>& met() const { return met_; }

  /// momentum of multi-lepton system before fit, after fit
  /// and difference in momentum (after - before) fit
  reco::Candidate::LorentzVector p4() const { return p4_; }
  reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
  reco::Candidate::LorentzVector dp4fitted() const { return dp4_; }

  /// missing transverse momentum before fit
  /// and fitted value of missing transverse momentum 
  reco::Candidate::LorentzVector p4MEt() const { return p4MEt_; }
  reco::Candidate::LorentzVector p4MEt_fitted() const { return (p4MEt_ - dp4_); }

  /// access to position of primary event vertex (tau lepton production vertex);
  /// refitted by NSVfit algorithm after excluding from fit tracks associated to tau lepton decay products
  ///
  /// NB: error on position returned by eventVertexErrSVrefitted method not 100% correct,
  ///     as uncertainty on "correction" to primary event vertex position
  ///     determined by NSVfit algorithm is not taken into account
  ///
  bool eventVertexSVrefittedIsValid() const { return eventVertexIsValid_; }
  AlgebraicVector3 eventVertexPosSVrefitted() const { return (eventVertexPosition_ + eventVertexPositionShift_); }
  const AlgebraicSymMatrix33& eventVertexErrSVrefitted() const { return eventVertexPositionErr_; }

  const AlgebraicVector3& eventVertexShiftSVrefitted() const { return eventVertexPositionShift_; }

  /// fit hypotheses of lepton resonances
  const edm::OwnVector<NSVfitResonanceHypothesis>& resonances() const { return resonances_; }
  const NSVfitResonanceHypothesis* resonance(const std::string& name) const
  {
    const NSVfitResonanceHypothesis* retVal = 0;
    for ( edm::OwnVector<NSVfitResonanceHypothesis>::const_iterator resonance = resonances_.begin();
	  resonance != resonances_.end(); ++resonance ) {
      if ( resonance->name() == name ) retVal = &(*resonance);
    }
    return retVal;
  }

  const TH1* histMassResults() const { return histMassResults_.get(); }

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitEventHypothesis::print>:" << std::endl;
    stream << " name = " << name_ << std::endl;
    stream << " barcode = " << barcode_ << std::endl;
    for ( edm::OwnVector<NSVfitResonanceHypothesis>::const_iterator resonance = resonances_.begin();
          resonance != resonances_.end(); ++resonance ) {
      resonance->print(stream);
    }
  }

  friend class NSVfitEventBuilderBase;
  template<typename T_type> friend class NSVfitProducerT;
  friend class NSVfitAlgorithmByIntegration;

 private:

  /// "human-readable" label for print-out
  std::string name_;

  /// unique identifier to associate hypothesis to likelihood function
  int barcode_;

  /// pointer to MET object from which this hypothesis was made
  edm::Ptr<reco::Candidate> met_;

  /// momentum of particle before fit
  reco::Candidate::LorentzVector p4_;

  /// difference in momentum (after - before) fit
  reco::Candidate::LorentzVector dp4_;

  /// missing transverse momentum before fit
  reco::Candidate::LorentzVector p4MEt_;

  /// position of primary event vertex (tau lepton production vertex);
  /// refitted by SVfit algorithm after excluding from fit tracks associated to tau lepton decay products
  AlgebraicVector3 eventVertexPosition_;
  AlgebraicSymMatrix33 eventVertexPositionErr_;
  AlgebraicVector3 eventVertexPositionShift_;
  bool eventVertexIsValid_;

  /// fit hypotheses for daughter particles
  edm::OwnVector<NSVfitResonanceHypothesis> resonances_;
  
  /// Nd-histogram containing full correlations of masses reconstructed for resonances
  std::auto_ptr<TH1> histMassResults_;
};

#endif


