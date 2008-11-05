import sys
import os
import tarfile
import shutil

dataset = sys.argv[1]
print "Making plots for "+dataset

# FWLite setup
from ROOT import *
from cmstools import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()

gROOT.Reset()

# open file and get TTree
file = TFile(dataset+".root");
events = file.Get("Events");

# Canvas
canvas = TCanvas("canvas");

events.SetAlias("genmc", "edmHepMCProduct_source__PROD.obj");
events.SetAlias("hits", "L1TrackerHits_l1TrackerHits__PROD.obj");
events.SetAlias("stubs", "L1TrackStubs_l1TrackStubs__PROD.obj");

# make directories for plots
try:
    os.mkdir(dataset);
except OSError:
    pass

dir = dataset + "/";

# MC distributions
#events.Draw("genmc.GetEvent()->event_number()", "");
#canvas.Print(dir+dataset+"_evtNumber.eps");


# hit distributions
events.Draw("hits@.size()", "");
canvas.Print(dir+dataset+"_nHits.eps");

events.Draw("hits.r()>>h1(100,50.,150.)", ""); 
canvas.Print(dir+dataset+"_hitsR.eps");

events.Draw("hits.z()>>h2(120,-120.,120.)", ""); 
canvas.Print(dir+dataset+"_hitsZ.eps");

events.Draw("hits.phi()>>h3(72,-3.1415,3.1415)", ""); 
h3.SetMinimum(0);
canvas.Print(dir+dataset+"_hitsPhi.eps");

events.Draw("hits.eta()>>h4(100,-2.,2.)", "");
canvas.Print(dir+dataset+"_hitsEta.eps");

# hit tower distributions
#events.Draw("hits.tower().l1tower().iphi()>>h5(72,0,72)", "");
#canvas.Print(dataset+"_hitsTowerIEta.eps");
#events.Draw("hits.tower().1tower().ieta()>>h6(72,0,72)", "");
#canvas.Print(dataset+"_hitsTowerIPhi.eps");

#. stub distributions
events.Draw("stubs@.size()", "");
canvas.Print(dir+dataset+"_nStubs.eps");

events.Draw("stubs.dirTheta()>>s1(72,0,3.1415)", "");
canvas.Print(dir+dataset+"_stubsTheta.eps");

events.Draw("stubs.posZ()>>s2(120,-120.,120.)", "");
canvas.Print(dir+dataset+"_stubsTheta.eps");

events.Draw("stubs.posPhi()>>s3(72,-3.1415,3.1415)", "");
s3.SetMinimum(0);
canvas.Print(dir+dataset+"_stubsPhi.eps");

events.Draw("stubs.dZ()>>s4(100,-3.,3.)", "");
canvas.Print(dir+dataset+"_stubsDZ.eps");

events.Draw("stubs.dPhi()>>s5(100,-0.1,0.1)", "");
canvas.Print(dir+dataset+"_stubsDPhi.eps");

events.Draw("stubs.dirTheta()>>s6(100,0,3.1415)", "");
canvas.Print(dir+dataset+"_stubsDirTheta.eps");

events.Draw("stubs.dirPhi()>>s7(100,1.471,1.671)", "");
canvas.Print(dir+dataset+"_stubsDirPhi.eps");

# cleanup
file.Close();

# make tarfile
t = tarfile.open(name = dataset+".tgz", mode = 'w:gz')
t.add(dir);
t.close()

# cleanup
shutil.rmtree(dir);
