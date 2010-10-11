#ifndef TANATRACK
#define TANATRACK


#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TVector3.h"

class TAnaTrack: public TObject {

public:
  TAnaTrack(int index = -1);
  ~TAnaTrack() { };
  void clear(); 
  void dump();

  // ----------------------------------------------------------------------

  // ----------------------------------------------------------------------
  int            fIndex;    // pointer into track block (e.g. in signal block)
  int            fMCID;     // MC truth ID
  int            fGenIndex; // index in generator block (if matched)

  // -- Tracking parameters
  int            fQ;                            // charge
  int            fHighPurity;                            //
  int            fDof, fHits;
  double         fChi2;
  double         fLip, fLipE, fTip3d, fTip3dE, fTip, fTipE;      // longitudinal and transverse impact parameters
  int            fIndxj;                          // index of closest jet (with et, dr cuts) used for Ipj
  double        fTrChi2norm;          // inner track ch2
  double        fDxybs;               //transevrse IP to beam spot
  double        fDxypv;               // to first pv
  double        fDxypvnm;            // to pv wo muon, or closest in z
  double        fDxyE;                //errors 
  double        fDzbs;              // long. IP dz for bs
  double        fDzpv;              // dz fro first pv
  double        fDzE;              // errors on long. IP
  double        fDxysimv;            //dxy for first simvertex
  double        fDzsimv;            // dz for first simvertex
  double        fMuonIsosumPT;     // muon isolation in dR=0.3
  double        fMuonIsoEcal;
  double        fMuonIsoHcal;
  double        fMuonCalocomp;    // calocomp from calorimeters
  double        fMuonVertexChi2;   // muon vertex ch2
  double        fMuonPtrel;  // ptrel to the closest selected jet used in 3dIP
   int           fMuonSelector;  //  for muon tracks TMLastStationAngTight
 // -- Hit pattern

  int  fMuonCSCHits, fMuonDTHits, fMuonRPCHits, fMuonHits;
  int  fBPIXHits, fFPIXHits, fPixelHits;
  int  fStripHits, fTECHits, fTIBHits, fTIDHits, fTOBHits;
  int  fBPIXLayers, fFPIXLayers, fPixelLayers;
  int  fStripLayers, fTECLayers, fTIBLayers, fTIDLayers, fTOBLayers;
  int  fExpectedHitsInner, fExpectedHitsOuter;
  int  fLostHits, fValidHitInFirstPixelBarrel;



  // -- Particle ID

  double         fMuID, fElID, fKaID;
  int            fMuType;

  // -- Momentum
  TVector3       fPlab;
  // vertex
  TVector3       fVertex;

private:

  ClassDef(TAnaTrack,1)

};

#endif
