#!/bin/bash
#
# File:   digital_clock.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014-2019 Nick Korotysh
#
# Created on November 9, 2014, 11:36 AM
#
# Clock launcher script
#

function detect_kde()
{
  if [[ "$XDG_CURRENT_DESKTOP" == "KDE" || "$XDG_SESSION_DESKTOP" == "KDE" ]]
  then
    echo 1
  else
    echo 0
  fi
}


function compare_versions()
{
  lhs=($(echo $1 | tr '.' '\n'))
  rhs=($(echo $2 | tr '.' '\n'))
  res=0
  for ((i=0; i<3; i++))
  do
    [[ $res -ne 0 ]] && break
    [[ ${lhs[i]} -gt ${rhs[i]} ]] && { res=1; }
    [[ ${lhs[i]} -lt ${rhs[i]} ]] && { res=-1; }
  done
  echo $res
}


function validate_system_qt()
{
  required_vers=$(echo $1 | sed 's/\..$/\.0/g')
  res=0
  qt_core=$(find /usr/lib* -name libQt5Core.so.5 2> /dev/null)
  if [[ -n "$qt_core" ]]
  then
    version=$(ls -1 $qt_core* | grep -Po '\d+\.\d+\.\d+')
    # validate system Qt version
    [[ $(compare_versions $version $required_vers) -ge 0 ]] && { res=1; }
  fi
  echo $res
}


# handle script arguments
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


appname=$(basename "$0" | sed s,\.sh$,,)

dirname=$(dirname "$0")
tmp="${dirname#?}"

if [[ "${dirname%$tmp}" != "/" ]]
then
  dirname=$PWD/$dirname
fi

# work in application directory
cd "$dirname"

qt_vers=$("./qt/libQt5Core.so.5" | grep -Po 'Qt \d+\.\d+\.\d+' | cut -c 3- -)

# on KDE systems try to use system Qt libraries instead of shipped ones
if [[ $(detect_kde) -eq 0 || $(validate_system_qt "$qt_vers") -eq 0 ]]
then
  export QT_QPA_PLATFORMTHEME=gtk2
  export LD_LIBRARY_PATH="$dirname/qt:$LD_LIBRARY_PATH"
fi
export LD_LIBRARY_PATH="$dirname:$LD_LIBRARY_PATH"
"./$appname" ${ARGS[@]}
