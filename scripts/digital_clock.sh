#!/bin/sh
#
# File:   digital_clock.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014-2016 Nick Korotysh
#
# Created on November 9, 2014, 11:36 AM
#
# Clock launcher script
#
# This script is slightly modified copy of script from
# http://doc.qt.io/qt-5/linux-deployment.html#creating-the-application-package
#

appname=`basename "$0" | sed s,\.sh$,,`

dirname=`dirname "$0"`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH
"$dirname/$appname" "$@"
