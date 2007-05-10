#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"

class TrackCountingComputer : public JetTagComputer
{
 public:
  TrackCountingComputer(const edm::ParameterSet  & parameters ){}
 
  float discriminator(const reco::BaseTagInfo & ti) const 
   {
    const reco::TrackCountingTagInfo * tkti = dynamic_cast<const reco::TrackCountingTagInfo *>(&ti);
    if(tkti!=0)
     return  tkti->discriminator(3,0); //FIXME: configurable
    else 
     {
        const reco::TrackIPTagInfo * tkip = dynamic_cast<const reco::TrackIPTagInfo *>(&ti);
      if(tkip!=0)  {
          const std::vector<Measurement1D> & impactParameters((tkip->impactParameters(0))); //FIXME: configurable
          std::multiset<float> significances;
          for(std::vector<Measurement1D>::const_iterator it = impactParameters.begin(); it!=impactParameters.end(); ++it)
           {
             //FIXME:add extra cuts here
             significances.insert(it->significance());
           }
          std::multiset<float>::iterator nth=significances.begin();
          for(int i=0;i<2 && nth!=significances.end();i++) nth++;  //3rd track, FIXME:configurable
          if(nth!=significances.end()) return *nth; else return -10.;
        }
        else {    //FIXME: exceptions
                 abort();
                return 0;
             }
     }
   }
};
