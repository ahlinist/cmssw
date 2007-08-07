#! /usr/bin/env bash

#   Create new flag that will be automatically put into ./interface/ and
# ./src/ folders. Programmers responsibility is to:
#
#   1. Register newly created Flag in boost/serialization via editing files:
#
#       - interface/RegisterFlagTxt.h
#       - interface/RegisterFlagXML.h
#
#   2. Modify new Flag to add support of any available children. [no children
#      are allowed by default]
#
#   3. Add new Flag to appropriate Makefile

# Author : Samvel Khalatian (samvel at fnal dot gov)
# Date   : 08/03/07
# License: GPL

# Include functions
. bash_print_error.sh
. bash_print_usage.sh
. bash_print_note.sh

# Declare constants
declare -r cNEW_FLAG_CLASS_NAME=${1:-""}
declare -r      cNEW_FLAG_TXT_H="./interface/${cNEW_FLAG_CLASS_NAME}Txt.h"
declare -r      cNEW_FLAG_XML_H="./interface/${cNEW_FLAG_CLASS_NAME}XML.h"
declare -r     cNEW_FLAG_TXT_CC="./src/${cNEW_FLAG_CLASS_NAME}Txt.cc"
declare -r     cNEW_FLAG_XML_CC="./src/${cNEW_FLAG_CLASS_NAME}XML.cc"
declare -r     cFLAG_TXT_TMPL_H="./interface/NewFlagTxt.h.template"
declare -r     cFLAG_XML_TMPL_H="./interface/NewFlagXML.h.template"
declare -r    cFLAG_TXT_TMPL_CC="./src/NewFlagTxt.cc.template"
declare -r    cFLAG_XML_TMPL_CC="./src/NewFlagXML.cc.template"


declare -r               nERR_NOT_SYMLINK=65
declare -r              nERR_MISSING_TMPL=66
declare -r    nERR_NO_NEW_FLAG_CLASS_NAME=67
declare -r nERR_WRONG_NEW_FLAG_CLASS_NAME=68
declare -r           nERR_NEW_FLAG_EXISTS=69

declare -r cUSAGE="`basename $0` <NEW_FLAG_CLASS_NAME>"

if [[ ! -h `basename $0` ]]; then
  printError "Script should be invoked via symbolic script.\n"
  printUsage
  exit $nERR_NOT_SYMLINK
fi

if [[ ! -f "$cFLAG_TXT_TMPL_H"  ||
      ! -f "$cFLAG_XML_TMPL_H"  ||
      ! -f "$cFLAG_TXT_TMPL_CC" ||
      ! -f "$cFLAG_XML_TMPL_CC" ]]; then

  printError "It seems that one of the input template files is missing. Make \
sure you are running script from appropriate folder and next files can be \
found in subfolders:\n\
  - '$cFLAG_TXT_TMPL_H'\n\
  - '$cFLAG_XML_TMPL_H'\n\
  - '$cFLAG_TXT_TMPL_CC'\n\
  - '$cFLAG_XML_TMPL_CC'\n"
  printUsage
  exit $nERR_MISSING_TMPL
fi

if [[ -z "$cNEW_FLAG_CLASS_NAME" ]]; then
  printError "New Flag Class name is not specified\n"
  printUsage
  exit $nERR_NO_NEW_FLAG_CLASS_NAME
fi

# Check if Flag Class name contains Txt or XML suffix
if [[ -nz `echo ${cNEW_FLAG_CLASS_NAME} | sed -n -e '\_\(Txt\|XML\)$_ p'` ]]; then
  printError "New Flag Class should not contain suffix of Txt or XML. It will \
 be automatically added. Remove them from name and try again\n"
  printUsage
  exit $nERR_WRONG_NEW_FLAG_CLASS_NAME
fi

