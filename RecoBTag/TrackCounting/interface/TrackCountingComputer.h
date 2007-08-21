#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "Math/GenVector/VectorUtil.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"

class TrackCountingComputer : public JetTagComputer
{
 public:
  TrackCountingComputer(const edm::ParameterSet  & parameters )
  {
     m_nthTrack         = parameters.getParameter<int>("nthTrack");
     m_ipType           = parameters.getParameter<int>("impactParamterType");
     m_deltaR          = parameters.getParameter<double>("deltaR");
  }
 
  float discriminator(const reco::BaseTagInfo & ti) const 
   {
    const reco::TrackCountingTagInfo * tkti = dynamic_cast<const reco::TrackCountingTagInfo *>(&ti);
    if(tkti!=0)
     return  tkti->discriminator(m_nthTrack,m_ipType); 
    else 
     {
        const reco::TrackIPTagInfo * tkip = dynamic_cast<const reco::TrackIPTagInfo *>(&ti);
      if(tkip!=0)  {
          const std::vector<Measurement1D> & impactParameters((tkip->impactParameters(m_ipType))); 
          const edm::RefVector<reco::TrackCollection> & tracks(tkip->selectedTracks());          
          std::multiset<float> significances;
          int i=0;
          for(std::vector<Measurement1D>::const_iterator it = impactParameters.begin(); it!=impactParameters.end(); ++it, i++)
           {
             //FIXME:add extra cuts here
          //   if(tracks[i])
                double delta  = ROOT::Math::VectorUtil::DeltaR((*tkip->jet()).p4().Vect(), (*tracks[i]).momentum());
                if(delta < m_deltaR)
                significances.insert(it->significance());
           }
          std::multiset<float>::reverse_iterator nth=significances.rbegin();
          for(int i=0;i<m_nthTrack-1 && nth!=significances.rend();i++) nth++;  
          if(nth!=significances.rend()) return *nth; else return -100.;
        }
        else 
          {
            //FIXME: report some error? 
            return -100. ;   
          }
     }
   }
 private:
   int m_nthTrack;
   int m_ipType;
   double m_deltaR;
};
