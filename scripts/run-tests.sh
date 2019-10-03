#!/bin/bash
set -ev
#Only build if not coming from a pull request
if [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  #Compile
  cd build && cmake .. && make
  #Test
  cp New-simu ../test/ && mv ../test && ./New-simu
fi
