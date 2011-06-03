/*
RooFit --- Copyright (c) 2000-2005, Regents of the University of California and Stanford University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "TauAnalysis/CandidateTools/interface/RooBifurGeneralizedExp.h"
#include "RooMath.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "TMath.h"

RooBifurGeneralizedExp::RooBifurGeneralizedExp(const char *name, const char *title,
			 RooAbsReal& _x, RooAbsReal& _pivot,
			 RooAbsReal& _leftExp, RooAbsReal& _leftGauss,
			 RooAbsReal& _rightExp, RooAbsReal& _rightGauss) :
  RooAbsPdf(name,title),
  x("x","Observable",this,_x),
  pivot("pivot","Pivot",this,_pivot),
  leftExp("leftExp","leftExp",this,_leftExp),
  leftGauss("leftGauss","leftGauss",this,_leftGauss),
  rightExp("rightExp","rightExp",this,_rightExp),
  rightGauss("rightGauss","rightGauss",this,_rightGauss) { }

//_____________________________________________________________________________
RooBifurGeneralizedExp::RooBifurGeneralizedExp(const RooBifurGeneralizedExp& other, const char* name) :
  RooAbsPdf(other,name), x("x",this,other.x),
  pivot("pivot",this,other.pivot),
  leftExp("leftExp",this,other.leftExp),
  leftGauss("leftGauss", this, other.leftGauss),
  rightExp("rightExp",this,other.rightExp),
  rightGauss("rightGauss", this, other.rightGauss) {}

//_____________________________________________________________________________
Double_t RooBifurGeneralizedExp::evaluate() const
{
  if (x < pivot) {
    Double_t difference = x - pivot;
    return TMath::Exp(difference*leftExp - difference*difference*leftGauss);
  } else {
    Double_t difference = pivot-x;
    return TMath::Exp(difference*rightExp - difference*difference*rightGauss);
  }

}


//_____________________________________________________________________________
Int_t RooBifurGeneralizedExp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
  //if (matchArgs(allVars,analVars,x)) return 1 ;
  //if (matchArgs(allVars,analVars,pivot)) return 2 ;
  return 0 ;
}

namespace {
  Double_t indefiniteIntegral(Double_t x, Double_t expTerm, Double_t gaussTerm) {
    // If the gauss is tiny use an alternative form that is numerically valid..
    if (gaussTerm < expTerm*1e-2) {
      return -1.0*TMath::Exp(-expTerm*x)/expTerm;
    }
    // Otherwise do the whole mess
    // (E^(a^2/(4 b)) Sqrt[\[Pi]] Erf[(a + 2 b x)/(2 Sqrt[b])])/(2 Sqrt[b])
    //
    // Compute the log
    double firstTermNumerator = (expTerm*expTerm)/(4.0*gaussTerm);

    double erfTermNumerator = TMath::Log(
        TMath::Erf(
          (expTerm + 2.0*gaussTerm*x)/(2.0*TMath::Sqrt(gaussTerm))));

    // Put the big (if gaussTerm is small) terms together first
    double bigTerms = firstTermNumerator - 0.5*TMath::Log(gaussTerm);

    double smallTerms = + 0.5*TMath::Log(TMath::Pi())
      + erfTermNumerator - TMath::Log(2);

    double totalLogTerm = bigTerms + smallTerms;
    return TMath::Exp(totalLogTerm);
  }
}

//_____________________________________________________________________________
Double_t RooBifurGeneralizedExp::analyticalIntegral(Int_t code, const char* rangeName) const
{
  //assert(code==1 || code==2) ;
  assert(code==1) ;

  Double_t min = x.min(rangeName);
  Double_t max = x.max(rangeName);

  if (max <= pivot) {
    // All left
    Double_t shiftedMin = pivot - max; // these are flipped
    Double_t shiftedMax = pivot - min;
    return indefiniteIntegral(shiftedMax, leftExp, leftGauss) -
      indefiniteIntegral(shiftedMin, leftExp, leftGauss);
  } else if (min >= pivot) {
    // All right
    Double_t shiftedMin = min - pivot;
    Double_t shiftedMax = max - pivot;
    return indefiniteIntegral(shiftedMax, rightExp, rightGauss) -
      indefiniteIntegral(shiftedMin, rightExp, rightGauss);
  } else {
    // Crosses
    assert(min < pivot && max > pivot);
    Double_t leftShiftedMin = 0.0;
    Double_t leftShiftedMax = pivot - min;
    Double_t leftContribution =
      indefiniteIntegral(leftShiftedMax, leftExp, leftGauss) -
      indefiniteIntegral(leftShiftedMin, leftExp, leftGauss);

    Double_t rightShiftedMin = 0.0;
    Double_t rightShiftedMax = max - pivot;
    Double_t rightContribution =
      indefiniteIntegral(rightShiftedMax, rightExp, rightGauss) -
      indefiniteIntegral(rightShiftedMin, rightExp, rightGauss);

    return leftContribution + rightContribution;
  }

}



