#ifndef Configurator_HH
#define Configurator_HH

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include "ICTypeDefs.hh"
#include "ICUtilityToolbox.hh"

/*
 * doesn't do hex base.
 */


namespace ICCoreUtils
{

  typedef std::map<std::string,std::string> ConfigMap;

  class ConfigArena
  {
  protected:
    static ConfigMap & Configuration();
    static void CleanEntry(std::string & entry);
    static bool mConfigParsed;
  };
  bool ConfigArena::mConfigParsed=false; 



  template<typename T> 
  class Configurator:public ConfigArena
  {
  public:
    Configurator(const std::string & aId,T def):
      mId(aId)
    {
      ConfigMap & data=ConfigArena::Configuration();
      ConfigMap::const_iterator it=data.find(mId);
      if(it!=data.end())
	{	
	  std::istringstream iss;
	  if(typeid(T)==typeid(bool))
	    {
	      //std::cout<<"ABool was found "<<it->second<<std::endl;
	      if(it->second==std::string("true")||it->second==std::string("1"))
		iss.str("1");
	    	    
	      if(it->second==std::string("false")||it->second==std::string("0"))
		iss.str("0");
	    }
	  else
	    {
	      iss.str(it->second);
	    }
	  iss>>mParam;
	}
      else
	{
	  mParam=def;
	}
    }
    
    inline const T & Parameter()const{std::cout<<mId<<":"<<mParam<<std::endl; return mParam;}
    
    /*
     * shouldn't need this one.
     *
     */
    //inline T & Parameter(){return mParam;}

    ~Configurator(){}
    
  private:
    std::string mId;
    T mParam;

  private:
    Configurator();
    Configurator(const Configurator &);
    Configurator & operator=(Configurator &);
  };
  
  

  void ConfigArena::CleanEntry(std::string &  entry)
  {
    if(entry.size()==0)
      return;
    //std::string temp(entry.size(),' ');
    unsigned long j=0;
    for(unsigned long i=0; i<entry.size(); i++)
      {
	if(entry[i]!=' ')
	  entry[j++]=entry[i];
	
      }
    entry.resize(j);
    //std::cout<<entry<<std::endl;
    //  return temp;
  }
  
  ConfigMap & ConfigArena::Configuration()
  {
    static ConfigMap Config; //ConfigMap();
    
    if(mConfigParsed)
      return Config;
    
    std::string lConfigFileName=ICCoreUtils::GetEnv("IIMAS_CONFIG_FILE");
    if(lConfigFileName.length()==0)
      lConfigFileName="ConfigFile.conf";
    
    std::ifstream input(lConfigFileName.c_str());
    if(!input){
      std::cerr<<lConfigFileName<<" not opened"<<std::endl;
      return Config;
    }
    while(input)
      {
	std::string buf;
	std::getline(input,buf);

	if(!input)
	  break;
	int firstchar=buf.find_first_not_of(" ");
	if(buf[firstchar]=='#'){
	  //std::cout<<"comment"<<std::endl;
	  continue;//comment
	}
	CleanEntry(buf);
	if(buf.size()==0)
	  continue;
	
	//
	
	int delim=buf.find_first_of(':');
	
	//std::cout<<__PRETTY_FUNCTION__<<" "<<buf.substr(0, delim)<<"   "<<buf.substr(delim+1, buf.length())<<std::endl;
										     
	if(delim>0)
	  {//insert key:value pair.
	    Config[buf.substr(0, delim)] = buf.substr(delim+1, buf.length());
	    
	  }
      }
    mConfigParsed=true;
    return Config;
  }

}//~namespace ICCoreUtils

#endif //~Configurator_HH

