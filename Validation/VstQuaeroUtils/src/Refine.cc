/*******************************************
Implements Refine, a class that "refines" a sample of events, 
cutting away events outside the boundaries we are able to analyze.
********************************************/

#include "Validation/VstQuaeroUtils/interface/Refine.hh"
#include <string>
#include <cfloat>
using namespace std;

/* Constructor.  Arguments include which collider (tev1, tev2, lep2, hera, or lhc)
   and which experiment (aleph, l3, cdf, d0, h1, or cms) Refine should assume.
   int _level specifies the level of refinement, with higher levels implementing 
   more refinement.  HintSpec is passed in as an argument if this refinement restricts 
   itself to a subsample in which a hint (of new physics) is observed.  */

Refine::Refine(string _collider, string _experiment, string partitionRuleName, int _level, HintSpec _hintSpec):
   collider(_collider), experiment(_experiment), 
   partitionRule((partitionRuleName=="" ? _collider+"-Vista" : partitionRuleName)), 
   level(_level), hintSpec(_hintSpec)
{
   // Ensure _collider and _experiment are understood
   assert(
      ((collider=="tev1")&&
       ((experiment=="d0"))) ||
      ((collider=="tev2")&&
       ((experiment=="cdf")||(experiment=="d0"))) ||
      ((collider=="lhc")&&
       ((experiment=="cms")||(experiment=="atlas"))) ||
      ((collider=="lep2")&&
       ((experiment=="aleph")||(experiment=="l3"))) ||
      ((collider=="hera")&&
       ((experiment=="h1")))
      );
   minWt = 0;
   return;
}


