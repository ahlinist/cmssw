#! /usr/bin/env bash

# Generic function that prints ERROR message

# Author  : Samvel Khalatian (ksamdev at gmail dot com)
# Date    : 10/06/06
# Licence : GPL

# History:
#   10/12/06  Added Include Guards

if [[ -z "${_PRINT_ERROR_FUNCTION}" ]]; then
  _PRINT_ERROR_FUNCTION=1

  # Print Error message
  # @arguments
  #   1  Error message /may be omitted/
  # @return
  #   none
  function printError {
    echo -e "[\033[31merror\033[0m] $1" >&2 
  }
fi # End _PRINT_ERROR_FUNCTION
