import os

os.system("./CalcLLH eeg")
os.system("./CalcLLH mmg")
execfile("add.py")
execfile("fit.py")
