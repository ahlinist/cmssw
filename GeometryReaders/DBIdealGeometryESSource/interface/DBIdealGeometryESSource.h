#ifndef GeometryReaders_DBIdealGeometryESSource_DBIdealGeometryESSource_H
#define GeometryReaders_DBIdealGeometryESSource_DBIdealGeometryESSource_H

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include <string>

class DBIdealGeometryESSource : public edm::eventsetup::ESProducer, 
                                  public edm::eventsetup::EventSetupRecordIntervalFinder
{
public:
    DBIdealGeometryESSource(const edm::ParameterSet & pset);
    virtual ~DBIdealGeometryESSource(); 
    const DDCompactView * produce(const IdealGeometryRecord &);
protected:
    virtual void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &,
				const edm::Timestamp &,edm::ValidityInterval &);
private:
    DBIdealGeometryESSource(const DBIdealGeometryESSource &);
    const DBIdealGeometryESSource & operator=(const DBIdealGeometryESSource &);
};


#endif
