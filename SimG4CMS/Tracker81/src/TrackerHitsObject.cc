#include "SimG4CMS/Tracker81/interface/TrackerHitsObject.h"

TrackerHitsObject::TrackerHitsObject(std::string n, TrackingSlaveSD::Collection& h): _hits(h), _name(n)
{}
