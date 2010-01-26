import glob
davis_mc_files = [
    "file:data_ntuple.root" ]

davis_data_files = [
    "file:data_ntuple.root" ]

castor_mc_files = []

castor_data_files = [] 

data_file_list = open("data_files.txt", "r")
for line in data_file_list.readlines():
   castor_data_files.append(line.strip())

mc_file_list = open("mc_files.txt", "r")
for line in mc_file_list.readlines():
   castor_mc_files.append(line.strip())

#tmp_mc_files = glob.glob("/tmp/friis/first_data/mc/*root")
#tmp_data_files = glob.glob("/tmp/friis/first_data/data/*root")
tmp_mc_files = glob.glob("/tmp/friis/FakeRate/MC/*root")
tmp_data_files = glob.glob("/tmp/friis/FakeRate/RD/*root")

