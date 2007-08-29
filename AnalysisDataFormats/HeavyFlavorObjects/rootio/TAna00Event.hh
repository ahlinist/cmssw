#ifndef TANA00EVENT
#define TANA00EVENT


#include "TObject.h"
#include "TClonesArray.h"

#include "TGenCand.hh"
#include "TAnaTrack.hh"
#include "TAnaCand.hh"
#include "TAnaVertex.hh"

class TAna00Event : public TObject {

public:

  TAna00Event() { };
  TAna00Event(Int_t Option);
  virtual ~TAna00Event() { };
  virtual void  Clear(Option_t *option ="");
  void          dump();

  // ----------------------------------------------------------------------
  // -- Generator block
  int                 nGenCands() {return fnGenCands;}
  TGenCand*           getGenCand(int n);
  virtual TGenCand*   addGenCand();
  void                dumpGenBlock();
  // For a given SimTrack, find the index in the generator block by doing a (p,id) matching
  int                 getGenIndex(double px, double py, double pz, int id);
  // Check whether a RecTrack/TAnaTrack has mother with ID in the generator block
  //  int                 isDescendant(TAnaTrack *pTrk, int ID, int matchCharge = 0);


  // -- RecTracks
  int                 nRecTracks() {return fnRecTracks;}
  TAnaTrack*          getRecTrack(int n);
  virtual TAnaTrack*  addRecTrack();

  // -- Signal Tracks (e.g. leptons)
  int                 nSigTracks() {return fnSigTracks;}
  TAnaTrack*          getSigTrack(int n);
  virtual TAnaTrack*  addSigTrack();

  // -- Signal Candidates (e.g. B_s)
  int                 nCands() {return fnCandidates;}
  TAnaCand*           getCand(int n);
  virtual TAnaCand*   addCand();

  // ----------------------------------------------------------------------
  int               fRunNumber, fEventNumber;
  int               fEventBits;
  int               fTriggerDecision, fDiMuonTriggerDecision;
  int               fTriggerWord1, fTriggerWord2, fTriggerWord3, fTriggerWord4 ;

  TAnaVertex        fPrimaryVertex;
  TAnaVertex        fPrimaryVertex2;


private:

  int               fnGenCands;
  TClonesArray      *fGenCands;

  int               fnRecTracks;
  TClonesArray      *fRecTracks;

  int               fnSigTracks;
  TClonesArray      *fSigTracks;

  int               fnCandidates;
  TClonesArray      *fCandidates;

  ClassDef(TAna00Event,1)

};

#endif
