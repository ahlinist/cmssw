//
// Author:  Jan Heyninck
// Created: Tue Apr  3 17:33:23 PDT 2007
//
// $Id: MtopUncertaintyCalc.h,v 1.1 2007/05/08 14:03:05 heyninck Exp $
//

#ifndef MtopUncertaintyCalc_h
#define MtopUncertaintyCalc_h

/**
  \class    MtopUncertaintyCalc MtopUncertaintyCalc.h "TopQuarkAnalysis/TopLeptonSelection/interface/MtopUncertaintyCalc.h"
  \brief    Steering class for the overall top-lepton likelihood

   MtopUncertaintyCalc allows to calculate and retrieve the overall top-lepton
   likelihood as defined in CMS Note 2006/024

  \author   Jan Heyninck
  \version  $Id: MtopUncertaintyCalc.h,v 1.1 2007/05/08 14:03:05 heyninck Exp $
*/

#include "AnalysisDataFormats/TopObjects/interface/TtSemiMassSolution.h"
#include "TMatrixD.h"
using namespace std;


class MtopUncertaintyCalc {

  public:
    MtopUncertaintyCalc();
    ~MtopUncertaintyCalc();	

    void  operator()(TtSemiMassSolution&);

  private:
    TopParticle jet[3];
    int jetparam, dim;
    double mtop;
    TMatrixD jCovM[3], dMdp[3];
    void readInput(TtSemiMassSolution&);
    void calcDmtDpMatrices();
    double calcDMtop();
};

#endif
