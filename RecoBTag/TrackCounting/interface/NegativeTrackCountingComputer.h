#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"

class NegativeTrackCountingComputer : public JetTagComputer
{
 public:
  NegativeTrackCountingComputer(const edm::ParameterSet  & parameters )
  {
     m_nthTrack         = parameters.getParameter<int>("nthTrack");
     m_ipType           = parameters.getParameter<int>("impactParamterType");
  }
 
  float discriminator(const reco::BaseTagInfo & ti) const 
   {
     const reco::TrackIPTagInfo * tkip = dynamic_cast<const reco::TrackIPTagInfo *>(&ti);
      if(tkip!=0)  {
          const std::vector<Measurement1D> & impactParameters((tkip->impactParameters(m_ipType))); 
          std::multiset<float> significances;
          for(std::vector<Measurement1D>::const_iterator it = impactParameters.begin(); it!=impactParameters.end(); ++it)
           {
             //FIXME:add extra cuts here
             significances.insert(it->significance());
           }
          std::multiset<float>::iterator nth=significances.begin();
          for(int i=0;i<m_nthTrack-1 && nth!=significances.end();i++) nth++;  
          if(nth!=significances.end()) return -(*nth); else return -100.;
        }
        else 
          {
            //FIXME: report some error? 
            return -100. ;   
          }
   }
 private:
   int m_nthTrack;
   int m_ipType;

};
