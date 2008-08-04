#ifndef __JetAna_Selector__
#define __JetAna_Selector__


#include <string>
#include <vector>
 
#include "boost/type_traits.hpp"
#include "boost/utility/enable_if.hpp"
 
#include "FWCore/Framework/interface/SelectorBase.h"
//#include "FWCore/Framework/interface/SelectorProvenance.h"
#include "DataFormats/Provenance/interface/Provenance.h"

#include <iostream>

namespace edm{
  class JetAnaSelector : public SelectorBase 
  {
  public:
    JetAnaSelector(){}
    JetAnaSelector(const std::vector<std::string>& mustHave, const std::vector<std::string>& noGo) :
      theMustHave(mustHave),theNoGo(noGo)
      { 
	theName="MUST_";
	for (std::vector<std::string>::const_iterator iter=theMustHave.begin();
	     iter!=theMustHave.end();++iter) theName+=(*iter);
	theName+="_NO_";
	for (std::vector<std::string>::const_iterator iter=theNoGo.begin();
	     iter!=theNoGo.end();++iter) theName+=(*iter);
      }
      
      JetAnaSelector(const JetAnaSelector &rho){
	theMustHave=rho.theMustHave;
	theNoGo=rho.theNoGo;
	theName=rho.theName;
      }
      
     const JetAnaSelector &operator=(const JetAnaSelector &rho){
	theMustHave=rho.theMustHave;
	theNoGo=rho.theNoGo;
	theName=rho.theName;
	return *this;
      }


      virtual bool doMatch(ConstBranchDescription const& p) const 
      {
	//note: mustHave/NoGo should be sets or something, to speed it up! (later... need PLOTS!)
	std::string bname(p.friendlyClassName());
	bname+="_";bname+=p.moduleLabel();bname+="_";
	bname+=p.productInstanceName();bname+="_";
	bname+=p.processName();
	//std::cout<<"SEARCHING: "<<bname<<std::endl;
	bool take=false;
	for (std::vector<std::string>::const_iterator iter=theMustHave.begin();
	     iter!=theMustHave.end();++iter) {
	  //std::cout<<"Lookin at: "<<*iter<<std::endl;
	  if (bname.find((*iter)) != std::string::npos) {
	    take=true;
	    break;
	  }
	}
	for (std::vector<std::string>::const_iterator iter=theNoGo.begin();
	     iter!=theNoGo.end();++iter) {
	  if (bname.find((*iter)) != std::string::npos) {
	    take=false;
	    break;
	  }
	}
	
      	return take;
      }
 
      virtual JetAnaSelector* clone() const
      {
	return new JetAnaSelector(*this);
      }
 
      std::string const& name() const
	{
	  return theName;
	}
 
  private:
      std::vector<std::string> theMustHave;
      std::vector<std::string> theNoGo;
      std::string theName;
  };
}


#endif
