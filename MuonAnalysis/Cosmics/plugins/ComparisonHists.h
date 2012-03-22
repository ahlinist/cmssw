#ifndef ComparisonHists_h
#define ComparisonHists_h

#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

class ComparisonHists {

  // A little object to handle filling histograms that compare a pair
  // of reconstructed quantities ("0" and "1" below, with associated
  // errors "e0" and "e1"), and optionally include comparing them both
  // versus MC truth ("2"). The histograms made are:
  //
  // - simple 1-D histograms of the quantities (h_0, h_1, h_2, h_e0, h_e1)
  // - 2-D scatterplots comparing the pair(s) (h_0v1, h_0v2, h_1v2)
  // - 1-D histograms of the differences:
  //   * absolute: h_diff (0 - 1), h_diff02 (0 - 2), h_diff12 (1 - 2)
  //   * relative: h_res0  (0 - 1)/0, h_res1  (0 - 1)/1,
  //               h_res02 (0 - 2)/2, h_res12 (1 - 2)/2
  //   * pulls, where "(+)" denotes adding in quadrature:
  //               h_pull (0 - 1)/(e0 (+) e1),
  //               h_pull02 (0 - 2)/e0, h_pull12 (1 - 2)/e1
  //
  // The simple 1-D histograms' and 2-D scatterplots' binning is
  // determined by nbins, xmin, and xmax. The difference histograms'
  // binning are similarly determined by e.g. nbins_diff, xmin_diff,
  // and xmax_diff.
  //
  // If root2=true, the differences are divided by sqrt(2) when
  // filling the histograms, except for the differences including
  // quantity 2 (assumed to be MC truth).
  //
  // The "2" histograms are not made if make_mctruth=false. Likewise,
  // if nbins_diff, nbins_res, or nbins_pull are <= 0, the
  // corresponding histograms are not made.
  //
  // base_name becomes the leading substring in the names of all the
  // histograms we make, while name_0 and name_1 are identifiers for
  // the two quantities. (name_2 is hard-coded to "mctruth".)
  //
  // As an explicit example, e.g. the cosmic splitting study, the two
  // names are "upper" and "lower". Assuming this, and that we're
  // comparing q/pT for tracker-only in the 200 < pT < 2000 GeV bin,
  // explicitly we then get histograms in folders named:
  //
  // - h_0:      "pT2002000/TkOnly/qinvpt/upper"
  // - h_1:      "pT2002000/TkOnly/qinvpt/lower"
  // - h_2:      "pT2002000/TkOnly/qinvpt/mctruth"
  // - h_e0:     "pT2002000/TkOnly/qinvpt/sigma_upper"
  // - h_e1:     "pT2002000/TkOnly/qinvpt/sigma_lower"
  // - h_0v1:    "pT2002000/TkOnly/qinvpt/upperVlower"
  // - h_0v2:    "pT2002000/TkOnly/qinvpt/upperVmctruth"
  // - h_1v2:    "pT2002000/TkOnly/qinvpt/lowerVmctruth"
  // - h_diff:   "pT2002000/TkOnly/qinvpt/upperDlower"
  // - h_diff02: "pT2002000/TkOnly/qinvpt/upperDmctruth"
  // - h_diff12: "pT2002000/TkOnly/qinvpt/lowerDmctruth"
  // - h_res0:   "pT2002000/TkOnly/qinvpt/upperR0lower"
  // - h_res1:   "pT2002000/TkOnly/qinvpt/upperR1lower"
  // - h_res02:  "pT2002000/TkOnly/qinvpt/upperR2mctruth"
  // - h_res12:  "pT2002000/TkOnly/qinvpt/lowerR2mctruth"
  // - h_pull:   "pT2002000/TkOnly/qinvpt/upperPlower"
  // - h_pull02: "pT2002000/TkOnly/qinvpt/upperPmctruth"
  // - h_pull12: "pT2002000/TkOnly/qinvpt/lowerPmctruth"
  //
  // fsdir, name_0, name_1, make_mctruth, and root2 are often
  // the same for many successive calls; check out
  // ComparisonHistsFactory farther below which helps with this.

public:
  ComparisonHists(TFileDirectory& fsdir,
		  const TString& name_0, const TString& name_1,
		  int nbins,       float xmin,      float xmax,
		  int nbins_diff,  float xmin_diff, float xmax_diff,
		  int nbins_res,   float xmin_res,  float xmax_res,
		  int nbins_pull,  float xmin_pull, float xmax_pull,
		  bool make_mctruth, bool root2_, bool make_scatterplots)
    : use_mctruth(make_mctruth), root2(root2_)
  {
    // "2" is for comparing to MC truth.
    static const TString name_2 = "mctruth";

    // Initialize everything to null pointers since we might not
    // initialize or use some at all, depending on the logic
    // below. For those histograms that can end up not being made, we
    // explicitly test for the null pointer in Fill().
    h_0 = h_1 = h_2 = 0;
    h_0v1 = h_0v2 = h_1v2 = h_0d1v0d2 = h_0d1v1d2 = 0;
    h_diff = h_res0 = h_res1 = h_pull = 0;
    h_diff02 = h_diff12 = h_res02 = h_res12 = h_pull02 = h_pull12 = 0;

    // Remember, TFileService takes care of deleting these pointers.
    h_0 = fsdir.make<TH1F>(name_0, "", nbins, xmin, xmax);
    h_1 = fsdir.make<TH1F>(name_1, "", nbins, xmin, xmax);

    h_e0 = fsdir.make<TH1F>("sigma_" + name_0, "", nbins, 0, xmax/4);
    h_e1 = fsdir.make<TH1F>("sigma_" + name_1, "", nbins, 0, xmax/4);

    if (make_mctruth)
      h_2 = fsdir.make<TH1F>(name_2, "", nbins, xmin, xmax);

    if (make_scatterplots) {
      h_0v1 = fsdir.make<TH2F>(name_0 + "V" + name_1, "", nbins, xmin, xmax, nbins, xmin, xmax);
      if (make_mctruth) {
	h_0v2 = fsdir.make<TH2F>(name_0 + "V" + name_2, "", nbins, xmin, xmax, nbins, xmin, xmax);
	h_1v2 = fsdir.make<TH2F>(name_1 + "V" + name_2, "", nbins, xmin, xmax, nbins, xmin, xmax);

	h_0d1v0d2 = fsdir.make<TH2F>(name_0 + "X" + name_1 + "X" + name_2, "", nbins_diff, xmin_diff, xmax_diff, nbins_diff, xmin_diff, xmax_diff);
	h_0d1v1d2 = fsdir.make<TH2F>(name_0 + "X" + name_1 + "Y" + name_2, "", nbins_diff, xmin_diff, xmax_diff, nbins_diff, xmin_diff, xmax_diff);
      }
    }

    if (nbins_diff > 0) {
      // Filled with value0 - value1.
      h_diff = fsdir.make<TH1F>(name_0 + "D"   + name_1, "", nbins_diff, xmin_diff, xmax_diff);

      if (make_mctruth) {
	h_diff02 = fsdir.make<TH1F>(name_0 + "D" + name_2, "", nbins_diff, xmin_diff, xmax_diff);
	h_diff12 = fsdir.make<TH1F>(name_1 + "D" + name_2, "", nbins_diff, xmin_diff, xmax_diff);
      }
    }

    if (nbins_res > 0) {
      // res_i is filled with (value0 - value1)/value_i.
      h_res0 = fsdir.make<TH1F>(name_0 + "R0"  + name_1, "", nbins_res, xmin_res, xmax_res);
      h_res1 = fsdir.make<TH1F>(name_0 + "R1"  + name_1, "", nbins_res, xmin_res, xmax_res);

      if (make_mctruth) {
	h_res02 = fsdir.make<TH1F>(name_0 + "R2" + name_2, "", nbins_res, xmin_res, xmax_res);
	h_res12 = fsdir.make<TH1F>(name_1 + "R2" + name_2, "", nbins_res, xmin_res, xmax_res);
      }
    }

    if (nbins_pull > 0) {
      // Pulls: (value0 - value1)/(error0 (+) error1).
      h_pull   = fsdir.make<TH1F>(name_0 + "P" + name_1, "", nbins_pull, xmin_pull, xmax_pull);

      if (make_mctruth) {
	h_pull02 = fsdir.make<TH1F>(name_0 + "P" + name_2, "", nbins_pull, xmin_pull, xmax_pull);
	h_pull12 = fsdir.make<TH1F>(name_1 + "P" + name_2, "", nbins_pull, xmin_pull, xmax_pull);
      }
    }

    // Allow for weights in Fill().
    if (h_0) h_0->Sumw2();
    if (h_1) h_1->Sumw2();
    if (h_2) h_2->Sumw2();
    if (h_e0) h_e0->Sumw2();
    if (h_e1) h_e1->Sumw2();
    if (h_0v1) h_0v1->Sumw2();
    if (h_0v2) h_0v2->Sumw2();
    if (h_1v2) h_1v2->Sumw2();
    if (h_0d1v0d2) h_0d1v0d2->Sumw2();
    if (h_0d1v1d2) h_0d1v1d2->Sumw2();
    if (h_diff) h_diff->Sumw2();
    if (h_diff02) h_diff02->Sumw2();
    if (h_diff12) h_diff12->Sumw2();
    if (h_res0) h_res0->Sumw2();
    if (h_res1) h_res1->Sumw2();
    if (h_res02) h_res02->Sumw2();
    if (h_res12) h_res12->Sumw2();
    if (h_pull) h_pull->Sumw2();
    if (h_pull02) h_pull02->Sumw2();
    if (h_pull12) h_pull12->Sumw2();
  }

