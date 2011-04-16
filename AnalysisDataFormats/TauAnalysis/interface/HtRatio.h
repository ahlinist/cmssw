#ifndef __AnalysisDataFormats_TauAnalysis_HtRatio_h__
#define __AnalysisDataFormats_TauAnalysis_HtRatio_h__

/** \class HtRatio
 */

class HtRatio
{
 public:
  /// constructor 
  explicit HtRatio() {}

  /// destructor
  ~HtRatio() {}
  
  double Ratio() const { return Ratio_; }
 
 private:

  /// allow only HtRatioAlgorithm to change values of data-members
  friend class HtRatioAlgorithm;

  void setRatio(double Ratio) { Ratio_ = Ratio; }

  /// ratio of tau pt and sum of jet pt
  double Ratio_;  
};

#endif
