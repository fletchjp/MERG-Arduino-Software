#!/usr/bin/env bash

#   GluCat : Generic library of universal Clifford algebra templates
#   pyclical-test-one-config-option.sh : Test one defined configuration of PyClical.
#
#   begin                : Sun 2016-04-03
#   copyright            : (C) 2016-2022 by Paul C. Leopardi
#
#   This library is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published
#   by the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   For licence details, see ${package_dir}/COPYING

# set -x

here=$(cd $(dirname ${0})/;pwd)
. ${here}/define-config-options.sh

line=$1
shift
args=$*

abbrev=$(eval echo $(match_option_pattern ${line} 1))
options=$(eval echo $(match_option_pattern ${line} 2))

pushd ${package_dir}/.. \
  > /dev/null

  rm -rf ${package_dir}.${line}
  cp -a ${package_dir} ${package_dir}.${line}

  pushd ${package_dir}.${line} \
    > /dev/null

    ./configure ${options} \
      > /dev/null
    make clean \
      > /dev/null

    pushd pyclical \
      > /dev/null

      make ${args} check \
        > make.out
      if [ -f check.out ]
      then
        mv check.out pyclical-test.check.out
      fi

    popd \
      > /dev/null

    make clean \
      > /dev/null

  popd \
    > /dev/null

popd \
  > /dev/null
