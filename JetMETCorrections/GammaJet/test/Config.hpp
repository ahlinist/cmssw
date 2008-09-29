// This file is copied from D0 Experiment's CAFE package

#ifndef CAFE_CONFIG_HPP__
#define CAFE_CONFIG_HPP__

#include <string>
#include <vector>
#include <set>

class TEnv;
class TDirectory;

namespace cafe {

    /**
     *
     * Provide access to configuration information
     * for a given package called 'name'.
     *
     * To use it, just instantiate an object of
     * this type. All lookups will be prepended with
     * the 'name' given.
     *
     * Config config("MyTest");
     * std::string value = config.get("MyValue","MyDefault");
     *
     * will look up 'Mytest.MyValue: ...' in the configuration file.
     *
     * The configuration file is searched in the following places:
     *
     * - ${CAFE_CONFIG} if the environment variable is set
     * - ./cafe.config  if it exists
     * - ${SRT_PRIVATE_CONTEXT}/cafe.config 
     *
     * \ingroup cafe
     *
     */
    class Config {
    public:
        explicit Config(const std::string& name);
	virtual ~Config();
        
	/// @name Accessor
	///
	/// Return the default value if the parameter isn't set
	//@{

	/// Get string value from config DB.
        std::string get(const std::string& key, const std::string& def);

	/// Get string value from config DB. This is needed because otherwise
	/// the 'bool' version of get() will be called for const char strings.
        std::string get(const std::string& key, const char *def);

	/// Get integer value from config DB.
        int         get(const std::string& key, int def);

	/// Get floating point value from config DB.
        double      get(const std::string& key, double def);

	/// Get boolean value from config DB.
	bool        get(const std::string& key, bool def);

	/// Get list of strings of config DB.
	std::vector<std::string> getVString(const std::string& key, const std::string& delim = " ");

	/// Get list of floats from config DB.
	std::vector<float> getVFloat(const std::string& key, const std::string& delim = " ");

	/// Get list of integers from config DB.
	std::vector<int> getVInt(const std::string& key, const std::string& delim = " ");

	//@}

	/// Set/Override an entry in the configuration database
        void        set(const std::string& key, const std::string& value);

        /// Dump config DB to given output stream.
	void dumpConfig(std::ostream& stream);

        /// Dump config DB to given TDirectory.
	void dumpConfig(TDirectory* const dir, std::string const name = "configDB");
	
	/// Check that every processor P has accessed all the
	/// variables of the form 'P.X: ...'
	/// 
	/// Report any variables that have not been accessed.
	static std::vector<std::string> checkConfiguration();

                /// return list of all variables specified for the processor with name "name"
	static std::vector<std::string> variables(const std::string& name) ;

        /// Re-run the variable expansion process.
        ///
        /// Normally this is done automaticall when all input files have 
        /// been read, so you don't need to call this. 
        static void resolve();

    private:
	static std::string replace(TEnv *env, const std::string& input);
    private:
        std::string _name;
        static TEnv& getEnv();

	/// A list of all accessed entries.
	static std::set<std::string> s_accessed;
    };
}

#endif // CAFE_CONFIG_HPP__