  void Fill(const double val[2], const double* err, const double weight, const double val2=0) {
    if (h_0)   h_0  ->Fill(val[0], weight);
    if (h_1)   h_1  ->Fill(val[1], weight);
    if (h_2)   h_2  ->Fill(val2,   weight);

    if (h_0v1) h_0v1->Fill(val[0], val[1], weight);
    if (h_0v2) h_0v2->Fill(val[0], val2,   weight);
    if (h_1v2) h_1v2->Fill(val[1], val2,   weight);

    if (err) {
      if (h_e0) h_e0->Fill(err[0]);
      if (h_e1) h_e1->Fill(err[1]);
    }

    double diff   = val[0] - val[1];
    double diff02 = val[0] - val2;
    double diff12 = val[1] - val2;

    if (h_0d1v0d2) h_0d1v0d2->Fill(diff, diff02, weight);
    if (h_0d1v1d2) h_0d1v1d2->Fill(diff, diff12, weight);

    double res0  = diff/val[0];
    double res1  = diff/val[1];
    double res02 = diff02/val2;
    double res12 = diff12/val2;

    // err is a pointer instead of a two-element array for this
    // reason: if null, we don't fill the pull histograms.
    double pull = -999, pull02 = -999, pull12 = -999;
    if (err) {
      pull = diff/sqrt(err[0]*err[0] + err[1]*err[1]);
      pull02 = diff02/err[0];
      pull12 = diff12/err[1];
    }

    if (root2) {
      static const double sqrt2 = sqrt(2.);
      diff /= sqrt2;
      res0 /= sqrt2;
      res1 /= sqrt2;
    }
      
    if (h_diff)   h_diff  ->Fill(diff,   weight);
    if (h_diff02) h_diff02->Fill(diff02, weight);
    if (h_diff12) h_diff12->Fill(diff12, weight);
    
    if (h_res0)  h_res0 ->Fill(res0,  weight);
    if (h_res1)  h_res1 ->Fill(res1,  weight);
    if (h_res02) h_res02->Fill(res02, weight);
    if (h_res12) h_res12->Fill(res12, weight);

    if (err) {
      if (h_pull)   h_pull  ->Fill(pull,   weight);
      if (h_pull02) h_pull02->Fill(pull02, weight);
      if (h_pull12) h_pull12->Fill(pull12, weight);
    }
  }

private:
  const bool use_mctruth;
  const bool root2;
  TH1F* h_0;
  TH1F* h_1;
  TH1F* h_2;
  TH1F* h_e0;
  TH1F* h_e1;
  TH2F* h_0v1;
  TH2F* h_0v2;
  TH2F* h_1v2;
  TH2F* h_0d1v0d2;
  TH2F* h_0d1v1d2;
  TH1F* h_diff;
  TH1F* h_res0;
  TH1F* h_res1;
  TH1F* h_pull;
  TH1F* h_diff02;
  TH1F* h_diff12;
  TH1F* h_res02;
  TH1F* h_res12;
  TH1F* h_pull02;
  TH1F* h_pull12;  
};

