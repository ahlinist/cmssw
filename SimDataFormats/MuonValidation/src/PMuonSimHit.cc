#include "SimDataFormats/MuonValidation/interface/PMuonSimHit.h"

void PMuonSimHit::putRawGenPart(int n)
{
  nRawGenPart = n;
  return;
}

void PMuonSimHit::putG4Vtx(std::vector<float> x, std::vector<float> y, 
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

void PMuonSimHit::putG4Trk(std::vector<float> pt,  std::vector<float> e,
		           std::vector<float> eta, std::vector<float> phi)
{
  nG4Trk = pt.size();
  G4Trk.resize(nG4Trk);
  for (int i = 0; i < nG4Trk; ++i) {
    G4Trk[i].pt  = pt[i];
    G4Trk[i].e   = e[i];
    G4Trk[i].eta = eta[i];
    G4Trk[i].phi = phi[i];
  }
  return;
}

void PMuonSimHit::putCSCHits (
                              std::vector<int>   _cscId,
                              std::vector<unsigned int> _detUnitId,
                              std::vector<float> _trackId , 
                              std::vector<float> _processType,
                              std::vector<float> _particleType, 
                              std::vector<float> _pabs,
                              std::vector<float> _globposz,
                              std::vector<float> _globposphi,
                              std::vector<float> _globposeta,
                              std::vector<float> _locposx, 
                              std::vector<float> _locposy, 
                              std::vector<float> _locposz,
                              std::vector<float> _locdirx, 
                              std::vector<float> _locdiry, 
                              std::vector<float> _locdirz,
                              std::vector<float> _locdirtheta, 
                              std::vector<float> _locdirphi, 
                              std::vector<float> _exitpointx, 
                              std::vector<float> _exitpointy, 
                              std::vector<float> _exitpointz,
                              std::vector<float> _entrypointx, 
                              std::vector<float> _entrypointy, 
                              std::vector<float> _entrypointz,
                              std::vector<float> _enloss, 
                              std::vector<float> _tof)   

{
  nCSCHits = _tof.size();
  CSCHits.resize(nCSCHits);
  for (int i = 0; i < nCSCHits; ++i) {
    CSCHits[i]._cscId           = _cscId[i];
    CSCHits[i]._detUnitId       = _detUnitId[i];
    CSCHits[i]._trackId         = _trackId[i];
    CSCHits[i]._processType     = _processType[i];
    CSCHits[i]._particleType    = _particleType[i];
    CSCHits[i]._pabs            = _pabs[i];
    CSCHits[i]._globposz        = _globposz[i];
    CSCHits[i]._globposphi      = _globposphi[i];
    CSCHits[i]._globposeta      = _globposeta[i];
    CSCHits[i]._locposx         = _locposx[i];
    CSCHits[i]._locposy         = _locposy[i];
    CSCHits[i]._locposz         = _locposz[i];
    CSCHits[i]._locdirx         = _locdirx[i];
    CSCHits[i]._locdiry         = _locdiry[i];
    CSCHits[i]._locdirz         = _locdirz[i];
    CSCHits[i]._locdirtheta     = _locdirtheta[i];
    CSCHits[i]._locdirphi       = _locdirphi[i];
    CSCHits[i]._exitpointx      = _exitpointx[i];
    CSCHits[i]._exitpointy      = _exitpointy[i];
    CSCHits[i]._exitpointz      = _exitpointz[i];
    CSCHits[i]._entrypointx     = _entrypointx[i];
    CSCHits[i]._entrypointy     = _entrypointy[i];
    CSCHits[i]._entrypointz     = _entrypointz[i];
    CSCHits[i]._enloss          = _enloss[i];
    CSCHits[i]._tof             = _tof[i];
  }
  return;
}

void PMuonSimHit::putDTHits  (std::vector<unsigned int> _detUnitId,
                              std::vector<float> _trackId , 
                              std::vector<float> _processType,
                              std::vector<float> _particleType, 
                              std::vector<float> _pabs,
                              std::vector<float> _globposz,
                              std::vector<float> _globposphi,
                              std::vector<float> _globposeta,
                              std::vector<float> _locposx, 
                              std::vector<float> _locposy, 
                              std::vector<float> _locposz,
                              std::vector<float> _locdirx, 
                              std::vector<float> _locdiry, 
                              std::vector<float> _locdirz,
                              std::vector<float> _locdirtheta, 
                              std::vector<float> _locdirphi, 
                              std::vector<float> _exitpointx, 
                              std::vector<float> _exitpointy, 
                              std::vector<float> _exitpointz,
                              std::vector<float> _entrypointx, 
                              std::vector<float> _entrypointy, 
                              std::vector<float> _entrypointz,
                              std::vector<float> _enloss, 
                              std::vector<float> _tof)   

{
  nDTHits = _tof.size();
  DTHits.resize(nDTHits);
  for (int i = 0; i < nDTHits; ++i) {
    DTHits[i]._detUnitId       = _detUnitId[i];
    DTHits[i]._trackId         = _trackId[i];
    DTHits[i]._processType     = _processType[i];
    DTHits[i]._particleType    = _particleType[i];
    DTHits[i]._pabs            = _pabs[i];
    DTHits[i]._globposz        = _globposz[i];
    DTHits[i]._globposphi      = _globposphi[i];
    DTHits[i]._globposeta      = _globposeta[i];
    DTHits[i]._locposx         = _locposx[i];
    DTHits[i]._locposy         = _locposy[i];
    DTHits[i]._locposz         = _locposz[i];
    DTHits[i]._locdirx         = _locdirx[i];
    DTHits[i]._locdiry         = _locdiry[i];
    DTHits[i]._locdirz         = _locdirz[i];
    DTHits[i]._locdirtheta     = _locdirtheta[i];
    DTHits[i]._locdirphi       = _locdirphi[i];
    DTHits[i]._exitpointx      = _exitpointx[i];
    DTHits[i]._exitpointy      = _exitpointy[i];
    DTHits[i]._exitpointz      = _exitpointz[i];
    DTHits[i]._entrypointx     = _entrypointx[i];
    DTHits[i]._entrypointy     = _entrypointy[i];
    DTHits[i]._entrypointz     = _entrypointz[i];
    DTHits[i]._enloss          = _enloss[i];
    DTHits[i]._tof             = _tof[i];
  }
  return;
}

void PMuonSimHit::putRPCHits (std::vector<unsigned int> _detUnitId,
                              std::vector<float> _trackId , 
                              std::vector<float> _processType,
                              std::vector<float> _particleType, 
                              std::vector<float> _pabs,
                              std::vector<float> _globposz,
                              std::vector<float> _globposphi,
                              std::vector<float> _globposeta,
                              std::vector<float> _locposx, 
                              std::vector<float> _locposy, 
                              std::vector<float> _locposz,
                              std::vector<float> _locdirx, 
                              std::vector<float> _locdiry, 
                              std::vector<float> _locdirz,
                              std::vector<float> _locdirtheta, 
                              std::vector<float> _locdirphi, 
                              std::vector<float> _exitpointx, 
                              std::vector<float> _exitpointy, 
                              std::vector<float> _exitpointz,
                              std::vector<float> _entrypointx, 
                              std::vector<float> _entrypointy, 
                              std::vector<float> _entrypointz,
                              std::vector<float> _enloss, 
                              std::vector<float> _tof)   

{
  nRPCHits = _tof.size();
  RPCHits.resize(nRPCHits);
  for (int i = 0; i < nRPCHits; ++i) {
    RPCHits[i]._detUnitId       = _detUnitId[i];
    RPCHits[i]._trackId         = _trackId[i];
    RPCHits[i]._processType     = _processType[i];
    RPCHits[i]._particleType    = _particleType[i];
    RPCHits[i]._pabs            = _pabs[i];
    RPCHits[i]._globposz        = _globposz[i];
    RPCHits[i]._globposphi      = _globposphi[i];
    RPCHits[i]._globposeta      = _globposeta[i];
    RPCHits[i]._locposx         = _locposx[i];
    RPCHits[i]._locposy         = _locposy[i];
    RPCHits[i]._locposz         = _locposz[i];
    RPCHits[i]._locdirx         = _locdirx[i];
    RPCHits[i]._locdiry         = _locdiry[i];
    RPCHits[i]._locdirz         = _locdirz[i];
    RPCHits[i]._locdirtheta     = _locdirtheta[i];
    RPCHits[i]._locdirphi       = _locdirphi[i];
    RPCHits[i]._exitpointx      = _exitpointx[i];
    RPCHits[i]._exitpointy      = _exitpointy[i];
    RPCHits[i]._exitpointz      = _exitpointz[i];
    RPCHits[i]._entrypointx     = _entrypointx[i];
    RPCHits[i]._entrypointy     = _entrypointy[i];
    RPCHits[i]._entrypointz     = _entrypointz[i];
    RPCHits[i]._enloss          = _enloss[i];
    RPCHits[i]._tof             = _tof[i];
  }
  return;

}
