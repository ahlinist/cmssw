#ifndef ESTBRAWDATA_ESRAWCOLLECTION_H
#define ESTBRAWDATA_ESRAWCOLLECTION_H

#include "TBDataFormats/ESTBRawData/interface/ESDCCHeaderBlock.h"
#include "DataFormats/Common/interface/SortedCollection.h"

typedef edm::SortedCollection<ESDCCHeaderBlock> ESRawDataCollection;

#endif
