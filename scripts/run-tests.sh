#!/bin/bash
set -ev
#Only build if not coming from a pull request
if [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  #Compile
  cd build && cmake .. && make
  #Test
  #cd ../test && cp ../build/New-simu . && ./New-simu
fi
