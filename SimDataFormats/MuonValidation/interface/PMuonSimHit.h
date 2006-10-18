#ifndef PMuonSimHit_h
#define PMuonSimHit_h

#include <vector>
#include <memory>

/// Class PMuonSimHit defines structure of simulated hits data in CSC,DT,RPC
/// for validation. It also includes vertex and track info.

class PMuonSimHit
{
 public:

  PMuonSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0), 
                 nCSCHits(0), nDTHits(0), nRPCHits(0) {}
  virtual ~PMuonSimHit(){}

  struct Vtx
  {
    Vtx(): x(0), y(0), z(0) {}
    float x;
    float y;
    float z;
  };

  struct Trk
  {
    Trk() : pt(0), e(0), eta(0), phi(0) {}
    float pt;
    float e;
    float eta;
    float phi;
  };


  struct CSC
  {
    CSC() :
         _cscId(0), 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0),
         _globposz(0),    _globposphi(0),  _globposeta(0), 
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}
 
    int   _cscId;
    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  struct DT
  {
    DT() : 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0), 
         _globposz(0),    _globposphi(0),  _globposeta(0),
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}

    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  struct RPC
  {
    RPC() : 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0), 
         _globposz(0),    _globposphi(0),  _globposeta(0),
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}

    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;

  typedef std::vector<CSC> CSCVector;
  typedef std::vector<DT>  DTVector;
  typedef std::vector<RPC>  RPCVector;

  /// put functions

  void putRawGenPart(int n);

  void putG4Vtx(std::vector<float> x,   std::vector<float> y,
                std::vector<float> z);
  void putG4Trk(std::vector<float> pt,  std::vector<float> e,
                std::vector<float> eta, std::vector<float> phi);  

  void putCSCHits(
               std::vector<int>  _cscId,
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
               std::vector<float> _tof);   

  void putDTHits(
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
               std::vector<float> _tof); 

  void putRPCHits(
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
               std::vector<float> _tof); 

  /// get functions

  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  int getnG4Trk() {return nG4Trk;}

  VtxVector getG4Vtx() {return G4Vtx;}
  TrkVector getG4Trk() {return G4Trk;}

  int getnCSCHits() {return nCSCHits;}
  CSCVector getCSCHits() {return CSCHits;}

  int getnDTHits() {return nDTHits;}
  DTVector getDTHits() {return DTHits;}

  int getnRPCHits() {return nRPCHits;}
  RPCVector getRPCHits() {return RPCHits;}

 
private:

  /// G4MC info

  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk;
 
  /// Hit info

  int nCSCHits;
  CSCVector CSCHits; 

  int nDTHits;
  DTVector DTHits; 

  int nRPCHits;
  RPCVector RPCHits; 

};

#endif
