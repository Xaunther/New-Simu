#!/bin/bash
set -ev
#Only build if not coming from a pull request
if [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  cd build && cmake .. && make
fi
