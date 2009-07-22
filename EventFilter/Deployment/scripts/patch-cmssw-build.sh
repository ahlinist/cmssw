#!/bin/bash
numArgs=$#
if [ $numArgs -lt 4 ]; then
    echo "Usage: patch-cmssw-build.sh CMSSW_X_Y_Z patchId {dev|pro|...} patchdir"
    exit -1
fi
CMSSW_VERSION=$1            # the CMSSW version, as known to scram
PATCH_ID=$2                 # an arbitrary tag which identifies the extra code (usually, "p1", "p2", ...)
PRO_DEV=$3                  # "pro", "dev", etc...
LOCAL_CODE_PATCHES_TOP=$4   # absolute path to the area where extra code to be compiled in can be found, equivalent to $CMSSW_BASE/src

# set environment variables
export PATH=$PATH:$PWD/EventFilter/Deployment/scripts
export CVSROOT=:pserver:anonymous@cmscvs.cern.ch:/cvs_server/repositories/CMSSW

# create a scram wrapper
export SCRAM_ARCH=slc4onl_ia32_gcc346
SCRAM_VERSION=`cat /opt/cmssw/$SCRAM_ARCH/etc/default-scramv1-version`
cat > EventFilter/Deployment/scripts/scramv1 <<EOF
#! /bin/bash
/opt/cmssw/$SCRAM_ARCH/lcg/SCRAMV1/$SCRAM_VERSION/bin/scram \$@
EOF
chmod +x EventFilter/Deployment/scripts/scramv1

# check for an online release vs. an online patch release
RELEASE_TYPE="online"
if echo $CMSSW_VERSION | grep -q "patch"; then
  RELEASE_TYPE="online-patch"
fi

# create a build area
rm -rf patch-cmssw-tmp
mkdir  patch-cmssw-tmp
cd     patch-cmssw-tmp
TOPDIR=$PWD

# create a local CMSSW area, import patches from the specidfied directory, and build them (except for the "test" directories)
scramv1 project CMSSW $CMSSW_VERSION
sed -e's#<classpath path=".*+test"/>##' -i $CMSSW_VERSION/config/BuildFile.xml
cd $CMSSW_VERSION/src
cp -r $LOCAL_CODE_PATCHES_TOP/* .
scramv1 b -j 4
cd $TOPDIR

echo "Moving patches to their destination"
mkdir -p $TOPDIR/opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION
cp -r $TOPDIR/$CMSSW_VERSION/src $TOPDIR/opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/
cp -r $TOPDIR/$CMSSW_VERSION/lib $TOPDIR/opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/

echo "Generating and populating summary directories"
CMSSW_ROOT_DIRECTORY=`grep RELEASETOP $CMSSW_VERSION/.SCRAM/$SCRAM_ARCH/Environment | cut -d= -f2`
cd $CMSSW_ROOT_DIRECTORY/src
install_env.pl $TOPDIR/opt/cmssw/$PRO_DEV/lib $TOPDIR/opt/cmssw/$PRO_DEV/module $TOPDIR/opt/cmssw/$PRO_DEV/env.txt $TOPDIR/opt/cmssw/$PRO_DEV/python

echo "Linking .rootmap plugin entries from $CMSSW_ROOT_DIRECTORY"
find $CMSSW_ROOT_DIRECTORY/external/$SCRAM_ARCH/lib -name '*.rootmap' | xargs -i ln -sf {} $TOPDIR/opt/cmssw/$PRO_DEV/lib
find $CMSSW_ROOT_DIRECTORY/lib/$SCRAM_ARCH          -name '*.rootmap' | xargs -i ln -sf {} $TOPDIR/opt/cmssw/$PRO_DEV/lib

cd $TOPDIR
for x in opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/lib/$SCRAM_ARCH/*.so
do
  if [ -e $x ]
      then
      ln -sf $PWD/$x opt/cmssw/$PRO_DEV/lib/
  fi
done

echo "Updating edm plugin registry with patches"
cd $TOPDIR/$CMSSW_VERSION
RESPONSE=`reset_edm_cache.pl $TOPDIR/opt/cmssw/$PRO_DEV/lib $TOPDIR/opt/cmssw/$PRO_DEV/module /dev/null`
echo "reset_edm_cache returned $RESPONSE"

### Extract the root version from the scram runtime here before making the symbolic links
RT=`scramv1 runtime -sh | grep ROOTSYS= | tr -d ";" | perl -p -e 's/.*=\"([^\"]*)\"$/$1/'`
echo "extracting symbolic link locations for root from scram runtime"
echo "  ROOTSYS at $RT"

# fix symbolic links for patched libs so it is visible after installation
cd $TOPDIR
for x in opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/lib/$SCRAM_ARCH/*.so
do
  if [ -e $x ]
      then
      ln -sf /$x opt/cmssw/$PRO_DEV/lib
  fi
done

cd $TOPDIR
# remove temporary directory holding the local patches
#rm -rf $TOPDIR/$CMSSW_VERSION
ln -s /opt/cmssw/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION                   opt/cmssw/$PRO_DEV/base
ln -s /opt/cmssw/$PRO_DEV/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION  opt/cmssw/$PRO_DEV/patch
ln -s $RT                                                                       opt/cmssw/$PRO_DEV/root
CMSSW_VERSION_CLEAN=`echo $CMSSW_VERSION | tr -d "-"`

echo "CMSSW_VERSION now $CMSSW_VERSION"
cat > patch-cmssw.spec <<EOF
Name: patch-cmssw-$PRO_DEV
Version: $CMSSW_VERSION_CLEAN$PATCH_ID
Release: 7
Summary: CMSSW Patches
License: Unknown
Group: Hacks
Packager: scram
Source: none
%define _tmppath $TOPDIR/patch-cmssw
BuildRoot: %{_tmppath}
BuildArch: i386
Provides:/opt/cmssw/$PRO_DEV/lib
Provides:/opt/cmssw/$PRO_DEV/python
Provides:/opt/cmssw/$PRO_DEV/base
Provides:/opt/cmssw/$PRO_DEV/root
Provides:/opt/cmssw/$PRO_DEV/env.txt
Requires:cms+$RELEASE_TYPE+$CMSSW_VERSION

%description
Creates directories with symbolic links to all cmssw-related libraries.
Installs patches to cmssw code

%prep

%build

%install
rm -rf \$RPM_BUILD_ROOT
mkdir -p \$RPM_BUILD_ROOT
tar -C $TOPDIR -c opt/cmssw | tar -xC \$RPM_BUILD_ROOT

%files
%defattr(-, root, root, -)
/opt/cmssw/$PRO_DEV/lib
/opt/cmssw/$PRO_DEV/python
/opt/cmssw/$PRO_DEV/env.txt
/opt/cmssw/$PRO_DEV/base
/opt/cmssw/$PRO_DEV/patch
/opt/cmssw/$PRO_DEV/root
/opt/cmssw/$PRO_DEV/patches
EOF
mkdir -p RPMBUILD/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
rpmbuild --define "_topdir `pwd`/RPMBUILD" -bb patch-cmssw.spec
#rm -rf patch-cmssw-tmp

# copy the RPM to a local folder
mkdir -p ~/quattor/patch
cp $TOPDIR/RPMBUILD/RPMS/i386/patch-cmssw-$PRO_DEV-$CMSSW_VERSION_CLEAN$PATCH_ID-*.i386.rpm ~/quattor/patch
