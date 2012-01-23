#include "MuonAnalysis/Cosmics/plugins/CosmicSplittingResolutionNtuple.h"

const char* track_nicks[n_tracks] = {
  "Global",
  "StAlone",
  "TkOnly",
  "TPFMS",
  "Picky",
  "DYT",
  "TMR",
  "SigSw",
  "TuneP",
  "StaGlb",
  "StaTko"
};

void branch_it(TTree* tree, const char* name, void* addr, const char* fmt) {
  const int sz = strlen(fmt) + 100;
  char* buf = new char[sz];
  snprintf(buf, sz, fmt, n_tracks);
  tree->Branch(name, addr, buf);
  delete[] buf;
}

void write_to_tree(TTree* tree, CosmicSplittingResolutionNtuple* nt) {
  branch_it(tree, "id", &nt->id, "id/i");
  branch_it(tree, "run", &nt->run, "run/i");
  branch_it(tree, "lumi", &nt->lumi, "lumi/i");
  branch_it(tree, "event", &nt->event, "event/i");
  branch_it(tree, "shared_hits", nt->shared_hits, "shared_hits[%i]/s");
  branch_it(tree, "prop_pos", nt->prop_pos, "prop_pos[%i][2][3]/F");
  branch_it(tree, "inner_pos", nt->inner_pos, "inner_pos[%i][2][3]/F");
  branch_it(tree, "outer_pos", nt->outer_pos, "outer_pos[%i][2][3]/F");
  branch_it(tree, "charge", nt->charge, "charge[%i][2]/S");
  branch_it(tree, "chi2", nt->chi2, "chi2[%i][2]/F");
  branch_it(tree, "ndof", nt->ndof, "ndof[%i][2]/s");
  branch_it(tree, "prob", nt->prob, "prob[%i][2]/F");
  branch_it(tree, "pixel_layers", nt->pixel_layers, "pixel_layers[%i][2]/s");
  branch_it(tree, "strip_layers", nt->strip_layers, "strip_layers[%i][2]/s");
  branch_it(tree, "pixel_hits", nt->pixel_hits, "pixel_hits[%i][2]/s");
  branch_it(tree, "strip_hits", nt->strip_hits, "strip_hits[%i][2]/s");
  branch_it(tree, "muon_hits", nt->muon_hits, "muon_hits[%i][2]/s");
  branch_it(tree, "qoverp", nt->qoverp, "qoverp[%i][2]/F");
  branch_it(tree, "error_qoverp", nt->error_qoverp, "error_qoverp[%i][2]/F");
  branch_it(tree, "prop_valid", nt->prop_valid, "prop_valid[%i][2]/O");
  branch_it(tree, "pt", nt->pt, "pt[%i][2]/F");
  branch_it(tree, "theta", nt->theta, "theta[%i][2]/F");
  branch_it(tree, "phi", nt->phi, "phi[%i][2]/F");
  branch_it(tree, "dxy", nt->dxy, "dxy[%i][2]/F");
  branch_it(tree, "dz", nt->dz, "dz[%i][2]/F");
  branch_it(tree, "bz", nt->bz, "bz[%i][2]/F");
  branch_it(tree, "kappa", nt->kappa, "kappa[%i][2]/F");
  branch_it(tree, "error_kappa", nt->error_kappa, "error_kappa[%i][2]/F");
  branch_it(tree, "error_pt", nt->error_pt, "error_pt[%i][2]/F");
  branch_it(tree, "error_theta", nt->error_theta, "error_theta[%i][2]/F");
  branch_it(tree, "error_phi", nt->error_phi, "error_phi[%i][2]/F");
  branch_it(tree, "error_dxy", nt->error_dxy, "error_dxy[%i][2]/F");
  branch_it(tree, "error_dz", nt->error_dz, "error_dz[%i][2]/F");
  branch_it(tree, "unprop_charge", nt->unprop_charge, "unprop_charge[%i][2]/S");
  branch_it(tree, "unprop_pt", nt->unprop_pt, "unprop_pt[%i][2]/F");
  branch_it(tree, "unprop_theta", nt->unprop_theta, "unprop_theta[%i][2]/F");
  branch_it(tree, "unprop_phi", nt->unprop_phi, "unprop_phi[%i][2]/F");
  branch_it(tree, "unprop_dxy", nt->unprop_dxy, "unprop_dxy[%i][2]/F");
  branch_it(tree, "unprop_dz", nt->unprop_dz, "unprop_dz[%i][2]/F");
  branch_it(tree, "unprop_error_pt", nt->unprop_error_pt, "unprop_error_pt[%i][2]/F");
  branch_it(tree, "unprop_error_theta", nt->unprop_error_theta, "unprop_error_theta[%i][2]/F");
  branch_it(tree, "unprop_error_phi", nt->unprop_error_phi, "unprop_error_phi[%i][2]/F");
  branch_it(tree, "unprop_error_dxy", nt->unprop_error_dxy, "unprop_error_dxy[%i][2]/F");
  branch_it(tree, "unprop_error_dz", nt->unprop_error_dz, "unprop_error_dz[%i][2]/F");
  branch_it(tree, "mc_vertex", nt->mc_vertex, "mc_vertex[3]/F");
  branch_it(tree, "unprop_mc_charge", &nt->unprop_mc_charge, "unprop_mc_charge/S");
  branch_it(tree, "unprop_mc_pt", &nt->unprop_mc_pt, "unprop_mc_pt/F");
  branch_it(tree, "unprop_mc_theta", &nt->unprop_mc_theta, "unprop_mc_theta/F");
  branch_it(tree, "unprop_mc_phi", &nt->unprop_mc_phi, "unprop_mc_phi/F");
  branch_it(tree, "mc_prop_pos", nt->mc_prop_pos, "mc_prop_pos[%i][3]/F");
  branch_it(tree, "mc_prop_valid", nt->mc_prop_valid, "mc_prop_valid[%i]/O");
  branch_it(tree, "mc_charge", nt->mc_charge, "mc_charge[%i]/S");
  branch_it(tree, "mc_pt", nt->mc_pt, "mc_pt[%i]/F");
  branch_it(tree, "mc_theta", nt->mc_theta, "mc_theta[%i]/F");
  branch_it(tree, "mc_phi", nt->mc_phi, "mc_phi[%i]/F");
  branch_it(tree, "mc_dxy", nt->mc_dxy, "mc_dxy[%i]/F");
  branch_it(tree, "mc_dz", nt->mc_dz, "mc_dz[%i]/F");
  branch_it(tree, "ref_charge", &nt->ref_charge, "ref_charge/S");
  branch_it(tree, "ref_chi2", &nt->ref_chi2, "ref_chi2/F");
  branch_it(tree, "ref_ndof", &nt->ref_ndof, "ref_ndof/s");
  branch_it(tree, "ref_pixel_layers", &nt->ref_pixel_layers, "ref_pixel_layers/s");
  branch_it(tree, "ref_strip_layers", &nt->ref_strip_layers, "ref_strip_layers/s");
  branch_it(tree, "ref_pixel_hits", &nt->ref_pixel_hits, "ref_pixel_hits/s");
  branch_it(tree, "ref_strip_hits", &nt->ref_strip_hits, "ref_strip_hits/s");
  branch_it(tree, "ref_muon_hits", &nt->ref_muon_hits, "ref_muon_hits/s");
  branch_it(tree, "ref_pt", &nt->ref_pt, "ref_pt/F");
  branch_it(tree, "ref_theta", &nt->ref_theta, "ref_theta/F");
  branch_it(tree, "ref_phi", &nt->ref_phi, "ref_phi/F");
  branch_it(tree, "ref_dxy", &nt->ref_dxy, "ref_dxy/F");
  branch_it(tree, "ref_dz", &nt->ref_dz, "ref_dz/F");
  branch_it(tree, "ref_error_pt", &nt->ref_error_pt, "ref_error_pt/F");
  branch_it(tree, "ref_error_theta", &nt->ref_error_theta, "ref_error_theta/F");
  branch_it(tree, "ref_error_phi", &nt->ref_error_phi, "ref_error_phi/F");
  branch_it(tree, "ref_error_dxy", &nt->ref_error_dxy, "ref_error_dxy/F");
  branch_it(tree, "ref_error_dz", &nt->ref_error_dz, "ref_error_dz/F");
  branch_it(tree, "hit_dt", &nt->hit_dt, "hit_dt/O");
  branch_it(tree, "hit_csc", &nt->hit_csc, "hit_csc/O");
  branch_it(tree, "tpfms_first_station", nt->tpfms_first_station, "tpfms_first_station[2]/s");
  branch_it(tree, "choice_tmr", nt->choice_tmr, "choice_tmr[2]/s");
  branch_it(tree, "choice_sigsw",  nt->choice_sigsw,  "choice_sigsw[2]/s");
  branch_it(tree, "choice_tunep", nt->choice_tunep, "choice_tunep[2]/s");
  branch_it(tree, "tt25", &nt->tt25, "tt25/O");
  branch_it(tree, "bzat0", &nt->bzat0, "bzat0/F");

  tree->SetAlias("ref_eta", "-log(tan(ref_theta / 2))");
}

