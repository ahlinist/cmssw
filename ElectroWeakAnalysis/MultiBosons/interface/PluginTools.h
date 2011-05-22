// system include files
#include <memory>
#include <string>
#include <vector>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"

namespace vgamma {

  //---------------------------------------------------------------------------
  template <typename CollectionType, typename DataType>
  void
  putValueMap ( edm::Event                   & iEvent,
                edm::Handle<CollectionType>  & iCollection,
                std::vector<DataType>        & iData,
                const std::string            & iName        )
  {
    using namespace std;
    using namespace edm;
  
    auto_ptr<ValueMap<DataType> > product( new ValueMap<DataType>() );
    typename ValueMap<DataType>::Filler filler( *product );
    filler.insert( iCollection, iData.begin(), iData.end() );
    filler.fill();

    iEvent.put( product, iName );
  } // PutValueMap


} // end namespace vgamma