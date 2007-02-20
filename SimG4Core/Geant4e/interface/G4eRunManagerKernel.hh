//
// class G4eRunManagerKernel
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
// class G4eRunManagerKernel
//
// Class description:
//
// This is the G4RunManagerKernel of the GEANT4e package, it allows G4eManager to implement the run initialization and termination 
// It does not inherit from G4RunManagerKernel but has a pointer to it. This is because G4RunManagerKernel cannot be inherited from, as G4RunManager creates one instance and then there would be two instances (what is forbidden)
//
// History:
// - Created. Pedro Arce, January 2005

#ifndef G4eRunManagerKernel_h
#define G4eRunManagerKernel_h 1

class G4RunManagerKernel;
class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4VUserPhysicsList;
class G4UserTrackingAction;
class G4UserSteppingAction;


class G4eRunManagerKernel 
//: public G4RunManagerKernel
{
public: // with description
  static G4eRunManagerKernel* GetRunManagerKernel();
  //  Static method which returns the singleton pointer of G4eRunManagerKernel or
  // its derived class.
  
private:
  static G4eRunManagerKernel* fRunManagerKernel;
  
public:
  G4eRunManagerKernel();
  virtual ~G4eRunManagerKernel();
  //  The constructor and the destructor. The user must construct this class
  // object at the beginning of his/her main() and must delete it at the 
  // bottom of the main().
  
public: // with description
  // file is executed.

  void SetUserInitialization(G4VUserDetectorConstruction* userInit);
  void SetUserInitialization(G4VPhysicalVolume* userInit);

  void SetUserInitialization(G4VUserPhysicsList* userInit);

  void SetUserAction(G4UserTrackingAction* userAction);
  void SetUserAction(G4UserSteppingAction* userAction);

  G4VUserPhysicsList* GetUserPhysicsList() const
  { return theUserPhysicsList; }

  void RunInitialization();
  void InitializeGeometry();
  void InitializePhysics();

  void RunTermination();

private:
  G4VUserPhysicsList* theUserPhysicsList;
  G4VPhysicalVolume* theUserWorld;

  G4RunManagerKernel* theG4RunManagerKernel;
};

#endif

