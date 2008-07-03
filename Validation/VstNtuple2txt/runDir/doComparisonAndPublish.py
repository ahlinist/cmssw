import os,sys,datetime,shutil

def usage():
    s=sys.argv[0]
    return '''Usage: %s -d data_file -b bkg_file
First applies checkDistributions to the two data files.
Then applies checkDistributionsWithCuts and publishes to the
  webpage defined in your environment (WWW_PUBLISH_REL)
Options:
-d      Name for new dataset (appears as datapoints)
-b      Name for reference dataset (appears as histogram)

Also, the legend for the signal can be set by the environment
--> vistaLegendDataEntry <--
''' % s

def filter(badfile):
    f=open(badfile,'r')
    g=open('tmp_file','w')
    badlines=0
    s=f.readlines()
    f.close()
    for line in s:
        t=line.split()
        if t[4]=="14000":
            g.write(line)
        else:
            badlines=badlines+1
    g.close()
    os.remove(badfile)
    shutil.copyfile('tmp_file',badfile)
    os.remove('tmp_file')
    print badlines,' bad lines in ',badfile
    


def start_comparisons(datafile,reffile,www):
    d = datetime.date.today()
    dprint=d.isoformat()
    if not os.path.exists('bkg') or not os.path.exists('sig'):
        return 0
    if os.path.exists('bkg/*'):
        os.remove('bkg/*')
    if os.path.exists('sig/*'):
        os.remove('sig/*')
    shutil.copyfile(datafile,'sig/sig')
    shutil.copyfile(reffile,'bkg/bkg')
    filter('bkg/bkg')
    filter('sig/sig')
    pwd=os.getcwd()
    os.system('partition lhc-Vista bkg/bkg')
    os.system('partition lhc-Vista sig/sig')
    os.system('checkDistributions '+pwd+' bkg sig lhc mc')
    os.system('./makeNormHtml.bash '+pwd+' -o '+www,)
    return 1

def main():
    
    if len(sys.argv) < 5:
        print usage()
    elif len(sys.argv) == 5 and sys.argv[1] == '-d' and sys.argv[3] == '-b':
        datafile = sys.argv[2]
        if not os.path.exists(datafile):
            print 'datafile ',datafile,' does not exist'
            return
        reffile = sys.argv[4]
        if not os.path.exists(reffile):
            print 'reffile ',reffile,' does not exist'
            return
        www=os.getenv("WWW_PUBLISH_REL")
        if www==None:
            print 'environment variable WWW_PUBLISH_REL not set '
            return
        start_comparisons(datafile,reffile,www)
    else:
        print usage()


if __name__ == '__main__':
    main()
