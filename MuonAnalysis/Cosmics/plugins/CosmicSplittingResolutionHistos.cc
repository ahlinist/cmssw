#include <boost/foreach.hpp>
#include "TFile.h"
#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "MuonAnalysis/Cosmics/plugins/ComparisonHists.h"
#include "MuonAnalysis/Cosmics/plugins/CosmicSplittingResolutionNtuple.h"

struct Loader {
  // This helper struct is used to determine the actual quantities to
  // compare (e.g. to put in the histograms). This determination can
  // include meddling in the order of the flat arrays in the ntuple,
  // as is done for "staglb"/"statko" to compare a particular pair of
  // upper/lower global/tracker-only and standalone pairs. Here we can
  // also choose between unpropagated and propagated values,
  // i.e. track values at PCA instead of propagating the lower track
  // to the upper track.

  double weight;
  double charge[2], chi2dof[2], pixel_hits[2], strip_hits[2], pixel_layers[2], strip_layers[2], muon_hits[2];
  double pt[2], inv_pt[2], qpt[2], qinv_pt[2], theta[2], phi[2], dxy[2], dz[2];
  double pt_error[2], inv_pt_error[2], theta_error[2], phi_error[2], dxy_error[2], dz_error[2];
  double mc_charge, mc_pt, mc_inv_pt, mc_qpt, mc_qinv_pt, mc_theta, mc_phi, mc_dxy, mc_dz;
  int tmr_choice[2], pmc_choice[2];

  // Return a weight for the given dataset id (0 = data, 1-3 = MC with
  // p>10, p>100, and p>500).
  static double get_weight(unsigned id) {
    if (!use_weights) return 1;
    static const double weights[4] = { 1., 1., 0.1375, 0.007983 };
    assert(id < 4);
    return weights[id];  
  }

  static bool use_weights;

  Loader(const CosmicSplittingResolutionNtuple* nt,
	 int track,                    // Which set of tracks to use, e.g. TPFMS.
	 track_pos use_in_staglb,      // Which standalone track to compare, upper or lower.
	 bool use_unpropagated_values, // Whether to use the propagated values.
	 bool flip_upper_charge) {     // Whether to flip the charge of the upper tracks.

    weight = get_weight(nt->id);

    // Most of the time we straightforwardly take upper (j=0) and
    // lower (j=1) pairs.
    for (int j = 0; j < 2; ++j) {
      int in = track, jm = j, jn = j;
      if (track == tk_staglb || track == tk_statko) {
	// But here, make the array {sta_lower, staglb_lower}, or
	// {sta_upper, staglb_upper} depending on the value of
	// use_in_staglb. (And e.g. staglb_lower == global_lower.)
	in = jm == 0 ? tk_stalone : track;
	jn = use_in_staglb;
      }

      chi2dof[jm]      = nt->chi2[in][jn] / nt->ndof[in][jn];
      pixel_hits[jm]   = nt->pixel_hits[in][jn];
      strip_hits[jm]   = nt->strip_hits[in][jn];
      pixel_layers[jm] = nt->pixel_layers[in][jn];
      strip_layers[jm] = nt->strip_layers[in][jn];
      muon_hits[jm]    = nt->muon_hits[in][jn];

      // Sometimes the upper track's charge needs to be flipped
      // (e.g. in collisions reconstruction on cosmics).
      const int q = flip_upper_charge && jn == upper ? -1 : 1;

      if (use_unpropagated_values || track == tk_stalone) {
	charge[jm]       = q * nt->unprop_charge[in][jn];
	pt[jm]           = nt->unprop_pt[in][jn];
	inv_pt[jm]       = 1/pt[jm]; // Already set pt[jm] from [in][jn] correctly, so use it here and in the few lines following.
	qpt[jm]          = charge[jm] * pt[jm];
	qinv_pt[jm]      = charge[jm] * inv_pt[jm];
	theta[jm]        = nt->unprop_theta[in][jn];
	phi[jm]          = nt->unprop_phi[in][jn];
	dxy[jm]          = nt->unprop_dxy[in][jn];
	dz[jm]           = nt->unprop_dz[in][jn];
	pt_error[jm]     = nt->unprop_error_pt[in][jn];
	inv_pt_error[jm] = nt->unprop_error_pt[in][jn] / pt[jm] / pt[jm];
	theta_error[jm]  = nt->unprop_error_theta[in][jn];
	phi_error[jm]    = nt->unprop_error_phi[in][jn];
	dxy_error[jm]    = nt->unprop_error_dxy[in][jn];
	dz_error[jm]     = nt->unprop_error_dz[in][jn];
      }
      else {
	charge[jm]       = q * nt->charge[in][jn];
        pt[jm] 	         = nt->pt[in][jn];
        inv_pt[jm]       = 1/pt[jm];
        qpt[jm]          = charge[jm] * pt[jm];
        qinv_pt[jm]      = charge[jm] * inv_pt[jm];
        theta[jm]        = nt->theta[in][jn];
        phi[jm]          = nt->phi[in][jn];
        dxy[jm]          = nt->dxy[in][jn];
        dz[jm] 	         = nt->dz[in][jn];
        pt_error[jm]     = nt->error_pt[in][jn];
        inv_pt_error[jm] = nt->error_pt[in][jn] / pt[jm] / pt[jm];
        theta_error[jm]  = nt->error_theta[in][jn];
        phi_error[jm]    = nt->error_phi[in][jn];
        dxy_error[jm]    = nt->error_dxy[in][jn];
        dz_error[jm]     = nt->error_dz[in][jn];
      }

      // MC is just the one track always, so no fancy "in" stuff.
      mc_charge   = nt->mc_charge[track];
      mc_pt 	  = nt->mc_pt[track];
      mc_inv_pt   = 1/mc_pt;
      mc_qpt 	  = mc_charge * mc_pt;
      mc_qinv_pt  = mc_charge * mc_inv_pt;
      mc_theta 	  = nt->mc_theta[track];
      mc_phi 	  = nt->mc_phi[track];
      mc_dxy 	  = nt->mc_dxy[track];
      mc_dz 	  = nt->mc_dz[track];
    }
  }
};

