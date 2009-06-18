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
#--------------------------------------------------------------------------------

def prepareConfigFile(configFile_orig = None, replacements = "",  configFile_mod = None):

    # check that configFile_orig and configFile_mod parameters are defined and non-empty
    if configFile_orig is None:
        raise ValueError("Undefined configFile_orig Parameter !!")
    if configFile_mod is None:
        raise ValueError("Undefined configFile_mod Parameter !!")

    # check that original config file (template) exists
    if not os.access(configFile_orig, os.F_OK):
        raise ValueError("Config file = " + configFile_orig + " does not exist !!")

    # remove all white-space characters from replacements parameter string
    replacements = replacements.replace(" ", "")

    # split replacements string into list of individual replace statements
    # (separated by ";" character)
    replaceStatements = replacements.split(";")

    # compose command-line argument for 'sed' 
    sedArgument = ""

    for replaceStatement in replaceStatements:

        # split replacement string into name, value pairs
        paramNameValuePair = replaceStatement.split("=")

        # check that replacement string matches 'paramName=paramValue' format
        if len(paramNameValuePair) != 2:
            raise ValueError("Invalid format of replace Statement: " + replaceStatement + " !!")

        # extract name and value to be used for replacement
        paramName = paramNameValuePair[0]    
        paramValue = paramNameValuePair[1]

        # "/" is a special character for sed
        # and needs to be escaped by a preceding backslash
        paramValue = paramValue.replace("/", "\/")

        #print("paramName = " + paramName)
        #print("paramValue = " + paramValue)

        # add sed argument for replacing paramName by paramValue
        sedArgument += "s/" + "#" + paramName + "#" + "/" + paramValue + "/; "
        
    # activate replace statements in config file
    # by removing (replace by empty string) "#__" comment indicators
    sedArgument += "s/#__//"

    # execute 'sed' to perform actual replacements
    sedCommand = 'sed -e "' + sedArgument + '" ' + configFile_orig + ' > ' + configFile_mod
    #print("sedCommand = " + sedCommand)
    subprocess.call(sedCommand, shell = True)
