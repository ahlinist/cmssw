#include "SimDataFormats/GlobalValidation/interface/PGlobalSimHit.h"

void PGlobalSimHit::putRawGenPart(int n)
{
  nRawGenPart = n;
  return;
}

void PGlobalSimHit::putG4Vtx(std::vector<float> x, std::vector<float> y, 
	       std::vector<float> z)
{
  nG4Vtx = x.size();
  G4Vtx.resize(nG4Vtx);
  for (int i = 0; i < nG4Vtx; ++i) {
    G4Vtx[i].x = x[i];
    G4Vtx[i].y = y[i];
    G4Vtx[i].z = z[i];
  }

  return;
}

void PGlobalSimHit::putG4Trk(std::vector<float> pt, std::vector<float> e)
{
  nG4Trk = pt.size();
  G4Trk.resize(nG4Trk);
  for (int i = 0; i < nG4Trk; ++i) {
    G4Trk[i].pt = pt[i];
    G4Trk[i].e = e[i];
  }

  return;
}

void PGlobalSimHit::putECalHits(std::vector<float> e, std::vector<float> tof,
				 std::vector<float> phi, 
				 std::vector<float> eta)
{
  nECalHits = e.size();
  ECalHits.resize(nECalHits);
  for (int i = 0; i < nECalHits; ++i) {
    ECalHits[i].e = e[i];
    ECalHits[i].tof = tof[i];
    ECalHits[i].phi = phi[i];
    ECalHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putPreShHits(std::vector<float> e, std::vector<float> tof,
				  std::vector<float> phi, 
				  std::vector<float> eta)
{
  nPreShHits = e.size();
  PreShHits.resize(nPreShHits);
  for (int i = 0; i < nPreShHits; ++i) {
    PreShHits[i].e = e[i];
    PreShHits[i].tof = tof[i];
    PreShHits[i].phi = phi[i];
    PreShHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putHCalHits(std::vector<float> e, std::vector<float> tof,
				 std::vector<float> phi, 
				 std::vector<float> eta)
{
  nHCalHits = e.size();
  HCalHits.resize(nHCalHits);
  for (int i = 0; i < nHCalHits; ++i) {
    HCalHits[i].e = e[i];
    HCalHits[i].tof = tof[i];
    HCalHits[i].phi = phi[i];
    HCalHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putPxlFwdHits(std::vector<float> tof, 
				   std::vector<float> z,
				   std::vector<float> phi, 
				   std::vector<float> eta)
{
  nPxlFwdHits = tof.size();
  PxlFwdHits.resize(nPxlFwdHits);
  for (int i = 0; i < nPxlFwdHits; ++i) {
    PxlFwdHits[i].tof = tof[i];
    PxlFwdHits[i].z = z[i];
    PxlFwdHits[i].phi = phi[i];
    PxlFwdHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putPxlBrlHits(std::vector<float> tof, 
				   std::vector<float> r,
				   std::vector<float> phi, 
				   std::vector<float> eta)
{
  nPxlBrlHits = tof.size(); 
  PxlBrlHits.resize(nPxlBrlHits);
  for (int i = 0; i < nPxlBrlHits; ++i) {
    PxlBrlHits[i].tof = tof[i];
    PxlBrlHits[i].r = r[i];
    PxlBrlHits[i].phi = phi[i];
    PxlBrlHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putSiFwdHits(std::vector<float> tof, 
				  std::vector<float> z,
				  std::vector<float> phi, 
				  std::vector<float> eta)
{
  nSiFwdHits = tof.size();
  SiFwdHits.resize(nSiFwdHits);
  for (int i = 0; i < nSiFwdHits; ++i) {
    SiFwdHits[i].tof = tof[i];
    SiFwdHits[i].z = z[i];
    SiFwdHits[i].phi = phi[i];
    SiFwdHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putSiBrlHits(std::vector<float> tof, std::vector<float> r,
				  std::vector<float> phi, 
				  std::vector<float> eta)
{
  nSiBrlHits = tof.size();
  SiBrlHits.resize(nSiBrlHits);
  for (int i = 0; i < nSiBrlHits; ++i) {
    SiBrlHits[i].tof = tof[i];
    SiBrlHits[i].r = r[i];
    SiBrlHits[i].phi = phi[i];
    SiBrlHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putMuonCscHits(std::vector<float> tof, 
				    std::vector<float> z,
				    std::vector<float> phi, 
				    std::vector<float> eta)
{
  nMuonCscHits = tof.size();
  MuonCscHits.resize(nMuonCscHits);
  for (int i = 0; i < nMuonCscHits; ++i) {
    MuonCscHits[i].tof = tof[i];
    MuonCscHits[i].z = z[i];
    MuonCscHits[i].phi = phi[i];
    MuonCscHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putMuonDtHits(std::vector<float> tof, 
				   std::vector<float> r,
				   std::vector<float> phi, 
				   std::vector<float> eta)
{
  nMuonDtHits = tof.size();
  MuonDtHits.resize(nMuonDtHits);
  for (int i = 0; i < nMuonDtHits; ++i) {
    MuonDtHits[i].tof = tof[i];
    MuonDtHits[i].r = r[i];
    MuonDtHits[i].phi = phi[i];
    MuonDtHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putMuonRpcFwdHits(std::vector<float> tof, 
				       std::vector<float> z,
				       std::vector<float> phi, 
				       std::vector<float> eta)
{
  nMuonRpcFwdHits = tof.size();
  MuonRpcFwdHits.resize(nMuonRpcFwdHits);
  for (int i = 0; i < nMuonRpcFwdHits; ++i) {
    MuonRpcFwdHits[i].tof = tof[i];
    MuonRpcFwdHits[i].z = z[i];
    MuonRpcFwdHits[i].phi = phi[i];
    MuonRpcFwdHits[i].eta = eta[i];
  }

  return;
}

void PGlobalSimHit::putMuonRpcBrlHits(std::vector<float> tof, 
				       std::vector<float> r,
				       std::vector<float> phi, 
				       std::vector<float> eta)
{
  nMuonRpcBrlHits = tof.size();
  MuonRpcBrlHits.resize(nMuonRpcBrlHits);
  for (int i = 0; i < nMuonRpcBrlHits; ++i) {
    MuonRpcBrlHits[i].tof = tof[i];
    MuonRpcBrlHits[i].r = r[i];
    MuonRpcBrlHits[i].phi = phi[i];
    MuonRpcBrlHits[i].eta = eta[i];
  }

  return;
}