bool Loader::use_weights = true;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T getOptionalTrackedParameter(const edm::ParameterSet& pset, const char* name, const T def) {
  return pset.existsAs<T>(name) ? pset.getParameter<T>(name) : def;
}

struct Bin {
  // The Bin handles all the histograms for a particular track type
  // and set of cuts. This object is responsible for applying the
  // binning cuts: e.g. all TPFMS tracks with 100 < pT < 200 GeV, or
  // all tracks with |eta| > 0.9.

  int track;        // Which track type, e.g. TPFMS.
  bool is_mc;       // Whether we're running on data or MC. // JMTBAD get this from the ntuple or from a global or from Histos::constants.
  std::string name; // A descriptive identifier, to be mangled below.

  // JMTBAD make these globals or Histos::constants.
  bool use_unpropagated_values; // Whether to use the propagated values.
  bool flip_upper_charge;       // Whether to flip the charge of the upper tracks.

  // The cut parameters.
  unsigned nbins_scale;
  unsigned run_bin;
  double   min_pt;
  double   max_pt;
  double   min_eta;
  double   max_eta;
  double   min_phi;
  double   max_phi;
  double   min_dxy;
  double   min_dz;
  double   max_dxy;
  double   max_dz;
  double   max_chi2dof;
  int      min_pixel_layers;
  int      min_strip_layers;
  int      min_tracker_hits;
  int      min_muon_hits;

  // The histograms (see ComparisonHists.h for what histograms it
  // actually makes.)
  ComparisonHists* h_chi2dof;
  ComparisonHists* h_pixel_hits;
  ComparisonHists* h_strip_hits;
  ComparisonHists* h_pixel_layers;
  ComparisonHists* h_strip_layers;
  ComparisonHists* h_muon_hits;
  ComparisonHists* h_charge;
  ComparisonHists* h_pt;
  ComparisonHists* h_inv_pt;
  ComparisonHists* h_qpt;
  ComparisonHists* h_qinv_pt;
  ComparisonHists* h_theta;
  ComparisonHists* h_phi;
  ComparisonHists* h_dxy;
  ComparisonHists* h_dz;

  bool ref_ok; // = track == tk_tkonly. If so, fill the below histograms.
  // Simple histos of the reference track quantities.
  TH1F* h_ref_p_unweighted;
  TH1F* h_ref_p;
  TH1F* h_ref_pt;
  TH1F* h_ref_eta;
  TH1F* h_ref_phi;
  TH1F* h_ref_dxy;
  TH1F* h_ref_dz;

  // Keep track of how many events seen per run, to be put in a
  // histogram later.
  std::map<unsigned, unsigned> runs_seen;

  // Set or scale the values in the passed vector. Useful for changing
  // the histogram binning, see below.
  void mod_res_v(std::vector<double>& v, double set=-1, double scale=-1) {
    for (std::vector<double>::iterator i = v.begin(), e = v.end(); i != e; ++i) {
      if (set > 0)
	*i = set;
      else if (scale > 0)
	*i *= scale;
    }
  }

