#ifndef TtSemiIdeogramScan_h
#define TtSemiIdeogramScan_h

// includes for kinematic fit
#include "TopQuarkAnalysis/TopKinFitter/src/TFitConstraintM.hh"
#include "TopQuarkAnalysis/TopKinFitter/src/TFitParticleEtThetaPhi.hh"
#include "TopQuarkAnalysis/TopKinFitter/src/TFitConstraintEp.hh"
#include "TopQuarkAnalysis/TopKinFitter/src/TKinFitter.hh"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"

// Root stuff
#include "TLorentzVector.h"
#include "TMatrixD.h"
#include "TMath.h"

using namespace std;

class TtSemiIdeogramScan {

  public:

    TtSemiIdeogramScan();
    TtSemiIdeogramScan(vector<double>&);
    ~TtSemiIdeogramScan();

    vector<pair<double,double> > getScanValues(TtSemiEvtSolution &asol);

  private:
    vector<double> mtvalues;
};


#endif
