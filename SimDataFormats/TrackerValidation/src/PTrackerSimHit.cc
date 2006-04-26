#include "SimDataFormats/TrackerValidation/interface/PTrackerSimHit.h"

void PTrackerSimHit::putRawGenPart(int n)
{
  nRawGenPart = n;
  return;
}

void PTrackerSimHit::putG4Vtx(std::vector<float> x, std::vector<float> y, 
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

void PTrackerSimHit::putG4Trk(std::vector<float> pt, std::vector<float> e,
		              std::vector<float> eta, std::vector<float> phi)
{
  nG4Trk = pt.size();
  G4Trk.resize(nG4Trk);
  for (int i = 0; i < nG4Trk; ++i) {
    G4Trk[i].pt = pt[i];
    G4Trk[i].e = e[i];
    G4Trk[i].eta = eta[i];
    G4Trk[i].phi = phi[i];
  }

  return;
}


void PTrackerSimHit::putHits (std::vector<int> _sysID, std::vector<float> _detUnitId,
	       std::vector<float>_trackId , std::vector<float>_processType,
	       std::vector<float>_particleType, std::vector<float> _pabs,
	       std::vector<float>_lpx, std::vector<float>_lpy, std::vector<float>_lpz,
	       std::vector<float>_ldx, std::vector<float>_ldy, std::vector<float>_ldz,
	       std::vector<float>_ldtheta, std::vector<float>_ldphi, 
	       std::vector<float>_exx, std::vector<float>_exy, std::vector<float>_exz,
	       std::vector<float>_enx, std::vector<float>_eny, std::vector<float>_enz,
	       std::vector<float>_eloss, std::vector<float>_tof)   

{
  nHits = _tof.size();
  Hits.resize(nHits);
  for (int i = 0; i < nHits; ++i) {
    Hits[i]._sysID = _sysID[i];
    Hits[i]._detUnitId = _detUnitId[i];
    Hits[i]._trackId = _trackId[i];
    Hits[i]._processType = _processType[i];
    Hits[i]._particleType = _particleType[i];
    Hits[i]._pabs = _pabs[i];
    Hits[i]._lpx = _lpx[i];
    Hits[i]._lpy = _lpy[i];
    Hits[i]._lpz = _lpz[i];
    Hits[i]._ldx = _ldx[i];
    Hits[i]._ldy = _ldy[i];
    Hits[i]._ldz = _ldz[i];
    Hits[i]._ldtheta = _ldtheta[i];
    Hits[i]._ldphi = _ldphi[i];
    Hits[i]._exx = _exx[i];
    Hits[i]._exy = _exy[i];
    Hits[i]._exz = _exz[i];
    Hits[i]._enx = _enx[i];
    Hits[i]._eny = _eny[i];
    Hits[i]._enz = _enz[i];
    Hits[i]._eloss = _eloss[i];
    Hits[i]._tof = _tof[i];
  }

  return;
}





