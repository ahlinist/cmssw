#!/bin/bash

#  Strings matching functions definition

#  Usage
#    . string_match.sh

#  Functions
#    - gawkMatchStrict() - Strict match of source and destination strings 
#+                         using gawk regular expressions
#    - gawkMatchBeg()    - Match beginning of source string w/ destination
#    - gawkMatchEnd()    - Match end of source string w/ destination
#    - gawkMatchAny()    - Find destination in source
#    - gawkMatch()       - Match custom regular expression in source

#  Comment
#    Do not use spaces in patterns otherwise use escape characters

#  Author  : Samvel Khalatian (ksam@mail.ru)
#  Date    : 01/10/2006
#  Modified: 01/24/2006


if [[ -z "${_MATCH_STRING}" ]]; then
  _MATCH_STRING=1

  #  Create Pattern if type: '(<match1>|<match2>|...|<matchN>)' based on a 
  #+ number of supplied arguments. Used for internal purposes
  #  Arguments:
  #    1     Variable Name where to put Pattern (dereferencing used)
  #    2...  matches
  #  Return:
  #    [none]
  getPattern() {
    local _cRefVar=$1
    shift

    eval "${_cRefVar}=\"($1\""
    shift
    while [[ -n $1 ]]
    do
      eval "${_cRefVar}=\"${!_cRefVar}|$1\""
      shift
    done

    eval "${_cRefVar}=\"${!_cRefVar})\""
  }

  #  Does String Match given regular expression?
  #  Arguments:
  #    1  Source String
  #    2  Gawk Regular expression
  #  Return:
  #    0  Yes
  #    1  No
  gawkMatch() {
    local _cReturnStatus=1
    
    [[ 2 == "$#" ]] && {
      `eval "echo "$1" | awk '{ if( match( \\\$0, /$2/)) { exit 0} exit 1}'"`
      _cReturnStatus=$?
    }

    return ${_cReturnStatus}
  }

  #  Does String match any pattern?
  #  Arguments:
  #    1     Source String
  #    2...  matches
  #  Return:
  #    0  Yes
  #    1  No
  gawkMatchStrict() {
    local _cReturnStatus=1

    [[ 1 < "$#" ]] && {
      local _cSource=$1
      shift
      local _cPattern
      getPattern _cPattern $*

      _cPattern="^${_cPattern}\$"

      gawkMatch "${_cSource}" "${_cPattern}"

      _cReturnStatus=$?
    }

    return ${_cReturnStatus}
  }

  #  Does String begin with one of given patterns?
  #  Arguments:
  #    1     Source String
  #    2...  matches
  #  Return:
  #    0  Yes
  #    1  No
  gawkMatchBeg() {
    local _cReturnStatus=1

    [[ 1 < "$#" ]] && {
      local _cSource=$1
      shift
      local _cPattern
      getPattern _cPattern $*

      _cPattern="^${_cPattern}"

      gawkMatch "${_cSource}" "${_cPattern}"

      _cReturnStatus=$?
    }

    return ${_cReturnStatus}
  }

  #  Does String end with one of given patterns?
  #  Arguments:
  #    1     Source String
  #    2...  matches
  #  Return:
  #    0  Yes
  #    1  No
  gawkMatchEnd() {
    local _cReturnStatus=1

    [[ 1 < "$#" ]] && {
      local _cSource=$1
      shift
      local _cPattern
      getPattern _cPattern $*

      _cPattern="${_cPattern}\$"

      gawkMatch "${_cSource}" "${_cPattern}"

      _cReturnStatus=$?
    }

    return ${_cReturnStatus}
  }

  #  Does String contain any pattern?
  #  Arguments:
  #    1     Source String
  #    2...  matches
  #  Return:
  #    0  Yes
  #    1  No
  gawkMatchAny() {
    local _cReturnStatus=1

    [[ 1 < "$#" ]] && {
      local _cSource=$1
      shift
      local _cPattern
      getPattern _cPattern $*

      _cPattern="${_cPattern}"

      gawkMatch "${_cSource}" "${_cPattern}"

      _cReturnStatus=$?
    }

    return ${_cReturnStatus}
  }
fi # End _MATCH_STRING
