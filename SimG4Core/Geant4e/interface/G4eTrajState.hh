//
// class G4eTrajState
//
// Class description:
//
// Base class for the trajectory state
//

// History:
//
// --------------------------------------------------------------------
//
#ifndef G4eTrajState_h
#define G4eTrajState_h

#include "G4Track.hh"
#include "G4Point3D.hh"
#include "G4Vector3D.hh"
#include "G4eTrajError.hh"

enum G4eTSType{ G4eTS_FREE, G4eTS_OS };

class G4eTrajState
{
public:
  G4eTrajState(){ };

  G4eTrajState( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4eTrajError& errmat = G4eTrajError(5,0) );
    // Construct by providing particle, position and momentum 

  virtual ~G4eTrajState(){};

  void SetData( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom );
    // Set particle, position and momentum

  void BuildCharge();
    // Build charge based on particle type

   friend
    std::ostream& operator<<(std::ostream&, const G4eTrajState& ts);

  virtual int PropagateError( const G4Track* ){ 
    G4cout << " THIS SHOULD NOT BE CALLED PropagateError type " << int(GetTSType()) << G4endl;
    return -1; };
    // Propagate the error along the step

  virtual int Update( const G4Track* ){ return -1; };
    // Update parameters from G4Track
 
  void UpdatePosMom( const G4Point3D& pos, const G4Vector3D& mom );
    // Update position and momentum

  void DumpPosMomError( std::ostream& out = G4cout ) const;
    // Dump position, momentum and error

  virtual void Dump( std::ostream& out = G4cout ) const = 0;
    // Abstract method to dump all TrajState parameters
  
  // Set and Get methods 
  const G4String& GetParticleType() const{
    return fParticleType;}

  G4Point3D GetPosition() const {
    return fPosition; }

  G4Vector3D GetMomentum() const {
    return fMomentum; }

  double GetCharge() const {
    return fCharge; }

  G4eTrajError GetError() const {
    return fError; }

  virtual G4eTSType GetTSType() const { return theTSType; }

  G4Track* GetG4Track() const{ 
    return theG4Track; }

  void SetParticleType( const G4String& partType ){
    fParticleType = partType;}

  virtual void SetPosition( const G4Point3D pos ) {
    fPosition = pos; }

  virtual void SetMomentum( const G4Vector3D& mom ) {
    fMomentum = mom; }

  virtual void SetError( G4eTrajError em ) {
    fError = em; }

  void SetG4Track( G4Track* trk ){
    theG4Track = trk; }

protected:
  G4String fParticleType;
  G4Point3D fPosition;
  G4Vector3D fMomentum;
  double fCharge;
  G4eTrajError fError;

  G4eTSType theTSType;

  G4Track* theG4Track;

  int iverbose;
};

#endif

