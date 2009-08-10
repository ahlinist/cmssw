import os
import subprocess

#--------------------------------------------------------------------------------
# Create python configuration file for execution of cmsRun job on the CERN batch system
#
# NOTE: Function needs to be passed all of the following three arguments
#
#      (1) configFile_orig
#          name of the original config file (template)
#      (2) replacements
#          list of replace statements to be applied to the original config file,
#          in the format 'paramName1=paramValue1; paramName2=paramValue2;...';
#          in the original config file, each occurence of any paramName
#          will be replaced by the associated paramValue (using the 'sed' utility)
#      (3) configFile_mod
#          name of modified config file including replacements
#
# Author: Christian Veelken, UC Davis
#
#
# Modified by gfball to use python os.* functions and replace statements, and accept lists and dictionaries as well as formatted strings.
#--------------------------------------------------------------------------------

def prepareConfigFile(configFile_orig = None, replacements = "",  configFile_mod = None):
    # check that configFile_orig and configFile_mod parameters are defined and non-empty
    if configFile_orig is None:
        raise ValueError("Undefined configFile_orig Parameter !")
    if configFile_mod is None:
        raise ValueError("Undefined configFile_mod Parameter !")

    # check that original config file (template) exists
    if not os.path.exists(configFile_orig):
        raise ValueError("Config file = " + configFile_orig + " does not exist !")

    # remove all white-space characters from replacements parameter string
    if isinstance(replacements,list):
      replaceStatements = replacements
    elif isinstance(replacements,dict):
      replaceStatements = [[k,v] for k,v in replacements.items()]
    elif isinstance(replacements,basestring):
      replaceStatements = [r.split("=",1) for r in replacements.replace(" ","").split(";")]
    else:
      raise ValueError("No idea what your replacement option is meant to accomplish"+replacements)
    
    f_cfg_orig = open(configFile_orig,"r")
    cfg_mod = f_cfg_orig.read()
    f_cfg_orig.close()
    
    for replaceStatement in replaceStatements:

        # check that replacement string matches 'paramName=paramValue' format
        if len(replaceStatement) != 2:
            raise ValueError("Invalid format of replace Statement: " + replaceStatement + " !")

        cfg_mod = cfg_mod.replace("#%s#"%replaceStatement[0],replaceStatement[1])

    
    cfg_mod = cfg_mod.replace('#__','')
        
    f_cfg_mod = open(configFile_mod,"w")
    f_cfg_mod.write(cfg_mod)
    f_cfg_mod.close()    
       