bool Refine::satisfiesCriteriaQ(QuaeroEvent& e)
{
   vector<QuaeroRecoObject> o = e.getObjects();
   double wt = e.getWeight();
   bool debug = false;

   // if((e.getEventType()=="sig")&&(wt>10))  wt = 10;

   double zvtx = e.getZVtx();
   double sumPt = 0;
   for(size_t i=0; i<o.size(); i++)
      sumPt += o[i].getFourVector().perp();
   sumPt += e.getPmiss().perp();
   string fs = partitionRule.getFinalState(e).getTextString();

   bool ans = true;

   /*************************************************
                         LEP II
   **************************************************/
   if((collider=="lep2")&&
      ((experiment=="l3")||(experiment=="aleph")))
   {

      if(experiment=="aleph")
      {
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="e")||
               (o[i].getObjectTypeSansSign()=="mu")||
               (o[i].getObjectTypeSansSign()=="tau")||
               (o[i].getObjectTypeSansSign()=="ph"))
               for(size_t j=0; j<o.size(); j++)
                  if(o[j].getObjectType()=="ph")
                     if(i!=j)
                        if((Math::deltaphi(o[i].getFourVector().phi(),o[j].getFourVector().phi())<3./180*M_PI)&&
                           (fabs(o[i].getFourVector().theta()-o[j].getFourVector().theta())<3./180*M_PI))
                        {
                           o[i] = QuaeroRecoObject(o[i].getObjectType(),
                                                   o[i].getFourVector()+o[j].getFourVector());
                           o.erase(o.begin()+j);
                           break;
                        }
      }

      // Cluster at a larger scale

      int k1=0,k2=0;
      while((k1>=0)&&(k2>=0))
      {
         k1=k2=-1;
         double y_cut = 0.01;
         double yij_min = y_cut;
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectType()=="j") ||
               (o[i].getObjectType()=="b") ||
               (o[i].getObjectTypeSansSign()=="tau"))
               for(size_t j=i+1; j<o.size(); j++)
                  if((o[j].getObjectType()=="j") ||
                     (o[j].getObjectType()=="b") ||
                     (o[i].getObjectTypeSansSign()=="tau"))
                  {
                     double yij = pow((o[i].getFourVector()+o[j].getFourVector()).m()/e.getRootS(),2.);
                     if(yij < yij_min)
                     {
                        yij_min = yij;
                        k1=i; k2=j;
                     }
                  }
         if((k1>=0)&&(k2>=0))
         {
            o[k1] = QuaeroRecoObject("j",o[k1].getFourVector()+o[k2].getFourVector());
            o.erase(o.begin()+k2);
         }
      }

      //      e = QuaeroEvent(e.getEventType(), e.getRunNumber(), e.getWeight(),  o, e.getRootS(), e.getZVtx());
      e.setObjects(o);
      fs = partitionRule.getFinalState(e).getTextString();

      bool nothingInterestingInEvent = 
         ((e.numberOfObjects(25,0.7)==0) ||
          (fs=="1pmiss") ||
          ((e.numberOfObjects(25,0.7)==1)&&(e.numberOfObjects("uncl",25,0.7)==1))
            );
      bool eventContainsFarForwardObject = false;
      for(size_t i=0; i<o.size(); i++)
         if((fabs(cos(o[i].getFourVector().theta()))>0.9)&&
            (o[i].getFourVector().e()>10))	  
            eventContainsFarForwardObject = true;      

      if(experiment=="l3")
      {

         // In events with two like-sign electrons, make them opposite sign
         if( (e.numberOfObjects("e+")==2) )
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectType()=="e+")
               {
                  o[i].chargeConjugate(); 
                  break;
               }
         if( (e.numberOfObjects("e-")==2) )
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectType()=="e-")
               {
                  o[i].chargeConjugate(); 
                  break;
               }

         /*
         // Massage electron energy of Monte Carlo
         if(e.getEventType()!="data")
         {
         for(size_t i=0; i<o.size(); i++)
         if((o[i].getObjectTypeSansSign()=="e")&&(o[i].getFourVector().e()>20))
         {
         double E = o[i].getFourVector().e();
         double q = 
         ( E - 
         (E>20)*1.0*(E -20)/((e.getRootS()-10)-20) -
         (E>e.getRootS()-10)*(E-(e.getRootS()-10))*3.5/10 -
         (E>e.getRootS())*(E-e.getRootS())/2 )
         / o[i].getFourVector().e();
         o[i] = QuaeroRecoObject(o[i].getObjectType(),o[i].getFourVector()*q);
         }
         }
         for(int i=0; i<o.size(); i++)
         {
         if((o[i].getObjectTypeSansSign()=="e")&&
		 (fabs(cos(o[i].getFourVector().theta()))>0.65))
         o[i].changeComponentType("ph");
         }
         e.setObjects(o);
         if((e.numberOfObjects("e+")==1)&&
	     (e.numberOfObjects("ph")==1))	    
         for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectType()=="ph")
         o[i].changeComponentType("e-");
         if((e.numberOfObjects("e-")==1)&&
	     (e.numberOfObjects("ph")==1))	    
         for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectType()=="ph")
         o[i].changeComponentType("e+");
         if((e.numberOfObjects("e")==0)&&
	     (e.numberOfObjects("ph")==2))	    
         for(size_t i=0; i<o.size(); i++)
         if((o[i].getObjectType()=="ph")&&
		 (fabs(cos(o[i].getFourVector().theta()))>0.65))
         o[i].changeComponentType( ((cos(o[i].getFourVector().theta())>0) ? "e-" : "e+") );
         e.setObjects(o);
         */

         bool poorlyMeasuredBackToBack = false;
         if((o.size()==2) &&
            (Math::deltaphi(o[0].getFourVector().phi(),o[1].getFourVector().phi())>3.0) &&
            (e.getPmiss().perp()>10) )
            poorlyMeasuredBackToBack = true;

         /*
           if(e.numberOfObjects("e")==1)
           {
           int j=-1;
           for(size_t i=0; i<o.size(); i++)
           if(o[i].getObjectType()=="ph")
           if((j<0)||(o[j].getFourVector().perp()<o[i].getFourVector().perp()))
           j=i;
           if(j>=0)
           {
           if(e.numberOfObjects("e+")==1)
           o[j].changeComponentType("e-");
           else
           o[j].changeComponentType("e+");
           }
           }

           // The simulation does not appear to do a good job of reproducing the tracking efficiency
           // for electrons with |cos(theta)| > 0.6.  We are removing events with an electron and with a
           // photon reconstructed with |cos(theta)| > 0.6.
           if( (e.numberOfObjects("e")==1) &&
           (e.numberOfObjects("ph")>=1) &&
           (fabs(cos(e.getThisObject("ph",1)->getFourVector().theta()))>0.6) )
           ans = false;
         */

         // Adjust energies
         if((e.getEventType()!="data")&&(level>=5))
         {
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectType()=="j")
                  o[i] = QuaeroRecoObject("j",o[i].getFourVector()*Math::gasdev(1,0.01));
               else if(o[i].getObjectTypeSansSign()=="mu")
                  o[i] = QuaeroRecoObject(o[i].getObjectType(),o[i].getFourVector()*Math::gasdev(1,0.05));
               else if((o[i].getObjectTypeSansSign()=="e") ||
                       (o[i].getObjectType()=="ph"))
                  o[i] = QuaeroRecoObject(o[i].getObjectType(),o[i].getFourVector()*0.99);
         }
         // Slightly smear azimuthal angle of Monte Carlo events
         if((e.getEventType()!="data")&&(level>=5))
            for(size_t i=0; i<o.size(); i++)
            {
               CLHEP::HepLorentzVector p = o[i].getFourVector();
               double pt = p.perp();
               double phi = p.phi()+Math::gasdev(0,0.005);
               p = CLHEP::HepLorentzVector(p.e(), CLHEP::Hep3Vector(pt*cos(phi),pt*sin(phi),p.pz()));
               o[i] = QuaeroRecoObject(o[i].getObjectType(),p);
            }

         e.setObjects(o);
         fs = partitionRule.getFinalState(e).getTextString();

         bool lowEnergySingleObjectEvent = false;
         int i1=-1;
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getFourVector().e()>10)
               if(i1<0)
                  i1=i;
               else
                  i1=99;
         if((i1>=0)&&(i1!=99))
            if((o[i1].getFourVector().e()<75)||
               (o[i1].getFourVector().e() > (e.getRootS()/2-10))||
               (fabs(cos(o[i1].getFourVector().theta()))>0.5))
               lowEnergySingleObjectEvent = true;	    

         bool forwardDoubleObjectEvent = false;
         i1=-1; int i2=-1;
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getFourVector().e()>10)
               if(i1<0)
                  i1=i;
               else
                  if(i2<0)
                     i2=i;
                  else
                     i1=99;
         if((i1>=0)&&(i2>=0)&&(i1!=99))
            if((fabs(cos(o[i1].getFourVector().theta()))>0.5)||
               (fabs(cos(o[i2].getFourVector().theta()))>0.5))
               forwardDoubleObjectEvent = true;



         /*
         // Remove events with poorly measured electron, producing collinear missing energy 
         if((fs=="1e+1e-1pmiss"))
         {
         double dphi = Math::deltaphi( e.getThisObject("e+")->getFourVector().phi(), e.getThisObject("e+")->getFourVector().phi() );
         if(dphi>3.0)
         ans = false;
         }
         */
         /* We see additional events in the data in the final states 1j and 1j1pmiss.
            From scanning events, we note that these appear to be due to events with a cosmic ray muon
            that brems in the hadronic calorimeter, screwing up the reconstruction of the event.
            The hypothesis that this excess is due to new physics is discarded by the fact that 
            no such excess is seen in the Aleph data, where we have been able to do a more thorough 
            job of removing cosmic ray events.  We therefore remove the 1j and 1j1pmiss final states. */
         /*
           bool disallowedFinalState = false;
           if( (fs=="1e+1pmiss") ||
           (fs=="1e-1pmiss") ||
           (fs=="1j") ||
           (fs=="1j1pmiss") )
           disallowedFinalState = true;

	    
           if((fs=="1e+1j")||(fs=="1e-1j"))
           {
           for(size_t i=0; i<o.size(); i++)
           if(o[i].getObjectType()=="j")
           {
           if(fs=="1e+1j")
           o[i].changeComponentType("e-");
           if(fs=="1e-1j")
           o[i].changeComponentType("e+");
           break;
           }
           fs = "1e+1e-";
           }
           if( (fs=="1e+1j1pmiss") ||
           (fs=="1e-1j1pmiss") ||
           (fs=="1j1ph1pmiss") ||
           (fs=="1j1ph1pmiss") )
           {
           double dphi = Math::deltaphi( e.getThisObject("j")->getFourVector().phi(), e.getPmiss().phi() );
           if((dphi<0.1)||(dphi>M_PI-0.1))
           ans = false;
           }
         */

         double pmissPolar = fabs(cos(e.getPmiss().theta()));
         bool missingEnergyPointingIntoSpacCal = ((e.getPmiss().e()>15)&&(pmissPolar>0.7)&&(pmissPolar<0.75));
         bool missingEnergyPointingDownBeamPipe = ((e.getPmiss().e()>15)&&(pmissPolar>0.9));	  
         bool hasObjectFarForward = false;
         for(size_t i=0; i<o.size(); i++)
            if((fabs(cos(o[i].getFourVector().theta()))>0.9)&&
               (o[i].getFourVector().e()>10))
               hasObjectFarForward = true;

         bool photonPmissEvent = false;
         if((fs=="1ph1pmiss")||
            (fs=="2ph1pmiss")||
            (fs=="3ph1pmiss")||
            (fs=="4ph1pmiss"))
            photonPmissEvent = true;

         bool photonOnlyEvent = false;
         if((fs=="1ph")||
            (fs=="2ph")||
            (fs=="3ph")||
            (fs=="4ph"))
            photonOnlyEvent = true;

         if( nothingInterestingInEvent ||
             missingEnergyPointingIntoSpacCal ||
             missingEnergyPointingDownBeamPipe ||
             lowEnergySingleObjectEvent ||
             forwardDoubleObjectEvent ||
             hasObjectFarForward
             //photonPmissEvent ||
             //photonOnlyEvent || 
             //poorlyMeasuredBackToBack ||
             //disallowedFinalState
            )
            ans = false;

      }

      if(experiment=="aleph")
      {
         bool cosmicEvent = false;
         if((e.numberOfObjects("mu+",10)==1)&&
            (e.numberOfObjects("mu-",10)==1))
         {
            CLHEP::HepLorentzVector mu1 = e.getThisObject("mu+",1)->getFourVector();
            CLHEP::HepLorentzVector mu2 = e.getThisObject("mu-",1)->getFourVector();
            //	      double m = (mu1+mu2).m();
            if( (Math::deltaphi(mu1.phi(),mu2.phi())>3.13) )
               cosmicEvent = true;
         }

         bool screwedUpReconstructionOfBremEvent = false;
         if((e.numberOfObjects("e+",10)==1)&&
            (e.numberOfObjects("e-",10)==1)&&
            (e.numberOfObjects("ph",10)==1))
         {
            CLHEP::HepLorentzVector e1 = e.getThisObject("e+",1)->getFourVector();
            CLHEP::HepLorentzVector e2 = e.getThisObject("e-",1)->getFourVector();
            if( (Math::deltaphi(e1.phi(),e2.phi())>3.0) )
               screwedUpReconstructionOfBremEvent = true;
         }

         bool conversion = false;
         if( (fs=="2e+") ||
             (fs=="2e-") ||
             (fs=="2mu+") ||
             (fs=="2mu-") )
            conversion = true;
         for(int i=0; i<e.numberOfObjects("e+",10); i++)
            for(int j=0; j<e.numberOfObjects("e-",10); j++)
               if( (e.getThisObject("e+",i+1)->getFourVector()+
                    e.getThisObject("e-",j+1)->getFourVector()).m() < 5 )
                  conversion = true;

         bool photonPmissEvent = false;
         if((fs=="1ph")||                    // contributions from cosmic rays that we do not model
            ((fs=="2ph")&&                   // One of two different issues:
             ((fabs(e.getRootS()-183)<1) ||  //   (1) Kyle and I never ran over the multiph-183 file
              ((e.getThisObject("ph",1)!=NULL)&& // (2) cosmic rays again
               (e.getThisObject("ph",2)!=NULL)&&
               (Math::deltaphi(e.getThisObject("ph",1)->getFourVector().phi(),
                               e.getThisObject("ph",2)->getFourVector().phi()) < 1))
                ))||
            (fs=="1ph1pmiss")||
            (fs=="2ph1pmiss")||
            (fs=="3ph1pmiss")||
            (fs=="4ph1pmiss"))
            photonPmissEvent = true;

         bool mistakenOneProngTau = false;
         if( (o.size()==2)&&
             ( (o[0].getObjectTypeSansSign()=="tau") ||
               (o[1].getObjectTypeSansSign()=="tau") ) &&
             ( (o[0].getObjectTypeSansSign()=="e") ||
               (o[0].getObjectTypeSansSign()=="mu") ||
               (o[0].getObjectTypeSansSign()=="tau") ) &&
             ( (o[1].getObjectTypeSansSign()=="e") ||
               (o[1].getObjectTypeSansSign()=="mu") ||
               (o[1].getObjectTypeSansSign()=="tau") ) &&
             ( Math::deltaphi(o[0].getFourVector().phi(),o[1].getFourVector().phi()) > 3.13 )
            )
            mistakenOneProngTau = true;
	  
         if((fs=="1e+1j")||(fs=="1e-1j"))
         {
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectType()=="j")
               {
                  if(fs=="1e+1j")
                     o[i].changeComponentType("e-");
                  if(fs=="1e-1j")
                     o[i].changeComponentType("e+");
                  break;
               }
            fs = "1e+1e-";
         }

         if(
            nothingInterestingInEvent ||
            eventContainsFarForwardObject ||
            cosmicEvent ||
            screwedUpReconstructionOfBremEvent ||
            conversion ||
            mistakenOneProngTau || 
            ((level>=5) &&
             (photonPmissEvent))
            )
            ans = false;
      }

   }


   /*************************************************
                          HERA
   **************************************************/
   if((collider=="hera")&&
      ((experiment=="h1")||(experiment=="zeus")))
   {

      double maxEta = 2.44; // = -log(tan(10./180*M_PI/2));
      double oPtMin = ((level<=1) ? 15 : 20);
	  
      if(e.getEventType()=="sig")
      {
         // All e's are e+'s in the H1 General Search
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getObjectType()=="e-")  
               o[i].chargeConjugate();
	  
         // Merge any jets with deltaR < 1.0
         for(size_t i=0; i<o.size(); i++)
            for(size_t j=i+1; j<o.size(); j++)
               if((o[i].getObjectType()=="j")&&
                  (o[j].getObjectTypeSansSign()=="j")&&
                  (Math::deltaR(o[i].getFourVector().phi(), 
                                Math::theta2eta(o[i].getFourVector().theta()), 
                                o[j].getFourVector().phi(), 
                                Math::theta2eta(o[j].getFourVector().theta())) < 1.0))
               {
                  o[i] = QuaeroRecoObject("j",o[i].getFourVector()+o[j].getFourVector());
                  o.erase(o.begin()+j);
                  j--;
               }
	  
         // Any object with 10 < theta < 140 degrees is converted to unclustered energy
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getFourVector().theta()*180./M_PI < 10)||
               (o[i].getFourVector().theta()*180./M_PI > 140))
               o[i].changeComponentType("uncl");
	  
         e.setObjects(o);
         fs = partitionRule.getFinalState(e).getTextString();
	  
         // Remove events with E - pz > 75 GeV, following the H1 General Search
         double energy=0, pz=0;
         for(size_t i=0; i<o.size(); i++)
         {
            energy += o[i].getFourVector().e();
            pz += fabs(o[i].getFourVector().pz());
         }
         if(energy-pz>75) // see email from Caron to Knuteson on 2/21/2005
            ans = false;
	  
         if((e.getEventType()=="sig")&&
            (level==11))
         {
            // KILL.qbg -- see investigateKills_summary.txt
            wt *= 0.99;
            // KILL.noz -- see investigateKills_summary.txt
            if((o.size()==2)&&
               ((o[0].getObjectType()=="ph")||
                (o[1].getObjectType()=="ph")))
               wt *= 0.99;
            // electrons are sometimes killed
            int z = e.numberOfObjects("e",oPtMin);
            if(z>=1)
            {
               if(e.numberOfObjects("j",oPtMin)==0)
                  wt *= pow(0.80,1.*z);
               if(e.numberOfObjects("j",oPtMin)==1)
                  wt *= pow(1729./2400*1.1,1.*z);
               if(e.numberOfObjects("j",oPtMin)==2)
                  wt *= pow(616./947,1.*z);
               if(e.numberOfObjects("j",oPtMin)==3)
                  wt *= pow(0.55,1.*z);
               if(e.numberOfObjects("j",oPtMin)==4)
                  wt *= pow(0.54,1.*z);
               if(e.getThisObject("e+",1)!=0)
               {
                  double eta = Math::theta2eta(e.getThisObject("e+",1)->getFourVector().theta());
                  wt *= (1-min(3.,max(-1.,eta-0.8))*0.10);
                  if(eta>1)
                     wt *= .5;
               }
            }
            // photons are sometimes killed
            if(e.numberOfObjects("ph",oPtMin)>=1)
            {
               if(e.numberOfObjects("j",oPtMin)==0)
                  wt *= 0.80;
               if(e.numberOfObjects("j",oPtMin)==1)
                  wt *= 16./36;
               if(e.numberOfObjects("j",oPtMin)>=2)
                  wt *= 0.25;
               if(e.getThisObject("ph",1)!=0)
                  wt *= (1-min(3.,max(-1.,(Math::theta2eta(e.getThisObject("ph",1)->getFourVector().theta())-1.2)))*0.20);
            }
            // muons are sometimes killed
            if(e.numberOfObjects("mu",oPtMin)>=1)
            {
               if(e.getThisObject("mu+",1)!=0)
               {
                  double eta = Math::theta2eta(e.getThisObject("mu+",1)->getFourVector().theta());
                  if(eta>1)
                     wt *= 0.5;
               }
            }
         }
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getObjectType()=="kill")
            {
               if((o[i].getFourVector().perp()>20)&&
                  (o[i].getFourVector().theta()*180./M_PI > 10)&&
                  (o[i].getFourVector().theta()*180./M_PI < 140))
                  ans = false;
               else
                  o[i].changeComponentType("uncl");
            }      
	  
         if(e.getEventType()!="data")
         {
            // Remove events containing a photon within deltaR < 1.0 of a jet
            for(size_t i=0; i<o.size(); i++)
               for(size_t j=0; j<o.size(); j++)
                  if((o[i].getObjectType()=="j")&&
                     (o[j].getObjectType()=="ph")&&
                     (Math::deltaR(o[i].getFourVector().phi(), 
                                   Math::theta2eta(o[i].getFourVector().theta()), 
                                   o[j].getFourVector().phi(), 
                                   Math::theta2eta(o[j].getFourVector().theta())) < 1.0))
                     ans = false;
	      
            // Remove events containing an electron within deltaR < 1.0 of a jet
            for(size_t i=0; i<o.size(); i++)
               for(size_t j=0; j<o.size(); j++)
                  if((o[i].getObjectType()=="j")&&
                     (o[j].getObjectTypeSansSign()=="e")&&
                     (Math::deltaR(o[i].getFourVector().phi(), 
                                   Math::theta2eta(o[i].getFourVector().theta()), 
                                   o[j].getFourVector().phi(), 
                                   Math::theta2eta(o[j].getFourVector().theta())) < 1.0))
                     ans = false;
         }
      }

      // Remove the 1mu+1pmiss final state
      // There is a large discrepancy here, hypothesized to result from a low-pT track 
      // being misreconstructed to high-pT
      // Also remove other final states with only one object
      if( ( ( e.numberOfObjects("e",oPtMin)+
              e.numberOfObjects("mu",oPtMin)+
              e.numberOfObjects("ph",oPtMin)+
              e.numberOfObjects("j",oPtMin,maxEta) ) < 2 ) )
         ans = false;

   }

   /*************************************************
                       Tevatron I
   **************************************************/
   if((collider=="tev1")&&
      (experiment=="d0"))
   {

      for(size_t i=0; i<o.size(); i++)
         if((o[i].getObjectType()=="e-")||
            (o[i].getObjectType()=="mu-"))  // all leptons are positive at D0 Run I
            o[i].chargeConjugate();
      
      for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectType()=="j")
            for(size_t j=i+1; j<o.size(); j++)
               if(o[j].getObjectType()=="j")
                  if(Math::deltaR(o[i].getFourVector().phi(),
                                  Math::theta2eta(o[i].getFourVector().theta()),
                                  o[j].getFourVector().phi(),
                                  Math::theta2eta(o[j].getFourVector().theta()))<0.7)
                  {
                     o[i] = QuaeroRecoObject("j",o[i].getFourVector()+o[j].getFourVector());
                     o.erase(o.begin()+j);
                     j--;
                  }


      e.setObjects(o);
      fs = partitionRule.getFinalState(e).getTextString();

      bool allowedFinalState = false;
      if(
         (fs=="1e+2j1pmiss")||
         (fs=="1e+3j1pmiss")||
         (fs=="1e+4j1pmiss")||
         (fs=="1e+5j1pmiss")||
         (fs=="1e+6j1pmiss")||

         (fs=="2e+2j")||
         (fs=="2e+3j")||
         (fs=="2e+4j")||

         (fs=="1e+1mu+")||
         (fs=="1e+1j1mu+")||
         (fs=="1e+2j1mu+")||
         (fs=="1e+3j1mu+")||

         (fs=="1e+1mu+1pmiss")||
         (fs=="1e+1j1mu+1pmiss")||
         (fs=="1e+2j1mu+1pmiss")||
         (fs=="1e+3j1mu+1pmiss")
         )
         allowedFinalState = true;
      
      bool allowedTopology = false;
      CLHEP::HepLorentzVector jetSum;
      for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectType()=="j")
            jetSum = jetSum + o[i].getFourVector();
      if((e.numberOfObjects("e+",20)>=1)&&
         (e.getPmiss().perp()>30)&&
         (e.numberOfObjects("j",20)>=2)&&
         ((e.getThisObject("e+",1)->getFourVector()+e.getPmiss()).m()>30)&&
         ((e.getThisObject("e+",1)->getFourVector()+e.getPmiss()).perp()>45)&&
         (jetSum.perp()>45)&&
         (Math::deltaphi(e.getThisObject("j",1)->getFourVector().phi(),e.getPmiss().phi())>0.25)&&
         (Math::deltaphi(e.getThisObject("j",2)->getFourVector().phi(),e.getPmiss().phi())>0.25))
         allowedTopology = true; // cuts_1e0mu0tau0ph2j0b1met
      if((e.numberOfObjects("e+",15)>=1)&&
         (e.numberOfObjects("mu+",15)>=1))
         allowedTopology = true; // cuts_1e1mu0tau0ph0j0b0met
      if((e.numberOfObjects("e+",20)>=2)&&
         (e.numberOfObjects("j",20)>=2))
         allowedTopology = true;	  

      if(e.getEventType()=="sig")
      {
         // These numbers come from Quaero_d0/particleIDefficiencies.txt
         if(e.numberOfObjects("e+")>=2)  // 2e0mu0tau0ph2j0b0met
            wt *= 0.70;
         if((e.numberOfObjects("e+")==1)&&
            (e.numberOfObjects("mu+")==0))  // 1e0mu0tau0ph2j0b1met
            wt *= 0.61;
         if((e.numberOfObjects("e+")==1)&&
            (e.numberOfObjects("mu+")==1))  // 1e1mu0tau0ph0j0b0met
            wt *= 0.30;
      }

      if(
         (!allowedFinalState)||
         (!allowedTopology)
         )
         ans = false;

   }

   /*************************************************
                       Tevatron II
   **************************************************/
   if((collider=="tev2")&&
      ((experiment=="d0")))
   {

      string eventType = e.getEventType();

      if(
         (eventType.find("data_1pc")!=string::npos)||
         (eventType.find("dataset_1pc")!=string::npos)||
         (eventType.find("data_10pc")!=string::npos)||
         (eventType.find("dataset_10pc")!=string::npos)||
         (eventType.find("hipt-data")!=string::npos)
         )
         eventType = "data";

      e.reType(eventType);

      string rn = e.getRunNumber();


      // Remove duplicate objects
      vector<string> orderedObjects;
      orderedObjects.push_back("e");
      orderedObjects.push_back("mu");
      orderedObjects.push_back("tau");
      orderedObjects.push_back("ph");
      orderedObjects.push_back("b");
      orderedObjects.push_back("j");
      for(size_t i=0; i<o.size(); i++)
      {
         vector<string>::iterator k=find(orderedObjects.begin(),orderedObjects.end(),o[i].getObjectTypeSansSign());
         if(k!=orderedObjects.end())
            for(size_t j=0; j<o.size(); j++)
               if((i!=j)&&(find(k+1,orderedObjects.end(),o[j].getObjectTypeSansSign())!=orderedObjects.end()))
                  if(Math::deltaR(o[i].getFourVector().phi(), 
                                  Math::theta2eta(o[i].getFourVector().theta()), 
                                  o[j].getFourVector().phi(), 
                                  Math::theta2eta(o[j].getFourVector().theta())) < 0.4)
                  {
                     o[j] = QuaeroRecoObject("uncl",o[j].getFourVector());
                  }
      }

      // Remove electrons in the ICD
      for(size_t i=0; i<o.size(); i++)
         if((o[i].getObjectTypeSansSign()=="e"))
         {
            double e_eta = Math::theta2eta(o[i].getFourVector().theta());
            if((abs(e_eta)>1.1)&&
               (abs(e_eta)<1.5))
               o[i] = QuaeroRecoObject("uncl",o[i].getFourVector());
         }

      e.setObjects(o);
      fs = partitionRule.getFinalState(e).getTextString();

      bool disallowedFinalState = false;
      if(level>=5)
      {
         if((fs=="1e+")||(fs=="1e-")||
            (fs=="1mu+")||(fs=="1mu-")||
            (fs=="1tau+")||(fs=="1tau-")||
            (fs=="1ph")||(fs=="1ph1pmiss")||
            (fs=="1j1pmiss_sumPt0-400")||(fs=="1j1pmiss_sumPt400+")||
            (fs=="1j_sumPt0-400")||(fs=="1j_sumPt400+")
            )
            disallowedFinalState = true;        // cut out "runt" final states
      }
      if(level>=7)
      {
         if(eventType!="data")
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectTypeSansSign()=="e")
                  if(drand48()<0.01)
                     o[i] = QuaeroRecoObject(((string)"e")+((o[i].getSign()=="+") ? "-" : "+"),o[i].getFourVector());
      }
      

      double ptThreshold0 = 10, ptThreshold01 = 15, ptThreshold1 = 20, ptThreshold12 = 25, ptThreshold2 = 35, ptThreshold3 = 50;
      if(level>=6)
      {
         ptThreshold0 = 17;
         ptThreshold01 = 20;
         ptThreshold1 = 25;
         ptThreshold12 = 30;
         ptThreshold2 = 40;
         ptThreshold3 = 60;
      }

      bool passesSingleCentralElectronTrigger = (e.numberOfObjects("e",ptThreshold1,1.0)>=1);
      bool passesSinglePlugElectronTrigger = (e.numberOfObjects("e",25,2.5)>=1);
      /*      bool passesPrescaledSinglePlugElectronTrigger = ((e.numberOfObjects("e",ptThreshold12,2.5)-
              e.numberOfObjects("e",ptThreshold12,1.0)>=1)&&
              ((eventType!="data")||
              ((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) ); */

      bool passesDiEmObjectTrigger = 
         ( ( (e.numberOfObjects("e",ptThreshold1,2.5)+
              e.numberOfObjects("ph",ptThreshold1,2.5)) >=2 ) &&
           ( (e.numberOfObjects("e",ptThreshold1+5,2.5)+
              e.numberOfObjects("ph",ptThreshold1+5,2.5)) >=1 ) );

      /* bool passesPrescaledSingleCentralElectronTrigger = 
         ( (e.numberOfObjects("e",ptThreshold01,1.0)>=1) &&
         ((eventType!="data")||
         ((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) );
         bool passesPrescaledSingleCentralMuonTrigger = 
         ( (e.numberOfObjects("mu",ptThreshold01,1.0)>=1) &&
         ((eventType!="data")||
         ((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) ); 
         bool passesPrescaledSingleCentralPhotonTrigger = 
         ( (e.numberOfObjects("ph",ptThreshold2,1.0)>=1) &&
         ((eventType!="data")||
         ((rn.length()>1)&&(rn.substr(rn.length()-2)=="00"))) ); */

      int cem8 = e.numberOfObjects("e",ptThreshold0,1.0);
      int cem20 = e.numberOfObjects("e",ptThreshold01,1.0);
      int pem8 = e.numberOfObjects("e",ptThreshold1,2.5)-e.numberOfObjects("e",ptThreshold1,1.0);
      int muo8 = e.numberOfObjects("mu",ptThreshold0,1.0);
      int muo20 = e.numberOfObjects("mu",ptThreshold01,1.0);
      int trk8 = e.numberOfObjects("tau",ptThreshold0,1.0);
      bool passesDileptonTrigger = 
         ( (cem8>=2) || (muo8>=2) || ((muo8>=1)&&(e.numberOfObjects("mu",ptThreshold0,2.5)>=2)) ||
           (cem8 && pem8) || (cem8 && muo8) || (cem20 && trk8) ||
           (pem8 && muo8) || (muo20 && trk8) );

      bool passesSingleMuonTrigger = false;
      for(size_t i=0; i<o.size(); i++)
         if((o[i].getObjectTypeSansSign()=="mu")&&
            (o[i].getFourVector().perp()>ptThreshold1)&&
            (fabs(Math::theta2eta(o[i].getFourVector().theta()))<2.5)&&
            (Math::deltaphi(o[i].getFourVector().phi(),-M_PI/2)>0.5))
            passesSingleMuonTrigger = true;

