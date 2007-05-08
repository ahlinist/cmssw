#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"

class TrackCountingComputer : public JetTagComputer
{
 public:
  TrackCountingComputer(const edm::ParameterSet  & parameters ){}
 
  float discriminator(const reco::BaseTagInfo & ti) const 
   {
    const reco::TrackCountingTagInfo * tkti = dynamic_cast<const reco::TrackCountingTagInfo *>(&ti);
    if(tkti!=0)
     return  tkti->discriminator(3,0);
    else 
     {
       //FIXME: exceptions
       abort();
      return 0;
     }
   }
};
