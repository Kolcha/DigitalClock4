#!/bin/bash
#
# File:   digital_clock.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014 Nick Korotysh
#
# Created on November 9, 2014, 11:36 AM
#
# Clock launcher script
#

script_path=$(readlink -f "$0")
cd "${script_path%/*}"

export LD_LIBRARY_PATH="$PWD:$LD_LIBRARY_PATH"
./digital_clock