//    bool passesDiTauTrigger = 
//	( (e.numberOfObjects("tau",ptThreshold0,1.0)>=2) );
      bool passesSinglePhotonTrigger = (e.numberOfObjects("ph",
                                                          (level<=5 ? 55 : 60),
                                                          1.0)>=1);
      bool passesDiPhotonTrigger =    (e.numberOfObjects("ph", ptThreshold1, 2.5)>=2);
      bool passesPhotonLeptonTrigger = ( ( (e.numberOfObjects("ph", ptThreshold1, 2.5)>=1) &&
                                           (e.numberOfObjects("e",ptThreshold0,1.0)+
                                            e.numberOfObjects("mu",ptThreshold0,1.0)
                                            // +e.numberOfObjects("tau",ptThreshold1,1.0)
                                              )>=1) );
//    bool passesPhotonBTrigger   = ( (e.numberOfObjects("ph", ptThreshold2, 1.0)>=1) &&
//				      (e.numberOfObjects("b",  ptThreshold0, 1.0)>=1) );
      bool passesCentralLeptonBTrigger   = ( ( (e.numberOfObjects("e", ptThreshold1, 1.0)+
                                                e.numberOfObjects("mu", ptThreshold1, 1.0)) >= 1 ) &&
                                             (e.numberOfObjects("b", ptThreshold0, 1.0)>=1) );
