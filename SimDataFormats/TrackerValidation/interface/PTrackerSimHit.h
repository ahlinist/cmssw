#ifndef PTrackerSimHit_h
#define PTrackerSimHit_h

#include <vector>
#include <memory>

class PTrackerSimHit
{

 public:

  PTrackerSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0), nHits(0) {}
  virtual ~PTrackerSimHit(){}

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


  struct Hit
  {
    Hit() : _sysID(0), _detUnitId(0), _trackId(0), _processType(0), 
            _particleType(0), _pabs(0), 
	    _lpx(0), _lpy(0), _lpz(0), 
	    _ldx(0), _ldy(0), _ldz(0), _ldtheta(0), _ldphi(0),
	    _exx(0), _exy(0), _exz(0),
	    _enx(0), _eny(0), _enz(0), _eloss(0), _tof(0) {}
    int   _sysID; 
    float _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _lpx;
    float _lpy;
    float _lpz;
    float _ldx;
    float _ldy;
    float _ldz;
    float _ldtheta;
    float _ldphi;
    float _exx;
    float _exy;
    float _exz;
    float _enx;
    float _eny;
    float _enz;
    float _eloss;
    float _tof;
  };


  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;
  typedef std::vector<Hit> HitVector;

  // put functions
  void putRawGenPart(int n);
  void putG4Vtx(std::vector<float> x, std::vector<float> y, std::vector<float> z);
  void putG4Trk(std::vector<float> pt, std::vector<float> e, std::vector<float> eta, std::vector<float> phi);  
  void putHits(std::vector<int> _sysID, std::vector<float> _detUnitId,
	       std::vector<float>_trackId , std::vector<float>_processType,
	       std::vector<float>_particleType, std::vector<float> _pabs,
	       std::vector<float>_lpx, std::vector<float>_lpy, std::vector<float>_lpz,
	       std::vector<float>_ldx, std::vector<float>_ldy, std::vector<float>_ldz,
	       std::vector<float>_ldtheta, std::vector<float>_ldphi, 
	       std::vector<float>_exx, std::vector<float>_exy, std::vector<float>_exz,
	       std::vector<float>_enx, std::vector<float>_eny, std::vector<float>_enz,
	       std::vector<float>_eloss, std::vector<float>_tof);   

  // get functions
  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  VtxVector getG4Vtx() {return G4Vtx;}
  int getnG4Trk() {return nG4Trk;}
  TrkVector getG4Trk() {return G4Trk;}
  int getnHits() {return nHits;}
  HitVector getHits() {return Hits;}

 private:

  // G4MC info
  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk; 
  // Tracker info
  int nHits;
  HitVector Hits; 


}; // end class declaration

#endif
