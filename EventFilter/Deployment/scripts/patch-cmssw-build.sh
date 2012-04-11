#!/bin/bash
numArgs=$#
if [ $numArgs -lt 4 ]; then
    echo "Usage: patch-cmssw-build.sh CMSSW_X_Y_Z patchId {dev|pro|...} patchdir"
    exit -1
fi
CMSSW_VERSION=$1            # the CMSSW version, as known to scram
PATCH_ID=$2                 # an arbitrary tag which identifies the extra code (usually, "p1", "p2", ...)
AREA=$3                     # "pro", "dev", etc...
LOCAL_CODE_PATCHES_TOP=$4   # absolute path to the area where extra code to be compiled in can be found, equivalent to $CMSSW_BASE/src

# set the RMP build architecture
BUILD_ARCH=$(uname -i)      # "i386" for SLC4, "x86_64" for SLC5

# check that scram is available
if ! which scram >& /dev/null; then
  source /opt/cmssw/cmsset_default.sh
fi

# set environment variables
export PATH=$PATH:$PWD/EventFilter/Deployment/scripts
export CVSROOT=:pserver:anonymous@cmscvs.cern.ch:/cvs_server/repositories/CMSSW

# define SCRAM_ARCH if unset
[ -n "$SCRAM_ARCH" ] || SCRAM_ARCH=slc5onl_amd64_gcc462
export SCRAM_ARCH

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