//    bool passesPlugElectronBTrigger  = ( (e.numberOfObjects("e", ptThreshold1, 2.5)>=1) &&
//					   (e.numberOfObjects("b", ptThreshold1, 1.0)>=1) );
      bool passesPlugElectronCentralTauTrigger  = ( (e.numberOfObjects("e", ptThreshold2, 2.5)>=1) &&
                                                    (e.numberOfObjects("tau", ptThreshold0, 1.0)>=1) );
      bool passesSingleJetTrigger = ((e.numberOfObjects("j", (level<=5 ? 150 : 200),1.0)+
                                      e.numberOfObjects("b", (level<=5 ? 150 : 200),1.0))
                                     >=1);
      bool passesSinglePlugPhotonTrigger = (e.numberOfObjects("ph", (level<=5 ? 250 : 300),2.5) >= 1 );

//    bool passesPlugPhotonBTrigger = ( (e.numberOfObjects("ph", ptThreshold2, 2.5) >= 1) &&
//					( (e.numberOfObjects("b",ptThreshold0,1.0)>=1) ) );

      bool passesCentralPhotonTauTrigger = ( (e.numberOfObjects("ph", ptThreshold2, 1.0)>=1) &&
                                             (e.numberOfObjects("tau",ptThreshold2, 1.0)>=1) );

      bool passesPlugPhotonTauTrigger = ( (e.numberOfObjects("ph", ptThreshold2, 2.5) >= 1) &&
                                          (e.numberOfObjects("tau",ptThreshold2,1.0)>=1) );
      bool passesPhoton_diTau_Trigger = ( (e.numberOfObjects("ph",ptThreshold2,2.5) >= 1) &&
                                          (e.numberOfObjects("tau",ptThreshold0,1.0)>=2) );
      bool passesPhoton_diB_Trigger = ( (e.numberOfObjects("ph",ptThreshold2,1.0) >= 1) &&
                                        (e.numberOfObjects("b",ptThreshold1,1.0)>=2) );
      bool passesPhoton_tauB_Trigger = ( (e.numberOfObjects("ph",ptThreshold2,2.5) >= 1) &&
                                         (e.numberOfObjects("b",ptThreshold1,1.0)>=1) &&
                                         (e.numberOfObjects("tau",ptThreshold1,1.0)>=1) );

      bool passesPrescaledJet25Trigger = ((e.numberOfObjects("j",40,1.0)+
                                           e.numberOfObjects("b",40,1.0) >= 1)&&
                                          (e.getEventType()!="data")&&            // labeled as "jet25" instead
                                          (e.getEventType()!="sig")&&
                                          ((eventType!="jet25")||(rn.substr(rn.length()-1)=="0"))); // take only every tenth event from jet25
      bool passesPrescaledJet25TauTrigger = ((e.numberOfObjects("j",40,1.0)+
                                              e.numberOfObjects("b",40,1.0) >= 1)&&
                                             (e.numberOfObjects("tau",ptThreshold0,1.0)>=1)&&
                                             (e.getEventType()!="data")&&            // labeled as "jet25" instead
                                             (e.getEventType()!="sig"));
      bool passesPrescaledJet25jetBTrigger = ((e.numberOfObjects("j",ptThreshold3,1.0)>=1)&&
                                              (e.numberOfObjects("b",ptThreshold1,1.0)>=1)&&
                                              (e.getEventType()!="data")&&            // labeled as "jet25" instead
                                              (e.getEventType()!="sig"));
      bool passesPrescaledJet25BTrigger = ((e.numberOfObjects("b",ptThreshold3,1.0)>=1)&&
                                           (e.getEventType()!="data")&&            // labeled as "jet25" instead
                                           (e.getEventType()!="sig"));


      bool passesTrigger = 
         ( passesSingleCentralElectronTrigger ||
           passesSinglePlugElectronTrigger ||
           passesDiEmObjectTrigger ||
           passesSingleMuonTrigger ||
           passesDileptonTrigger ||
           // passesDiTauTrigger ||
           passesSinglePhotonTrigger ||
           passesSinglePlugPhotonTrigger ||
           passesDiPhotonTrigger ||
           passesPhotonLeptonTrigger ||
           // passesPhotonBTrigger ||
           passesCentralLeptonBTrigger ||
           // passesPlugElectronBTrigger ||
           passesPlugElectronCentralTauTrigger ||
           passesSingleJetTrigger ||
           passesPrescaledJet25Trigger ||
           passesPrescaledJet25TauTrigger ||
           passesPrescaledJet25jetBTrigger ||
           passesPrescaledJet25BTrigger ||
           /*
             passesPrescaledSingleCentralElectronTrigger ||
             passesPrescaledSingleCentralMuonTrigger ||
             passesPrescaledSinglePlugElectronTrigger ||
             passesPrescaledSingleCentralPhotonTrigger ||
           */
           // passesPlugPhotonBTrigger ||
           passesCentralPhotonTauTrigger ||
           passesPlugPhotonTauTrigger ||
           passesPhoton_diTau_Trigger ||
           passesPhoton_diB_Trigger ||
           passesPhoton_tauB_Trigger
            );

      if((e.getEventType()=="sig")&&
         (!passesTrigger)&&
         (e.numberOfObjects("j",40,1.0)+e.numberOfObjects("b",40,1.0) >= 1))
      {
         if(level>=10)
            wt *= 0.00012;        // jet25 prescale
         passesTrigger = true;
      }

      if(level>=5)
         if((!passesTrigger)||
            (disallowedFinalState))
            ans = false;
   }
  

   if((collider=="tev2")&&
      ((experiment=="cdf")))
   {

      string eventType = e.getEventType();

      if( (zvtx==0) && 
          ( ( (eventType.length()>=6) &&
              (eventType.substr(0,6)=="cosmic") ) ||
            (eventType=="sig") ) )
      {
         while((zvtx==0)||(fabs(zvtx)>60))
            zvtx = Math::gasdev(0,30.);         // units are cm
         for(size_t i=0; i<o.size(); i++)
         {
            double newObjectEta = QuaeroRecoObject::getEventEta("cdf", o[i].getObjectType(), 
                                                                Math::theta2eta(o[i].getFourVector().theta()), zvtx);
            double newObjectPt = o[i].getFourVector().e()*fabs(sin(Math::eta2theta(newObjectEta)));
            o[i] = 
               QuaeroRecoObject(o[i].getObjectType(),
                                QuaeroRecoObject::setLorentzVectorMPtEtaPhi(o[i].getFourVector().m(), 
                                                                            newObjectPt,
                                                                            newObjectEta,
                                                                            o[i].getFourVector().phi()));

         }
         e.setObjects(o);
      }
      else
      {
         if((e.getZVtx()==0)||
            (fabs(e.getZVtx())>60))
            return(false); // does not contain a legal ZVertex
      }

      //remove Nj_sumPt400+ events where j2 has pT < 75, because they are regular QCD events that coincide with a cosmic.  So, j2 is prompt (and so is j3,j4 etc), j1 is cosmic, and typically j2 pT < 75 GeV.
      if(e.sumPt()>=400 &&
         e.numberOfObjects("j") >= 2 &&
         e.numberOfObjects("b")==0 &&
         e.numberOfObjects("e")==0 &&
         e.numberOfObjects("mu")==0 &&
         e.numberOfObjects("tau")==0 &&
         e.numberOfObjects("ph")==0 
         ) {
         if(e.getThisObject("j",2)->getFourVector().perp() < 75)
            return(false);
      }

      // Remove suspected cosmic rays
      bool failsCosmicCut = false;

      // Note events that have a kill object
      bool killed = false;
      for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectType()=="kill")
            killed = true;
	
      // Remove b's and tau's from the plug
      if((eventType=="data")||
         (eventType=="jet20")||
         (eventType=="sig"))	 
         for(size_t i=0; i<o.size(); i++)
            if(((o[i].getObjectTypeSansSign()=="b")||
                (o[i].getObjectTypeSansSign()=="tau"))&&
               //(fabs(QuaeroRecoObject::getDetectorEta("cdf", o[i].getObjectTypeSansSign(), Math::theta2eta(o[i].getFourVector().theta()), e.getZVtx()))>1.0)
               (fabs(Math::theta2eta(o[i].getFourVector().theta()))>1.0)
               )
               o[i] = QuaeroRecoObject("j",o[i].getFourVector());

      // Remove taus with |eta|<0.1
      // These taus tend to be misreconstructed electrons in our Monte Carlo samples
      /*
        for(size_t i=0; i<o.size(); i++)
        if((o[i].getObjectTypeSansSign()=="tau")&&
        (fabs(QuaeroRecoObject::getDetectorEta("cdf", o[i].getObjectTypeSansSign(), Math::theta2eta(o[i].getFourVector().theta()), e.getZVtx()))<0.1))
        o[i] = QuaeroRecoObject("j",o[i].getFourVector());
      */
      
      // If this is a mrenna or mad sample, then it was reconstructed with gen5, with different tau id.
      // The p(e->tau) rate should be doubled in the Z->ee samples, where one e fakes a tau.
      // This modification is motivated by a Vista 1e+1tau-[12]j:mass(e+,tau-) discrepancy.
      if(((eventType.length()>=11)&&
          (eventType.substr(0,11)=="mrenna_e+e-")) ||
         ((eventType.length()>=8)&&
          (eventType.substr(0,8)=="mad_e+e-")))
      {
         string electronCharge="";
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getObjectTypeSansSign()=="e")
               if(electronCharge=="")
                  electronCharge = o[i].getSign();
               else
                  electronCharge="moreThanOneElectron";
         if(electronCharge.length()==1)
            for(size_t i=0; i<o.size(); i++)
               if((o[i].getObjectTypeSansSign()=="tau")&&
                  (fabs(Math::theta2eta(o[i].getFourVector().theta()))<1.0)&&
                  (o[i].getFourVector().perp()>partitionRule.getPmin()))
                  if(o[i].getSign()!=electronCharge)  // the tau and electron have opposite charge, hence the tau could have come from the other electron
                     wt *= 2;
      }

      // Check if event has an electron and another EM object within deltaR < 0.2
      for(size_t i=0; i<o.size(); i++)
         if(o[i].getObjectTypeSansSign()=="e")
            for(size_t j=i+1; j<o.size(); j++)
               if((o[j].getObjectTypeSansSign()=="e")||
                  (o[j].getObjectTypeSansSign()=="ph"))
                  if(Math::deltaR(o[i].getFourVector().phi(), 
                                  Math::theta2eta(o[i].getFourVector().theta()), 
                                  o[j].getFourVector().phi(), 
                                  Math::theta2eta(o[j].getFourVector().theta())) < 0.2)
                  {
                     o[j] = QuaeroRecoObject("uncl",o[j].getFourVector());
                  }
      
      if(level>5)
      {
         // An electron with 1.1<|detEta|<1.2 has dubious charge
         string chargeOfWellDeterminedLepton = "";
         int iOfDubiousChargeElectron = -1;
         for(size_t i=0; i<o.size(); i++)
         {
            if((o[i].getObjectTypeSansSign()=="mu") // ||(o[i].getObjectTypeSansSign()=="tau")
               )
               chargeOfWellDeterminedLepton += o[i].getSign();
            else if(o[i].getObjectTypeSansSign()=="e")
            {
               double detEta = QuaeroRecoObject::getDetectorEta("cdf", "e", o[i].getFourVector().pseudoRapidity(), zvtx);
               if((fabs(detEta)>1.1)&&(fabs(detEta)<1.2))
                  iOfDubiousChargeElectron=i;
               else
                  chargeOfWellDeterminedLepton += o[i].getSign();
            }
         }
         if((iOfDubiousChargeElectron>=0)&&
            (chargeOfWellDeterminedLepton==o[iOfDubiousChargeElectron].getSign()))
            o[iOfDubiousChargeElectron].chargeConjugate();

         // Fix sign of plug electrons
         string centralElectronIfPresent = "";
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="e")&&
               (fabs(Math::theta2eta(o[i].getFourVector().theta()))<0.9))
               centralElectronIfPresent = o[i].getObjectType();
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="e")&&
               (fabs(Math::theta2eta(o[i].getFourVector().theta()))>0.9))
               if(centralElectronIfPresent==o[i].getObjectType())
                  o[i].chargeConjugate();
         // Two plug electrons with the same sign should have opposite sign
         string plugElectronIfPresent = "";
         //	  double pTofHighestPtPlugElectron = 0;
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="e")&&
               (fabs(Math::theta2eta(o[i].getFourVector().theta()))>0.9))
               for(size_t j=i+1; j<o.size(); j++)
                  if((o[j].getObjectTypeSansSign()=="e")&&
                     (fabs(Math::theta2eta(o[j].getFourVector().theta()))>0.9)&&
                     (o[i].getObjectType()==o[j].getObjectType()))
                     if(o[i].getFourVector().perp()<o[j].getFourVector().perp())
                        o[i].chargeConjugate();
                     else
                        o[j].chargeConjugate();

         // Check if event has lepton with deltaR < 0.75 from nearest jet
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="e")||
               (o[i].getObjectTypeSansSign()=="tau")||
               (o[i].getObjectTypeSansSign()=="mu"))
               for(size_t j=0; j<o.size(); j++)
                  if(j!=i)
                     if(((o[j].getObjectTypeSansSign()=="j")
                         ||(o[j].getObjectTypeSansSign()=="b")
                         ||(o[j].getObjectTypeSansSign()=="tau")
                           )&&
                        (o[j].getFourVector().perp()>partitionRule.getPmin())&&
                        (fabs(Math::theta2eta(o[j].getFourVector().theta()))<2.5))
                        if(Math::deltaR(o[i].getFourVector().phi(), 
                                        Math::theta2eta(o[i].getFourVector().theta()), 
                                        o[j].getFourVector().phi(), 
                                        Math::theta2eta(o[j].getFourVector().theta())) < 0.75)
                           return(false); // contains lepton too close to a jet
	  
         // Correct for triggerEfficiencies between CMUP and CMX
         if((eventType!="data")&&
            (eventType!="jet20"))
         {
            string locationOfSingleTriggerMuon = "";
            for(size_t i=0; i<o.size(); i++)
               if((o[i].getObjectTypeSansSign()=="e")||
                  (o[i].getObjectTypeSansSign()=="tau")||
                  (o[i].getObjectTypeSansSign()=="ph")||
                  ((o[i].getObjectTypeSansSign()=="mu")&&
                   (locationOfSingleTriggerMuon!="")))
               {
                  locationOfSingleTriggerMuon="thereIsAnotherTriggerObject";
                  break;
               }
               else if(o[i].getObjectTypeSansSign()=="mu")
                  if(fabs(Math::theta2eta(o[i].getFourVector().theta()))<0.6)
                     locationOfSingleTriggerMuon=="CMUP";
                  else
                     locationOfSingleTriggerMuon=="CMX";
            if(locationOfSingleTriggerMuon=="CMUP")
               wt *= 0.98; // CMUP trigger efficiency is 2% less than "average" muon efficiency
            if(locationOfSingleTriggerMuon=="CMX")
               wt *= 1.02; // CMX trigger efficiency is 2% higher than "average" muon efficiency
         }

         // Impose Bayesian interpretation on muon momentum
         for(size_t i=0; i<o.size(); i++)
            if((o[i].getObjectTypeSansSign()=="mu"))
            {
               double phat = o[i].getFourVector().perp();
               double p = phat;
               double sigma = 0.002;
               if(p<200) sigma=(phat-60)/140*0.002;  // this (and the next line, the if statement) is hack in order to not run into trouble at trigger thresholds
               if(sigma>0.001)
                  p = (-1/phat + sqrt(1/(phat*phat) + 8 * (sigma*sigma)))/(4*(sigma*sigma));
               o[i] = QuaeroRecoObject(o[i].getObjectType(),o[i].getFourVector() * (p/phat) );
            }
      }

      // Any object with (|eta| > 2.5) || (pT < 15) is converted to unclustered energy
      // Option:  change to pT < 10 GeV for investigating low-pT leptons
      if(!((hintSpec.collider=="tev2")&&
           (hintSpec.experiment=="cdf")&&
           (hintSpec.finalState=="lowPtDileptons")))
         for(size_t i=0; i<o.size(); i++)
            if( (fabs(Math::theta2eta(o[i].getFourVector().theta()))>2.5) ||
                (o[i].getFourVector().perp()<15) ) 
               o[i] = QuaeroRecoObject("uncl",o[i].getFourVector());
      CLHEP::HepLorentzVector uncl = CLHEP::HepLorentzVector(0,0,0,0);
      for(size_t i=o.size()-1; ; i--) {
         if(o[i].getObjectType()=="uncl")
         {
            uncl = uncl + o[i].getFourVector();
            o.erase(o.begin()+i);
         }
         if( i==0 ) break;
      }
      if(uncl.perp()>0)
         o.push_back(QuaeroRecoObject("uncl",uncl));

      double highJetPtThreshold = 200; // units are GeV
      bool usingChargedTrackJets = false;
      if(usingChargedTrackJets)
         highJetPtThreshold = 100; // units are GeV

      if(level>=5) 
      {	     
         if((level>=10)&&
            ((eventType=="sig")||
             (usingChargedTrackJets)))
         {
            // Cluster events passing SingleJetTrigger to a significantly higher scale (say 50 GeV)
            double kTscale = 15;
            bool lowPtJets = false;
            bool highPtJets = false;
            if((e.numberOfObjects("j",highJetPtThreshold,1.0)+
                e.numberOfObjects("b",highJetPtThreshold,1.0) >= 1)||
               (usingChargedTrackJets))
            {
               kTscale = 50;
               highPtJets = true;
            }
            else if(e.numberOfObjects("e",25,1)+
                    e.numberOfObjects("mu",25,1)+
                    e.numberOfObjects("ph",25,2.5)==0)
               lowPtJets = true;
	      
            if(highPtJets)
            {
               int k1=0,k2=0;
               double minDeltaRjj = 0;
               while(minDeltaRjj<0.7)
               {
                  k1=k2=-1;
                  minDeltaRjj=0.7;
                  for(size_t i=0; i<o.size(); i++)
                     if((o[i].getObjectType()=="j") ||
                        (o[i].getObjectType()=="b"))
                        for(size_t j=i+1; j<o.size(); j++)
                           if((o[j].getObjectType()=="j") ||
                              (o[j].getObjectType()=="b"))
                           {
                              double deltaRjj = Math::deltaR(o[i].getFourVector().phi(), Math::theta2eta(o[i].getFourVector().theta()), 
                                                             o[j].getFourVector().phi(), Math::theta2eta(o[j].getFourVector().theta()));
                              if(deltaRjj < minDeltaRjj)
                              {
                                 minDeltaRjj = deltaRjj;
                                 k1=i; k2=j;
                              }
                           }
                  if((k1>=0)&&(k2>=0))
                  {
                     o[k1] = QuaeroRecoObject("j",o[k1].getFourVector()+o[k2].getFourVector());
                     o.erase(o.begin()+k2);
                  }
               }
            }
	      
            int k1=0,k2=0;
            while((k1>=0)&&(k2>=0))
            {
               k1=k2=-1;
               int nj=0;
               for(size_t i=0; i<o.size(); i++)
                  if(o[i].getObjectType()=="j")
                     nj++;
               // Find the highest pt jet
               bool noIndex = true;
               size_t highestPtJet = 0;
               double highestJetPt = 0;
               for(size_t i=0; i<o.size(); i++)
                  if((o[i].getObjectType()=="j")||
                     (o[i].getObjectType()=="b"))
                     if(o[i].getFourVector().perp()>highestJetPt)
                     {
                        highestJetPt = o[i].getFourVector().perp();
                        highestPtJet = i;
                        noIndex = false;
                     }
		  
               double m_cut = (((nj>4)&&(!lowPtJets)) ? FLT_MAX : kTscale); 
               double mij_min = m_cut;
               for(size_t i=0; i<o.size(); i++)
                  if( noIndex || i!=highestPtJet)
                     if((o[i].getObjectType()=="j") ||
                        (o[i].getObjectType()=="b") ||
                        (o[i].getObjectTypeSansSign()=="tau"))
                        for(size_t j=i+1; j<o.size(); j++)
                           if( noIndex || j!=highestPtJet)
                              if((o[j].getObjectType()=="j") ||
                                 (o[j].getObjectType()=="b") ||
                                 (o[i].getObjectTypeSansSign()=="tau"))
                              {
                                 double mij = (o[i].getFourVector()+o[j].getFourVector()).m();
                                 if(mij < mij_min)
                                 {
                                    mij_min = mij;
                                    k1=i; k2=j;
                                 }
                              }
               if((k1>=0)&&(k2>=0))
               {
                  o[k1] = QuaeroRecoObject("j",o[k1].getFourVector()+o[k2].getFourVector());
                  o.erase(o.begin()+k2);
               }
            }
         }
      }

      // Flag events that contain a plug objects that has been counted twice: as both a photon and an electron
      // This is a reconstruction screw-up
      {
         int iph = -1;
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getObjectType()=="ph")
               if(iph==-1)
                  iph = i;
               else
                  iph = -2;
         if(iph>=0)
         {	      
            CLHEP::HepLorentzVector ph = o[iph].getFourVector();
            bool killThePhoton = false;
            for(size_t i=0; i<o.size(); i++)
               if(o[i].getObjectTypeSansSign()=="e")
               {
                  CLHEP::HepLorentzVector ele = o[i].getFourVector();
                  if(((fabs(Math::theta2eta(ele.theta()))>1)||
                      (fabs(Math::theta2eta(ph.theta()))>1))&&
                     (Math::deltaR(ele.phi(),ele.theta(),
                                   ph.phi(),ph.theta())<1.8)&&
                     ((fabs(ele.e()-ph.e())<5)||
                      ((fabs(ele.e()-ph.e())<10)&&
                       (fabs(max(fabs(Math::theta2eta(ele.theta())),
                                 fabs(Math::theta2eta(ph.theta())))-1.2)<0.1))))
                     killThePhoton = true;
               }
            if(killThePhoton)
            {
               //o.erase(o.begin()+iph);
               o[iph] = QuaeroRecoObject("uncl",o[iph].getFourVector());
            }
         }
      }

      
      e.setObjects(o);
      e.reVertex(zvtx);
      fs = partitionRule.getFinalState(e).getTextString();
      string rn = e.getRunNumber();
      string::size_type pos = rn.find (".",0);   // find the dot
      string runNumberString = rn.substr(0,pos); // keep only the run number string.
      //      int runNumber = atoi(runNumberString.c_str());
   
      if(level>=5)
      {
         bool disallowedFinalState = false;
         if((fs=="1j_sumPt0-400")||
            (fs=="1j1pmiss_sumPt0-400")||
            (fs=="1b_sumPt0-400")||
            (fs=="1b1pmiss_sumPt0-400")||
            (fs=="1b_sumPt400+"))
            disallowedFinalState = true;        // cut out low-pt 1j and 1j1pmiss final states
         if((fs=="1e+")||(fs=="1e-")||
            (fs=="1mu+")||(fs=="1mu-"))
            disallowedFinalState = true;        // cut out 1e and 1mu final states
         if((fs=="1tau+")||(fs=="1tau-")||
            (fs=="1pmiss1tau+")||(fs=="1pmiss1tau-"))
            disallowedFinalState = true;        // cut out 1tau final states
         if((fs=="1ph"))
            disallowedFinalState = true;        // cut out 1ph final state	  
         if((fs=="1pmiss"))
            disallowedFinalState = true;        // cut out 1pmiss final state

         if(disallowedFinalState)
            return(false);

         /* The CDF silicon detector was not as efficient at identifying bottom quarks early in Run II.
            The jet20 trigger had a lower prescale early in Run II.
            The sumPt0-400 final states with identified bottom quarks thus need to have reduced SM prediction. */
         if((e.getEventType()!="data")&&
            (e.getEventType()!="jet20")&&
            ((fs=="1b1j_sumPt0-400")||
             (fs=="1b2j_sumPt0-400")||
             (fs=="1b3j_sumPt0-400")||
             (fs=="1b4j_sumPt0-400")||
             // In early runs we did not have as good b-tagging *nor* as good phoenix electron id, 
             // so we take a hit for this in the first ~150 pb^-1
             ((e.numberOfObjects("e",15,2.5)-
               e.numberOfObjects("e",15,1.0)>=1)&&
              (e.numberOfObjects("b",15,1.0)>=1))
               ))
            wt *= 0.98;
      }

      bool jet20DuplicateEvent = false;
      if(e.getEventType()=="jet20")
      {
         if(rn=="179056.3633137")
            jet20DuplicateEvent = true;
      }


      double ptThreshold0 = 10, ptThreshold01 = 15, ptThreshold1 = 20, ptThreshold12 = 25, ptThreshold2 = 35, ptThreshold3 = 50, ptThreshold4 = 75;
      if(level>=6)
      {
         ptThreshold0 = 17;
         ptThreshold01 = 20;
         ptThreshold1 = 25;
         ptThreshold12 = 30;
         ptThreshold2 = 40;
         ptThreshold3 = 60;
         ptThreshold4 = 90;
      }

      bool passesTrigger = false;
      if((e.numberOfObjects("e")>=1))
      {
         if(
            ( (e.numberOfObjects("e",ptThreshold1,1.0)>=1)
              // &&((eventType!="data")||((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) 
               ) ||      // passesSingleCentralElectron25Trigger
	     
            (e.numberOfObjects("e",ptThreshold2,1.0)>=1) ||      // passesSingleCentralElectron40Trigger
	     
            ( (e.numberOfObjects("e",ptThreshold1,1.0)>=1) &&
              (e.getPmiss().perp()>ptThreshold0) &&
              (e.numberOfObjects("j",ptThreshold0,2.5)+
               e.numberOfObjects("b",ptThreshold0,1.0)>=2) )    ||    // passesSingleCentralElectronWjetsTrigger
	     
            ( (e.numberOfObjects("e",ptThreshold2,2.5)>=1)            // passesSinglePlugElectron40Trigger
              // && ((eventType!="data")||((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) 
               ) ||

            (e.numberOfObjects("e",ptThreshold3,2.5)>=1) ||                // passesSinglePlugElectron60Trigger
	     
            ( (e.numberOfObjects("e",ptThreshold2,2.5)>=1) &&
              (e.getPmiss().perp()>ptThreshold0) &&
              (e.numberOfObjects("j",ptThreshold0,2.5)+
               e.numberOfObjects("b",ptThreshold0,1.0)>=2) )    ||    // passesSinglePlugElectronWjetsTrigger

            /*
              ((e.numberOfObjects("e",ptThreshold12,2.5)-
              e.numberOfObjects("e",ptThreshold12,1.0)>=1)&&
              ((eventType!="data")||
              ((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) ) ||   // passesDoublyPrescaledSinglePlugElectronTrigger
            */

            ( ( (e.numberOfObjects("e",ptThreshold1,2.5)+
                 e.numberOfObjects("ph",ptThreshold1,2.5)) >=2 ) &&
              ( e.numberOfObjects("e",ptThreshold1,2.5) >= 1 ) ) ||   // passesDiEmObjectTrigger with electron
	     
            ( (e.numberOfObjects("e",ptThreshold01,1.0)>=1)
              && ((eventType!="data")||((rn.length()>2)&&(rn.substr(rn.length()-2)=="00"))) 
               ) ||     // passesDoublyPrescaledSingleCentralElectron20Trigger
	     
            ( (e.numberOfObjects("ph", ptThreshold1, 2.5)>=1) &&
              (e.numberOfObjects("e",ptThreshold1,1.0)>=1) )   ||      // passesCentralElectronPlusPhotonTrigger
      
            ( (e.numberOfObjects("e", ptThreshold1, 1.0)>=1) &&
              (e.numberOfObjects("b", ptThreshold0, 1.0)>=1) ) ||       //  passesCentralElectronBTrigger
            /*
              ( (e.numberOfObjects("e", ptThreshold1, 2.5)>=1) &&
              (e.numberOfObjects("b", ptThreshold1, 1.0)>=1) ) ||  // passesPlugElectronBTrigger
            */
            ( (e.numberOfObjects("e", ptThreshold2, 2.5)>=1) &&
              (e.numberOfObjects("tau", ptThreshold0, 1.0)>=1) )    // passesPlugElectronCentralTauTrigger  

            )
            passesTrigger = true;
      }
      if((!passesTrigger)&&(e.numberOfObjects("mu")>=1))
      {
         if(
            (e.numberOfObjects("mu",ptThreshold2,1.0)>=1) ||                   // passesSingleCentralMuon40Trigger
	     
            ( (e.numberOfObjects("mu",ptThreshold1,1.0)>=1)
              // && ((eventType!="data")||((rn.length()>1)&&(rn.substr(rn.length()-1)=="0"))) 
               )  ||       // passesSingleCentralMuon25Trigger
	     
            ( (e.numberOfObjects("mu",ptThreshold01,1.0)>=1)
              && ((eventType!="data")||((rn.length()>2)&&(rn.substr(rn.length()-2)=="00"))) 
               )  ||       // passesDoublyPrescaledSingleCentralMuon20Trigger
	     
            ( (e.numberOfObjects("mu",ptThreshold1,1.0)>=1) &&
              (e.getPmiss().perp()>ptThreshold0) &&
              (e.numberOfObjects("j",ptThreshold0,2.5)+
               e.numberOfObjects("b",ptThreshold0,1.0)>=2) )    ||    // passesSingleCentralMuonWjetsTrigger
	     
            ( (e.numberOfObjects("ph", ptThreshold1, 2.5)>=1) &&
              (e.numberOfObjects("mu",ptThreshold0,1.0)>=1) )      ||           // passesMuonPhotonTrigger

            ( (e.numberOfObjects("mu", ptThreshold1, 1.0)>=1) &&
              (e.numberOfObjects("b", ptThreshold0, 1.0)>=1) )        // passesCentralMuonBTrigger

            )
            passesTrigger = true;
      }
      if((!passesTrigger)&&(e.numberOfObjects("tau")>=1))
      {
         if(e.numberOfObjects("tau",ptThreshold1,1.0)>=2)               // passesDiTauTrigger
         {
            int numberOfTausInCentralDetector=0;
            for(size_t j=0; j<o.size(); j++)
               if((o[j].getObjectTypeSansSign()=="tau")&&
                  (fabs(QuaeroRecoObject::getDetectorEta("cdf", "tau", Math::theta2eta(o[j].getFourVector().theta()), e.getZVtx()))<1))
                  numberOfTausInCentralDetector++;
            if(numberOfTausInCentralDetector>=2)   // need to have two taus with |detEta|<1, in addition to |eta|<1.
               passesTrigger=true;
         }

         if(
            ( (e.numberOfObjects("ph", ptThreshold2, 1.0)>=1) &&
              (e.numberOfObjects("tau",ptThreshold2, 1.0)>=1) ) ||   // passesCentralPhotonTauTrigger

            ( (e.numberOfObjects("ph", ptThreshold2, 2.5) >= 1) &&
              (e.numberOfObjects("tau",ptThreshold2,1.0)>=1) )  ||  // passesPlugPhotonTauTrigger 

            ( (e.numberOfObjects("ph",ptThreshold2,2.5) >= 1) &&
              (e.numberOfObjects("tau",ptThreshold0,1.0)>=2) ) ||  // passesPhoton_diTau_Trigger

            ( (e.numberOfObjects("ph",ptThreshold2,2.5) >= 1) &&
              (e.numberOfObjects("b",ptThreshold1,1.0)>=1) &&
              (e.numberOfObjects("tau",ptThreshold1,1.0)>=1) )      // passesTauPhotonBTrigger

            )
            passesTrigger = true;
      }
      if((!passesTrigger)&&(e.numberOfObjects("ph", ptThreshold1)>=1))
      {
         if(
	     
            //( (e.numberOfObjects("ph",ptThreshold1,1.0)>=1) &&
            //  ((eventType!="data")||                                        // passesPrescaledSingleCentralPhotonTrigger
            //	((rn.length()>1)&&(rn.substr(rn.length()-2)=="00"))) ) ||    // We use this trigger to allow us to get a good handle of j->ph fakes between 25 and 60 GeV
	     
	     
            (e.numberOfObjects("ph",(level<=5 ? 55 : 60),1.0)>=1)  ||     // passesSinglePhotonTrigger
	     
            ( (e.numberOfObjects("ph", ptThreshold1, 2.5)>=2) &&
              (e.numberOfObjects("ph", ptThreshold1, 1.0)>=1) ) ||       // passesDiPhotonTrigger
            /*
              ( (e.numberOfObjects("ph", ptThreshold2, 1.0)>=1) &&
              (e.numberOfObjects("b",  ptThreshold0, 1.0)>=1) )  ||    // passesPhotonBTrigger 
            */
            (e.numberOfObjects("ph", (level<=5 ? 250 : 300),2.5) >= 1 ) ||  // passesSinglePlugPhotonTrigger
            /*
              ( (e.numberOfObjects("ph", ptThreshold2, 2.5) >= 1) &&
              (e.numberOfObjects("b",ptThreshold0,1.0)>=1) ) ||  // passesPlugPhotonBTrigger
            */
            ( (e.numberOfObjects("ph",ptThreshold2,1.0) >= 1) &&
              (e.numberOfObjects("b",ptThreshold1,1.0)>=2) )   // passesPhoton_diB_Trigger

            )
            passesTrigger = true;	    
      }
      if((!passesTrigger)&&(e.numberOfObjects("b", ptThreshold2, 1.0)>=2))
      {
         if(
            /*
              ( (e.numberOfObjects("b", ptThreshold4, 1.0)>=2) ) ||     // passesDiBjetTrigger 
            */

            ( (e.numberOfObjects("b", ptThreshold3, 1.0)>=3) &&
              (e.numberOfObjects("b", ptThreshold4, 1.0)>=1) )        // passesTriBjetTrigger

            )
            passesTrigger = true;
      }
      if((!passesTrigger))
      {
         if(
            ((e.numberOfObjects("j", (level<=5 ? 150 : highJetPtThreshold),1.0)+
              e.numberOfObjects("b", (level<=5 ? 150 : highJetPtThreshold),1.0))>=1) || // passesSingleJetTrigger

            ((e.numberOfObjects("j",40,1.0)+e.numberOfObjects("b",40,1.0) >= 1)&&
             (eventType!="data")&&            // labeled as "jet20" instead
             (eventType!="sig")&&
             ((eventType!="jet20")||(rn.substr(rn.length()-1)=="0")) // take only every tenth event from jet20
               ) ||                         // passesPrescaledJet20Trigger
 
            ((e.numberOfObjects("j",40,1.0)+e.numberOfObjects("b",40,1.0) >= 1)&&
             (e.numberOfObjects("tau",ptThreshold0,1.0)>=1)&&
             (eventType!="data")&&            // labeled as "jet20" instead
             (eventType!="sig"))  ||                          // passesPrescaledJet20TauTrigger
        
            ((e.numberOfObjects("j",ptThreshold3,1.0)>=1)&&
             (e.numberOfObjects("b",ptThreshold1,1.0)>=1)&&
             (eventType!="data")&&            // labeled as "jet20" instead
             (eventType!="sig"))  ||                          // passesPrescaledJet20jetBTrigger
      
            ((e.numberOfObjects("b",ptThreshold3,1.0)>=1)&&
             (eventType!="data")&&            // labeled as "jet20" instead
             (eventType!="sig"))                   // passesPrescaledJet20BTrigger
            )
            passesTrigger = true;
      }
      if(!passesTrigger)
      {
         int cem8 = e.numberOfObjects("e",ptThreshold0,1.0);
         int cem20 = e.numberOfObjects("e",ptThreshold01,1.0);
         int pem8 = e.numberOfObjects("e",ptThreshold1,2.5)-e.numberOfObjects("e",ptThreshold1,1.0);
         int muo8 = e.numberOfObjects("mu",ptThreshold0,1.0);
         int muo20 = e.numberOfObjects("mu",ptThreshold01,1.0);
         int trk8 = e.numberOfObjects("tau",ptThreshold0,1.0);
         bool passesDileptonTrigger = 
            ( // (cem8>=2) ||   
               (muo8>=2) || 
               ((muo8>=1)&&(e.numberOfObjects("mu",ptThreshold0,2.5)>=2)) ||
               // (cem8 && pem8) || 
               (cem8 && muo8) || 
               (cem20 && trk8) ||
               (pem8 && muo8) || 
               (muo20 && trk8) ||
               ((hintSpec.collider=="tev2")&&
                (hintSpec.experiment=="cdf")&&
                (hintSpec.finalState=="lowPtDileptons")&&
                (((e.numberOfObjects("mu",4,0.6)>=1)&&
                  (e.numberOfObjects("mu",4,1.0)>=2))||
                 (e.numberOfObjects("e",4,1.0)>=2)))
               );
         if(passesDileptonTrigger)
            passesTrigger = true;
      }

      if((eventType=="sig")&&
         (!passesTrigger)&&
         (e.numberOfObjects("j",40,1.0)+e.numberOfObjects("b",40,1.0) >= 1))
      {
         if(level>=10)
            wt *= 0.00012;        // jet20 prescale
         passesTrigger = true;
      }

      // Cosmic processes cannot produce b jets
      bool thisIsAnUnintendedUseOfThisGeneratedProcess = false;	  
      if( ( ( (eventType.length()>=6) &&
              (eventType.substr(0,6)=="cosmic") ) ) )
         for(size_t i=0; i<o.size(); i++)
            if(o[i].getObjectType()=="b")
               thisIsAnUnintendedUseOfThisGeneratedProcess = true;
      if( ( (eventType=="mrenna_e+e-")||
            (eventType=="mrenna_e+e-j")||
            (eventType=="mad_e+e-") ) &&
          (fs=="1b1e+") )
         thisIsAnUnintendedUseOfThisGeneratedProcess = true;
      // Our reconstruction can reconstruct a mrenna_e+e- Monte Carlo event as containing an electron and a jet.
      // Our misReconstruction can than call that jet a tau by applying p(j->tau).
      // In the data, this does not happen.  
      // This contribution to our background estimate in the 1e1tauX final states must therefore be removed.
      if( ( (eventType=="mrenna_e+e-")||
            (eventType=="mrenna_e+e-j")||
            (eventType=="mad_e+e-") ) &&
          ((fs=="1e+1tau+") ||
           (fs=="1e+1tau-"))
         )
         thisIsAnUnintendedUseOfThisGeneratedProcess = true;
      // Pythia W->lnu should not fake 1l1pmiss1tau+.
      // This should be handled by MadEvent W->lnu (+1 or more reconstructed jets)
      if( ( (eventType=="sewkad")||        // Pythia W->enu
            (eventType=="wewk7m")||        // Pythia W->munu
            (eventType=="wexo0m")||
            (eventType=="we0s8m") ) &&
          ( ( (fs.find("1e")!=string::npos) ||
              (fs.find("1mu")!=string::npos) ) &&
            (fs.find("1tau")!=string::npos) ) )
         thisIsAnUnintendedUseOfThisGeneratedProcess = true;
      // zx0sd[em] should not be used for m(ll)>20 GeV
      if( (eventType=="zx0sde") && // Pythia Z->ee, m>10 GeV
          ( (e.numberOfObjects("e+")==0) ||
            (e.numberOfObjects("e-")==0) ||
            ( (e.getThisObject("e+")->getFourVector()+e.getThisObject("e-")->getFourVector()).m() > 20) // units are GeV
             ) )
         thisIsAnUnintendedUseOfThisGeneratedProcess = true;
      if( (eventType=="zx0sdm") && // Pythia Z->mumu, m>10 GeV
          ( (e.numberOfObjects("mu+")==0) ||
            (e.numberOfObjects("mu-")==0) ||
            ( (e.getThisObject("mu+")->getFourVector()+e.getThisObject("mu-")->getFourVector()).m() > 20) // units are GeV
             ) )
         thisIsAnUnintendedUseOfThisGeneratedProcess = true;

      if(level>=5)
         if(
            (!passesTrigger) || 
            failsCosmicCut ||
            thisIsAnUnintendedUseOfThisGeneratedProcess ||
            jet20DuplicateEvent ||
            killed
            )	  {
            ans = false;
            if (debug)
               cout << " Refined out because " <<
                  "[(!passesTrigger)||failsCosmicCut||thisIsAnUnintendedUseOfThisGeneratedProcess||jet20DuplicateEvent||killed]=[" <<
                  (!passesTrigger) <<"||"<<
                  failsCosmicCut <<"||"<<
                  thisIsAnUnintendedUseOfThisGeneratedProcess <<"||"<<
                  jet20DuplicateEvent <<"||"<<
                  killed << "]" << endl;
         }
   }

   if((collider=="lhc")&&
      ((experiment=="cms")))
   {
      if(o.size()==0) return(false);
      for(size_t i=o.size()-1; ; i--) {
         if(o[i].getObjectType()=="j" && o[i].getFourVector().perp() < 20.0 )
         {
            o.erase(o.begin()+i);
         }
         if( i==0 ) break;
      }


  
   }
  

   /*************************************************
                      Cleanup
   **************************************************/

   e.reWeight(wt);
   e.setObjects(o);
   e.reVertex(zvtx);



   /*************************************************
                         Hint
   **************************************************/
   if(hintSpec.active==true)
   {
      if(collider!=hintSpec.collider)
         ans = false;
      if(experiment!=hintSpec.experiment)
         ans = false;
      if(fs!=hintSpec.finalState)
      {
         if(((hintSpec.finalState=="1bb1w")||
             (hintSpec.finalState=="1bb1w+")||
             (hintSpec.finalState=="1bb1w-"))&&
            ((fs=="1b1e+1j1pmiss")||
             (fs=="1b1j1mu+1pmiss")
               ))
            ; // okay
         if((fs.length()>hintSpec.finalState.length())&&
            (hintSpec.finalState==fs.substr(0,hintSpec.finalState.length()))&&
            (fs.substr(hintSpec.finalState.length(),6)=="_sumPt"))
            ; // okay
         else
            ans = false;  // not okay
      }
      if(e.sumPt()<hintSpec.sumPtCut)
         ans = false;
   }

   return(ans);
}