  Bin(const edm::ParameterSet& pset, const int track_, bool is_mc_)
    : track(track_),
      is_mc(is_mc_),
      name(pset.getParameter<std::string>("name")),
      ref_ok(track == tk_tkonly)
  {
    nbins_scale      = getOptionalTrackedParameter<unsigned>(pset, "nbins_scale",           1);
    run_bin          = getOptionalTrackedParameter<unsigned>(pset, "run_bin",               0);
    min_pt           = getOptionalTrackedParameter<double>  (pset, "min_pt",                0);
    max_pt           = getOptionalTrackedParameter<double>  (pset, "max_pt",             2000);
    min_eta          = getOptionalTrackedParameter<double>  (pset, "min_eta",               0);
    max_eta          = getOptionalTrackedParameter<double>  (pset, "max_eta",               2.5);
    min_phi          = getOptionalTrackedParameter<double>  (pset, "min_phi",              -3.5);
    max_phi          = getOptionalTrackedParameter<double>  (pset, "max_phi",               3.5);
    min_dxy          = getOptionalTrackedParameter<double>  (pset, "min_dxy",               0);
    max_dxy          = getOptionalTrackedParameter<double>  (pset, "max_dxy",             100);
    min_dz           = getOptionalTrackedParameter<double>  (pset, "min_dz",                0);
    max_dz           = getOptionalTrackedParameter<double>  (pset, "max_dz",              100);
    max_chi2dof      = getOptionalTrackedParameter<double>  (pset, "max_chi2dof",           1e99);
    min_pixel_layers = getOptionalTrackedParameter<int>     (pset, "min_pixel_layers",      0);
    min_strip_layers = getOptionalTrackedParameter<int>     (pset, "min_strip_layers",      0);
    min_tracker_hits = getOptionalTrackedParameter<int>     (pset, "min_tracker_hits",      0);
    min_muon_hits    = getOptionalTrackedParameter<int>     (pset, "min_muon_hits",         0);

    edm::Service<TFileService> fs;
    TFileDirectory bindir = fs->mkdir(name);
    TFileDirectory trkdir = bindir.mkdir(track_nicks[track]);

    const bool root2 = track != tk_staglb && track != tk_statko;
    ComparisonHistsFactory fac(trkdir, "upper", "lower", is_mc, root2);

    // Explicitly use the vector copy constructor so we can change the
    // values if necessary below (scale by 10x for stand-alone, for
    // example)..
    std::vector<double> diff_scales(pset.getParameter<std::vector<double> >("diff_scales"));
    std::vector<double> res_scales (pset.getParameter<std::vector<double> >("res_scales" ));
    std::vector<double> pull_scales(pset.getParameter<std::vector<double> >("pull_scales"));

    if (track == tk_stalone || track == tk_staglb || track == tk_statko) {
      mod_res_v(diff_scales, min_pt > 150 ? 6 : 3);
      mod_res_v( res_scales, min_pt > 150 ? 6 : 3);
      mod_res_v(pull_scales, 15);
    }

    h_chi2dof      = fac.Create("chi2dof",      100/nbins_scale,  0, 10);
    h_pixel_hits   = fac.Create("pixelhits",     10,  0, 10);
    h_strip_hits   = fac.Create("striphits",     25,  0, 25);
    h_pixel_layers = fac.Create("pixellayers",    5,  0,  5);
    h_strip_layers = fac.Create("striplayers",   15,  0, 15);
    h_muon_hits    = fac.Create("muonhits",      55,  0, 55);
    h_charge       = fac.Create("charge",         3, -1,  2, 3, -3, 3);
    h_pt           = fac.Create("pt",           200/nbins_scale, min_pt, max_pt, 100/nbins_scale, -diff_scales[0], diff_scales[0], -res_scales[0], res_scales[0], -pull_scales[0], pull_scales[0]);
    h_inv_pt       = fac.Create("invpt",        200/nbins_scale, 0, 5/min_pt,    100/nbins_scale, -diff_scales[1], diff_scales[1], -res_scales[1], res_scales[1], -pull_scales[1], pull_scales[1]);
    h_qpt          = fac.Create("qpt",          200/nbins_scale, min_pt, max_pt, 100/nbins_scale, -diff_scales[0], diff_scales[0], -res_scales[0], res_scales[0], -pull_scales[0], pull_scales[0]);
    h_qinv_pt      = fac.Create("qinvpt",       200/nbins_scale, 0, 5/min_pt,    100/nbins_scale, -diff_scales[1], diff_scales[1], -res_scales[1], res_scales[1], -pull_scales[1], pull_scales[1]);
    h_theta        = fac.Create("theta",        200/nbins_scale, 0, 3.15,        100/nbins_scale, -diff_scales[2], diff_scales[2], -res_scales[2], res_scales[2], -pull_scales[2], pull_scales[2]);
    h_phi          = fac.Create("phi",          200/nbins_scale, -3.15, 3.15,    100/nbins_scale, -diff_scales[3], diff_scales[3], -res_scales[3], res_scales[3], -pull_scales[3], pull_scales[3]);
    h_dxy          = fac.Create("dxy",          200/nbins_scale, -0.1, 0.1,      100/nbins_scale, -diff_scales[4], diff_scales[4], -res_scales[4], res_scales[4], -pull_scales[4], pull_scales[4]);
    h_dz           = fac.Create("dz",           200/nbins_scale, -0.3, 0.3,      100/nbins_scale, -diff_scales[5], diff_scales[5], -res_scales[5], res_scales[5], -pull_scales[5], pull_scales[5]);

    // Only bother to make the reference histograms once.
    if (ref_ok) {
      h_ref_p_unweighted = bindir.make<TH1F>("ref_p_unweighted",   "", 200/nbins_scale, 0, 2000);
      h_ref_p   = bindir.make<TH1F>("ref_p",   "", 200/nbins_scale, 0, 2000);
      h_ref_pt  = bindir.make<TH1F>("ref_pt",  "", 200/nbins_scale, 0, 2000);
      h_ref_eta = bindir.make<TH1F>("ref_eta", "", 200/nbins_scale, -3, 3);
      h_ref_phi = bindir.make<TH1F>("ref_phi", "", 200/nbins_scale, -3.15, 3.15);
      h_ref_dxy = bindir.make<TH1F>("ref_dxy", "", 200/nbins_scale, -40, 40);
      h_ref_dz  = bindir.make<TH1F>("ref_dz",  "", 200/nbins_scale, -40, 40);
      h_ref_p  ->Sumw2();
      h_ref_pt ->Sumw2();
      h_ref_eta->Sumw2();
      h_ref_phi->Sumw2();
      h_ref_dxy->Sumw2();
      h_ref_dz ->Sumw2();
    }
    else
      h_ref_p_unweighted = h_ref_p = h_ref_pt = h_ref_eta = h_ref_phi = h_ref_dxy = h_ref_dz = 0;
  }