# Check if there is already flag available with given class name
if [[ -f "$cNEW_FLAG_TXT_H"  ||
      -f "$cNEW_FLAG_XML_H"  ||
      -f "$cNEW_FLAG_TXT_CC" ||
      -f "$cNEW_FLAG_XML_CC" ]]; then

  printError "Flags already exists with specified Class name. Choose another 
 one please.\n"
  printUsage
  exit $nERR_NEW_FLAG_EXISTS
fi

# --[ CREATE FLAGS ]--
declare -r cFLAG_SAFE_GUARDS=`echo ${cNEW_FLAG_CLASS_NAME} | tr 'a-z' 'A-Z'`

# interface/<FLAG>Txt.h
sed -e "s_NewFlagXML_${cNEW_FLAG_CLASS_NAME}XML_g" \
    -e "s_NewFlagTxt_${cNEW_FLAG_CLASS_NAME}Txt_g" \
    -e "s_NEWFLAGXML_${cFLAG_SAFE_GUARDS}XML_g" \
    -e "s/NEW_FLAG_TXT_TEMPLATE_H/${cFLAG_SAFE_GUARDS}_TXT_H/" \
    -e "\_^// Created: _ s_[0-9]\{1,2\}/[0-9]\{1,2\}/[0-9]\{1,4\}_`date +%m/%d/%y`_" \
    < "${cFLAG_TXT_TMPL_H}" > "${cNEW_FLAG_TXT_H}"

# interface/<FLAG>XML.h
sed -e "s_NewFlagXML_${cNEW_FLAG_CLASS_NAME}XML_g" \
    -e "s_NewFlagTxt_${cNEW_FLAG_CLASS_NAME}Txt_g" \
    -e "s_NEWFLAGTXT_${cFLAG_SAFE_GUARDS}TXT_g" \
    -e "s/NEW_FLAG_XML_TEMPLATE_H/${cFLAG_SAFE_GUARDS}_XML_H/" \
    -e "\_^// Created: _ s_[0-9]\{1,2\}/[0-9]\{1,2\}/[0-9]\{1,4\}_`date +%m/%d/%y`_" \
    < "${cFLAG_XML_TMPL_H}" > "${cNEW_FLAG_XML_H}"

# src/<FLAG>Txt.cc
sed -e "s_NewFlagXML_${cNEW_FLAG_CLASS_NAME}XML_g" \
    -e "s_NewFlagTxt_${cNEW_FLAG_CLASS_NAME}Txt_g" \
    -e "s/NEW\sFLAG\sTXT/${cFLAG_SAFE_GUARDS} TXT/" \
    -e "\_^// Created: _ s_[0-9]\{1,2\}/[0-9]\{1,2\}/[0-9]\{1,4\}_`date +%m/%d/%y`_" \
    < "${cFLAG_TXT_TMPL_CC}" > "${cNEW_FLAG_TXT_CC}"

# src/<FLAG>XML.cc
sed -e "s_NewFlagXML_${cNEW_FLAG_CLASS_NAME}XML_g" \
    -e "s_NewFlagTxt_${cNEW_FLAG_CLASS_NAME}Txt_g" \
    -e "s/NEW\sFLAG\sXML/${cFLAG_SAFE_GUARDS} XML/" \
    -e "\_^// Created: _ s_[0-9]\{1,2\}/[0-9]\{1,2\}/[0-9]\{1,4\}_`date +%m/%d/%y`_" \
    < "${cFLAG_XML_TMPL_CC}" > "${cNEW_FLAG_XML_CC}"

# User instructions
echo -e "New Flag \033[1m${cNEW_FLAG_CLASS_NAME}\033[0m was created successfully. List of added files:"
echo
echo "${cNEW_FLAG_TXT_H}"
echo "${cNEW_FLAG_XML_H}"
echo "${cNEW_FLAG_TXT_CC}"
echo "${cNEW_FLAG_XML_CC}"
echo
printNote "Don't forget to register newly created flag. Edit interface/RegisterFlag{Txt,XML}.h for that."
printNote "Children policy should be defined by programmer and not taken care of automatically."
printNote "Take care of adding new Flag to appropriate Makefile to enable it's compilation."
echo

exit 0
