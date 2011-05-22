//

/**
  \class    ObjectMultiplicityProducer"
  \brief    Produces a number equal to size of the source collection
            Inspired by PhysicsTools/TagAndProbe/plugins/ObjectMultiplicityCounter.cc
            The difference is that it only produces one number per event,
            not one number per object.

  \author   Jan Veverka
  
*/

#include <memory>
#include <vector>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace vgamma {
  namespace plugins {

    //-------------------------------------------------------------------------
    template<typename CollectionType>
    class ObjectMultiplicityProducer : public edm::EDProducer {
      public:
        explicit ObjectMultiplicityProducer(const edm::ParameterSet & iConfig);
        virtual ~ObjectMultiplicityProducer() ;

        virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
        typedef std::vector<edm::ParameterSet> VPSet;
  
      private:
        VPSet sources_;
    };

    //-------------------------------------------------------------------------
    template<typename CollectionType>
    ObjectMultiplicityProducer<CollectionType>::ObjectMultiplicityProducer(
      const edm::ParameterSet & iConfig
    ) : sources_( iConfig.getParameter<VPSet>( "sources" ) )
    {
      // loop over vector of sources
      for ( VPSet::const_iterator cfg = sources_.begin();
            cfg != sources_.end(); ++cfg )
      {
        produces<uint32_t>(
          cfg->getUntrackedParameter<std::string>(
              // output instance name is an optional parameter `label'
             "label",
              // default instance name = src collection label
              cfg->getParameter<edm::InputTag>("src").label()
          ) // end of getUntrackedParameter<...>(...)
        ); // end of produces<uint32_>(...)
      } // end of loop over sources
    } // end of ctor
    
    
    //-------------------------------------------------------------------------
    template<typename CollectionType>
    ObjectMultiplicityProducer<CollectionType>::~ObjectMultiplicityProducer() 
    { }
    
    //-------------------------------------------------------------------------
    template<typename CollectionType>
    void 
    ObjectMultiplicityProducer<CollectionType>::produce( 
      edm::Event & iEvent,
      const edm::EventSetup & iSetup ) {
      using namespace edm;
      // loop over all sources
      for ( VPSet::const_iterator cfg = sources_.begin();
            cfg != sources_.end(); ++cfg)
      {
        // read input
        InputTag srcTag = cfg->getParameter<InputTag>("src");
        Handle<CollectionType> srcHandle;
        iEvent.getByLabel( srcTag,  srcHandle );

        // book the product
        std::auto_ptr<uint32_t> count( new uint32_t );

        // lazy parsing, to allow cutting on variables not in base class
        StringCutObjectSelector< typename CollectionType::value_type, true >
          cut( cfg->getUntrackedParameter<std::string>( "cut", "" ), true );

        // count
        for ( typename CollectionType::const_iterator it = srcHandle->begin();
              it != srcHandle->end(); ++it )
          if ( cut( *it ) ) (*count)++;

        // fill
        iEvent.put( count,
                    cfg->getUntrackedParameter<std::string>(
                      "label",
                      srcTag.label() // default instance name
                    )
                  );
      } // end loop over all sources
    } // end of ObjectMultiplicityProducer<CollectionType>::produce(..)

  } // end of namespace plugins
} // end of namespace vgamma
