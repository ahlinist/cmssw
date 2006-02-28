#ifndef PGlobalSimHit_h
#define PGlobalSimHit_h

#include <vector>
#include <memory>

class PGlobalSimHit
{

 public:

  PGlobalSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0),  
    nECalHits(0), nPreShHits(0), nHCalHits(0), nPxlFwdHits(0),
    nPxlBrlHits(0), nSiFwdHits(0), nSiBrlHits(0),
    nMuonDtHits(0), nMuonCscHits(0), nMuonRpcFwdHits(0),
    nMuonRpcBrlHits(0) {}
  virtual ~PGlobalSimHit(){}

  struct Vtx
  {
    Vtx(): x(0), y(0), z(0) {}
    float x;
    float y;
    float z;
  };

  struct Trk
  {
    Trk() : pt(0), e(0) {}
    float pt;
    float e;
  };

  struct CalHit
  {
    CalHit() : e(0), tof(0), phi(0), eta(0) {}
    float e;
    float tof;
    float phi;
    float eta;
  };

  struct FwdHit
  {
    FwdHit() : tof(0), z(0), phi(0), eta(0) {}
    float tof;
    float z;
    float phi;
    float eta;
  };

  struct BrlHit
  {
    BrlHit() : tof(0), r(0), phi(0), eta(0) {}
    float tof;
    float r;
    float phi;
    float eta;
  };

  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;
  typedef std::vector<CalHit> CalVector;
  typedef std::vector<FwdHit> FwdVector;
  typedef std::vector<BrlHit> BrlVector;

  // put functions
  void putRawGenPart(int n);
  void putG4Vtx(std::vector<float> x, std::vector<float> y, 
		 std::vector<float> z);
  void putG4Trk(std::vector<float> pt, std::vector<float> e);
  void putECalHits(std::vector<float> e, std::vector<float> tof,
		    std::vector<float> phi, std::vector<float> eta);
  void putPreShHits(std::vector<float> e, std::vector<float> tof,
		     std::vector<float> phi, std::vector<float> eta);
  void putHCalHits(std::vector<float> e, std::vector<float> tof,
		    std::vector<float> phi, std::vector<float> eta);
  void putPxlFwdHits(std::vector<float> tof, std::vector<float> z,
		       std::vector<float> phi, std::vector<float> eta);
  void putPxlBrlHits(std::vector<float> tof, std::vector<float> r,
		      std::vector<float> phi, std::vector<float> eta);
  void putSiFwdHits(std::vector<float> tof, std::vector<float> z,
		      std::vector<float> phi, std::vector<float> eta);
  void putSiBrlHits(std::vector<float> tof, std::vector<float> r,
		     std::vector<float> phi, std::vector<float> eta);
  void putMuonCscHits(std::vector<float> tof, std::vector<float> z,
		       std::vector<float> phi, std::vector<float> eta);
  void putMuonDtHits(std::vector<float> tof, std::vector<float> r,
		      std::vector<float> phi, std::vector<float> eta);
  void putMuonRpcFwdHits(std::vector<float> tof, std::vector<float> z,
			   std::vector<float> phi, std::vector<float> eta);
  void putMuonRpcBrlHits(std::vector<float> tof, std::vector<float> r,
			  std::vector<float> phi, std::vector<float> eta);  


  // get functions
  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  VtxVector getG4Vtx() {return G4Vtx;}
  int getnG4Trk() {return nG4Trk;}
  TrkVector getG4Trk() {return G4Trk;}
  int getnECalHits() {return nECalHits;}
  CalVector getECalHits() {return ECalHits;}
  int getnPreShHits() {return nPreShHits;}
  CalVector getPreShHits() {return PreShHits;}
  int getnHCalHits() {return nHCalHits;}
  CalVector getHCalHits() {return HCalHits;}
  int getnPxlFwdHits() {return nPxlFwdHits;}
  FwdVector getPxlFwdHits() {return PxlFwdHits;}
  int getnPxlBrlHits() {return nPxlBrlHits;}
  BrlVector getPxlBrlHits() {return PxlBrlHits;}
  int getnSiFwdHits() {return nSiFwdHits;}
  FwdVector getSiFwdHits() {return SiFwdHits;}
  int getnSiBrlHits() {return nSiBrlHits;}
  BrlVector getSiBrlHits() {return SiBrlHits;}  
  int getnMuonDtHits() {return nMuonDtHits;}
  BrlVector getMuonDtHits() {return MuonDtHits;}
  int getnMuonCscHits() {return nMuonCscHits;}
  FwdVector getMuonCscHits() {return MuonCscHits;}
  int getnMuonRpcFwdHits() {return nMuonRpcFwdHits;}
  FwdVector getMuonRpcFwdHits() {return MuonRpcFwdHits;}
  int getnMuonRpcBrlHits() {return nMuonRpcBrlHits;}
  BrlVector getMuonRpcBrlHits() {return MuonRpcBrlHits;}  

 private:

  // G4MC info
  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk; 

  // ECal info
  int nECalHits;
  CalVector ECalHits; 
  int nPreShHits;
  CalVector PreShHits; 

  // HCal info
  int nHCalHits;
  CalVector HCalHits;

  // Tracker info
  int nPxlFwdHits;
  FwdVector PxlFwdHits; 
  int nPxlBrlHits;
  BrlVector PxlBrlHits;
  int nSiFwdHits;
  FwdVector SiFwdHits; 
  int nSiBrlHits;
  BrlVector SiBrlHits;  

  // Muon info
  int nMuonDtHits;
  BrlVector MuonDtHits;
  int nMuonCscHits;
  FwdVector MuonCscHits;
  int nMuonRpcFwdHits;
  FwdVector MuonRpcFwdHits;  
  int nMuonRpcBrlHits;
  BrlVector MuonRpcBrlHits;

}; // end class declaration

#endif