//////////////////////////////////////////////////////////////////////

class ComparisonHistsFactory {
private:
  TFileDirectory& fsdir;
  const TString prefix;
  const TString n0;
  const TString n1;
  const bool mc_truth;
  const bool root2;
  const bool scatterplots;

public:
  ComparisonHistsFactory(TFileDirectory& fsdir_, const char* n0_, const char* n1_, bool mc_truth_=false, bool root2_=true, bool scatterplots_=false)
    : fsdir(fsdir_), n0(n0_), n1(n1_), mc_truth(mc_truth_), root2(root2_), scatterplots(scatterplots_)
    {}

  ComparisonHists* Create(const char* base_name,
			  int nbins, float xmin, float xmax,
			  int nbins_diff=-1, // Use the same nbins for diff, res, pull.
			  float xmin_diff=-1, float xmax_diff=-1,
			  float xmin_res =-1, float xmax_res=-1,
			  float xmin_pull=-5, float xmax_pull=5) {
    
    TFileDirectory d = fsdir.mkdir(base_name);
    return new ComparisonHists(d,
			       n0, n1,
			       nbins,      xmin,      xmax,
			       nbins_diff, xmin_diff, xmax_diff,
			       nbins_diff, xmin_res,  xmax_res,
			       nbins_diff, xmin_pull, xmax_pull,
			       mc_truth, root2, scatterplots);
  }
};

#endif