  ~Bin() {
    delete h_chi2dof;
    delete h_pixel_hits;
    delete h_strip_hits;
    delete h_pixel_layers;
    delete h_strip_layers;
    delete h_muon_hits;
    delete h_charge;
    delete h_pt;
    delete h_inv_pt;
    delete h_qpt;
    delete h_qinv_pt;
    delete h_theta;
    delete h_phi;
    delete h_dxy;
    delete h_dz;

    // Make a histogram of events/run. Again, just do it for TkOnly as
    // it will be the same for the rest of the tracks. Don't do it for
    // MC, as the histograms will be entirely uninteresting.
    if (ref_ok && run_bin == 0 && !is_mc) {
      const size_t n = runs_seen.size();
      edm::Service<TFileService> fs;
      TH1I* h = fs->make<TH1I>(TString(name.c_str()) + "_runs_seen", "", n, 0, n);
      size_t i = 0; 
      for (std::map<unsigned, unsigned>::const_iterator it = runs_seen.begin(), ite = runs_seen.end(); it != ite; ++it, ++i) {
	h->GetXaxis()->SetBinLabel(i+1, TString::Format("%u", it->first));
	h->SetBinContent(i+1, int(it->second));
      }
    }
  }

  bool accept(const CosmicSplittingResolutionNtuple* nt, ostream* debug=0) const {
    // Determine whether to keep this event based on our cuts. The
    // cuts are on the reference track quantities.

    if (run_bin > 0 && nt->run != run_bin) {
      if (debug) *debug << "failed: run != " << run_bin << "\n";
      return false;
    }

    if (nt->ref_pt < min_pt || nt->ref_pt > max_pt) {
      if (debug) *debug << "failed: pt: " << nt->ref_pt << " min_pt: " << min_pt << " max_pt: " << max_pt << "\n";
      return false;
    }

    const double ref_eta = -log(tan(nt->ref_theta/2));
    const double aeta = fabs(ref_eta);
    if (aeta < min_eta || aeta > max_eta) {
      if (debug) *debug << "failed: eta: " << ref_eta << " min_eta: " << min_eta << " max_eta: " << max_eta << "\n";
      return false;
    }

    if (nt->ref_phi < min_phi || nt->ref_phi > max_phi) {
      if (debug) *debug << "failed: phi: " << nt->ref_phi << " min_phi: " << min_phi << " max_phi: " << max_phi << "\n";
      return false;
    }

    const double adxy = fabs(nt->ref_dxy);
    if (adxy < min_dxy || adxy > max_dxy) {
      if (debug) *debug << "failed: dxy: " << nt->ref_dxy << " min_dxy: " << min_dxy << " max_dxy: " << max_dxy << "\n";
      return false;
    }

    const double adz = fabs(nt->ref_dz);
    if (adz < min_dz || adz > max_dz) {
      if (debug) *debug << "failed: dz: " << nt->ref_dz << " min_dz: " << min_dz << " max_dz: " << max_dz << "\n";
      return false;
    }

    const double ref_chi2dof = nt->ref_chi2/nt->ref_ndof;
    if (ref_chi2dof > max_chi2dof) {
      if (debug) *debug << "failed: chi2/dof: " << ref_chi2dof << " max: " << max_chi2dof << "\n";
      return false;
    }

    if (nt->ref_pixel_layers < min_pixel_layers) {
      if (debug) *debug << "failed: pixel layers: " << nt->ref_pixel_layers << " min: " << min_pixel_layers << "\n";
      return false;
    }
  
    if (nt->ref_strip_layers < min_strip_layers) {
      if (debug) *debug << "failed: strip layers: " << nt->ref_strip_layers << " min: " << min_strip_layers << "\n";
      return false;
    }
  
    const int ref_tracker_hits = nt->ref_pixel_hits + nt->ref_strip_hits;
    if (ref_tracker_hits < min_tracker_hits) {
      if (debug) *debug << "failed: tracker hits: " << ref_tracker_hits << " min: " << min_tracker_hits << "\n";
      return false;
    }
  
    if (nt->ref_muon_hits < min_muon_hits) {
      if (debug) *debug << "failed: muon hits: " << nt->ref_muon_hits << " min: " << min_muon_hits << "\n";
      return false;
    }
    
    return true;
  }

