#include "SimDataFormats/GlobalRecHitValidation/interface/PGlobalRecHit.h"

void PGlobalRecHit::putEBCalRecHits(std::vector<float> re,
				    std::vector<float> she)
{
  nEBCalRecHits = re.size();
  EBCalRecHits.resize(nEBCalRecHits);
  for (int i = 0; i < nEBCalRecHits; ++i) {
    EBCalRecHits[i].RE = re[i];
    EBCalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putEECalRecHits(std::vector<float> re,
				    std::vector<float> she)
{
  nEECalRecHits = re.size();
  EECalRecHits.resize(nEECalRecHits);
  for (int i = 0; i < nEECalRecHits; ++i) {
    EECalRecHits[i].RE = re[i];
    EECalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putESCalRecHits(std::vector<float> re,
				    std::vector<float> she)
{
  nESCalRecHits = re.size();
  ESCalRecHits.resize(nESCalRecHits);
  for (int i = 0; i < nESCalRecHits; ++i) {
    ESCalRecHits[i].RE = re[i];
    ESCalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putHBCalRecHits(std::vector<float> rec,
				    std::vector<float> r,
				    std::vector<float> she)
{
  nHBCalRecHits = rec.size();
  HBCalRecHits.resize(nHBCalRecHits);
  for (int i = 0; i < nHBCalRecHits; ++i) {
    HBCalRecHits[i].REC = rec[i];
    HBCalRecHits[i].R = r[i];
    HBCalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putHECalRecHits(std::vector<float> rec,
				    std::vector<float> r,
				    std::vector<float> she)
{
  nHECalRecHits = rec.size();
  HECalRecHits.resize(nHECalRecHits);
  for (int i = 0; i < nHECalRecHits; ++i) {
    HECalRecHits[i].REC = rec[i];
    HECalRecHits[i].R = r[i];
    HECalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putHOCalRecHits(std::vector<float> rec,
				    std::vector<float> r,
				    std::vector<float> she)
{
  nHOCalRecHits = rec.size();
  HOCalRecHits.resize(nHOCalRecHits);
  for (int i = 0; i < nHOCalRecHits; ++i) {
    HOCalRecHits[i].REC = rec[i];
    HOCalRecHits[i].R = r[i];
    HOCalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putHFCalRecHits(std::vector<float> rec,
				    std::vector<float> r,
				    std::vector<float> she)
{
  nHFCalRecHits = rec.size();
  HFCalRecHits.resize(nHFCalRecHits);
  for (int i = 0; i < nHFCalRecHits; ++i) {
    HFCalRecHits[i].REC = rec[i];
    HFCalRecHits[i].R = r[i];
    HFCalRecHits[i].SHE = she[i];
  }

  return;
}

void PGlobalRecHit::putTIBL1RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIBL1RecHits = rx.size();
  TIBL1RecHits.resize(nTIBL1RecHits);
  for (int i = 0; i < nTIBL1RecHits; ++i) {
    TIBL1RecHits[i].RX = rx[i];
    TIBL1RecHits[i].RY = ry[i];
    TIBL1RecHits[i].SX = sx[i];
    TIBL1RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIBL2RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIBL2RecHits = rx.size();
  TIBL2RecHits.resize(nTIBL2RecHits);
  for (int i = 0; i < nTIBL2RecHits; ++i) {
    TIBL2RecHits[i].RX = rx[i];
    TIBL2RecHits[i].RY = ry[i];
    TIBL2RecHits[i].SX = sx[i];
    TIBL2RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIBL3RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIBL3RecHits = rx.size();
  TIBL3RecHits.resize(nTIBL3RecHits);
  for (int i = 0; i < nTIBL3RecHits; ++i) {
    TIBL3RecHits[i].RX = rx[i];
    TIBL3RecHits[i].RY = ry[i];
    TIBL3RecHits[i].SX = sx[i];
    TIBL3RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIBL4RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIBL4RecHits = rx.size();
  TIBL4RecHits.resize(nTIBL4RecHits);
  for (int i = 0; i < nTIBL4RecHits; ++i) {
    TIBL4RecHits[i].RX = rx[i];
    TIBL4RecHits[i].RY = ry[i];
    TIBL4RecHits[i].SX = sx[i];
    TIBL4RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTOBL1RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTOBL1RecHits = rx.size();
  TOBL1RecHits.resize(nTOBL1RecHits);
  for (int i = 0; i < nTOBL1RecHits; ++i) {
    TOBL1RecHits[i].RX = rx[i];
    TOBL1RecHits[i].RY = ry[i];
    TOBL1RecHits[i].SX = sx[i];
    TOBL1RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTOBL2RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTOBL2RecHits = rx.size();
  TOBL2RecHits.resize(nTOBL2RecHits);
  for (int i = 0; i < nTOBL2RecHits; ++i) {
    TOBL2RecHits[i].RX = rx[i];
    TOBL2RecHits[i].RY = ry[i];
    TOBL2RecHits[i].SX = sx[i];
    TOBL2RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTOBL3RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTOBL3RecHits = rx.size();
  TOBL3RecHits.resize(nTOBL3RecHits);
  for (int i = 0; i < nTOBL3RecHits; ++i) {
    TOBL3RecHits[i].RX = rx[i];
    TOBL3RecHits[i].RY = ry[i];
    TOBL3RecHits[i].SX = sx[i];
    TOBL3RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTOBL4RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTOBL4RecHits = rx.size();
  TOBL4RecHits.resize(nTOBL4RecHits);
  for (int i = 0; i < nTOBL4RecHits; ++i) {
    TOBL4RecHits[i].RX = rx[i];
    TOBL4RecHits[i].RY = ry[i];
    TOBL4RecHits[i].SX = sx[i];
    TOBL4RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIDW1RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIDW1RecHits = rx.size();
  TIDW1RecHits.resize(nTIDW1RecHits);
  for (int i = 0; i < nTIDW1RecHits; ++i) {
    TIDW1RecHits[i].RX = rx[i];
    TIDW1RecHits[i].RY = ry[i];
    TIDW1RecHits[i].SX = sx[i];
    TIDW1RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIDW2RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIDW2RecHits = rx.size();
  TIDW2RecHits.resize(nTIDW2RecHits);
  for (int i = 0; i < nTIDW2RecHits; ++i) {
    TIDW2RecHits[i].RX = rx[i];
    TIDW2RecHits[i].RY = ry[i];
    TIDW2RecHits[i].SX = sx[i];
    TIDW2RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTIDW3RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTIDW3RecHits = rx.size();
  TIDW3RecHits.resize(nTIDW3RecHits);
  for (int i = 0; i < nTIDW3RecHits; ++i) {
    TIDW3RecHits[i].RX = rx[i];
    TIDW3RecHits[i].RY = ry[i];
    TIDW3RecHits[i].SX = sx[i];
    TIDW3RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW1RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW1RecHits = rx.size();
  TECW1RecHits.resize(nTECW1RecHits);
  for (int i = 0; i < nTECW1RecHits; ++i) {
    TECW1RecHits[i].RX = rx[i];
    TECW1RecHits[i].RY = ry[i];
    TECW1RecHits[i].SX = sx[i];
    TECW1RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW2RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW2RecHits = rx.size();
  TECW2RecHits.resize(nTECW2RecHits);
  for (int i = 0; i < nTECW2RecHits; ++i) {
    TECW2RecHits[i].RX = rx[i];
    TECW2RecHits[i].RY = ry[i];
    TECW2RecHits[i].SX = sx[i];
    TECW2RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW3RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW3RecHits = rx.size();
  TECW3RecHits.resize(nTECW3RecHits);
  for (int i = 0; i < nTECW3RecHits; ++i) {
    TECW3RecHits[i].RX = rx[i];
    TECW3RecHits[i].RY = ry[i];
    TECW3RecHits[i].SX = sx[i];
    TECW3RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW4RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW4RecHits = rx.size();
  TECW4RecHits.resize(nTECW4RecHits);
  for (int i = 0; i < nTECW4RecHits; ++i) {
    TECW4RecHits[i].RX = rx[i];
    TECW4RecHits[i].RY = ry[i];
    TECW4RecHits[i].SX = sx[i];
    TECW4RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW5RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW5RecHits = rx.size();
  TECW5RecHits.resize(nTECW5RecHits);
  for (int i = 0; i < nTECW5RecHits; ++i) {
    TECW5RecHits[i].RX = rx[i];
    TECW5RecHits[i].RY = ry[i];
    TECW5RecHits[i].SX = sx[i];
    TECW5RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW6RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW6RecHits = rx.size();
  TECW6RecHits.resize(nTECW6RecHits);
  for (int i = 0; i < nTECW6RecHits; ++i) {
    TECW6RecHits[i].RX = rx[i];
    TECW6RecHits[i].RY = ry[i];
    TECW6RecHits[i].SX = sx[i];
    TECW6RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW7RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW7RecHits = rx.size();
  TECW7RecHits.resize(nTECW7RecHits);
  for (int i = 0; i < nTECW7RecHits; ++i) {
    TECW7RecHits[i].RX = rx[i];
    TECW7RecHits[i].RY = ry[i];
    TECW7RecHits[i].SX = sx[i];
    TECW7RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putTECW8RecHits(std::vector<float> rx, 
				    std::vector<float> ry,
				    std::vector<float> sx, 
				    std::vector<float> sy)
{
  nTECW8RecHits = rx.size();
  TECW8RecHits.resize(nTECW8RecHits);
  for (int i = 0; i < nTECW8RecHits; ++i) {
    TECW8RecHits[i].RX = rx[i];
    TECW8RecHits[i].RY = ry[i];
    TECW8RecHits[i].SX = sx[i];
    TECW8RecHits[i].SY = sy[i];    
  }

  return;
}

void PGlobalRecHit::putBRL1RecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nBRL1RecHits = rx.size();
  BRL1RecHits.resize(nBRL1RecHits);
  for (int i = 0; i < nBRL1RecHits; ++i) {
    BRL1RecHits[i].RX = rx[i];
    BRL1RecHits[i].RY = ry[i];
    BRL1RecHits[i].SX = sx[i];
    BRL1RecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putBRL2RecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nBRL2RecHits = rx.size();
  BRL2RecHits.resize(nBRL2RecHits);
  for (int i = 0; i < nBRL2RecHits; ++i) {
    BRL2RecHits[i].RX = rx[i];
    BRL2RecHits[i].RY = ry[i];
    BRL2RecHits[i].SX = sx[i];
    BRL2RecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putBRL3RecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nBRL3RecHits = rx.size();
  BRL3RecHits.resize(nBRL3RecHits);
  for (int i = 0; i < nBRL3RecHits; ++i) {
    BRL3RecHits[i].RX = rx[i];
    BRL3RecHits[i].RY = ry[i];
    BRL3RecHits[i].SX = sx[i];
    BRL3RecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putFWD1pRecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nFWD1pRecHits = rx.size();
  FWD1pRecHits.resize(nFWD1pRecHits);
  for (int i = 0; i < nFWD1pRecHits; ++i) {
    FWD1pRecHits[i].RX = rx[i];
    FWD1pRecHits[i].RY = ry[i];
    FWD1pRecHits[i].SX = sx[i];
    FWD1pRecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putFWD1nRecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nFWD1nRecHits = rx.size();
  FWD1nRecHits.resize(nFWD1nRecHits);
  for (int i = 0; i < nFWD1nRecHits; ++i) {
    FWD1nRecHits[i].RX = rx[i];
    FWD1nRecHits[i].RY = ry[i];
    FWD1nRecHits[i].SX = sx[i];
    FWD1nRecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putFWD2pRecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nFWD2pRecHits = rx.size();
  FWD2pRecHits.resize(nFWD2pRecHits);
  for (int i = 0; i < nFWD2pRecHits; ++i) {
    FWD2pRecHits[i].RX = rx[i];
    FWD2pRecHits[i].RY = ry[i];
    FWD2pRecHits[i].SX = sx[i];
    FWD2pRecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putFWD2nRecHits(std::vector<float> rx, 
				   std::vector<float> ry,
				   std::vector<float> sx, 
				   std::vector<float> sy)
{
  nFWD2nRecHits = rx.size();
  FWD2nRecHits.resize(nFWD2nRecHits);
  for (int i = 0; i < nFWD2nRecHits; ++i) {
    FWD2nRecHits[i].RX = rx[i];
    FWD2nRecHits[i].RY = ry[i];
    FWD2nRecHits[i].SX = sx[i];
    FWD2nRecHits[i].SY = sy[i];
  }

  return;
}

void PGlobalRecHit::putDTRecHits(std::vector<float> rhd, 
				 std::vector<float> shd)
{
  nDTRecHits = rhd.size();
  DTRecHits.resize(nDTRecHits);
  for (int i = 0; i < nDTRecHits; ++i) {
    DTRecHits[i].RHD = rhd[i];
    DTRecHits[i].SHD = shd[i];
  }

  return;
}

void PGlobalRecHit::putCSCRecHits(std::vector<float> rhphi, 
				  std::vector<float> rhperp, 
				  std::vector<float> shphi)
{
  nCSCRecHits = rhphi.size();
  CSCRecHits.resize(nCSCRecHits);
  for (int i = 0; i < nCSCRecHits; ++i) {
    CSCRecHits[i].RHPHI = rhphi[i];
    CSCRecHits[i].RHPERP = rhperp[i];
    CSCRecHits[i].SHPHI = shphi[i];
  }

  return;
}

void PGlobalRecHit::putRPCRecHits(std::vector<float> rhx, 
				  std::vector<float> shx)
{
  nRPCRecHits = rhx.size();
  RPCRecHits.resize(nRPCRecHits);
  for (int i = 0; i < nRPCRecHits; ++i) {
    RPCRecHits[i].RHX = rhx[i];
    RPCRecHits[i].SHX = shx[i];
  }

  return;
}
