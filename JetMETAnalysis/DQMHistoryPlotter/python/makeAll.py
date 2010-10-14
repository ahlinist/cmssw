#!/usr/bin/python
# usage: python makeAll.py <resultdir>
# ex: python makeAll.py Certification_2010_04_12

import sys
import os
import re
directory=sys.argv[1]

os.system("./makeWebpage.py %s"%directory)

os.system("mkdir "+directory)
os.system("cp index.html "+directory)
os.system("mkdir "+directory+"/Jets")
os.system("mkdir "+directory+"/MET")

os.system("cp *Jet*.png "+directory+"/Jets")
os.system("./makeWebpage.py  ./"+directory+"/Jets/")

os.system("cp *MET*.png "+directory+"/MET")
os.system("./makeWebpage.py  ./"+directory+"/MET/")