# create a local CMSSW area, import patches from the specified directory, and build them (except for the "test" directories)
scram project CMSSW $CMSSW_VERSION
sed -e's#<classpath path=".*+test"/>##' -i $CMSSW_VERSION/config/BuildFile.xml
cd $CMSSW_VERSION/src
[ "$(ls $LOCAL_CODE_PATCHES_TOP/)" ] && cp -r $LOCAL_CODE_PATCHES_TOP/* .
scram b USER_CXXFLAGS="-g" -j 8
cd $TOPDIR

echo "Moving patches to their destination"
mkdir -p $TOPDIR/opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION
cp -r $TOPDIR/$CMSSW_VERSION/src $TOPDIR/opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/
cp -r $TOPDIR/$CMSSW_VERSION/lib $TOPDIR/opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/

# add links to the external data directories
echo "Linking extrnal data to their destination"
TARGET=$TOPDIR/opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/src
for CMSSWDATA in $(cd $CMSSW_VERSION; scram tool info cmsswdata | grep CMSSW_SEARCH_PATH | cut -d= -f2 | tr ':' ' '); do
    PACKAGE=$(echo $CMSSWDATA | sed -e's#.*/data-\(\w\+\)-\(\w\+\)/.*#\1/\2#')
    # make sure the parent directory for the links exists
    mkdir -p $TARGET/$PACKAGE
    ln -s $CMSSWDATA/$PACKAGE/* $TARGET/$PACKAGE
done

echo "Generating and populating summary directories"
# this should be equivalent to $CMSSW_RELEASE_BASE
CMSSW_ROOT_DIRECTORY=`grep RELEASETOP $CMSSW_VERSION/.SCRAM/$SCRAM_ARCH/Environment | cut -d= -f2`
cd $CMSSW_ROOT_DIRECTORY/src
install_env.pl $TOPDIR/opt/cmssw/$AREA/lib $TOPDIR/opt/cmssw/$AREA/module $TOPDIR/opt/cmssw/$AREA/env.txt $TOPDIR/opt/cmssw/$AREA/python

echo "Linking .rootmap plugin entries from $CMSSW_ROOT_DIRECTORY"
find $CMSSW_ROOT_DIRECTORY/external/$SCRAM_ARCH/lib -name '*.rootmap' | xargs -i ln -sf {} $TOPDIR/opt/cmssw/$AREA/lib
find $CMSSW_ROOT_DIRECTORY/lib/$SCRAM_ARCH          -name '*.rootmap' | xargs -i ln -sf {} $TOPDIR/opt/cmssw/$AREA/lib

# link debug symbols from the base release
echo "Linking .debug symbols from $CMSSW_ROOT_DIRECTORY"
TARGET=$TOPDIR/opt/cmssw/$AREA/lib
mkdir -p $TARGET/.debug/
for FILE in $TARGET/*.so; do
    ORIG=$(dirname $(readlink $FILE))
    NAME=$(basename $FILE).debug
    DEBUG=$ORIG/.debug/$NAME
    [ -f $DEBUG ] && ln -s $DEBUG $TARGET/.debug/
done

cd $TOPDIR
for x in opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/lib/$SCRAM_ARCH/*.so
do
  if [ -e $x ]
      then
      ln -sf $PWD/$x opt/cmssw/$AREA/lib/
  fi
done

echo "Updating edm plugin registry with patches"
cd $TOPDIR/$CMSSW_VERSION
RESPONSE=`reset_edm_cache.pl $TOPDIR/opt/cmssw/$AREA/lib $TOPDIR/opt/cmssw/$AREA/module /dev/null`
echo "reset_edm_cache returned $RESPONSE"

### Extract the root version from the scram runtime here before making the symbolic links
[ -n "$ROOTSYS" ] || ROOTSYS=`scramv1 runtime -sh | grep ROOTSYS= | tr -d ";" | perl -p -e 's/.*=\"([^\"]*)\"$/$1/'`
echo "extracting symbolic link locations for root from scram runtime"
echo "  ROOTSYS at $ROOTSYS"

# fix symbolic links for patched libs so it is visible after installation
cd $TOPDIR
for x in opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION/lib/$SCRAM_ARCH/*.so
do
  if [ -e $x ]
      then
      ln -sf /$x opt/cmssw/$AREA/lib
  fi
done

cd $TOPDIR
# remove temporary directory holding the local patches
#rm -rf $TOPDIR/$CMSSW_VERSION
ln -s /opt/cmssw/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION                opt/cmssw/$AREA/base
ln -s /opt/cmssw/$AREA/patches/$SCRAM_ARCH/cms/$RELEASE_TYPE/$CMSSW_VERSION  opt/cmssw/$AREA/patch
ln -s $ROOTSYS                                                               opt/cmssw/$AREA/root
CMSSW_VERSION_CLEAN=`echo $CMSSW_VERSION | tr -d "-"`

echo "CMSSW_VERSION now $CMSSW_VERSION"
cat > patch-cmssw.spec <<EOF
Name: patch-cmssw-$AREA
Version: $CMSSW_VERSION_CLEAN$PATCH_ID
Release: 7
Summary: CMSSW Patches
License: Unknown
Group: Hacks
Packager: scram
Source: none
%define _tmppath $TOPDIR/patch-cmssw
BuildRoot: %{_tmppath}
BuildArch: $BUILD_ARCH
AutoReqProv: no
Provides:/opt/cmssw/$AREA/lib
Provides:/opt/cmssw/$AREA/python
Provides:/opt/cmssw/$AREA/base
Provides:/opt/cmssw/$AREA/root
Provides:/opt/cmssw/$AREA/env.txt
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
/opt/cmssw/$AREA/env.txt
/opt/cmssw/$AREA/base
/opt/cmssw/$AREA/lib
/opt/cmssw/$AREA/patch
/opt/cmssw/$AREA/patches
/opt/cmssw/$AREA/python
/opt/cmssw/$AREA/root
EOF
mkdir -p RPMBUILD/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
rpmbuild --define "_topdir `pwd`/RPMBUILD" -bb patch-cmssw.spec
#rm -rf patch-cmssw-tmp

# copy the RPM to a local folder
VERSION=$(echo $CMSSW_VERSION | sed -e's/_ONLINE$//')
mkdir -p /cmsswrelease/$VERSION/patch
cp $TOPDIR/RPMBUILD/RPMS/$BUILD_ARCH/patch-cmssw-$AREA-$CMSSW_VERSION_CLEAN$PATCH_ID-*.$BUILD_ARCH.rpm /cmsswrelease/$VERSION/patch
