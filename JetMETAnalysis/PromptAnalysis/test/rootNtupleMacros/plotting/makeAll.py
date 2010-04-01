#!/usr/bin/python

import sys
import os
import re

os.system("mkdir CaloJets")
os.system("mkdir PFJets")
os.system("mkdir JPTJets")

################################################
os.system("mv *PF*.gif PFJets/")
os.system("mv *JPT*.gif JPTJets/")
os.system("mv *.gif CaloJets/")


os.system("cp Webpage.html ./CaloJets")
os.system("./makeWebpage.py ./CaloJets/")
os.system("mv ./CaloJets/index.html ./CaloJets/CaloJets.html")


os.system("cp Webpage.html ./PFJets")
os.system("./makeWebpage.py ./PFJets/")
os.system("mv ./PFJets/index.html ./PFJets/PFJets.html")


os.system("cp Webpage.html ./JPTJets")
os.system("./makeWebpage.py ./JPTJets/")
os.system("mv ./JPTJets/index.html ./JPTJets/JPTJets.html")

