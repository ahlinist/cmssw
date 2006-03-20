/** \file
 *
 *  $Date: 2006/03/07 15:27:21 $
 *  $Revision: 1.2 $
 *  \author N. Amapane - CERN
 */

#include <Geometry/DTGeometry/interface/DTGeometry.h>
#include <Geometry/CommonDetUnit/interface/GeomDetUnit.h>
#include <Geometry/CommonDetUnit/interface/GeomDetType.h>

#include <algorithm>
#include <iostream>

DTGeometry::DTGeometry() {}

DTGeometry::~DTGeometry(){
  // delete all the chambers (which will delete the SL which will delete the
  // layers)
  for (std::vector<DTChamber*>::const_iterator ich=theChambers.begin();
       ich!=theChambers.end(); ++ich) delete (*ich);
}

const DTGeometry::DetTypeContainer&  DTGeometry::detTypes() const{
  static DetTypeContainer  theDetTypes;
  // FIXME - fill it at runtime
  return theDetTypes;
}


void DTGeometry::add(DTChamber* ch) {
  theDets.push_back(ch);
  theChambers.push_back(ch);
  theDetMap.insert(std::pair<DetId,GeomDet*>(ch->geographicalId(),ch));
}


void DTGeometry::add(DTSuperLayer* sl) {
  theDets.push_back(sl);
  theSuperLayers.push_back(sl);
  theDetMap.insert(std::pair<DetId,GeomDet*>(sl->geographicalId(),sl));
}


void DTGeometry::add(DTLayer* l) {
  theDetUnits.push_back(l);
  theDets.push_back(l);
  theLayers.push_back(l); 
  theDetMap.insert(std::pair<DetId,GeomDet*>(l->geographicalId(),l));
}


const DTGeometry::DetUnitContainer& DTGeometry::detUnits() const{
  return theDetUnits;
}


const DTGeometry::DetContainer& DTGeometry::dets() const{
  return theDets; 
}


const DTGeometry::DetIdContainer& DTGeometry::detUnitIds() const{
  static DetIdContainer    theDetUnitIds;
  // FIXME - fill it at runtime
  return theDetUnitIds;
}


const DTGeometry::DetIdContainer& DTGeometry::detIds() const{
  static DetIdContainer theDetIds;
  // FIXME - fill it at runtime
  return theDetIds;
}


const GeomDetUnit* DTGeometry::idToDetUnit(DetId id) const{
  return dynamic_cast<const GeomDetUnit*>(idToDet(id));
}


const GeomDet* DTGeometry::idToDet(DetId id) const{
  DTDetMap::const_iterator i = theDetMap.find(id);
  return (i != theDetMap.end()) ?
    i->second : 0 ;
}


const std::vector<DTChamber*>& DTGeometry::chambers() const{
  return theChambers;
}


const std::vector<DTSuperLayer*>& DTGeometry::superLayers() const{
  return theSuperLayers;
}


const std::vector<DTLayer*>& DTGeometry::layers() const{
  return theLayers;
}


const DTChamber* DTGeometry::chamber(DTChamberId id) const {
  return (const DTChamber*)(idToDet(id));
}


const DTSuperLayer* DTGeometry::superLayer(DTSuperLayerId id) const {
  return (const DTSuperLayer*)(idToDet(id));
}


const DTLayer* DTGeometry::layer(DTLayerId id) const {
  return (const DTLayer*)(idToDet(id));
}
