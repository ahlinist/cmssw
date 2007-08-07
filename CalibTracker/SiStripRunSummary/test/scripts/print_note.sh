#! /usr/bin/env bash

# Generic function that prints Note with some additional message if any
# provided

# Author  : Samvel Khalatian (ksamdev at gmail dot com)
# Date    : 10/23/06
# Licence : GPL

# History:

if [[ -z "${_PRINT_NOTE_FUNCTION}" ]]; then
  _PRINT_NOTE_FUNCTION=1

  # Print custom message /if any provided/
  # @arguments
  #   1  Custom message /may be omitted/
  # @return
  #   none
  function printNote {
    echo -e "[\033[1mNote\033[0m] $1"
  }
fi # End _PRINT_NOTE_FUNCTION
