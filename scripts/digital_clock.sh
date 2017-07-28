#!/bin/bash
#
# File:   digital_clock.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014-2017 Nick Korotysh
#
# Created on November 9, 2014, 11:36 AM
#
# Clock launcher script
#
# This script is improved version of script from
# http://doc.qt.io/qt-5/linux-deployment.html#creating-the-application-package
#

autostart=0

ARGS=()
for var in "$@"
do
  if [[ "$var" != '--autostart' ]]
  then
    ARGS+=("$var")
  else
    autostart=1
  fi
done

[[ $autostart -ne 0 ]] && sleep 15


appname=`basename "$0" | sed s,\.sh$,,`

dirname=`dirname "$0"`
tmp="${dirname#?}"

if [[ "${dirname%$tmp}" != "/" ]]
then
  dirname=$PWD/$dirname
fi

export LD_LIBRARY_PATH="$dirname:$LD_LIBRARY_PATH"
"$dirname/$appname" "${ARGS[@]}"

