#ifndef PGlobalRecHit_h
#define PGlobalRecHit_h

#include <vector>
#include <memory>

class PGlobalRecHit
{
 public:

  PGlobalRecHit(): nEBCalRecHits(0), nEECalRecHits(0), nESCalRecHits(0),
    nHBCalRecHits(0), nHECalRecHits(0), nHOCalRecHits(0), nHFCalRecHits(0),
    nTIBL1RecHits(0), nTIBL2RecHits(0), nTIBL3RecHits(0), nTIBL4RecHits(0),
    nTOBL1RecHits(0), nTOBL2RecHits(0), nTOBL3RecHits(0), nTOBL4RecHits(0),
    nTIDW1RecHits(0), nTIDW2RecHits(0), nTIDW3RecHits(0),
    nTECW1RecHits(0), nTECW2RecHits(0), nTECW3RecHits(0), nTECW4RecHits(0), 
    nTECW5RecHits(0), nTECW6RecHits(0), nTECW7RecHits(0), nTECW8RecHits(0),
    nBRL1RecHits(0), nBRL2RecHits(0), nBRL3RecHits(0), 
    nFWD1pRecHits(0), nFWD1nRecHits(0), nFWD2pRecHits(0), nFWD2nRecHits(0),
    nDTRecHits(0), nCSCRecHits(0), nRPCRecHits(0) {}
  virtual ~PGlobalRecHit(){}

  ////////////
  // ECal Info
  ////////////
  struct ECalRecHit
  {
    ECalRecHit(): RE(0), SHE(0) {}
    float RE; //reconstructed energy
    float SHE; //simhit energy
  };
  typedef std::vector<ECalRecHit> ECalRecHitVector;
  //put functions
  void putEBCalRecHits(std::vector<float> re, std::vector<float> she);
  void putEECalRecHits(std::vector<float> re, std::vector<float> she);
  void putESCalRecHits(std::vector<float> re, std::vector<float> she);
  //get functions
  int getnEBCalRecHits() {return nEBCalRecHits;}  
  int getnEECalRecHits() {return nEECalRecHits;}
  int getnESCalRecHits() {return nESCalRecHits;}  
  ECalRecHitVector getEBCalRecHits() {return EBCalRecHits;}  
  ECalRecHitVector getEECalRecHits() {return EECalRecHits;}
  ECalRecHitVector getESCalRecHits() {return ESCalRecHits;}  

