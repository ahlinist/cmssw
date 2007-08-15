//
#include "SimG4Core/Geant4e/interface/G4eNavigator.hh"
#include "globals.hh"

#include "SimG4Core/Geant4e/interface/G4eManager.hh"
#include "SimG4Core/Geant4e/interface/G4eTargetSurface.hh"


#define geant4e

G4eNavigator::G4eNavigator():G4Navigator()
{
  G4cout << "creating G4eNavigator" << G4endl;}

G4eNavigator::~G4eNavigator()
{
}


G4double G4eNavigator::ComputeStep (const G4ThreeVector &pGlobalPoint,
			   const G4ThreeVector &pDirection,
			   const G4double pCurrentProposedStepLength,
			   G4double &pNewSafety)
{
  /*t
    G4VPhysicalVolume  *motherPhysical = fHistory.GetTopVolume();
  G4VSolid *motherSolid = motherPhysical->GetLogicalVolume()->GetSolid();
  
  G4VisExtent ext = motherSolid->GetExtent();
  G4cout << motherSolid->GetName() << " newSafety " <<  pNewSafety << " " <<  std::max(ext.GetXmax()-ext.GetXmin(), std::max(ext.GetYmax()-ext.GetYmin(), ext.GetZmax()-ext.GetZmin() )) << G4endl; 
  pNewSafety = std::max(ext.GetXmax()-ext.GetXmin(), std::max(ext.GetYmax()-ext.GetYmin(), ext.GetZmax()-ext.GetZmin() )); 
  */

  double Step = G4Navigator::ComputeStep(pGlobalPoint,pDirection,pCurrentProposedStepLength,pNewSafety);
  
#ifdef geant4e
  G4eManager *g4emgr = G4eManager::GetG4eManager();
  if (g4emgr !=0){

    const G4eTarget* target = g4emgr->GetTarget();
    //    G4cout << " G4eNavigator::ComputeStep target = " << target << G4endl; 
    if( target != 0 ) {
      G4double StepPlane = target->GetDistanceFromPoint(pGlobalPoint,pDirection);
      if( StepPlane < 0. ) StepPlane = 1.E9; //negative means target is crossed, will not be found
      //t   if( StepPlane < 0. ) StepPlane = DBL_MAX; //negative means target is crossed, will not be found
   if( G4eManager::verbose() >= 5 ) G4cout << " G4eNavigator::ComputeStep target step " << StepPlane << " transp step " << Step << G4endl;

#ifdef G4EVERBOSE
      if( G4eManager::verbose() >= 5 ) target->Dump( "G4eNavigator::ComputeStep Target " );
#endif
      
      if(StepPlane<Step){
#ifdef G4EVERBOSE
	if( G4eManager::verbose() >= 2 ) std::cout << "G4eNavigator::ComputeStep TargetCloserThanBoundary " << StepPlane << " < " << Step << std::endl;
#endif
	Step = StepPlane;
	g4emgr->SetState(G4eState_TargetCloserThanBoundary);
      } else {
	g4emgr->SetState(G4eState_Propagating);
      }
    }

  }

#endif

  pNewSafety = ComputeSafety(pGlobalPoint, pCurrentProposedStepLength);

#ifdef G4EVERBOSE
  if( G4eManager::verbose() >= 3 ) std::cout << "G4eNavigator::ComputeStep " << Step << " ComputeSafety " << pNewSafety << std::endl;
#endif


  return Step;

}

G4double G4eNavigator::ComputeSafety( const G4ThreeVector &pGlobalpoint,
                                     const G4double pMaxLength)
{
  double newSafety = G4Navigator::ComputeSafety(pGlobalpoint,pMaxLength);

#ifdef geant4e

  G4eManager *g4emgr = G4eManager::GetG4eManager();
  if (g4emgr !=0){

    const G4eTarget* target = g4emgr->GetTarget();
    if( target != 0 ) {
      G4double distance = target->GetDistanceFromPoint(pGlobalpoint);
      
      if(distance<newSafety){
	newSafety = distance;
	//      g4emgr->SetState(G4eState(G4eState_TargetCloserThanBoundary) );
      } else{
	//      g4emgr->SetState(G4eState(G4eState_Init) );
      }
    }
  }

#endif

  return newSafety;

} 
