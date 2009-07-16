#ifndef __AnalysisDataFormats_TauAnalysis_MEtTopology_h__
#define __AnalysisDataFormats_TauAnalysis_MEtTopology_h__

/** \class MEtTopology
 *
 * Information about distribution of energy deposits in the event 
 * with respect to direction of missing Et vector
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: MEtTopology.h,v 1.1 2009/07/11 15:35:01 veelken Exp $
 *
 */

class MEtTopology
{
 public:
  /// constructor 
  explicit MEtTopology() {}

  /// destructor
  ~MEtTopology() {}
  
  /// get H1-like sums of calorimeter energy deposits
  /// projected on direction of missing transverse momentum vector
  double Vparallel() const { return Vparallel_; }
  double Vanti() const { return Vanti_; }
 
 private:

  /// allow only MEtTopologyAlgorithm to change values of data-members
  friend class MEtTopologyAlgorithm;

  /// set H1-like sums of calorimeter energy deposits
  /// projected on direction of missing transverse momentum vector
  void setVparallel(double Vparallel) { Vparallel_ = Vparallel; }
  void setVanti(double Vanti) { Vanti_ = Vanti; }

  /// H1-like sums of calorimeter energy deposits
  /// projected on direction of missing transverse momentum vector
  double Vparallel_;  
  double Vanti_;
};

#endif