void Refine::setMinWt(double _minWt)
{
   assert(_minWt>=0);
   minWt = _minWt;
   return;
}

bool Refine::passesMinWt(QuaeroEvent& e, double& runningWeight)
{


   bool passesSpecialCriteria = false;
   if((collider=="tev2")&&
      (experiment=="cdf"))
      passesSpecialCriteria = passesSpecialSleuthCriteriaQ(e);

   double minWt1 = minWt;
   if(passesSpecialCriteria)
      minWt1 = minWt/10;
  
   if(e.getWeight()>=minWt1)
      return(true);
   else
   {
      runningWeight += e.getWeight();
      if(((int)(runningWeight/minWt1)) > ((int)((runningWeight-e.getWeight())/minWt1)))
      {
         runningWeight -= minWt1;
         e.reWeight(minWt1);
         return(true);
      }
   }
   return(false);
}



bool Refine::passesSpecialSleuthCriteriaQ(const QuaeroEvent& e)
{
   bool passesSpecialCriteria = false;

   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5) >= 1) &&
      (e.numberOfObjects("j",15,2.5)+
       e.numberOfObjects("b",15,2.5) >=4 ))        
      passesSpecialCriteria = true;                 // top lepton + jets
   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5) >= 2) &&
      (e.numberOfObjects("j",15,2.5)+
       e.numberOfObjects("b",15,2.5) >=2 ))        
      passesSpecialCriteria = true;                 // top dilepton
   if((e.numberOfObjects("e",15,2.5) >= 1) &&
      (e.numberOfObjects("mu",15,2.5) >= 1))
      passesSpecialCriteria = true;                 // 1e1muX
   if((e.numberOfObjects("e",20,2.5) >= 1) &&
      (e.numberOfObjects("tau",20,2.5) >= 1))
      passesSpecialCriteria = true;                 // 1e1tauX
   if((e.numberOfObjects("tau",15,2.5) >= 1)&&
      (e.numberOfObjects("j",17,2.5) >= 2)&&
      (e.sumPt()>400))
      passesSpecialCriteria = true;                 // 1jj1tau
   if((e.numberOfObjects("tau",15,2.5) >= 1)&&
      (e.sumPt()>400))
      passesSpecialCriteria = true;                 // 1pmiss1tau
   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5)>=1)&&
      (e.numberOfObjects("ph",15,2.5)>=1)&&
      (e.numberOfObjects("j",15,2.5)>=3)&&
      (e.getPmiss().perp()>12) &&
      (e.sumPt() > 200))
      passesSpecialCriteria = true;                 // 1e+2jj1ph1pmiss
   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5)>=2)&&
      (e.numberOfObjects("b",15,1.0)>=1)&&
      (e.numberOfObjects("j",15,2.5)>=1)&&
      (e.getPmiss().perp()>12) &&
      (e.sumPt() > 200))
      passesSpecialCriteria = true;                 // 1e+1e-1bb1pmiss
   if((e.numberOfObjects("b",200,2.5)>=1)&&
      (e.getPmiss().perp()>50) &&
      (e.sumPt() > 700))
      passesSpecialCriteria = true;                 // 1bb1pmiss
   if((e.numberOfObjects("j",15,2.5)>=4)&&
      (e.getPmiss().perp()>50) &&
      (e.sumPt() > 800))
      passesSpecialCriteria = true;                 // 1bb1pmiss
   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5)>=1)&&
      (e.numberOfObjects("ph",15,2.5)>=2)&&
      (e.getPmiss().perp()>12))
      passesSpecialCriteria = true;                 // 1e+2ph1pmiss
   if(e.numberOfObjects("e+",15,2.5)+
      e.numberOfObjects("mu+",15,2.5)>=2)
      passesSpecialCriteria = true;                 // 2e+X
   if((e.numberOfObjects("e",15,2.5)+
       e.numberOfObjects("mu",15,2.5)>=3))
      passesSpecialCriteria = true;                 // trilepton

   return(passesSpecialCriteria);
}

