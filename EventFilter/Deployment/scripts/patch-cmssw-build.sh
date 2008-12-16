#!/bin/bash
numArgs=$#;
if [ $numArgs -lt 3 ]; then
    echo "Usage: patch-cmssw-build.sh CMSSW_X_Y_Z patchId [dev/pro] patchdir";
    exit -1;
fi
export PATH=${PATH}:$PWD/EventFilter/Deployment/scripts
export SCRAM_ARCH=slc4onl_ia32_gcc346
sver=`cat  /opt/cmssw/$SCRAM_ARCH/etc/default-scramv1-version`
cat > EventFilter/Deployment/scripts/scramv1 <<EOF
echo "This is SCRAM_ARCH $SCRAM_ARCH"
/opt/cmssw/slc4onl_ia32_gcc346/lcg/SCRAMV1/$sver/bin/scram \$@
EOF
chmod +x EventFilter/Deployment/scripts/scramv1
CMS_SW_VERSION=$1;
PRO_DEV=$3;
LOCAL_CODE_PATCHES_TOP=$4;
PATCH_ID=$2;
#source /opt/cmssw/cmsset_default.sh; no longer needed 
mkdir patch-cmssw-tmp;
cd patch-cmssw-tmp;
echo "now in $PWD";
TOPD=$PWD;
scramv1 project CMSSW $CMS_SW_VERSION;
cd $CMS_SW_VERSION/src;
echo "now in $PWD";
# dirty kludge for now should get packages with tags from CVS 
echo "I am supposed to check out some code here";
cp -r $LOCAL_CODE_PATCHES_TOP/* .;
# end kludge
scramv1 b;
cd -;
echo "now in $PWD";
echo "Moving patches to their destination";
mkdir -p  opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION;
cp -r $TOPD/$CMS_SW_VERSION/src $TOPD/opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/;
cp -r $TOPD/$CMS_SW_VERSION/lib $TOPD/opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/;
cp -r $TOPD/$CMS_SW_VERSION/module $TOPD/opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/;
echo "Generating and populating summary directories";
cd /opt/cmssw/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/src/; 
echo "now in $PWD";
install_env.pl $TOPD/opt/cmssw/$PRO_DEV/lib/ $TOPD/opt/cmssw/$PRO_DEV/module/ $TOPD/opt/cmssw/$PRO_DEV/env.txt $TOPD/opt/cmssw/$PRO_DEV/python; 
cd -;
echo "now in $PWD";
for x in opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/lib/slc4onl_ia32_gcc346/*.so 
do
  if [ -e $x ] 
      then 
      ln -sf $PWD/$x opt/cmssw/$PRO_DEV/lib;
  fi
done
for x in opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/module/slc4onl_ia32_gcc346/*.reg
do
  if [ -e $x ] 
      then 
      ln -sf /$x opt/cmssw/$PRO_DEV/module;
  fi
done
echo "Pasting seal cache entries from patch into mainstream";
cat opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/module/slc4onl_ia32_gcc346/.cache >>  opt/cmssw/$PRO_DEV/module/.cache;
echo "Updating edm plugin registry with patches";
cd $TOPD/$CMS_SW_VERSION;
echo "now in $PWD";
RESPONSE=`reset_edm_cache.pl $TOPD/opt/cmssw/$PRO_DEV/lib $TOPD/opt/cmssw/$PRO_DEV/module /dev/null`;
echo "reset_edm_cache returned $RESPONSE";
### Extract the root seal and oracle version from the scram runtime here before
### making the symbolic links
RT=`scramv1 runtime -sh | grep ROOTSYS= | tr -d ";" | perl -p -e 's/.*=\"([^\"]*)\"$/$1/'`
SL=`scramv1 runtime -sh | grep SEAL= | tr -d ";" | perl -p -e 's/.*=\"([^\"]*)\"$/$1/'`
echo "extracting symbolic link locations for root and seal from scram runtime \n"; 
echo "rootsys at $RT\n";
echo "seal at $SL\n"; 
#fix symbolic links for patched libs so it is visible after installation
cd $TOPD;
echo "now in $PWD";
for x in opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION/lib/slc4onl_ia32_gcc346/*.so 
do
  if [ -e $x ] 
      then 
      ln -sf /$x opt/cmssw/$PRO_DEV/lib;
  fi
done
cd $TOPD;
echo "now in $PWD";
# remove temporary directory holding the local patches
#rm -rf $TOPD/$CMS_SW_VERSION;
ln -s /opt/cmssw/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION opt/cmssw/$PRO_DEV/base
ln -s /opt/cmssw/$PRO_DEV/patches/slc4onl_ia32_gcc346/cms/online/$CMS_SW_VERSION opt/cmssw/$PRO_DEV/patch
ln -s $RT opt/cmssw/$PRO_DEV/root
ln -s $SL opt/cmssw/$PRO_DEV/seal
CMS_SW_VERSION_CLEAN=`echo $CMS_SW_VERSION | tr -d "-"`; 
echo "CMS_SW_VERSION now $CMS_SW_VERSION";
cat > patch-cmssw.spec <<EOF
Name: patch-cmssw-$PRO_DEV
Version: $CMS_SW_VERSION_CLEAN$PATCH_ID
Release: 7
Summary: CMSSW Patches
License: Unknown
Group: Hacks
Packager: scram
Source: none
%define _tmppath $TOPD
BuildRoot: %{_tmppath}/patch-cmssw
BuildArch: i386
Provides:/opt/cmssw/$PRO_DEV/lib
Provides:/opt/cmssw/$PRO_DEV/module
Provides:/opt/cmssw/$PRO_DEV/python
Provides:/opt/cmssw/$PRO_DEV/base
Provides:/opt/cmssw/$PRO_DEV/root
Provides:/opt/cmssw/$PRO_DEV/seal
Provides:/opt/cmssw/$PRO_DEV/env.txt
Requires:cms+online+$CMS_SW_VERSION


%description
Creates directories with symbolic links to all cmssw-related libraries.
Installs patches to cmssw code

%prep
%build
%install
rm -rf \$RPM_BUILD_ROOT
mkdir -p \${RPM_BUILD_ROOT}
tar -C $TOPD -c opt/cmssw |tar -xC \$RPM_BUILD_ROOT
%files
/opt/cmssw/$PRO_DEV/lib
/opt/cmssw/$PRO_DEV/module
/opt/cmssw/$PRO_DEV/python
/opt/cmssw/$PRO_DEV/env.txt
/opt/cmssw/$PRO_DEV/base
/opt/cmssw/$PRO_DEV/patch
/opt/cmssw/$PRO_DEV/root
/opt/cmssw/$PRO_DEV/seal
/opt/cmssw/$PRO_DEV/patches
EOF
mkdir -p RPMBUILD/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
rpmbuild --define "_topdir `pwd`/RPMBUILD" -bb patch-cmssw.spec
#rm -rf patch-cmssw-tmp;