  ////////////
  // HCal Info
  ////////////
  struct HCalRecHit
  {
    HCalRecHit(): REC(0), SHE(0) {}
    float REC; //reconstructed energy in cone
    float SHE; //simhit energy sum
  };
  typedef std::vector<HCalRecHit> HCalRecHitVector;
  //put functions
  void putHBCalRecHits(std::vector<float> rec, std::vector<float> she);
  void putHECalRecHits(std::vector<float> rec, std::vector<float> she);
  void putHOCalRecHits(std::vector<float> rec, std::vector<float> she);
  void putHFCalRecHits(std::vector<float> rec, std::vector<float> she);
  //get functions
  int getnHBCalRecHits() {return nHBCalRecHits;}  
  int getnHECalRecHits() {return nHECalRecHits;}  
  int getnHOCalRecHits() {return nHOCalRecHits;}  
  int getnHFCalRecHits() {return nHFCalRecHits;}  
  HCalRecHitVector getHBCalRecHits() {return HBCalRecHits;}  
  HCalRecHitVector getHECalRecHits() {return HECalRecHits;}  
  HCalRecHitVector getHOCalRecHits() {return HOCalRecHits;}  
  HCalRecHitVector getHFCalRecHits() {return HFCalRecHits;}  

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  ///////////////
  // SiStrip info
  ///////////////
  struct SiStripRecHit
  {
    SiStripRecHit(): RX(0), RY(0), SX(0), SY(0) {}
    float RX; //reconstructed x
    float RY; //reconstructed y
    float SX; //simulated x
    float SY; //simulated y
  };
  typedef std::vector<SiStripRecHit> SiStripRecHitVector;
  //put functions
  void putTIBL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW5RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW6RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW7RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW8RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  //get functions
  int getnTIBL1RecHits() {return nTIBL1RecHits;}  
  int getnTIBL2RecHits() {return nTIBL2RecHits;}  
  int getnTIBL3RecHits() {return nTIBL3RecHits;}  
  int getnTIBL4RecHits() {return nTIBL4RecHits;}  
  int getnTOBL1RecHits() {return nTOBL1RecHits;}  
  int getnTOBL2RecHits() {return nTOBL2RecHits;}  
  int getnTOBL3RecHits() {return nTOBL3RecHits;}  
  int getnTOBL4RecHits() {return nTOBL4RecHits;}
  int getnTIDW1RecHits() {return nTIDW1RecHits;}
  int getnTIDW2RecHits() {return nTIDW2RecHits;}
  int getnTIDW3RecHits() {return nTIDW3RecHits;} 
  int getnTECW1RecHits() {return nTECW1RecHits;}
  int getnTECW2RecHits() {return nTECW2RecHits;}
  int getnTECW3RecHits() {return nTECW3RecHits;}
  int getnTECW4RecHits() {return nTECW4RecHits;}
  int getnTECW5RecHits() {return nTECW5RecHits;}
  int getnTECW6RecHits() {return nTECW6RecHits;}
  int getnTECW7RecHits() {return nTECW7RecHits;}
  int getnTECW8RecHits() {return nTECW8RecHits;} 
  SiStripRecHitVector getTIBL1RecHits() {return TIBL1RecHits;}  
  SiStripRecHitVector getTIBL2RecHits() {return TIBL2RecHits;}  
  SiStripRecHitVector getTIBL3RecHits() {return TIBL3RecHits;}  
  SiStripRecHitVector getTIBL4RecHits() {return TIBL4RecHits;}
  SiStripRecHitVector getTOBL1RecHits() {return TOBL1RecHits;}  
  SiStripRecHitVector getTOBL2RecHits() {return TOBL2RecHits;}  
  SiStripRecHitVector getTOBL3RecHits() {return TOBL3RecHits;}  
  SiStripRecHitVector getTOBL4RecHits() {return TOBL4RecHits;}   
  SiStripRecHitVector getTIDW1RecHits() {return TIDW1RecHits;}
  SiStripRecHitVector getTIDW2RecHits() {return TIDW2RecHits;}
  SiStripRecHitVector getTIDW3RecHits() {return TIDW3RecHits;} 
  SiStripRecHitVector getTECW1RecHits() {return TECW1RecHits;}
  SiStripRecHitVector getTECW2RecHits() {return TECW2RecHits;}
  SiStripRecHitVector getTECW3RecHits() {return TECW3RecHits;}
  SiStripRecHitVector getTECW4RecHits() {return TECW4RecHits;}
  SiStripRecHitVector getTECW5RecHits() {return TECW5RecHits;}
  SiStripRecHitVector getTECW6RecHits() {return TECW6RecHits;}
  SiStripRecHitVector getTECW7RecHits() {return TECW7RecHits;}
  SiStripRecHitVector getTECW8RecHits() {return TECW8RecHits;}

  ///////////////
  // SiPixel info
  ///////////////
  struct SiPixelRecHit
  {
    SiPixelRecHit(): RX(0), RY(0), SX(0), SY(0) {}
    float RX; //reconstructed x
    float RY; //reconstructed y
    float SX; //simulated x
    float SY; //simulated y
  };
  typedef std::vector<SiPixelRecHit> SiPixelRecHitVector;
  //put functions
  void putBRL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putBRL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putBRL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD1pRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD1nRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD2pRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD2nRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  //get functions
  int getnBRL1RecHits() {return nBRL1RecHits;}  
  int getnBRL2RecHits() {return nBRL2RecHits;}  
  int getnBRL3RecHits() {return nBRL3RecHits;}
  int getnFWD1pRecHits() {return nFWD1pRecHits;}  
  int getnFWD1nRecHits() {return nFWD1nRecHits;}    
  int getnFWD2pRecHits() {return nFWD2pRecHits;}  
  int getnFWD2nRecHits() {return nFWD2nRecHits;}  
  SiPixelRecHitVector getBRL1RecHits() {return BRL1RecHits;}  
  SiPixelRecHitVector getBRL2RecHits() {return BRL2RecHits;}  
  SiPixelRecHitVector getBRL3RecHits() {return BRL3RecHits;}  
  SiPixelRecHitVector getFWD1pRecHits() {return FWD1pRecHits;}
  SiPixelRecHitVector getFWD1nRecHits() {return FWD1nRecHits;} 
  SiPixelRecHitVector getFWD2pRecHits() {return FWD2pRecHits;}
  SiPixelRecHitVector getFWD2nRecHits() {return FWD2nRecHits;} 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  struct DTRecHit
  {
    DTRecHit(): RHD(0), SHD(0) {}
    float RHD; //distance of rechit from wire
    float SHD; //distance of simhit from wire
  };
  typedef std::vector<DTRecHit> DTRecHitVector;
  //put functions
  void putDTRecHits(std::vector<float> rhd, std::vector<float> shd);
  //get functions
  int getnDTRecHits() {return nDTRecHits;}  
  DTRecHitVector getDTRecHits() {return DTRecHits;}  

