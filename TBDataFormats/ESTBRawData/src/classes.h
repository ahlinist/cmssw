#include <vector>
#include <TBDataFormats/ESTBRawData/interface/ESDCCHeaderBlock.h>
#include <TBDataFormats/ESTBRawData/interface/ESKCHIPBlock.h>
#include <TBDataFormats/ESTBRawData/interface/ESRawDataCollections.h>
#include <TBDataFormats/ESTBRawData/interface/ESLocalRawDataCollections.h>
#include <DataFormats/Common/interface/Wrapper.h>

namespace {
  namespace {
    ESDCCHeaderBlock ESDCC_;
    std::vector<ESDCCHeaderBlock> vESDCC_;
    edm::SortedCollection<ESDCCHeaderBlock> scESDCC_;
    ESRawDataCollection ESDC_;
    edm::Wrapper<ESRawDataCollection> theESDC_;
    edm::Wrapper< edm::SortedCollection<ESDCCHeaderBlock> > theESDCC_;

    ESKCHIPBlock ESKCHIP_;
    std::vector<ESKCHIPBlock> vESKCHIP_;
    edm::SortedCollection<ESKCHIPBlock> scESKCHIP_;
    ESLocalRawDataCollection ESLDC_;
    edm::Wrapper<ESLocalRawDataCollection> theESLDC_;
    edm::Wrapper< edm::SortedCollection<ESKCHIPBlock> > theESKCHIP_;
  }
}
