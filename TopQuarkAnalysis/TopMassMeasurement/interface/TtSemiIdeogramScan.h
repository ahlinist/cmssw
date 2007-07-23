#ifndef TtSemiIdeogramScan_h
#define TtSemiIdeogramScan_h

// includes for kinematic fit
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtThetaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
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
