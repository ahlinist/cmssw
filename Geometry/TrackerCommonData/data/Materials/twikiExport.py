#!/usr/bin/python

import optparse
import ConfigParser

class Constituent:
    def __init__(self, line):
        if  len(line.split('"')) < 5 or len(line.split('"')[4].split()) < 3:
            raise StandardError , "not a well formed Constituent: "+constString
        self.theDescription = line.split('"')[1]
        self.theName = line.split('"')[3]
        self.theMass = line.split('"')[4].split()[0]
        self.theCount = line.split('"')[4].split()[1]
        self.theType = line.split('"')[4].split()[2]
        
    def __str__(self):
        return "Name: "+self.theName+" desc: "+self.theDescription+" mass "+self.theMass+" type "+self.theType+" count "+self.theCount
        
class Material:
    def __init__(self, matString, comment):
        if matString == "" or not matString[0] == "#":
            raise StandardError , "not a valid material Definition: "+matString
        line = matString[1:]

        if    len( line.split('"') ) < 5 or len( line.split('"')[4].split() ) < 2:
            raise StandardError , "not a well formed Material Definition: "+matString            
        self.theDescription = line.split('"')[1]
        self.theName = line.split('"')[3]
        self.theMcVolume = line.split('"')[4].split()[0]
        self.theComment = comment
        self.theConstituents = []

    def addConstituent(self, constString):
        if constString  == "" or not constString[0] == "*":
            raise StandardError , "not a valid Constituent: "+constString
        line = constString[1:]
        self.theConstituents.append( Constituent(line) )

        number = int( line.split('"')[0].split()[0] )
        if not len(self.theConstituents) == number:
            raise StandardError, "Constituent Number mismatch for "+str(len(self.theConstituents))+" in: "+line
    
    def __str__(self):
        result = "[ "+self.theName+" Description: "+self.theDescription+" MC-Volume:"+str(self.theMcVolume)+"\n"
        result += "Comments:\n"+self.theComment
        return result

#parses the .in File
def parseInFile(inFile, config):
    comment = ""
    materials = []
    i = 0
    for line in inFile.split("\n"):
        i=i+1
        if i < int(config["parsing"]["intro"]):
            continue
        if not line == "" and line[0] == "#":
            material = Material( line , comment )
            if not material.theName == "END":
                materials.append( material )
            comment = ""

            #print "Name: "+name+" Description: "+description+" mcVolume: "+str(mcVolume)
        elif not line == "" and line[0] == "*":
            
                materials[-1].addConstituent(line)
        else:
            ignore = False
            for char in config["parsing"]["ignorelines"]:
                if len(line.strip()) == line.strip().count(char) and not len(line) == 0:
                    ignore = True
            if not ignore:
                comment += line+"\n"
    
    return materials

def getTwiki(materials,config):
    result = config["twiki"]["pagename"]+"\n"
    for mat in materials:
        result += config["twiki"]["heading"]+mat.theName+"\n"
        result += "short Description: *"+mat.theDescription+"* <br>\n"
        result += "Mounte Carlo Volume: "+mat.theMcVolume+" cm^3 <br>\n"
        result += "\n"+config["twiki"]["subheading"]+"Comments\n"
        result += "<verbatim>\n"
        result += mat.theComment
        result += "</verbatim>\n"        

        result += "\n---+++!!Material\n"
        result += "  | *Description* | *Material Name* | *Mass* | *Count* | *Type* |\n"
        for const in mat.theConstituents:
            result += "  | ="+const.theDescription+"= | ="+const.theName+"= | "+const.theMass+" | "+const.theCount+" | "+const.theType+" |\n"
        result += "\n"

    return result

#reads back one [section] of [config]
def readSection(config,section):
    result = {}
    for option in config.options(section):
        result[option] = config.get(section,option)
    return result
    

# reads the twikiConfig.ini
def readConfig(fileName):
    config = ConfigParser.ConfigParser()   
    config.read(fileName)
    result = {}
#    result["general"] = readSection("general")
    result["parsing"] = readSection(config,"parsing")
    result["twiki"] = readSection(config,"twiki")


    return result

#main
def main():
    optParser = optparse.OptionParser()
    optParser.add_option("-i", "--input", dest="inFile",
                  help="the .in material description that is the input", metavar="INPUT")
    optParser.add_option("-o", "--output", dest="output",
                  help="the file to put the twiki formated output in (default: <.in-Filename>.twiki)", metavar="TWIKIOUT")
    optParser.add_option("-c", "--config", dest="config",
                  help="configuration to use (default twikiConfig.ini)", metavar="CONFIG")

    (options, args) = optParser.parse_args()

    if options.inFile == None:
        raise StandardError, "no .in File given!"
    if options.output == None:
        options.output = options.inFile.replace(".in",".twiki")
    if options.config == None:
        options.config = "twikiConfig.ini"
    
    config = readConfig(options.config)

    inFile = open(options.inFile,"r")
    inFileContent = inFile.read()
    inFile.close()

    materials = parseInFile(inFileContent, config)
    twikiString = getTwiki(materials, config)


    outFile = open(options.output,"w")
    outFile.write(twikiString)
    outFile.close()

main()
