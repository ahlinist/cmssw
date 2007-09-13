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
//   m_cutPixelHits     =  m_config.getParameter<int>("minimumNumberOfPixelHits"); //FIXME: use or remove
     m_cutMaxTIP        =  m_config.getParameter<double>("maximumTransverseImpactParameter"); // used
     m_cutMinPt         =  m_config.getParameter<double>("minimumTransverseMomentum"); // used
     m_cutMaxDecayLen   =  m_config.getParameter<double>("maximumDecayLength"); //used
     m_cutMaxDistToAxis =  m_config.getParameter<double>("maximumDistanceToJetAxis"); //used
 
  }

 
  float discriminator(const reco::BaseTagInfo & ti) const 
   {
          std::multiset<float> significances = orderedSignificances(ti);
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

 protected:
     std::multiset<float> orderedSignificances(const reco::BaseTagInfo & ti)    {


    const reco::TrackCountingTagInfo * tkti = dynamic_cast<const reco::TrackCountingTagInfo *>(&ti);
    if(tkti!=0)
     return  tkti->discriminator(m_nthTrack,m_ipType);
    else
     {
        const reco::TrackIPTagInfo * tkip = dynamic_cast<const reco::TrackIPTagInfo *>(&ti);
      if(tkip!=0)  {
          const std::vector<TrackIPTagInfo::TrackIPData> & impactParameters((tkip->impactParameterData()));
          const edm::RefVector<reco::TrackCollection> & tracks(tkip->selectedTracks());
          std::multiset<float> significances;
          int i=0;
          for(std::vector<TrackIPTagInfo::TrackIPData>::const_iterator it = impactParameters.begin(); it!=impactParameters.end(); ++it, i++)
           {
          if( tracks[i].pt() > m_cutMinPt  &&                          // minimum pt
                 fabs(tracks[i].d0()) < m_cutMaxTIP &&                // max transverse i.p.
                 fabs(impactParameters[i].distanceToJetAxis) < m_cutMaxDistToAxis  &&        // distance to JetAxis
                 (impactParameters[i].closestToJetAxis-tkip.primaryVertex().globalPosition()).mag() < m_cutMaxDecayLen// &&                 // max decay len
//                 track[i].hitPattern().numberOfValidPixelHits() >= m_cutPixelHits //min # pix hits
             )
              {
                double delta  = ROOT::Math::VectorUtil::DeltaR((*tkip->jet()).p4().Vect(), (*tracks[i]).momentum());
                if(delta < m_deltaR)
                 significances.insert( ((impactParamterType==0)?it->ip3d():it->ip2d()).significance() );
              }
           }




   }


    
 private:
   int m_nthTrack;
   int m_ipType;
   double m_deltaR;

//     int  m_cutPixelHits;
    double  m_cutMaxTIP;
    double  m_cutMinPt;
    double  m_cutMaxDecayLen;
    double m_cutMaxDistToAxis;

};