  bool check_and_fill(const CosmicSplittingResolutionNtuple* nt, std::ostringstream* out=0) {
    // Check that this Bin accepts the event, and if so, fill the
    // histograms.

    if (out) *out << "Bin " << name << ":\n";
    bool ok = accept(nt, out);
    if (out) *out << "Bin " << name << (ok ? " ACCEPTED" : " rejected") << " the ref track.\n";
    if (!ok) return false;

    // JMTBAD expose this
    static const bool only_lower_in_staglb = true;

    if (!is_mc) {
      if (runs_seen.find(nt->run) == runs_seen.end())
	runs_seen[nt->run] = 1;
      else
	runs_seen[nt->run] += 1;
    }

    if (ref_ok) {
      const double ref_p = nt->ref_pt/sin(nt->ref_theta);
      h_ref_p_unweighted->Fill(ref_p);
      const double w = Loader::get_weight(nt->id);
      h_ref_p->Fill(ref_p, w);
      h_ref_pt->Fill(nt->ref_pt, w);
      h_ref_eta->Fill(-log(tan(nt->ref_theta / 2)), w);
      h_ref_phi->Fill(nt->ref_phi, w);
      h_ref_dxy->Fill(nt->ref_dxy, w);
      h_ref_dz->Fill(nt->ref_dz, w);
    }

    for (int j = 0; j < 2; ++j) {
      track_pos for_staglb = only_lower_in_staglb ? lower : track_pos(j);
      const Loader loader(nt, track, for_staglb, use_unpropagated_values, flip_upper_charge);
    
      h_chi2dof     ->Fill(loader.chi2dof,      0, loader.weight);
      h_pixel_hits  ->Fill(loader.pixel_hits,   0, loader.weight);
      h_strip_hits  ->Fill(loader.strip_hits,   0, loader.weight);
      h_pixel_layers->Fill(loader.pixel_layers, 0, loader.weight);
      h_strip_layers->Fill(loader.strip_layers, 0, loader.weight);
      h_muon_hits   ->Fill(loader.muon_hits,    0, loader.weight);

      // Don't worry about the use of the mc_* values here when
      // is_mc=false; ComparisonHists has been told already not to
      // use them.
      h_charge ->Fill(loader.charge,  0,                   loader.weight, loader.mc_charge );
      h_pt     ->Fill(loader.pt,      loader.pt_error,     loader.weight, loader.mc_pt     );
      h_inv_pt ->Fill(loader.inv_pt,  loader.inv_pt_error, loader.weight, loader.mc_inv_pt );
      h_qpt    ->Fill(loader.qpt,     loader.pt_error,     loader.weight, loader.mc_qpt    );
      h_qinv_pt->Fill(loader.qinv_pt, loader.inv_pt_error, loader.weight, loader.mc_qinv_pt);
      h_theta  ->Fill(loader.theta,   loader.theta_error,  loader.weight, loader.mc_theta  );
      h_phi    ->Fill(loader.phi,     loader.phi_error,    loader.weight, loader.mc_phi    );
      h_dxy    ->Fill(loader.dxy,     loader.dxy_error,    loader.weight, loader.mc_dxy    );
      h_dz     ->Fill(loader.dz,      loader.dz_error,     loader.weight, loader.mc_dz     );

      // Only purpose of the j=0,1 loop is to use both upper and
      // lower STA-GLB comparisons in the histograms. Break if we're
      // not doing staglb.
      if (only_lower_in_staglb || track != tk_staglb || track != tk_statko)
	break;
    }

    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////

class CosmicSplittingResolutionHistos : public edm::EDAnalyzer {
  // The histogrammer. Despite this being an EDAnalyzer, we don't
  // actually use EDM root files, just a plain ntuple (specified in
  // CosmicSplittingResolutionNtuple). CMSSW python config files are
  // nice; probably a way to make some standalone executable that can
  // get edm::ParameterSets from a supplied python script, but this is
  // already done (and cmsRun now has very little overhead).

public:
  explicit CosmicSplittingResolutionHistos(const edm::ParameterSet&);
  ~CosmicSplittingResolutionHistos();

private:
  // Whether we're running on MC or data. JMTBAD take from ntuple?
  const bool is_mc;
  // Only use events from the specified sample id (0 = data, 1-3 = MC
  // for p > 10, p > 100, p > 500 samples). -1 = default = use
  // anything.
  const int only_sample;
  // Whether to check for "wrong" MC sample (i.e. only take 10<p<100 GeV
  // events from id=1, 100<p<500 GeV events from id=2, etc.)
  const bool check_for_wrong_sample;
  // The path to the ROOT file with the ntuple.
  const std::string filename;
  // The directory in the ROOT file in which the ntuple (TTree) exists.
  const std::string directory;

  // The Bin above makes histograms depending on the specified cuts,
  // but here we can also specify overall cuts that are applied to all
  // Bins.

  // Minimum numbers of muon hits, and pixel/strip layers.
  const int min_muon_hits; 
  const int min_pixel_layers;
  const int min_strip_layers;
  // The maximum muon station (e.g. MB 1,2,3,4) allowed in TPFMS.
  const int max_tpfms_station;
  // Whether to allow any DT/CSC hits.
  const bool no_dt_allowed;
  const bool no_csc_allowed;
  // Whether to check the delta phi between the tracker and standalone
  // tracks that went into the global fit.
  const bool check_tksta_dphi;
  // Whether to check the counts of shared hits.
  const bool check_shared_hits;
  // Whether to use the unpropagated values (both upper and lower
  // quantities taken from values at PCA) or the propagated ones
  // (lower track propagated to PCA of upper track). Almost
  // irrelevant, but standalone tracks are the exception.
  const bool use_unpropagated_values;
  // Whether collisions (pp) reconstruction was used; currently only
  // determines whether to flip the charge of the upper track.
  const bool pp_reco_mode;
  // The list of good runs (no longer done at ntupling time, or to
  // force a subset after the fact).
  const std::vector<unsigned> force_run_list;
  // The list of events (run, lumi, event 3-tuples flattened) that are
  // used only (optional, for e.g. making 1-1 comparisons).
  const std::vector<unsigned> force_event_list;
  // Whether to require or veto technical trigger bit 25 (special
  // barrel RPC cosmic trigger in collisions menu).
  const bool require_tt25;
  const bool require_not_tt25;
  // Check magnetic field strength to make sure we're using muons
  // reconstructed correctly.
  const double min_bfield;

  // End of cuts. Since we select <5% of events, useful to be able to
  // get a copy of the ntuple with just those selected.
  const bool copy_selected_events;

  // The Bins, which control the histogramming (see Bin above). The
  // specification is taken from the config file (see below).
  std::vector<Bin*> bins;

  // Keep track of how many events fail the cut sequence at which cut
  // ("error").
  TH1F* errors;
  enum error_code { error_none, error_bad_run, error_bfield, error_bad_event, error_tt25, error_wrong_sample, error_propagation, error_prop_mc,
		    error_muon_hits, error_pixels, error_strips, error_tpfms_station, error_dt, error_csc, error_tksta_dphi, error_shared_hits,
		    error_last };

  // Monitoring plots for track positions.
  TH2I* h_global_upper_inner_pos_xy;
  TH2I* h_global_upper_inner_pos_rz;
  TH2I* h_global_upper_outer_pos_xy;
  TH2I* h_global_upper_outer_pos_rz;
  TH2I* h_global_lower_inner_pos_xy;
  TH2I* h_global_lower_inner_pos_rz;
  TH2I* h_global_lower_outer_pos_xy;
  TH2I* h_global_lower_outer_pos_rz;

  // For debug dumps;
  std::ostringstream out;
  // The ROOT TFile, TTree, and the ntuple struct.
  TFile* file;
  TTree* tree;
  CosmicSplittingResolutionNtuple* nt;
  // The copied ntuple (see copy_selected_events comment above).
  TTree* events_used;

  // Used by cut() to check whether the run is in the good run list.
  bool run_is_bad(unsigned run);

  // Used by cut() for only allowing specific events.
  bool event_is_bad(unsigned run, unsigned lumi, unsigned event);

  // Apply the cuts described by the parameters above.
  error_code cut();

  // Event loop.
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
};

CosmicSplittingResolutionHistos::CosmicSplittingResolutionHistos(const edm::ParameterSet& cfg)
  : is_mc(cfg.getParameter<bool>("is_mc")),
    only_sample(cfg.getParameter<int>("only_sample")),
    check_for_wrong_sample(cfg.getParameter<bool>("check_for_wrong_sample")),
    filename(cfg.getParameter<std::string>("filename")),
    directory(cfg.getParameter<std::string>("directory")),
    min_muon_hits(cfg.getParameter<int>("min_muon_hits")),
    min_pixel_layers(cfg.getParameter<int>("min_pixel_layers")),
    min_strip_layers(cfg.getParameter<int>("min_strip_layers")),
    max_tpfms_station(cfg.getParameter<int>("max_tpfms_station")),
    no_dt_allowed(cfg.getParameter<bool>("no_dt_allowed")),
    no_csc_allowed(cfg.getParameter<bool>("no_csc_allowed")),
    check_tksta_dphi(cfg.getParameter<bool>("check_tksta_dphi")),
    check_shared_hits(cfg.getParameter<bool>("check_shared_hits")),
    use_unpropagated_values(cfg.getParameter<bool>("use_unpropagated_values")),
    pp_reco_mode(cfg.getParameter<bool>("pp_reco_mode")),
    force_run_list(cfg.getParameter<std::vector<unsigned> >("force_run_list")),
    force_event_list(cfg.getParameter<std::vector<unsigned> >("force_event_list")),
    require_tt25(cfg.getParameter<bool>("require_tt25")),
    require_not_tt25(cfg.getParameter<bool>("require_not_tt25")),
    min_bfield(cfg.getParameter<double>("min_bfield")),
    copy_selected_events(cfg.getParameter<bool>("copy_selected_events"))
{
  if (only_sample >= 0)
    Loader::use_weights = false;

  if (force_event_list.size() % 3 != 0)
    throw cms::Exception("CosmicSplittingResolutionHistos") << "force_event_list size must be a multiple of 3\n";

  // Make the bins from the vector of ParameterSets. See the config
  // file for how this works. Make a set of Bins for each type of
  // track (e.g. {..., pT100200, pT2002000, ... } x {..., TkOnly, TPFMS, ... }
  for (int i = 0; i < n_tracks; ++i)
    BOOST_FOREACH(const edm::ParameterSet& bin_pset, cfg.getParameter<std::vector<edm::ParameterSet> >("bins")) {
      Bin* b = new Bin(bin_pset, i, is_mc);
      // JMTBAD rework these flags as in the above JMTBAD
      b->use_unpropagated_values = use_unpropagated_values;
      b->flip_upper_charge = pp_reco_mode;
      bins.push_back(b);
    }

  edm::Service<TFileService> fs;

  // Make a summary historgam of how many events failed which cuts.
  errors = fs->make<TH1F>("errors", "", error_last, 0, error_last);
  errors->GetXaxis()->SetBinLabel(1 + error_none,                "none");
  errors->GetXaxis()->SetBinLabel(1 + error_bad_run,             "bad_run");
  errors->GetXaxis()->SetBinLabel(1 + error_bfield,              "bfield");
  errors->GetXaxis()->SetBinLabel(1 + error_bad_event,           "bad_event");
  errors->GetXaxis()->SetBinLabel(1 + error_tt25,                "tt25");
  errors->GetXaxis()->SetBinLabel(1 + error_wrong_sample,        "wrong_sample");
  errors->GetXaxis()->SetBinLabel(1 + error_propagation,         "propagation");
  errors->GetXaxis()->SetBinLabel(1 + error_prop_mc,             "prop_mc");
  errors->GetXaxis()->SetBinLabel(1 + error_muon_hits,           "muon_hits");
  errors->GetXaxis()->SetBinLabel(1 + error_pixels,              "pixels");
  errors->GetXaxis()->SetBinLabel(1 + error_strips,              "strips");
  errors->GetXaxis()->SetBinLabel(1 + error_tpfms_station,       "tpfms_station");
  errors->GetXaxis()->SetBinLabel(1 + error_dt,                  "dt");
  errors->GetXaxis()->SetBinLabel(1 + error_csc,                 "csc");
  errors->GetXaxis()->SetBinLabel(1 + error_tksta_dphi,          "tksta_dphi");
  errors->GetXaxis()->SetBinLabel(1 + error_shared_hits,         "shared_hits");

  h_global_upper_inner_pos_xy = fs->make<TH2I>("h_global_upper_inner_pos_xy", "", 300, -750, 750, 300, -750, 750);
  h_global_upper_outer_pos_xy = fs->make<TH2I>("h_global_upper_outer_pos_xy", "", 300, -750, 750, 300, -750, 750);
  h_global_lower_inner_pos_xy = fs->make<TH2I>("h_global_lower_inner_pos_xy", "", 300, -750, 750, 300, -750, 750);
  h_global_lower_outer_pos_xy = fs->make<TH2I>("h_global_lower_outer_pos_xy", "", 300, -750, 750, 300, -750, 750);
  h_global_upper_inner_pos_rz = fs->make<TH2I>("h_global_upper_inner_pos_rz", "", 430, -1075, 1075, 320, -800, 800);
  h_global_upper_outer_pos_rz = fs->make<TH2I>("h_global_upper_outer_pos_rz", "", 430, -1075, 1075, 320, -800, 800);
  h_global_lower_inner_pos_rz = fs->make<TH2I>("h_global_lower_inner_pos_rz", "", 430, -1075, 1075, 320, -800, 800);
  h_global_lower_outer_pos_rz = fs->make<TH2I>("h_global_lower_outer_pos_rz", "", 430, -1075, 1075, 320, -800, 800);

  // Load the tree, and branch to our ntuple object.
  tree = 0;
  const char* tree_name = "t";
  file = new TFile(filename.c_str());
  if (file != 0 && file->IsOpen()) {
    TDirectory* dir = (TDirectory*)file->Get(directory.c_str());
    if (dir != 0) {
      tree = (TTree*)dir->Get(tree_name);

      TFileDirectory d = fs->mkdir("copied_histograms");
      static const char* to_copy[3] = {"track_multiplicity", "muon_multiplicity", "errors"};
      for (int i = 0; i < 3; ++i) {
	TH1F* h = (TH1F*)dir->Get(to_copy[i]);
	// Why can't you just Clone to an existing histogram?  Copy
	// the bin contents, skip errors for now. If there's labels
	// (i.e. the errors histogram), copy those too.
	TH1F* hnew = d.make<TH1F>(h->GetName(), h->GetTitle(), h->GetNbinsX(), h->GetXaxis()->GetBinLowEdge(1), h->GetXaxis()->GetBinLowEdge(h->GetNbinsX()+1));
	for (int j = 0; j <= h->GetNbinsX()+1; ++j)
	  hnew->SetBinContent(j, h->GetBinContent(j));
	if (h->GetXaxis()->GetLabels())
	  for (int j = 1; j <= h->GetNbinsX(); ++j)
	    hnew->GetXaxis()->SetBinLabel(j, h->GetXaxis()->GetBinLabel(j));
      }
    }
  }
  
  if (tree == 0)
    throw cms::Exception("CosmicSplittingResolutionHistos") << "couldn't get tree '" << tree_name << "' from file " << filename << " in directory " << directory << "!\n";

  out << "Tree '" << tree_name << "' from file " << filename << " in directory " << directory << " has " << tree->GetEntriesFast() << " entries.\n";

  nt = new CosmicSplittingResolutionNtuple;
  read_from_tree(tree, nt);

  // Can make a copy of the ntuple for just the selected events.
  if (copy_selected_events) {
    events_used = fs->make<TTree>("events_used", "");
    write_to_tree(events_used, nt);
  }
}

CosmicSplittingResolutionHistos::~CosmicSplittingResolutionHistos() {
  BOOST_FOREACH(Bin* b, bins)
    delete b;
  file->Close();
  delete file;
  delete nt;
}

bool CosmicSplittingResolutionHistos::run_is_bad(unsigned run) {
  // JMTBAD optimize
  if (force_run_list.size() > 0)
    return std::find(force_run_list.begin(), force_run_list.end(), run) == force_run_list.end();
  else
    return false;
}

bool CosmicSplittingResolutionHistos::event_is_bad(unsigned run, unsigned lumi, unsigned event) {
  // JMTBAD optimize
  size_t n = force_event_list.size();
  if (n > 0) {
    assert(n % 3 == 0);
    bool found = false;
    for (size_t i = 0; i < n; i += 3) {
      if (run == force_event_list[i] && lumi == force_event_list[i+1] && event == force_event_list[i+2]) {
	found = true;
	break;
      }
    }
    return not found;
  }
  else
    return false;
}

CosmicSplittingResolutionHistos::error_code CosmicSplittingResolutionHistos::cut() {
  // Apply the cuts, keeping track using the errors histogram defined
  // above.

  if (!is_mc && run_is_bad(nt->run))
    return error_bad_run;

  if (nt->bzat0 < min_bfield)
    return error_bfield;

  if (event_is_bad(nt->run, nt->lumi, nt->event))
    return error_bad_event;

  if (require_tt25 && !nt->tt25)
    return error_tt25;

  if (require_not_tt25 && nt->tt25)
    return error_tt25;

  if (is_mc && check_for_wrong_sample) {
    if (only_sample >= 0 && nt->id != unsigned(only_sample))
      return error_wrong_sample; // JMTBAD different error code

    double unprop_mc_p = nt->unprop_mc_pt/sin(nt->unprop_mc_theta);
    if ((nt->id == mc_10 && unprop_mc_p > 100) || (nt->id == mc_100 && unprop_mc_p > 500))
      return error_wrong_sample;
  }

  // Make sure all the tracks were able to propagate (except don't
  // worry about STA for now).
  if (!use_unpropagated_values) {
    for (int i = 0; i < n_tracks; ++i) {
      if (i == tk_stalone)
	continue;

      for (int j = 0; j < 2; ++j) {
	if (!nt->prop_valid[i][j]) {
	  out << "Event failed selection on track " << i << "," << j << ": propagation failed!\n";
	  return error_propagation;
	}
      }

      if (is_mc && !nt->mc_prop_valid[i]) {
	out << "Event failed selection on track " << i << ": MC propagation failed!\n";
	return error_prop_mc;
      }
    }
  }

  if (min_muon_hits > 0)
    for (int j = 0; j < 2; ++j) {
      if (nt->muon_hits[tk_global][j] < min_muon_hits) {
	out << "Event failed selection on global track " << j << ": muon hits: " << nt->muon_hits[tk_global][j] << "/" << min_muon_hits << "\n";
	return error_muon_hits;
      }
    }

  if (min_pixel_layers > 0 || min_strip_layers > 0)
    for (int i = 0; i < n_tracks; ++i) {
      // Don't check stand-alone, which by definition has no pixel or
      // strip hits.
      if (i == tk_stalone)
	continue;

      for (int j = 0; j < 2; ++j) {
	if (nt->pixel_layers[i][j] < min_pixel_layers) {
	  out << "Event failed selection on track " << i << "," << j << ": pixel layers: " << nt->pixel_layers[i][j] << "/" << min_pixel_layers << "\n";
	  return error_pixels;
	}
	if (nt->strip_layers[i][j] < min_strip_layers) {
	  out << "Event failed selection on track " << i << "," << j << ": strip layers: " << nt->strip_layers[i][j] << "/" << min_strip_layers << "\n";
	  return error_strips;
	}
      }
    }
  
  if (max_tpfms_station > 0)
    for (int j = 0; j < 2; ++j)
      if (nt->tpfms_first_station[j] > max_tpfms_station) {
	out << "Event failed selection: TPFMS track " << j << " hit station " << nt->tpfms_first_station[j] << " max allowed: " << max_tpfms_station << "\n";
	return error_tpfms_station;
      }

  if (no_dt_allowed && nt->hit_dt) {
    out << "Event failed selection: some track hit a DT!\n";
    return error_dt;
  }

  if (no_csc_allowed && nt->hit_csc) {
    out << "Event failed selection: some track hit a CSC!\n";
    return error_csc;
  }

  // Make sure the tracker-standalone matching went well. Do this by
  // checking delta phi between the two track fits.
  if (check_tksta_dphi)
    for (int j = 0; j < 2; ++j) {
      double dphi = reco::deltaPhi(nt->phi[tk_tkonly][j], nt->phi[tk_stalone][j]);
      if (fabs(dphi) > 0.1) {
        out << "Event failed selection: tracker-standalone mismatch for global muon #" << j << ": " << dphi << "\n";
        return error_tksta_dphi;
      }
    }

  // Don't allow upper, lower tracks to share hits.
  if (check_shared_hits)
    for (int i = 0; i < n_tracks; ++i)
      if (nt->shared_hits[i])
	return error_shared_hits;

  return error_none;
}

float xy_to_r(float x, float y) {
  float r = sqrt(x*x + y*y);
  if (y < 0)
    r *= -1;
  return r;
}

void CosmicSplittingResolutionHistos::analyze(const edm::Event&, const edm::EventSetup&) {
  printf("\nEvent loop:\n");

  long jentry;
  for (jentry = 0; jentry < tree->GetEntriesFast(); ++jentry) {
    if (tree->LoadTree(jentry) < 0) break;
    if (tree->GetEntry(jentry) <= 0) continue;

    error_code e = cut();
    errors->Fill(e);
    if (e != error_none)
      continue;

    h_global_upper_inner_pos_xy->Fill(nt->inner_pos[tk_global][upper][0], nt->inner_pos[tk_global][upper][1]);
    h_global_upper_outer_pos_xy->Fill(nt->outer_pos[tk_global][upper][0], nt->outer_pos[tk_global][upper][1]);
    h_global_lower_inner_pos_xy->Fill(nt->inner_pos[tk_global][lower][0], nt->inner_pos[tk_global][lower][1]);
    h_global_lower_outer_pos_xy->Fill(nt->outer_pos[tk_global][lower][0], nt->outer_pos[tk_global][lower][1]);

    h_global_upper_inner_pos_rz->Fill(nt->inner_pos[tk_global][upper][2], xy_to_r(nt->inner_pos[tk_global][upper][0], nt->inner_pos[tk_global][upper][1]));
    h_global_upper_outer_pos_rz->Fill(nt->outer_pos[tk_global][upper][2], xy_to_r(nt->outer_pos[tk_global][upper][0], nt->outer_pos[tk_global][upper][1]));
    h_global_lower_inner_pos_rz->Fill(nt->inner_pos[tk_global][lower][2], xy_to_r(nt->inner_pos[tk_global][lower][0], nt->inner_pos[tk_global][lower][1]));
    h_global_lower_outer_pos_rz->Fill(nt->outer_pos[tk_global][lower][2], xy_to_r(nt->outer_pos[tk_global][lower][0], nt->outer_pos[tk_global][lower][1]));
    
    if (copy_selected_events)
      events_used->Fill();

    BOOST_FOREACH(Bin* b, bins)
      b->check_and_fill(nt); //&out);
  }

  printf("\ndone with %li events!\n", jentry);
  //std::cout << out.str();
}

DEFINE_FWK_MODULE(CosmicSplittingResolutionHistos);
