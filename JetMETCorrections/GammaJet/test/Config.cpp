// This file is copied from D0 Experiment's CAFE package

//#include "cafe/Config.hpp"
#include "Config.hpp"

#include "TEnv.h"
#include "TOrdCollection.h"
#include "TObjArray.h"
#include "TString.h"
#include "TObjString.h"
#include "THashList.h"
#include "TDirectory.h"
#include "TSystem.h"

#include <memory>
#include <iostream>
#include <set>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace cafe {

    /// static 
    std::set<std::string> Config::s_accessed;

    Config::Config(const std::string& name)
        : _name(name)
    {}

    Config::~Config()
    {}

    std::string Config::replace(TEnv *env, const std::string& input)
    {
	using namespace std;
	string result(input);

	string::size_type pos = result.find("%{");
	if(pos != string::npos) {
	    string::size_type end = result.find('}', pos);
	    if(end != string::npos && result.find('.', pos) < end) {
		string newkey   = result.substr(pos + 2, end - pos - 2);
		s_accessed.insert(newkey);
		string newvalue = env->GetValue(newkey.c_str(), "");
		if (newvalue == "") { 
		    throw std::runtime_error("No value for " + newkey + " defined in the config files."); 
                }
		result.replace(pos, end - pos + 1, newvalue);

		// recursive replace if the new string doesn't point back to itself.
		if ( newvalue.find(newkey) == string::npos ) {
		    result = replace(env, result);
                }
	    }
	    pos = end;
	}
	return result;	
    }

    std::string Config::get(const std::string& key, const std::string& def)
    {
	return get(key, def.c_str());
    }

    
    std::string Config::get(const std::string& key, const char *def)
    {
        std::string full = _name + '.' + key;
	s_accessed.insert(full);
	return getEnv().GetValue(full.c_str(), def);
    }

    int Config::get(const std::string& key, int def)
    {
        std::string full = _name + '.' + key;
	s_accessed.insert(full);
	return getEnv().GetValue(full.c_str(), def);
    }
    
    double Config::get(const std::string& key, double def)
    {
        std::string full = _name + '.' + key;
	s_accessed.insert(full);
	return getEnv().GetValue(full.c_str(), def);        
    }
    
    bool Config::get(const std::string& key, bool def)
    {
        std::string full = _name + '.' + key;
	s_accessed.insert(full);
	return getEnv().GetValue(full.c_str(), def);
    }

    std::vector<std::string> Config::getVString(const std::string& key, const std::string& delim)
    {
	s_accessed.insert(_name + '.' + key);

	TString s(get(key, "").c_str());
	std::vector<std::string> result;
	TObjArray *tokens = s.Tokenize(delim.c_str());
	{
	    TIter iter(tokens);
	    while(TObject *p = iter.Next()) {
		TObjString *item = (TObjString*)p;
		result.push_back(item->GetString().Data());
	    }
	}
	delete tokens;
	return result;
    }

    std::vector<float> Config::getVFloat(const std::string& key, const std::string& delim)
    {
	s_accessed.insert(_name + '.' + key);
	
        TString s(get(key, "").c_str());
        std::vector<float> result;
        TObjArray *tokens = s.Tokenize(delim.c_str());
        TIter iter(tokens);
        while(TObject *p = iter.Next()) {
            TObjString *item = (TObjString*)p;
            result.push_back(atof(item->GetString().Data()));
        }
        delete tokens;
        return result;
     }

     std::vector<int> Config::getVInt(const std::string& key, const std::string& delim)
     {
	s_accessed.insert(_name + '.' + key);

        TString s(get(key, "").c_str());
        std::vector<int> result;
        TObjArray *tokens = s.Tokenize(delim.c_str());
        TIter iter(tokens);
        while(TObject *p = iter.Next()) {
            TObjString *item = (TObjString*)p;
            result.push_back(strtol(item->GetString().Data(),0,0));
        }
        delete tokens;
        return result;
     }

    void Config::set(const std::string& key, const std::string& value)
    {
        getEnv().SetValue(key.c_str(), value.c_str());
    }

    void Config::dumpConfig(std::ostream& stream)
    {
      TIter next(getEnv().GetTable());
      TEnvRec *er;
      static const char *lc[] = { "Global", "User", "Local", "Changed" };

      stream << "cafe::Config: Dumping configuration settings" << std::endl;
      while ((er = (TEnvRec*) next()))
        stream << Form("%-25s: %-30s [%s]",
		       er->GetName(),er->GetValue(),lc[er->GetLevel()]) << std::endl;
      stream << "cafe::Config: End of configuration settings" << std::endl;

      return;
    }

    void Config::dumpConfig(TDirectory* const dir, std::string const name)
    {
      if (dir) {
        TObjArray array(getEnv().GetTable()->GetSize());
	array.SetOwner(kTRUE);

	TIter next(getEnv().GetTable());
	TEnvRec *er;
	static const char *lc[] = { "Global", "User", "Local", "Changed" };

	size_t index = 0;
	while ((er = (TEnvRec*) next())) {
	  TObjString* str = new TObjString(Form("%-25s: %-30s [%s]",
						er->GetName(),
						er->GetValue(),
						lc[er->GetLevel()]));
	  array[index] = str;
	  ++index;
	} // while

	dir->cd();
	array.Write(name.c_str(),TObject::kSingleKey);
	//      array.Write();
      }

      return;
    }

    namespace {
	std::auto_ptr<TEnv> _cleanup;
    }
    
    TEnv& Config::getEnv()
    {
        static TEnv *_env;
        
        if(_env == 0) {
            // Search order:
            //   $CAFE_CONFIG  (putenv() by main routine if set on command line !
            //   ./cafe.config
            //   $SRT_PRIVATE_CONTEXT/cafe.config
            //
            if(const char *filename = gSystem->Getenv("CAFE_CONFIG")) {
                std::cout << "Reading configuration file: " << filename << std::endl;

		// We can't default construct TEnv, instead we have to pass
		// a string that we think nobody uses for a config file name.
                _env = new TEnv("!@#$%^&");
		_env->ReadFile(filename, kEnvLocal);
		if(_env->GetTable()->GetSize() == 0) {
                    delete _env;
                    _env = 0;
		    throw std::runtime_error(std::string("Config: specified configfile not found: ") + filename);
                }
            }
            
            if(_env == 0) {
                _env = new TEnv("!@#$%^&");
		_env->ReadFile("./cafe.config", kEnvLocal);
                if(_env->GetTable()->GetSize() == 0) {
                    delete _env;
                    _env = 0;
                }
            }

            if(_env == 0) {
                std::string ctx;
                if(const char *c = getenv("SRT_PRIVATE_CONTEXT")) {
                    ctx += c;
                }
                ctx += "./cafe.config";
                _env = new TEnv("!@#$%^&");
		_env->ReadFile(ctx.c_str(), kEnvLocal);
            }

            // now load all include files

	    // the files we have already seen
            std::set<std::string> _files_included;
            
            bool add_more = false;
            do {
		add_more = false;
		TString s(_env->GetValue("cafe.Include", "")) ;
		std::auto_ptr<TObjArray> includes(s.Tokenize(" ,\t"));
		TIter iter(includes.get());
		while(TObject *p = iter.Next()) {
		    std::string filename(((TObjString*)p)->GetString().Data());
		    if(_files_included.count(filename) > 0) continue;
                    if(gSystem->AccessPathName(filename.c_str(), kReadPermission)) {
                        throw std::runtime_error("Include file does not exist: " + filename);
                    }
		    std::cout << "Including configuration file: " << filename << std::endl;
		    _env->ReadFile((filename).c_str(), kEnvLocal);
		    _files_included.insert(filename);
		    add_more = true;
		}
            } while(add_more);

	    s_accessed.insert("cafe.Include");
	    _cleanup = std::auto_ptr<TEnv>(_env);

        }
        return *_env;
    }

    void Config::resolve()
    {
        TEnv& env(getEnv());
        TIter next(env.GetTable());
        while(TEnvRec *rec = (TEnvRec *)next()) {
            const char *str = rec->GetValue();
            if(strstr(str, "%{")) {
                std::string new_value = replace(&env, str);
                env.SetValue(rec->GetName(), new_value.c_str());
            }
            // remove space in the end of the config parameter
	    std::string st = str ;
	    if (st.rfind(" ") == st.size()-1) {
	      std::string::size_type pos ;
	      while((pos = st.rfind(" ")) == st.size()-1 && st.size() > 0) 
		st.erase(pos) ;
	      env.SetValue(rec->GetName(), st.c_str());
	    }
	}
    }

    std::vector<std::string> Config::checkConfiguration()
    {
	std::set<std::string> processors;
	
	// Pass 1, find all Processor 'P'
	for(std::set<std::string>::iterator it = s_accessed.begin();
	    it != s_accessed.end();
	    ++it) {
	    std::string::size_type dot = (*it).find('.');
	    if(dot != std::string::npos) {
		std::string processor_name = (*it).substr(0, dot);
		processors.insert(processor_name);
		
	    } // else ignore anything without a '.'
	}

	// Pass 2, 
	std::vector<std::string> result;

	TIter next(getEnv().GetTable());
	while (TEnvRec *rec = (TEnvRec*) next()) {
	    std::string name(rec->GetName());
	    std::string::size_type dot = name.find('.');
	    if(dot != std::string::npos) {
		if(processors.find(name.substr(0, dot)) != processors.end()) {
		    if(s_accessed.find(name) == s_accessed.end()) {
			result.push_back(name);
		    }
		}
	    }
	}

	return result;
    }


  std::vector<std::string> Config::variables(const std::string& name) 
  {
	
    std::vector<std::string> result;
    
    TIter next(getEnv().GetTable());
    while (TEnvRec *rec = (TEnvRec*) next()) {
      std::string pname(rec->GetName());
      std::string::size_type dot = pname.find('.');
      if(dot != std::string::npos &&
	 name ==pname.substr(0, dot))
	result.push_back(pname.substr(dot+1));
    }			 
    return result ;
  }

}