  /////////////////
  // CSC info
  /////////////////
  struct CSCRecHit
  {
    CSCRecHit(): RHPHI(0), RHPERP(0), SHPHI(0) {}
    float RHPHI; //reconstructed hit phi
    float RHPERP; //reconstructed hit perp
    float SHPHI; //simulated hit phi
  };
  typedef std::vector<CSCRecHit> CSCRecHitVector;
  //put functions
  void putCSCRecHits(std::vector<float> rhphi, std::vector<float> rhperp, 
		     std::vector<float> shphi);
  //get functions
  int getnCSCRecHits() {return nCSCRecHits;}  
  CSCRecHitVector getCSCRecHits() {return CSCRecHits;}  

  /////////////////
  // RPC info
  /////////////////
  struct RPCRecHit
  {
    RPCRecHit(): RHX(0), SHX(0) {}
    float RHX; //reconstructed hit x
    float SHX; //simulated hit x
  };
  typedef std::vector<RPCRecHit> RPCRecHitVector;
  //put functions
  void putRPCRecHits(std::vector<float> rhx, std::vector<float> shx);
  //get functions
  int getnRPCRecHits() {return nRPCRecHits;}  
  RPCRecHitVector getRPCRecHits() {return RPCRecHits;} 

 private:

  ////////////
  // ECal info
  ////////////
  int nEBCalRecHits;
  ECalRecHitVector EBCalRecHits;
  int nEECalRecHits;
  ECalRecHitVector EECalRecHits;
  int nESCalRecHits;
  ECalRecHitVector ESCalRecHits;

  ////////////
  // HCal info
  ////////////
  int nHBCalRecHits;
  HCalRecHitVector HBCalRecHits;
  int nHECalRecHits;
  HCalRecHitVector HECalRecHits;
  int nHOCalRecHits;
  HCalRecHitVector HOCalRecHits;
  int nHFCalRecHits;
  HCalRecHitVector HFCalRecHits;

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  //////////////
  //SiStrip info
  //////////////
  int nTIBL1RecHits;  
  SiStripRecHitVector TIBL1RecHits;
  int nTIBL2RecHits;  
  SiStripRecHitVector TIBL2RecHits;
  int nTIBL3RecHits; 
  SiStripRecHitVector TIBL3RecHits;
  int nTIBL4RecHits;  
  SiStripRecHitVector TIBL4RecHits;
  int nTOBL1RecHits;
  SiStripRecHitVector TOBL1RecHits;
  int nTOBL2RecHits;  
  SiStripRecHitVector TOBL2RecHits;
  int nTOBL3RecHits;  
  SiStripRecHitVector TOBL3RecHits;
  int nTOBL4RecHits; 
  SiStripRecHitVector TOBL4RecHits;
  int nTIDW1RecHits;   
  SiStripRecHitVector TIDW1RecHits;
  int nTIDW2RecHits;
  SiStripRecHitVector TIDW2RecHits;
  int nTIDW3RecHits;
  SiStripRecHitVector TIDW3RecHits; 
  int nTECW1RecHits;
  SiStripRecHitVector TECW1RecHits;
  int nTECW2RecHits;
  SiStripRecHitVector TECW2RecHits;
  int nTECW3RecHits;
  SiStripRecHitVector TECW3RecHits;
  int nTECW4RecHits;
  SiStripRecHitVector TECW4RecHits;
  int nTECW5RecHits;
  SiStripRecHitVector TECW5RecHits;
  int nTECW6RecHits;
  SiStripRecHitVector TECW6RecHits;
  int nTECW7RecHits;
  SiStripRecHitVector TECW7RecHits;
  int nTECW8RecHits;
  SiStripRecHitVector TECW8RecHits;

  //////////////
  //SiPixel info
  //////////////
  int nBRL1RecHits;
  SiPixelRecHitVector BRL1RecHits;
  int nBRL2RecHits;  
  SiPixelRecHitVector BRL2RecHits; 
  int nBRL3RecHits; 
  SiPixelRecHitVector BRL3RecHits; 
  int nFWD1pRecHits; 
  SiPixelRecHitVector FWD1pRecHits;
  int nFWD1nRecHits;
  SiPixelRecHitVector FWD1nRecHits; 
  int nFWD2pRecHits;
  SiPixelRecHitVector FWD2pRecHits;
  int nFWD2nRecHits;
  SiPixelRecHitVector FWD2nRecHits; 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  int nDTRecHits;
  DTRecHitVector DTRecHits; 

  /////////////////
  // CSC info
  ////////////////
  int nCSCRecHits;
  CSCRecHitVector CSCRecHits;

  /////////////////
  // RPC info
  ////////////////
  int nRPCRecHits;
  RPCRecHitVector RPCRecHits;
 
}; // end class declaration

#endif //PGlobalRecHitHit_h
