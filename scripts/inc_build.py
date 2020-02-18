#!/usr/bin/env python3

import os
import re
import sys


def inc_build(rc_file):
    f = open(rc_file, "rb")
    data = f.read()
    f.close()

    def __inc_build(m):
        return re.sub(rb'\d+$', str(int(m[4]) + 1).encode('ascii'), m[0])

    data = re.sub(rb'FILEVERSION\s+(\d+),(\d+),(\d+),(\d+)', __inc_build, data)
    data = re.sub(rb'(\d+)\.(\d+)\.(\d+)\.(\d+)', __inc_build, data)

    f = open(rc_file, "wb")
    f.write(data)
    f.close()


if (len(sys.argv) > 1):
    for arg in sys.argv[1:]:
        inc_build(arg)
else:
    from glob import glob
    pwd = os.getcwd()
    lst = [y for x in os.walk(pwd) for y in glob(os.path.join(x[0], '*.rc'))]
    for arg in lst:
        inc_build(arg)
