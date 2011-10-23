#ifndef TauAnalysis_FittingTools_TH2DynamicRebinner_h  
#define TauAnalysis_FittingTools_TH2DynamicRebinner_h

/** \class TH2DynamicRebinner
 *
 * Dynamic Rebinning of TH2 histogram, in order to merge bins with low signal or background statistics.
 * The result of the rebinning is returned as histogram of type TH1.
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: TH2DynamicRebinner.h,v 1.1 2011/10/23 15:17:47 veelken Exp $
 *
 */

#include <TH2.h>
#include <TH1.h>
#include <TMath.h>

#include <list>
#include <iostream>

namespace TH2DynamicRebinner_namespace
{
  struct binEntryType
  {
    binEntryType(int id, const TH2* histogram_sig, const TH2* histogram_bgr, int binX, int binY)
      : id_(id)
    {
      binContent_sig_ = histogram_sig->GetBinContent(binX, binY);
      binError2_sig_ = TMath::Power(histogram_sig->GetBinError(binX, binY), 2.);
      
      binContent_bgr_ = histogram_bgr->GetBinContent(binX, binY);
      binError2_bgr_ = TMath::Power(histogram_bgr->GetBinError(binX, binY), 2.);

      binContent_sum_ = binContent_sig_ + binContent_bgr_;
      binError2_sum_ = binError2_sig_ + binError2_bgr_;

      binsX_.push_back(binX);
      binsY_.push_back(binY);

      gravCenterX_ = histogram_sig->GetXaxis()->GetBinCenter(binX);
      gravCenterY_ = histogram_sig->GetYaxis()->GetBinCenter(binY);
    }
    ~binEntryType() {}

    void merge(const binEntryType& other)
    {
      binContent_sig_ += other.binContent_sig_;
      binError2_sig_ += other.binError2_sig_;

      binContent_bgr_ += other.binContent_bgr_;
      binError2_bgr_ += other.binError2_bgr_;

      binContent_sum_ = binContent_sig_ + binContent_bgr_;
      binError2_sum_ = binError2_sig_ + binError2_bgr_;

      binsX_.insert(binsX_.end(), other.binsX_.begin(), other.binsX_.end());
      binsY_.insert(binsY_.end(), other.binsY_.begin(), other.binsY_.end());

      double gravCenterX_new = 0.;
      double gravCenterY_new = 0.;
      double norm = 0.;

      if ( binContent_sum_ > 0. ) {
	gravCenterX_new += binContent_sum_*gravCenterX_;
	gravCenterY_new += binContent_sum_*gravCenterY_;
	norm += binContent_sum_;
      }

      if ( other.binContent_sum_ > 0. ) {
	gravCenterX_new += other.binContent_sum_*other.gravCenterX_;
	gravCenterY_new += other.binContent_sum_*other.gravCenterY_;
	norm += other.binContent_sum_;
      }

      if ( norm > 0. ) {
	gravCenterX_new /= norm;
	gravCenterY_new /= norm;
      }

      gravCenterX_ = gravCenterX_new;
      gravCenterY_ = gravCenterY_new;      
    }

    bool operator<(const binEntryType& other) const
    {
      return (id_ < other.id_);
    }

    mutable int id_;

    double binContent_sig_;
    double binError2_sig_;

    double binContent_bgr_;
    double binError2_bgr_;

    double binContent_sum_;
    double binError2_sum_;

    std::vector<int> binsX_;
    std::vector<int> binsY_;

    double gravCenterX_;
    double gravCenterY_;
  };
}

class TH2DynamicRebinner
{
 public:  

  explicit TH2DynamicRebinner(TH2*, TH2*, double = -1.e0, double = 1.e-1);
  ~TH2DynamicRebinner();
  
  TH1* operator()(const TH2*) const;

  void print(std::ostream&) const;

 private:

  double minBinContent_sig_;
  double minBinContent_bgr_;

  std::list<TH2DynamicRebinner_namespace::binEntryType> binMapping_;
  int numMappedBins_;
};

#endif  


