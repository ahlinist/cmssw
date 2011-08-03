#ifndef TauAnalysis_CandidateTools_mTauTauMinAlgo_h
#define TauAnalysis_CandidateTools_mTauTauMinAlgo_h

/** mTauTauMinAlgo
 *
 * This function implements the algorithm for reconstructing a lower bound on the tau-pair mass.
 *
 * The algorithm is described in:
 *  "Speedy Higgs boson discovery in decays to tau lepton pairs: h --> tau tau"
 *  by Alan J. Barr, Sky T. French, James A. Frost, Christopher G. Lester,
 *  arXiv: 1106.2322v1 [hep-ph]
 *
 * The code has been provided by the authors 
 * and has been integrated into the TauAnalysis software by
 *  Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: mTauTauMinAlgo.h,v 1.2 2011/07/17 15:59:54 veelken Exp $
 *
 */

// function parameters
//-------------------------------------------------------------------------------
//   se, sx, sy, sz: 
//     Energy, px, py, pz of visible decay products of first tau lepton
//   te, tx, ty, tz: 
//     Energy, px, py, pz of visible decay products of second tau lepton
//   pmissx, pmissy:
//     components of reconstructed missing transverse momentum (MEt) in px, py direction
//   mtau:
//     nominal tau lepton mass (1.777 GeV)
//
// return value
//-------------------------------------------------------------------------------
//   minimal value for "full" tau-pair mass compatible with four-momenta
//   of visible tau decay products and with reconstructed MEt
//   
//   NOTE: negative return values indicate that algorithm found no valid solution !!
//  
double mTauTauMin(const double se, const double sx, const double sy, const double sz,
		  const double te, const double tx, const double ty, const double tz,
		  const double pmissx, const double pmissy,
		  const double mtau);

#endif