void read_from_tree(TTree* tree, CosmicSplittingResolutionNtuple* nt) {
  tree->SetBranchAddress("id", &nt->id);
  tree->SetBranchAddress("run", &nt->run);
  tree->SetBranchAddress("lumi", &nt->lumi);
  tree->SetBranchAddress("event", &nt->event);
  tree->SetBranchAddress("shared_hits", nt->shared_hits);
  tree->SetBranchAddress("prop_pos", nt->prop_pos);
  tree->SetBranchAddress("inner_pos", nt->inner_pos);
  tree->SetBranchAddress("outer_pos", nt->outer_pos);
  tree->SetBranchAddress("charge", nt->charge);
  tree->SetBranchAddress("chi2", nt->chi2);
  tree->SetBranchAddress("ndof", nt->ndof);
  tree->SetBranchAddress("prob", nt->prob);
  tree->SetBranchAddress("pixel_layers", nt->pixel_layers);
  tree->SetBranchAddress("strip_layers", nt->strip_layers);
  tree->SetBranchAddress("pixel_hits", nt->pixel_hits);
  tree->SetBranchAddress("strip_hits", nt->strip_hits);
  tree->SetBranchAddress("muon_hits", nt->muon_hits);
  tree->SetBranchAddress("qoverp", nt->qoverp);
  tree->SetBranchAddress("error_qoverp", nt->error_qoverp);
  tree->SetBranchAddress("prop_valid", nt->prop_valid);
  tree->SetBranchAddress("pt", nt->pt);
  tree->SetBranchAddress("theta", nt->theta);
  tree->SetBranchAddress("phi", nt->phi);
  tree->SetBranchAddress("dxy", nt->dxy);
  tree->SetBranchAddress("dz", nt->dz);
  tree->SetBranchAddress("bz", nt->bz);
  tree->SetBranchAddress("kappa", nt->kappa);
  tree->SetBranchAddress("error_kappa", nt->error_kappa);
  tree->SetBranchAddress("error_pt", nt->error_pt);
  tree->SetBranchAddress("error_theta", nt->error_theta);
  tree->SetBranchAddress("error_phi", nt->error_phi);
  tree->SetBranchAddress("error_dxy", nt->error_dxy);
  tree->SetBranchAddress("error_dz", nt->error_dz);
  tree->SetBranchAddress("unprop_charge", nt->unprop_charge);
  tree->SetBranchAddress("unprop_pt", nt->unprop_pt);
  tree->SetBranchAddress("unprop_theta", nt->unprop_theta);
  tree->SetBranchAddress("unprop_phi", nt->unprop_phi);
  tree->SetBranchAddress("unprop_dxy", nt->unprop_dxy);
  tree->SetBranchAddress("unprop_dz", nt->unprop_dz);
  tree->SetBranchAddress("unprop_error_pt", nt->unprop_error_pt);
  tree->SetBranchAddress("unprop_error_theta", nt->unprop_error_theta);
  tree->SetBranchAddress("unprop_error_phi", nt->unprop_error_phi);
  tree->SetBranchAddress("unprop_error_dxy", nt->unprop_error_dxy);
  tree->SetBranchAddress("unprop_error_dz", nt->unprop_error_dz);
  tree->SetBranchAddress("mc_vertex", nt->mc_vertex);
  tree->SetBranchAddress("unprop_mc_charge", &nt->unprop_mc_charge);
  tree->SetBranchAddress("unprop_mc_pt", &nt->unprop_mc_pt);
  tree->SetBranchAddress("unprop_mc_theta", &nt->unprop_mc_theta);
  tree->SetBranchAddress("unprop_mc_phi", &nt->unprop_mc_phi);
  tree->SetBranchAddress("mc_prop_pos", nt->mc_prop_pos);
  tree->SetBranchAddress("mc_prop_valid", nt->mc_prop_valid);
  tree->SetBranchAddress("mc_charge", nt->mc_charge);
  tree->SetBranchAddress("mc_pt", nt->mc_pt);
  tree->SetBranchAddress("mc_theta", nt->mc_theta);
  tree->SetBranchAddress("mc_phi", nt->mc_phi);
  tree->SetBranchAddress("mc_dxy", nt->mc_dxy);
  tree->SetBranchAddress("mc_dz", nt->mc_dz);
  tree->SetBranchAddress("ref_charge", &nt->ref_charge);
  tree->SetBranchAddress("ref_chi2", &nt->ref_chi2);
  tree->SetBranchAddress("ref_ndof", &nt->ref_ndof);
  tree->SetBranchAddress("ref_pixel_layers", &nt->ref_pixel_layers);
  tree->SetBranchAddress("ref_strip_layers", &nt->ref_strip_layers);
  tree->SetBranchAddress("ref_pixel_hits", &nt->ref_pixel_hits);
  tree->SetBranchAddress("ref_strip_hits", &nt->ref_strip_hits);
  tree->SetBranchAddress("ref_muon_hits", &nt->ref_muon_hits);
  tree->SetBranchAddress("ref_pt", &nt->ref_pt);
  tree->SetBranchAddress("ref_theta", &nt->ref_theta);
  tree->SetBranchAddress("ref_phi", &nt->ref_phi);
  tree->SetBranchAddress("ref_dxy", &nt->ref_dxy);
  tree->SetBranchAddress("ref_dz", &nt->ref_dz);
  tree->SetBranchAddress("ref_error_pt", &nt->ref_error_pt);
  tree->SetBranchAddress("ref_error_theta", &nt->ref_error_theta);
  tree->SetBranchAddress("ref_error_phi", &nt->ref_error_phi);
  tree->SetBranchAddress("ref_error_dxy", &nt->ref_error_dxy);
  tree->SetBranchAddress("ref_error_dz", &nt->ref_error_dz);
  tree->SetBranchAddress("hit_dt", &nt->hit_dt);
  tree->SetBranchAddress("hit_csc", &nt->hit_csc);
  tree->SetBranchAddress("tpfms_first_station", nt->tpfms_first_station);
  tree->SetBranchAddress("choice_tmr", nt->choice_tmr);
  tree->SetBranchAddress("choice_sigsw", nt->choice_sigsw);
  tree->SetBranchAddress("choice_tunep", nt->choice_tunep);
  tree->SetBranchAddress("tt25", &nt->tt25);
  tree->SetBranchAddress("bzat0", &nt->bzat0);
}
