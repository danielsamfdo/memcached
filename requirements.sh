#!/bin/sh


UNAME_S="$(uname -s)"

if["$UNAME_S" -eq "Linux"];then
  sudo apt-get install libboost-all-dev
fi