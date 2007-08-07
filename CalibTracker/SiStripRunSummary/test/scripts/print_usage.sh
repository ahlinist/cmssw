#! /usr/bin/env bash

# Generic function that prints scripts USAGE + some additional message if any
# provided

# Author  : Samvel Khalatian (ksamdev at gmail dot com)
# Date    : 10/06/06
# Licence : GPL

# History:
#   10/12/06  Changed Usage color to GREEN and slightly modified function:
#             it's functionality is completely the same
#   10/12/06  Added Include Guards

if [[ -z "${_PRINT_USAGE_FUNCTION}" ]]; then
  _PRINT_USAGE_FUNCTION=1

  # Print custom message /if any provided/ + cUSAGE
  # @arguments
  #   1  Custom message /may be omitted/
  # @return
  #   none
  function printUsage {
    [[ -n $1 ]] && echo -e "$1\n"

    echo -e "[\033[32musage\033[0m] $cUSAGE"
  }
fi # End _PRINT_USAGE_FUNCTION
