#ifndef GeometryReaders_DBIdealGeometryESSource_DBIdealGeometryESSource_H
#define GeometryReaders_DBIdealGeometryESSource_DBIdealGeometryESSource_H

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include <string>

class DBIdealGeometryESSource : public edm::ESProducer, 
                                  public edm::EventSetupRecordIntervalFinder
{
public:
    DBIdealGeometryESSource(const edm::ParameterSet & pset);
    virtual ~DBIdealGeometryESSource(); 
    std::auto_ptr<DDCompactView> produceGeom(const IdealGeometryRecord &);
    std::auto_ptr<DDCompactView> produceMagField(const IdealMagneticFieldRecord &);
    std::auto_ptr<DDCompactView> produce();
protected:
    virtual void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &,
				const edm::IOVSyncValue &, edm::ValidityInterval &);
 private:
    DBIdealGeometryESSource(const DBIdealGeometryESSource &);
    const DBIdealGeometryESSource & operator=(const DBIdealGeometryESSource &);
    std::string rootNodeName_;
};


#endif
